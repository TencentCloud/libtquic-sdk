//
//  QADSplashQUICStats.h
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface TQUICStats : NSObject

@property (nonatomic, assign) BOOL isValid;        // if this stats is valid.

@property (nonatomic, assign) BOOL isQuic;         // if it is quic, else it is tcp.

@property (nonatomic, assign) BOOL is0rtt;         // Only valid if it is quic.

@property (nonatomic, assign) BOOL isConnReuse;   // if reuse an exist quic connection.

@property (nonatomic, assign) NSInteger connectMS;  // connect cost in millionseconds.

@property (nonatomic, assign) NSInteger ttfbMS;     // first byte cost from send request in millionseconds.

// Only valid if it is quic.
@property (nonatomic, assign) NSInteger completeMS; // all bytes received cost from send request.

@property (nonatomic, assign) NSInteger srttUS;  // Smoothed RTT in microseconds.

@property (nonatomic, assign) NSInteger packetsSent;  // Number of packets be sent.

@property (nonatomic, assign) NSInteger packetsRetransmitted;  // Number of packets be retransmitted.

@property (nonatomic, assign) NSInteger bytesSent;

@property (nonatomic, assign) NSInteger bytesRetransmitted;

@property (nonatomic, assign) NSInteger packetsLost;  // Number of packets be lost when sent data.

@property (nonatomic, assign) NSInteger packetsReceived; // Total packets received

@property (nonatomic, assign) NSInteger bytesReceived;  // Total bytes received including packet format.

@property (nonatomic, assign) NSInteger streamBytesReceived;  // Total bytes received including duplicated data.

@property (nonatomic, assign) NSInteger errorCode; // 错误码

@property (nonatomic, copy) NSString *errorMsg; // 错误码

@property (nonatomic, copy) NSString *ip; // ip

@property (nonatomic, copy) NSString *connId; // quic connection id

@property (nonatomic, assign) NSInteger dnsMS;  // dns time in millionseconds.

@property (nonatomic, assign) NSInteger dnsCode;  // dns error code.

@property (nonatomic, assign) BOOL isDefaultIP;   // if remote ip defaultly.

@property (nonatomic, copy, readonly) NSDictionary *reportInfo;

@property (nonatomic, assign) NSInteger cancelDurationMS;

@property (nonatomic, assign) NSInteger shloDurationMS;

@end

NS_ASSUME_NONNULL_END
