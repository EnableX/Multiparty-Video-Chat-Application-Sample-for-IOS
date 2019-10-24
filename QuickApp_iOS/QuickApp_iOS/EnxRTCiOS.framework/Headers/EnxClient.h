@import WebRTC;
#import <Foundation/Foundation.h>
#import "EnxLogger.h"
#import "EnxSignalingChannel.h"
#import "EnxClientDelegate.h"
#import "EnxClientState.h"
#import "EnxRoom.h"

typedef RTCSessionDescription * (^SDPHackCallback)(RTCSessionDescription *description);

static SDPHackCallback sdpHackCallback;
static NSString *preferredVideoCodec;


/**
 Returns *EnxClientState* stringified.
 
 @param state EnxClientState.
 
 @return NSString*
 */
extern NSString* clientStateToString(EnxClientState state);

@class EnxClient;
@class EnxRoom;

///-----------------------------------
/// @name EnxClient Interface
///-----------------------------------

@interface EnxClient : NSObject <EnxSignalingChannelDelegate>

///-----------------------------------
/// @name Properties
///-----------------------------------
@property(weak,nonatomic)EnxRoom *enxRoom;
/// EnxClientDelegate instance.
@property (weak, nonatomic) id<EnxClientDelegate> delegate;
/// Local Stream assigned to this client.
//@property (weak, nonatomic) RTCMediaStream *localStream;
/// Max bitrate allowed for this client to use.
@property NSNumber *maxBitrate;
/// Should bitrate be limited to `maxBitrate` value?
@property BOOL limitBitrate;
/// Peer socket id assigned by Enx for signaling P2P connections.
@property  NSString *peerSocketId;
/// The streamId
@property NSString *streamId;
@property (nonatomic,readonly) int maxNumberOfLayers;

///-----------------------------------
/// @name Initializers
///-----------------------------------

//- (instancetype)initWithDelegate:(id<EnxClientDelegate>)delegate;
- (instancetype)initWithDelegate:(id<EnxClientDelegate>)delegate withEnxRoom:(EnxRoom *)room withMaxVideoLayers:(int)layer isAudioOnly:(BOOL)isAudioOnly;

///-----------------------------------
/// @name Instance Methods
///-----------------------------------

- (void)disconnect;

///-----------------------------------
/// @name Class Methods
///-----------------------------------

+ (void)setPreferredVideoCodec:(NSString *)codec;
+ (NSString *)getPreferredVideoCodec;
+ (void)hackSDPWithBlock:(SDPHackCallback)callback;
-(void)resetSDPonClient:(NSString *)mediaConfigurationCodec bandWidth:(NSString *)bandWidth;
@end
