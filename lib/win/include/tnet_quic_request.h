// Copyright (c) 2018 The Tencent Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TENCENT_TNET_QUIC_REQUEST_H
#define TENCENT_TNET_QUIC_REQUEST_H

#if defined(TNET_SHARED_LIB)
  #if defined(WIN32)
    #if defined(TNET_IMPLEMENTATION)
      #define TNET_EXPORT __declspec(dllexport)
    #else
      #define TNET_EXPORT __declspec(dllimport)
    #endif
  #else
    #if defined(TNET_IMPLEMENTATION)
      #define TNET_EXPORT __attribute__((visibility("default")))
    #else
      #define TNET_EXPORT
    #endif
  #endif
#else
  #define TNET_EXPORT
#endif

#include <cstdint>

namespace stgw {
class TnetRequestFront;
}

struct TnetStats {
  bool is_valid;        // if this stats is valid.
  bool is_quic;         // if it is quic, else it is tcp.
  bool is_0rtt;         // Only valid if it is quic.
  bool is_conn_reuse;   // if reuse an exist quic connection.
  std::uint64_t connect_ms;  // connect cost in millionseconds.
  std::uint64_t ttfb_ms;     // first byte cost from send request in millionseconds.

  // Only valid if it is quic.
  std::uint64_t complete_ms; // all bytes received cost from send request.
  std::uint64_t srtt_us;  // Smoothed RTT in microseconds.
  std::uint64_t packets_sent;  // Number of packets be sent.
  std::uint64_t packets_retransmitted;  // Number of packets be retransmitted.

  std::uint64_t bytes_sent;
  std::uint64_t bytes_retransmitted;
  std::uint64_t packets_lost;  // Number of packets be lost when sent data.
  std::uint64_t packets_received; // Total packets received
  std::uint64_t bytes_received;  // Total bytes received including packet format.
  std::uint64_t stream_bytes_received;  // Total bytes received including duplicated data.
  std::uint64_t dns_ms;
  std::uint64_t cancel_duration_ms;
  std::uint64_t shlo_duration_ms;
  std::uint64_t preprocess_duration_ms;
  int  dns_code;
  bool is_default_ip;
  char remote_ip[64];
  char conn_id[32];
  int engine_type;  // only used if ConnectAndSend
  int sock_bind_type; // only used if ConnectAndSend
  int migration_count;
};

enum CongestionType {
  kCubicBytes,
  kRenoBytes,
  kBBR,
  kPCC,
  kGoogCC // kGoogCC is equal to BBR in quic.
};

enum RaceType {
  kOnlyQUIC,
  kQUICHTTP,
  kOnlyHTTP
};

class TNET_EXPORT TnetConfig {
 public: 
  TnetConfig();
  TnetConfig(const TnetConfig& c);
  // The max receive window for a whole session.
  // unit is bytes, default is 15 MB, max is 24 MB
  // The window size of session must be larger than
  // a single stream's size. This size affects all
  // the streams within this session.
  std::uint32_t nSessionMaxRecvWindowSize;
  // The max receive window for a single stream
  // unit is bytes, default is 6 MB, max is 16 MB
  std::uint32_t nStreamMaxRecvWindowSize;
  // Set race type for backup.
  RaceType race_type;
  // Whether employ a sync-mechanism to prevent the QuicChromiumPacketReader
  // from reading too fast. If this mode is on, the data consuming speed
  // will be well controlled by the speed you process OnDataRecv().
  // So you can lower the data receiving speed by blocking this method.
  // But becareful if you do so, the next TnetQuicRequest you start will be
  // effected, because there is only one global event thread.
  // The best practice for now is to unblock all the old TnetRequestDelegates
  // before you start a new one. If there is a much stronger demand for massive
  // concurrent requests, we will improve this in future.
  bool sync_read;
  // If it is used to upload large file, it is recommeded to set it true
  bool upload_optimize_;
  // The congestion control algorithm, it's cubic_bytes defaultly.
  CongestionType congestion_type_;

