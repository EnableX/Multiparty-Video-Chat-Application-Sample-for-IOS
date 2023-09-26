@import WebRTC;
#import <Foundation/Foundation.h>
#import "RTCSessionDescription+JSON.h"

typedef NS_ENUM(NSInteger, EnxSignalingMessageType) {
    kEnxSignalingMessageTypeCandidate,
    kEnxSignalingMessageTypeOffer,
    kEnxSignalingMessageTypeAnswer,
    kEnxSignalingMessageTypeBye,
    kEnxSignalingMessageTypeReady,
	kEnxSignalingMessageTypeTimeout,
	kEnxSignalingMessageTypeFailed,
	kEnxSignalingMessageTypeStarted,
	kEnxSignalingMessageTypeBandwidthAlert,
	kEnxSignalingMessageTypeDataStream,
    kEnxSignalingMessageTypeInitializing,
	kEnxSignalingMessageTypeUpdateAttribute
};

@interface EnxSignalingMessage : NSObject

- (instancetype)initWithStreamId:(id)streamId peerSocketId:(NSString *)peerSocketId;

@property(nonatomic, readonly) EnxSignalingMessageType type;
@property(readonly) NSString *streamId;
@property(readonly) NSString *peerSocketId;


+ (EnxSignalingMessage *)messageFromDictionary:(NSDictionary *)dictionary;
- (NSData *)JSONData;

@end

@interface EnxICECandidateMessage : EnxSignalingMessage

@property(nonatomic, readonly) RTCIceCandidate *candidate;

- (instancetype)initWithCandidate:(RTCIceCandidate *)candidate
                         streamId:(NSString *)streamId
                     peerSocketId:(NSString *)peerSocketId;

@end

@interface EnxSessionDescriptionMessage : EnxSignalingMessage

@property(nonatomic, readonly) RTCSessionDescription *sessionDescription;

- (instancetype)initWithDescription:(RTCSessionDescription *)description
                        streamId:(NSString *)streamId
                       peerSocketId:(NSString *)peerSocketId;

@end

@interface EnxByeMessage : EnxSignalingMessage
@end

@interface EnxReadyMessage : EnxSignalingMessage
@end

@interface EnxTimeoutMessage : EnxSignalingMessage
@end

@interface EnxFailedMessage : EnxSignalingMessage
@end

@interface EnxInitializingMessage : EnxSignalingMessage

@property NSString *agentId;

- (instancetype)initWithStreamId:(NSString *)streamId agentId:(NSString *)agentId;

@end

@interface EnxStartedMessage : EnxSignalingMessage
@end

@interface EnxBandwidthAlertMessage : EnxSignalingMessage
@end

@interface EnxDataStreamMessage : EnxSignalingMessage

@property(nonatomic, strong) NSDictionary* data;

- (instancetype)initWithStreamId:(id)streamId withData:(NSDictionary*) data;

@end

@interface EnxUpdateAttributeMessage : EnxSignalingMessage

@property(nonatomic, strong) NSDictionary* attribute;

- (instancetype)initWithStreamId:(id)streamId withAttribute:(NSDictionary*) attribute;

@end
