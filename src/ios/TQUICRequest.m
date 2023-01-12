//
//  TQuicRequest.m
//

#import "TQUICRequest.h"
#import <tnet_quic_request.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

@interface TQUICRequest()

@property (nonatomic, strong) NSMutableDictionary *quicAllHeaderFields;

@property (nonatomic, copy) NSString *ip;

@property (nonatomic, copy) NSString *host;

@property (nonatomic, copy) NSString *httpMethod;

@property (nonatomic, assign) NSInteger port;

@property (nonatomic, strong) NSData* body;

@end



@implementation TQUICRequest

+ (instancetype)requestWithBlock:(void (^)(TQUICRequest *request))block {
    TQUICRequest *request = TQUICRequest.new;
    [request addPort:443];
    [request addHttpMethod:@"POST"];
    block(request);
    if (!request.ip) {
        [request addIP:[self ipFromHost:request.host]];
    }
    [request.quicAllHeaderFields removeObjectForKey:@"Host"];
    return request;
}

- (void)addUrl:(NSString *)urlStr {
    
    NSURL *url = [NSURL URLWithString:urlStr];

    NSAssert(url, @"url 有误");

    self.quicAllHeaderFields[@":scheme"] = url.scheme;

    NSString *path = url.path;

    if (url.query) {
        path = [NSString stringWithFormat:@"%@?%@", path, url.query];
    }

    if (path.length != 0 ) {
        if (![[path substringToIndex:1] isEqualToString:@"/"]) {
            path = [NSString stringWithFormat:@"/?%@",url.query];
        }
    } else {
        path = @"/";
    }

    self.quicAllHeaderFields[@":path"] = path;
    
    [self addHost:url.host];
}

- (void)addHost:(NSString *)host {
    self.host = host;
    self.quicAllHeaderFields[@":authority"] = host;
}

- (void)addIP:(NSString *)ip {
    self.ip = ip;
}

- (void)addHttpMethod:(NSString *)httpMethod {
    self.httpMethod = httpMethod;
    self.quicAllHeaderFields[@":method"] = httpMethod;
}

- (void)addBody:(NSData*)body {
    self.body = body;
}

- (void)addPort:(NSInteger)port {
    self.port = port;
}

- (void)addHeaderFileds:(NSDictionary *)headerFileds {
    if (!headerFileds) { return; }
    [self.quicAllHeaderFields addEntriesFromDictionary:headerFileds];
}

- (NSMutableDictionary *)quicAllHeaderFields {
    if (!_quicAllHeaderFields) {
        _quicAllHeaderFields = NSMutableDictionary.dictionary;
    }
    return _quicAllHeaderFields;
}

- (NSDictionary *)allHeaderFields {
    return self.quicAllHeaderFields.copy;
}

@end
