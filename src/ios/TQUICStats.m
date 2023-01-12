//
//  QADSplashQUICStats.m
//

#import "TQUICStats.h"
#import <objc/runtime.h>
#import <objc/message.h>

@implementation TQUICStats

- (NSString *)description {
    NSMutableString *desc = NSMutableString.string;
    [desc appendFormat:@"isValid=%d\n", self.isValid];
    [desc appendFormat:@"isQuic=%d\n", self.isQuic];
    [desc appendFormat:@"is0rtt=%d\n", self.is0rtt];
    [desc appendFormat:@"isConnReuse=%d\n", self.isConnReuse];
    [desc appendFormat:@"connectMS=%zdms\n", self.connectMS];
    [desc appendFormat:@"ttfbMS=%zdms\n", self.ttfbMS];
    [desc appendFormat:@"completeMS=%zdms\n", self.completeMS];
    [desc appendFormat:@"srttUS=%zdus\n", self.srttUS];
    [desc appendFormat:@"packetsSent=%zd\n", self.packetsSent];
    [desc appendFormat:@"packetsRetransmitted=%zd\n", self.packetsRetransmitted];
    [desc appendFormat:@"bytesSent=%zd\n", self.bytesSent];
    [desc appendFormat:@"bytesRetransmitted=%zd\n", self.bytesRetransmitted];
    [desc appendFormat:@"packetsLost=%zd\n", self.packetsLost];
    [desc appendFormat:@"packetsReceived=%zd\n", self.packetsReceived];
    [desc appendFormat:@"bytesReceived=%zd\n", self.bytesReceived];
    [desc appendFormat:@"streamBytesReceived=%zd\n", self.streamBytesReceived];
    [desc appendFormat:@"errorCode=%zd\n", self.errorCode];
    return desc.copy;
}

- (NSDictionary *)reportInfo {
    NSMutableDictionary *dict = NSMutableDictionary.dictionary;
    dict[@"isValid"] = @(self.isValid);
    dict[@"isQuic"] = @(self.isQuic);
    dict[@"is0rtt"] = @(self.is0rtt);
    dict[@"isConnReuse"] = @(self.isConnReuse);
    dict[@"connectDuration"] = @(self.connectMS);
    dict[@"waitDuration"] = @(self.ttfbMS);
    dict[@"totalDuration"] = @(self.completeMS);
    dict[@"srttUS"] = @(self.srttUS);
    dict[@"packetsSent"] = @(self.packetsSent);
    dict[@"packetsRetransmitted"] = @(self.packetsRetransmitted);
    dict[@"bytesSent"] = @(self.bytesSent);
    dict[@"bytesRetransmitted"] = @(self.bytesRetransmitted);
    dict[@"packetsLost"] = @(self.packetsLost);
    dict[@"packetsReceived"] = @(self.packetsReceived);
    dict[@"bytesReceived"] = @(self.bytesReceived);
    dict[@"streamBytesReceived"] = @(self.streamBytesReceived);
    dict[@"protocolName"] = @"quic";
    dict[@"errorCode"] = @(self.errorCode);
    dict[@"errorMsg"] = self.errorMsg;
    dict[@"vip"] = self.ip;
    dict[@"connId"] = self.connId;
    dict[@"dnsDuration"] = @(self.dnsMS);
    dict[@"dnsCode"] = @(self.dnsCode);
    dict[@"isDefaultIP"] = @(self.isDefaultIP);
    dict[@"cancelDuration"] = @(self.cancelDurationMS);
    dict[@"shloDuration"] = @(self.shloDurationMS);
    return dict.copy;
}

@end
