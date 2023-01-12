//
//  TQuicDataTask.m
//

#import "TQUICService.h"
#import <sys/utsname.h>
#import <Tquic/tnet_quic_request.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>

#import "TQUICConfig.h"
#import "TQUICResponse.h"
#import "TQUICRequest.h"
#import "TQUICStats.h"

class TnetAsyncDelegate : public TnetRequestDelegate {
public:
    
protected:
    TQUICReqDidConnectHandler didConnect_;
    TQUICReqDidReceiveResponseHandler didReceiveResponse_;
    TQUICReqDidSendBodyDataHandler didSendBodyData_;
    TQUICReqDidFinishHandler didFinish_;
    TQUICReqDidCloseHandler didClose_;
    TQUICRequest *quicReqeust_;
    std::string response_str_;

private:
    TnetQuicRequest* shared_tquic_request_;
    int64_t totolSentBytes;
    
public:
    TnetAsyncDelegate() {}
    
    TnetAsyncDelegate(TQUICRequest *quicRequest) {
        totolSentBytes = 0;
        this->quicReqeust_ = quicRequest;
    }

    void InitWithQuicRequest(TQUICRequest *quicRequest, TnetQuicRequest* tquic_request) {
        totolSentBytes = 0;
        this->quicReqeust_ = quicRequest;
        this->shared_tquic_request_ = tquic_request;
        response_str_.clear();
    }
    
    void resisterDidConnect(TQUICReqDidConnectHandler didConnect) {
        this->didConnect_ = didConnect;
    }

    void resisterDidReceiveResponse(TQUICReqDidReceiveResponseHandler didReceiveResponse) {
        this->didReceiveResponse_ = didReceiveResponse;
    }
    
    void resisterDidSentBodyData(TQUICReqDidSendBodyDataHandler didSentBodyData) {
        this->didSendBodyData_ = didSentBodyData;
    }
    
    void resisterDidFinish(TQUICReqDidFinishHandler completion) {
        this->didFinish_ = completion;
    }

    void resisterDidClose(TQUICReqDidCloseHandler completion) {
        this->didClose_ = completion;
    }

    ~TnetAsyncDelegate() override {
        this->quicReqeust_ = nullptr;
        this->didConnect_ = nullptr;
        this->didReceiveResponse_ = nullptr;
        this->didSendBodyData_ = nullptr;
        this->didFinish_ = nullptr;
        this->didClose_ = nullptr;
    }

#pragma mark URLRequest::Delegate methods

    NSData *bodyData() {
        NSData *body = this->quicReqeust_.body;
        return body;
    }

    void sendData(NSData *data) {
        unsigned long dataLength = data.length;
        const void *_Nullable rawData = [data bytes];
        char *src = (char *)rawData;
        NSLog(@"Tquic SendRequest with size: %lu and end", (unsigned long)dataLength);

        if (this->didSendBodyData_ != nullptr) {
            this->didSendBodyData_(dataLength, dataLength, dataLength);
        } else {
            shared_tquic_request_->SendRequest(src, dataLength, true);
        }
    }
    
    //连接建立成功的回调
    void OnConnect(int error_code) override {
        if (quicReqeust_ && quicReqeust_.body) {
            NSData *data = bodyData();
            sendData(data);
        }
    }

    // 收到数据并且输出
    void OnDataRecv(const char* buf, const int buf_len) override {
        NSString *responseStr = [NSString stringWithUTF8String:buf];
        BOOL isHeader = [responseStr hasPrefix:@"HTTP/1.1"];
        if (isHeader) {
            TQUICResponse *response = [TQUICResponse responseWithStr:responseStr];

            if (this->didReceiveResponse_ != nullptr) {
                this->didReceiveResponse_(response);
            }

        } else {
            response_str_.append(buf, buf_len);
        }
    }

    void OnConnectionClose(int error_code, const char* error_detail) override {
        NSError *error = [[NSError alloc] initWithDomain:NSURLErrorDomain code:error_code userInfo:@{
            NSLocalizedDescriptionKey: [NSString stringWithUTF8String:error_detail]
        }];
        NSLog(@"Tquic OnConnectionClose with error: %@",error);
        if (this->didClose_ != nullptr) {
            this->didClose_(error);
        }
    }

    void OnRequestFinish(int stream_error) override {
        NSError *error;
        if (stream_error != 0) {
            error = [[NSError alloc] initWithDomain:NSURLErrorDomain code:stream_error userInfo:nil];
        }
    }
};


TnetConfig ToTnetConfig(TQUICConfig* config) {
    TnetConfig tconfig;

    if (config.raceType == TQUICRaceTypeOnlyQUIC) {
        tconfig.race_type = kOnlyQUIC;
    } else if (config.raceType == TQUICRaceTypeQUICHTTP) {
        tconfig.race_type = kQUICHTTP;
    } else if (config.raceType == TQUICRaceTypeOnlyHTTP) {
        tconfig.race_type = kOnlyHTTP;
    }
    tconfig.upload_optimize_ = config.uploadOptimize;
    tconfig.total_timeout_millisec_ = config.totalTimeoutMS;
    tconfig.connect_timeout_millisec_ = config.connectTimeoutMS;
    tconfig.idle_timeout_millisec_ = config.idleTimeoutMS;
    tconfig.quic_version_ = config.quicVersion;
    tconfig.force_zero_rtt_ = config.force0rtt;
    return tconfig;
}


@interface TQUICService () {
  std::unique_ptr<TnetQuicRequest> tnet_quic_request_;
  std::unique_ptr<TnetAsyncDelegate> quic_delegate_;
}
@end

@implementation TQUICService

- (id)init {
    TQUICConfig *config = [TQUICConfig splashConfig];
    TnetConfig tconfig = ToTnetConfig(config);
    quic_delegate_->resisterDidFinish(nullptr);
    quic_delegate_.reset(new TnetAsyncDelegate());
    tnet_quic_request_.reset(new TnetQuicRequest(quic_delegate_.get(), tconfig))

    return self;
}

- (void)dealloc {
    // clean delegate
    tnet_quic_request_->SetDelegate(nullptr);
    quic_delegate_->resisterDidFinish(nullptr);
}

+ (void)cancel {
    tnet_quic_request_->CancelRequest();
}

+ (void)startRequest:(TQUICRequest *)quicRequest
                      completion:(TQUICReqDidFinishHandler)completion {
    

    quic_delegate_->InitWithQuicRequest(quicRequest, tnet_quic_request_);
    quic_delegate_->resisterDidFinish(completion);

    NSDictionary *allHeaderFields = quicRequest.allHeaderFields;
    for (NSString *key in allHeaderFields) {
        const char* value = [allHeaderFields[key] UTF8String];
        const char* ukey = [[key lowercaseString] UTF8String];
        tnet_quic_request_->AddHeaders(ukey, value);
    }
    if (quicReqeust_.ip) {
        // connect with ip directly
        tnet_quic_request_->Connect(quicRequest.host.UTF8String,
                                    quicReqeust_.ip.UTF8String,
                                    (int)quicRequest.port,
                                    80);
    } else {
       // connect with dns
       tnet_quic_request_->ConnecWithDomain(quicRequest.host.UTF8String, (int)quicRequest.port);
    }
}

}

@end
