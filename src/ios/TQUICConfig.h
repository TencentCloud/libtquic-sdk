//
//  TQUICConfig.h

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, TQUICRaceType) {
    TQUICRaceTypeOnlyQUIC = 0,
    TQUICRaceTypeQUICHTTP,
    TQUICRaceTypeOnlyHTTP
};

typedef NS_ENUM(NSInteger, TQUICCongestionType) {
    TQUICCongestionTypeCubicBytes,
    TQUICCongestionTypeRenoBytes,
    TQUICCongestionTypeBBR,
    TQUICCongestionTypePCC,
    TQUICCongestionTypeGoogCC // kGoogCC is equal to BBR in quic.
};


@interface TQUICConfig : NSObject

/// The max receive window for a whole session.
/// unit is bytes, default is 15 MB, max is 24 MB
/// The window size of session must be larger than
/// a single stream's size. This size affects all
/// the streams within this session.
@property (nonatomic, assign, readonly) NSUInteger nSessionMaxRecvWindowSize;

/// The max receive window for a single stream
/// unit is bytes, default is 6 MB, max is 16 MB
@property (nonatomic, assign, readonly) NSUInteger nStreamMaxRecvWindowSize;

/// Set race type for backup.
@property (nonatomic, assign, readonly) TQUICRaceType raceType;

/// Whether employ a sync-mechanism to prevent the QuicChromiumPacketReader
/// from reading too fast. If this mode is on, the data consuming speed
/// will be well controlled by the speed you process OnDataRecv().
/// So you can lower the data receiving speed by blocking this method.
/// But becareful if you do so, the next TnetQuicRequest you start will be
/// effected, because there is only one global event thread.
/// The best practice for now is to unblock all the old TnetRequestDelegates
/// before you start a new one. If there is a much stronger demand for massive
/// concurrent requests, we will improve this in future.
@property (nonatomic, assign, readonly) BOOL syncRead;

/// If it is used to upload large file, it is recommeded to set it true
@property (nonatomic, assign, readonly) BOOL  uploadOptimize;

/// The congestion control algorithm, it's cubic_bytes defaultly.
@property (nonatomic, assign, readonly) TQUICCongestionType congestionType;

/// If use plaintext to send data.
@property (nonatomic, assign, readonly) BOOL isPlaintext;

/// If use custom protocol to send data.
@property (nonatomic, assign, readonly) BOOL isCustom;

/// Represent total timeout set by upper layer.
@property (nonatomic, assign, readonly) CFTimeInterval totalTimeoutMS;

/// Represent max connect time, if triggerd, OnConnectionClose would be called.
@property (nonatomic, assign, readonly) CFTimeInterval connectTimeoutMS;

/// Represent max packet idle time, if two packet interval time exceed it,
/// OnConnectionClose would be called.
/// 4000 milliseconds defaultly before handshake complete.
/// 600 seconds defaultly after handshake.
@property (nonatomic, assign, readonly) CFTimeInterval idleTimeoutMS;

/// Specify quic version, only support quic 35-46, it is 46 defaultly.
@property (nonatomic, assign, readonly) CFTimeInterval quicVersion;

/// default is NO.
@property (nonatomic, assign, readonly) BOOL useSessionReuse;

@property (nonatomic, assign, readonly) BOOL debugUse1rtt;

/// 强制0rtt
@property (nonatomic, assign, readonly) BOOL force0rtt;


+ (instancetype)splashConfig;

/// The max receive window for a whole session.
/// unit is bytes, default is 15 MB, max is 24 MB
/// The window size of session must be larger than
/// a single stream's size. This size affects all
/// the streams within this session.
- (void)addNSessionMaxRecvWindowSize:(NSUInteger)nSessionMaxRecvWindowSize;

/// The max receive window for a single stream
/// unit is bytes, default is 6 MB, max is 16 MB
- (void)addNStreamMaxRecvWindowSize:(NSUInteger)nStreamMaxRecvWindowSize;

/// Set race type for backup.
- (void)addRaceType:(TQUICRaceType)raceType;

/// Whether employ a sync-mechanism to prevent the QuicChromiumPacketReader
/// from reading too fast. If this mode is on, the data consuming speed
/// will be well controlled by the speed you process OnDataRecv().
/// So you can lower the data receiving speed by blocking this method.
/// But becareful if you do so, the next TnetQuicRequest you start will be
/// effected, because there is only one global event thread.
/// The best practice for now is to unblock all the old TnetRequestDelegates
/// before you start a new one. If there is a much stronger demand for massive
/// concurrent requests, we will improve this in future.
- (void)addSyncRead:(BOOL)syncRead;

/// If it is used to upload large file, it is recommeded to set it true
- (void)addUploadOptimize:(BOOL)uploadOptimize;

/// The congestion control algorithm, it's cubic_bytes defaultly.
- (void)addCongestionType:(TQUICCongestionType)congestionType;

/// If use plaintext to send data.
- (void)addIsPlaintext:(BOOL)isPlaintext;

/// If use custom protocol to send data.
- (void)addIsCustom:(BOOL)isCustom;

/// Represent total timeout set by upper layer.
- (void)addTotalTimeoutMS:(CFTimeInterval)totalTimeoutMS;

/// Represent max connect time, if triggerd, OnConnectionClose would be called.
- (void)addConnectTimeoutMS:(CFTimeInterval)connectTimeoutMS;

/// Represent max packet idle time, if two packet interval time exceed it,
/// OnConnectionClose would be called.
/// 4000 milliseconds defaultly before handshake complete.
/// 600 seconds defaultly after handshake.
- (void)addIdleTimeoutMS:(CFTimeInterval)idleTimeoutMS;

/// Specify quic version, only support quic 35-46, it is 46 defaultly.
- (void)addQuicVersion:(NSInteger)quicVersion;

/// default is NO.
- (void)addUseSessionReuse:(BOOL)useSessionReuse;

- (void)addDebugUse1rtt:(BOOL)debugUse1rtt;

- (void)addForce0rtt:(BOOL)force0rtt;

@end

NS_ASSUME_NONNULL_END
