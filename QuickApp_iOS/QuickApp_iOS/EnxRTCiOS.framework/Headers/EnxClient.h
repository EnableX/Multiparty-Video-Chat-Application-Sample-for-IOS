@import WebRTC;
#import <Foundation/Foundation.h>
#import "EnxLogger.h"
#import "EnxSignalingChannel.h"
#import "EnxClientDelegate.h"
#import "EnxClientState.h"


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

///-----------------------------------
/// @name EnxClient Interface
///-----------------------------------

@interface EnxClient : NSObject <EnxSignalingChannelDelegate>

///-----------------------------------
/// @name Properties
///-----------------------------------

/// EnxClientDelegate instance.
@property (weak, nonatomic) id<EnxClientDelegate> delegate;
/// Local Stream assigned to this client.
@property (strong, nonatomic) RTCMediaStream *localStream;
/// Max bitrate allowed for this client to use.
@property NSNumber *maxBitrate;
/// Should bitrate be limited to `maxBitrate` value?
@property BOOL limitBitrate;
/// Peer socket id assigned by Enx for signaling P2P connections.
@property  NSString *peerSocketId;
/// The streamId
@property NSString *streamId;

///-----------------------------------
/// @name Initializers
///-----------------------------------

- (instancetype)initWithDelegate:(id<EnxClientDelegate>)delegate;
- (instancetype)initWithDelegate:(id<EnxClientDelegate>)delegate
                  andPeerFactory:(RTCPeerConnectionFactory *)peerFactory;
- (instancetype)initWithDelegate:(id<EnxClientDelegate>)delegate
                     peerFactory:(RTCPeerConnectionFactory *)peerFactory
                    peerSocketId:(NSString *)peerSocketId;
- (instancetype)initWithDelegate:(id<EnxClientDelegate>)delegate
                     peerFactory:(RTCPeerConnectionFactory *)peerFactory
                        streamId:(NSString *)streamId
                    peerSocketId:(NSString *)peerSocketId;
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
