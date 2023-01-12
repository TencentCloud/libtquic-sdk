//
//  TQuicRequest.h

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface TQUICRequest : NSObject

@property (nonatomic, strong, readonly) NSDictionary *allHeaderFields;

@property (nonatomic, copy, readonly) NSString *ip;

@property (nonatomic, copy, readonly) NSString *host;

@property (nonatomic, copy, readonly) NSString *httpMethod;

@property (nonatomic, strong, readonly) NSData *body;

@property (nonatomic, assign, readonly) NSInteger port;

@property (nonatomic, assign, readonly) NSInteger tcpPort;

- (instancetype)init __attribute__((unavailable("call other method instead")));

+ (instancetype)new __attribute__((unavailable("call other method instead")));

+ (instancetype)requestWithBlock:(void (^)(TQUICRequest *request))block;

- (void)addUrl:(NSString *)urlStr;

/// Default: POST
- (void)addHttpMethod:(NSString *)httpMethod;

- (void)addIP:(NSString *)ip;

- (void)addBody:(NSData *)body;

/// Default: 443
- (void)addPort:(NSInteger)port;

- (void)addHeaderFileds:(NSDictionary *)headerFileds;

@end

NS_ASSUME_NONNULL_END
