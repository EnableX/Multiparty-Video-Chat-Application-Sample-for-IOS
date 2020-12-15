@import WebRTC;
#import <Foundation/Foundation.h>
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
@property (nonatomic,readonly) int maxbandWidth;
@property (nonatomic,strong) NSString *peerId;

///-----------------------------------
/// @name Initializers
///-----------------------------------

-(void)getPara;

//- (instancetype)initWithDelegate:(id<EnxClientDelegate>)delegate;
// for Publisger Stream
- (instancetype)initPublishClientWithDelegate:(id<EnxClientDelegate>)delegate withEnxRoom:(EnxRoom *)room withMaxVideoLayers:(int)layer withMaxBandWidth:(int)bandWidth isAudioOnly:(BOOL)isAudioOnly ;

// for Subscriber Stream
-(instancetype)initSubscriberClientWithDelegate:(id<EnxClientDelegate>)delegate withEnxRoom:(EnxRoom *)room;

// for CanVas Stream
-(instancetype)initCanvasClientWithDelegate:(id<EnxClientDelegate>)delegate withEnxRoom:(EnxRoom *)room;

// for Screen Share Stream
-(instancetype)initScreenShareClientWithDelegate:(id<EnxClientDelegate>)delegate withEnxRoom:(EnxRoom *)room;

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
-(void)setClientBitrate;
@end
