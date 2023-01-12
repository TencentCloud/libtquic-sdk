//
//  TQuicDataTask.h
//

#import <Foundation/Foundation.h>

@class TQUICConfig;
@class TQUICRequest;
@class TQUICResponse;
@class TQUICStats;

NS_ASSUME_NONNULL_BEGIN

typedef void (^TQUICReqDidConnectHandler)(BOOL sucessed);

typedef void (^TQUICReqDidReceiveResponseHandler)(TQUICResponse *response);

typedef void (^TQUICReqDidSendBodyDataHandler)(int64_t bytesSent, int64_t totolSentBytes, int64_t totalBytesExpectedToSend);

typedef void (^TQUICReqDidFinishHandler)(NSData *data, TQUICStats *stats, NSError *error);

typedef void (^TQUICReqDidCloseHandler)(NSError *error);

@interface TQUICService : NSObject

+ (TQUICService *)singleInstance;

@property (nonatomic, copy) TQUICReqDidConnectHandler didConnectHandler;

@property (nonatomic, copy) TQUICReqDidCloseHandler didCloseHandler;

@property (nonatomic, copy) TQUICReqDidReceiveResponseHandler didReceiveResponse;

@property (nonatomic, copy) TQUICReqDidSendBodyDataHandler didSendBodyData;

+ (void)cancel;

+ (void)startRequest:(TQUICRequest *)quicRequest
                           completion:(TQUICReqDidFinishHandler)completion;

@end

NS_ASSUME_NONNULL_END
