//
//  TQuicResponse.m
//

#import "TQUICResponse.h"

@interface TQUICResponse ()

@property (nonatomic, readwrite) NSInteger statusCode;

@property (readwrite, copy) NSDictionary *allHeaderFields;

@property (readwrite, copy) NSString *httpVersion;

@end

@implementation NSString (filteCharacter)

- (NSString *)filteCharacter {
    NSString *temp = [self stringByReplacingOccurrencesOfString:@" " withString:@""];
    temp = [self stringByReplacingOccurrencesOfString:@"\r" withString:@""];
    temp = [self stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    return temp;
}

@end

@implementation TQUICResponse

- (instancetype)initWithStatusCode:(NSInteger)statusCode
                       HTTPVersion:(NSString *)HTTPVersion
                      headerFields:(NSDictionary<NSString *, NSString *> *)headerFields {
    if (self = [super init]) {
        self.statusCode = statusCode;
        self.allHeaderFields = headerFields;
        self.httpVersion = HTTPVersion;
    }
    return self;
}

+ (instancetype)responseWithStr:(NSString *)str {
    TQUICResponse *response = TQUICResponse.new;
    NSArray *tempArray = [str componentsSeparatedByString:@"\n"];
    NSMutableArray *headers = tempArray.mutableCopy;
    NSArray *baseInfo = [tempArray[0] componentsSeparatedByString:@" "];
    if (baseInfo.count < 3) {
        @throw [NSException exceptionWithName:@"QADSplashQuicResponse" reason:@"服务器响应格式不合法" userInfo:nil];
    }
    str = [str filteCharacter];

    for (int i = 1; i < tempArray.count; i++) {
        headers[i - 1] = tempArray[i];
    }

    NSMutableDictionary *allHedares = NSMutableDictionary.dictionary;
    for (int i = 0; i < headers.count; i++) {
        NSString *header = headers[i];
        header = [header filteCharacter];
        NSArray *headerArr = [header componentsSeparatedByString:@": "];
        if (headerArr.count > 1) {
            NSString *key = headerArr[0];
            NSString *value = headerArr[1];
            value = [value stringByTrimmingCharactersInSet:NSCharacterSet.whitespaceAndNewlineCharacterSet];
            value = [value stringByTrimmingCharactersInSet:NSCharacterSet.whitespaceCharacterSet];
            allHedares[key] = [NSString stringWithFormat:@"%@", value];
        }
    }
    response.allHeaderFields = [allHedares copy];
    response.httpVersion = baseInfo[0];
    response.statusCode = [baseInfo[1] integerValue];
    return response;
}

@end