  // If use plaintext to send data.
  bool is_plaintext_;
  // If use custom protocol to send data.
  bool is_custom_;
  // Represent total timeout set by upper layer.
  int total_timeout_millisec_;
  // Represent max connect time, if triggerd, OnConnectionClose would be called.
  int connect_timeout_millisec_;
  // Represent max packet idle time, if two packet interval time exceed it,
  // OnConnectionClose would be called.
  // 4000 milliseconds defaultly before handshake complete.
  // 600 seconds defaultly after handshake.
  int idle_timeout_millisec_;
  // Specify quic version, only support quic 39-46, it is 43 defaultly.
  int quic_version_;

  // default is false.
  bool use_session_reuse_;
  
  // default is false.
  bool force_zero_rtt_;

  bool debug_use_1rtt_;
};

class TNET_EXPORT TnetRequestDelegate {
 public:
  TnetRequestDelegate() {}
  virtual ~TnetRequestDelegate() {}
  // Called when the handshake with server compeleted.
  virtual void OnConnect(int error_code) = 0;
  // Called when data is available.
  virtual void OnDataRecv(const char* buf,
                          const int buf_len) = 0;
  // Called if this connection be closed.
  virtual void OnConnectionClose(int error_code,
                                 const char* error_detail) = 0;

  // Called if last request is finished, is_done refers that if the
  // request has received all data.
  // stream_error = 0 means no error, other code refer to
  // quic_error_codes.h<QuicRstStreamErrorCode>
  virtual void OnRequestFinish(int stream_error) {}
}; // class TnetRequestDelegate

class TNET_EXPORT TnetQuicRequest {
 public:
  // user_delegate is the callback for invoker to get noticed for conn & data events
  // 
  TnetQuicRequest(TnetRequestDelegate* user_delegate, TnetConfig config);
  ~TnetQuicRequest();

  void SetDelegate(TnetRequestDelegate* user_delegate);

  // Connect server by IP and Port asynchronously.
  // Make sure to set domain(host) in the host
  // which needed by crypto handshake.
  void Connect (const char* host,
                const char* ip,
                const int port,
  // tcp port is used in tcp backup job which avoid the
  // quic connect failed.
  // tcp only support HTTP/1.1 now, so the tcp_port = 80 usually.
                const int tcp_port);
  // only for quic.
  void ConnectWithDomain(const char* host,
                         const int port);
  
  // only for quic, is fin directly
  void ConnectAndSend(const char* host,
                      const int port,
                      const char* body,
                      const int body_len,
                      bool fin);
  
  /* Add the headers you want in header_map with key-value format.
     Some special headers need to be set with http/2 format.
     For example, fill the special headers:
     AddHeaders(":path", "/")
     AddHeaders(":method", "GET") or "POST"
     Other headers can set as usual format.
  */
  void AddHeaders(const char* key,
                  const char* value);
  // Return true if the handshake with server is compeleted.
  bool IsConnected();

  
  // Sends an HTTP request and does not wait for response before returning.
  void SendRequest(const char* body, const int body_len, bool fin);

  void SendQuicMessage(const char* body, const int body_len);

  // Cancel the running request.
  void CancelRequest();

  // Close the running quic connection.
  void CloseConnection();

  // Get statistics for the last request.
  TnetStats GetTnetStates();

 private:
  TnetQuicRequest();
  stgw::TnetRequestFront* front_;
};  // class TnetQuicRequest

#ifdef __cplusplus
extern "C" {
#endif

// Print log to stdout defaultly, use this to write in file. 
// if delete_old_file = true, delete the file if it is exist
// and create a new file.
// if delete_old_file = false, log would be append to exist file,
// if file not exist, then create a new file.
#if defined(WIN32)
TNET_EXPORT void SetTquicLog(const wchar_t* filepath, bool delete_old_file);
#else
TNET_EXPORT void SetTquicLog(const char* filepath, bool delete_old_file);
#endif

// Get tquic-sdk version, should be 1.3.8.1
TNET_EXPORT const char* GetTquicVersion();

// only for quic
TNET_EXPORT void TquicPreconnect(const char* domain,
                                 const char* default_ip,
                                 const int port);

#ifdef __cplusplus
}
#endif

#endif  // #ifndef TENCENT_TNET_QUIC_REQUEST_H
