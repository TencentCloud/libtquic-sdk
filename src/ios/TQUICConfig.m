//
//  TQUICConfig.m

#import "TQUICConfig.h"

@interface TQUICConfig()

@property (nonatomic, assign) NSUInteger nSessionMaxRecvWindowSize;

@property (nonatomic, assign) NSUInteger nStreamMaxRecvWindowSize;

@property (nonatomic, assign) TQUICRaceType raceType;

@property (nonatomic, assign) BOOL syncRead;

@property (nonatomic, assign) BOOL  uploadOptimize;

@property (nonatomic, assign) TQUICCongestionType congestionType;

@property (nonatomic, assign) BOOL isPlaintext;

@property (nonatomic, assign) BOOL isCustom;

@property (nonatomic, assign) CFTimeInterval totalTimeoutMS;

@property (nonatomic, assign) CFTimeInterval connectTimeoutMS;

@property (nonatomic, assign) CFTimeInterval idleTimeoutMS;

@property (nonatomic, assign) CFTimeInterval quicVersion;

@property (nonatomic, assign) BOOL useSessionReuse;

@property (nonatomic, assign) BOOL debugUse1rtt;

@property (nonatomic, assign) BOOL force0rtt;

@end

@implementation TQUICConfig

+ (instancetype)splashConfig {
    TQUICConfig *config = TQUICConfig.new;
    [config addTotalTimeoutMS:1000];
    [config addUploadOptimize:YES];
    [config addCongestionType:TQUICCongestionTypeBBR];
    [config addRaceType:TQUICRaceTypeOnlyQUIC];
    [config addQuicVersion:43];
    [config addForce0rtt:YES];
    [config addConnectTimeoutMS:1000];
    [config addIdleTimeoutMS:10000];
    return config;
}

- (void)addNSessionMaxRecvWindowSize:(NSUInteger)nSessionMaxRecvWindowSize {
    self.nSessionMaxRecvWindowSize = nSessionMaxRecvWindowSize;
}

- (void)addNStreamMaxRecvWindowSize:(NSUInteger)nStreamMaxRecvWindowSize {
    self.nStreamMaxRecvWindowSize = nStreamMaxRecvWindowSize;
}

- (void)addRaceType:(TQUICRaceType)raceType {
    self.raceType = raceType;
}

- (void)addSyncRead:(BOOL)syncRead {
    self.syncRead = syncRead;
}

- (void)addUploadOptimize:(BOOL)uploadOptimize {
    self.uploadOptimize = uploadOptimize;
}


- (void)addCongestionType:(TQUICCongestionType)congestionType {
    self.congestionType = congestionType;
}

- (void)addIsPlaintext:(BOOL)isPlaintext {
    self.isPlaintext = isPlaintext;
}

- (void)addIsCustom:(BOOL)isCustom {
    self.isCustom = isCustom;
}

- (void)addTotalTimeoutMS:(CFTimeInterval)totalTimeoutMS {
    self.totalTimeoutMS = totalTimeoutMS;
}

- (void)addConnectTimeoutMS:(CFTimeInterval)connectTimeoutMS {
    self.connectTimeoutMS = connectTimeoutMS;
}

- (void)addIdleTimeoutMS:(CFTimeInterval)idleTimeoutMS {
    self.idleTimeoutMS = idleTimeoutMS;
}

- (void)addQuicVersion:(NSInteger)quicVersion {
    self.quicVersion = quicVersion;
}

- (void)addUseSessionReuse:(BOOL)useSessionReuse {
    self.useSessionReuse = useSessionReuse;
}

- (void)addDebugUse1rtt:(BOOL)debugUse1rtt {
    self.debugUse1rtt = debugUse1rtt;
}

- (void)addForce0rtt:(BOOL)force0rtt {
    self.force0rtt = force0rtt;
}

@end
