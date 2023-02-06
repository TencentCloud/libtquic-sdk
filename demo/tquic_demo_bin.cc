#include "tnet_quic_request.h"  // 导入API头文件

#include <cstdlib>

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
using namespace std;

mutex s_lock;
condition_variable s_cv;

enum RETValue {
  OK = 0,
  UNKNOW = 1,
  FAIL = 2,
  FINISH = 3
};

/*
1. new TnetRequest对象
2. 调用Connect
3. 等待OnConnect或者OnConnectionClose回调，如果是OnConnectionClose，则请求失败
4. 如果是OnConnect,在回调函数或者唤醒业务线程，调用SendRequest发送请求
5. 等待OnDataRecv回调，并将数据拷贝，不要堵塞这个回调
6. 等待OnRequestFinish回调,回调后说明请求已结束，判断OnRequestFinish状态码是否为0，不为0说明异常结束


OnConnectionClose回调处理
整个请求生命都有可能遇到，遇到后
1. 可以析构掉TnetRequest对象，退出tquic
2. 可以发起Connect再次连接，重复上面的步骤
*/

// Delegate里的回调,都由tquic线程来执行,业务不需要额外开辟线程
class DemoDelegate : public TnetRequestDelegate {
 public:
  DemoDelegate() {
    connect_ret = request_ret = UNKNOW;
  }
  
  void OnConnect(int error_code) override {
    // error_code不需要判断，一定是0
    connect_ret = OK;
    // 唤醒业务线程
    s_cv.notify_all();
  }

  void OnDataRecv(const char* buf, const int buf_len) override {
    // 1. 务必在该回调中，将数据读取并拷贝完，离开函数后，不保证指针指向数据完整性
    // 2. 不要用重操作堵塞该函数，建议唤醒其他线程继续处理
    // 3. 使用buf_len去读取数据，不要直接读buf，比如不要用strlen(buf);防止被特殊字符截断
    string data_(buf, buf_len);
    cout << "Recv response data: " << data_ << endl;
  }

  // 请求结束回调函数，表示请求粒度，OnConnectionClose表示连接粒度
  void OnRequestFinish(int stream_error) override{
    if (stream_error == 0) {
      // 请求正常结束
      request_ret = FINISH;
    } else {
      // 请求异常结束
      request_ret = FAIL;
      cout << "Request finish with error: " << stream_error << endl;
    }
    // 唤醒业务线程
    s_cv.notify_all();
  }

  void OnConnectionClose(int error_code,
                         const char* error_detail) override {
    if (error_code == 0) {
      // 错误码为0，是客户端调用CloseConnection主动关闭连接，无错误发生
      connect_ret = FINISH;
    } else {
      // 出现连接错误，注：连接长时间空闲导致超时断开也会进入这里
      connect_ret = FAIL;
      cout << "Connnection close with error: " << error_code
           << " and detail is " << error_detail << endl;
    }
    // 唤醒业务线程
    s_cv.notify_all();
  }

  RETValue GetConnRet() {
    return connect_ret;
  }

  RETValue GetRequestRet() {
    return request_ret;
  }

 private:
  RETValue connect_ret;
  RETValue request_ret;
};


int main() {
  string host = "www.example.com";
  string vip = "10.0.0.1";
  int port = 443;

  // 设置日志打印到文件,默认输出到标准输出,可以不调用
  SetTquicLog("./tquic.log", true);

  TnetConfig config;
  
  // 设置拥塞算法，默认值即为BBR
  config.congestion_type_ = kBBR;

  // 设置连接超时时间，单位为ms，默认为4000ms
  config.connect_timeout_millisec_ = 2000;

  // 设置连接空闲时间，单位为ms，默认值为与服务端协商值，一般为90000ms
  config.idle_timeout_millisec_ = 1000;

  // 设置quic版本，默认就是43，建议无需修改
  config.quic_version_ = 43;

  // 开启连接复用，默认就是true
  config.use_session_reuse_ = true;

  // 是否使用QUIC传输私有协议，为true代表传输私有协议
  // 默认为false即使用h2/h3
  config.is_custom_ = false;


  DemoDelegate* demo_delegate_ = new DemoDelegate();

  // new一个quic对象，进行连接和请求
  TnetQuicRequest* request_ = new TnetQuicRequest(demo_delegate_, config);

  // 针对VIP发起QUIC直连，host仅用于握手，不用于域名解析
  request_->Connect(host.c_str(), vip.c_str(), port, 80);
  // eg. 另外一种连接接口：通过域名发起QUIC连接，底层会进行DNS解析
  // request_->ConnectWithDomain(host.c_str(), port);

  unique_lock<mutex> lk(s_lock);
  // 业务线程阻塞，等待被唤醒
  s_cv.wait(lk);

  if (demo_delegate_->GetConnRet() != OK) {
    return -1;
  }

  // 设置url path
  request_->AddHeaders(":path", "/abc");
  // 添加自定义头部
  request_->AddHeaders("X-Info", "test");

  // 如果带body，底层默认是POST，如果不带body，底层默认是GET
  // 不需要设置Content-length，底层会默认加上
  string body = "abcd";
  // 如果is_custom_ == true, 则只传输body内容（私有协议不包含头部）
  // 默认使用h2/h3（未设置is_custom_或is_custom_==false）
  // body非空则为POST请求，否则为GET请求
  request_->SendRequest("abcd", body.length(), true);
  // eg.发GET请求：request_->SendRequest("", 0, true);

  // 业务线程阻塞，等待被唤醒
  s_cv.wait(lk);

  // 获取底层统计信息
  TnetStats stats = request_->GetTnetStates();
  std::cout << "TnetStats is "
            << stats.is_valid << " "
            << stats.is_quic << " "
            << stats.is_0rtt << endl << "connect: "
            << stats.connect_ms << endl << "ttfb: "
            << stats.ttfb_ms << endl << "total: "
            << stats.complete_ms << endl
            << stats.srtt_us << " "
            << stats.packets_sent << " "
            << stats.packets_retransmitted << " "
            << stats.bytes_sent << " "
            << stats.bytes_retransmitted << " "
            << stats.packets_lost << " "
            << stats.packets_received << " "
            << stats.bytes_received << " "
            << stats.stream_bytes_received << endl;
  // 客户端主动关闭连接,也可以不调用
  request_->CloseConnection();
  delete request_;
  // 务必保证delegate在request之后析构
  delete demo_delegate_;

}
