//
//  TQuicResponse.h

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface TQUICResponse : NSObject

@property (readonly) NSInteger statusCode;

@property (readonly, copy) NSDictionary *allHeaderFields;

@property (readonly, copy) NSString *httpVersion;

+ (instancetype)responseWithStr:(NSString *)str;

- (nullable instancetype)initWithStatusCode:(NSInteger)statusCode
                                HTTPVersion:(nullable NSString *)HTTPVersion
                               headerFields:(nullable NSDictionary<NSString *, NSString *> *)headerFields;
@end

NS_ASSUME_NONNULL_END
