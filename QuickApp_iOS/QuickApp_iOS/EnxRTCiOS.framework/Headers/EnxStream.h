@import WebRTC;
#import <Foundation/Foundation.h>
#import "EnxLogger.h"
#import "EnxSignalingChannel.h"
#import "EnxSignalingMessage.h"
#import "EnxPlayerView.h"

@class EnxPlayerView;
@class EnxRoom;
@class EnxStream;

@protocol EnxStreamDelegate <NSObject>
@optional

/**
 Fired when audio self mute/unmute events call on EnxStream object.
 
 @param data NSDictionary gives the result of Audio event.
 */
-(void)didAudioEvents:(NSDictionary *_Nullable)data;

/**
 Fired when video self On/Off events call on EnxStream object.
 
 @param data NSDictionary gives the result of Video event.
 */

-(void)didVideoEvents:(NSDictionary *_Nullable)data;

/**
 Fired when a data stream is received.
 
 @param data stream message received.
 
 */
- (void)didReceiveData:(NSDictionary *_Nullable)data;


/**
 
 Fired when a hard mute video alert moderator received from server.
 
 @param stream Instance of the stream where event happen.
 @param data hardVideoMute info on a stream.
 
 */
- (void)stream:(EnxStream *_Nullable)stream didHardVideoMute:(NSArray *_Nullable)data;
/**
 
 Fired when a hard unmute video alert moderator received from server.
 
 @param stream Instance of the stream where event happen.
 @param data hardVideoUnmute info on a stream.
 
 */
- (void)stream:(EnxStream *_Nullable)stream didHardVideoUnMute:(NSArray *_Nullable)data;
/**
 
 Fired when a hard mute video alert participant received from server.
 
 @param stream Instance of the stream where event happen.
 @param data Receive hard mute video info on a stream.

 */
- (void)stream:(EnxStream *_Nullable)stream didReceivehardMuteVideo:(NSArray *_Nullable)data;
/**
 
 Fired when a hard unmute video alert participant received from server.
 
 @param stream Instance of the stream where event happen.
 @param data Receive hard unmute video info on a stream.

 */
- (void)stream:(EnxStream *_Nullable)stream didRecivehardUnmuteVideo:(NSArray *_Nullable)data;


/**
 
 Fired when a self mute video alert participant received from server.
 
 @param stream Instance of the stream where event happen.
 @param data self mute video info on a stream.
 
 Paricipant delegate

 */
- (void)stream:(EnxStream *_Nullable)stream didSelfMuteVideo:(NSArray *_Nullable)data;
/**
 Fired when a self unmute video alert participant received from server.
 
 @param stream Instance of the stream where event happen.
 @param data self unmute video info on a stream.
 
 Paricipant delegate

 }
 */
- (void)stream:(EnxStream *_Nullable)stream didSelfUnmuteVideo:(NSArray *_Nullable)data;


/**
 Fired when a self mute audio alert participant received from server.
 
 @param stream Instance of the stream where event happen.
 @param data self mute audio info on a stream.
 
 Paricipant delegate

 */
- (void)stream:(EnxStream *_Nullable)stream didSelfMuteAudio:(NSArray *_Nullable)data;
/**
 Fired when a self unmute audio alert participant received from server.
 
 @param stream Instance of the stream where event happen.
 
 @param data self unmute audio info on a stream.
 
 Paricipant delegate

 */
- (void)stream:(EnxStream *_Nullable)stream didSelfUnmuteAudio:(NSArray *_Nullable)data;


/**
 
 There would be listener for moderator when hardmute used by moderator. For this delegates are:
 
 Moderator Delegates
 */
- (void)didhardMuteAudio:(NSArray *_Nullable)Data;

/**
 
 There would be listener for moderator when hardmute used by moderator. For this delegates are:
 
 Moderator Delegates
 
 */
- (void)didhardUnMuteAudio:(NSArray *_Nullable)Data;

/**
 Paricipant Delegates
 */

- (void)didRecievedHardMutedAudio:(NSArray *_Nullable)Data;
/**
 Paricipant Delegates
 */

- (void)didRecievedHardUnmutedAudio:(NSArray *_Nullable)Data;




@end

/**
 @interface EnxStream
 
 Represents a wrapper around an audio/video RTC stream that can be used to
 access local media and publish it in a EnxRoom, or receive video from.
 */
@interface EnxStream: NSObject

///-----------------------------------
/// @name Initializers
///-----------------------------------

/**
 Creates an instace of EnxStream capturing audio/video from the host device
 with given Audio and Video contraints.
 
 Notice that the constraints passed to this initializer will also be set as default
 constraint properties for defaultAudioConstraints and defaultVideoConstraints.
 
 @param videoConstraints RTCMediaConstraints that apply to this stream.
 @param audioConstraints RTCMediaConstraints that apply to this stream.
 
 @see initLocalStream:
 @see initLocalStreamWithOptions:attributes:videoConstraints:audioConstraints:
 
 @return instancetype
 */
- (instancetype _Nonnull)initLocalStreamVideoConstraints:(nullable RTCMediaConstraints *)videoConstraints
                                        audioConstraints:(nullable RTCMediaConstraints *)audioConstraints __attribute__((deprecated("This API is depricated.Use getLocalStream: in EnxRoom")));

/**
 Creates an instace of EnxStream capturing audio/video from the host device
 providing options, attributes and Audio and Video contraints.
 
 Notice that the constraints passed to this initializer will also be set as default
 constraint properties for defaultAudioConstraints and defaultVideoConstraints.
 
 @param options dictionary. @see kStreamOption for options keys.
 @param attributes dictionary. @see setAttributes.
 @param videoConstraints RTCMediaConstraints that apply to this stream.
 @param audioConstraints RTCMediaConstraints that apply to this stream.
 
 @see initLocalStream:
 @see initLocalStreamVideoConstraints:audioConstraints:
 
 @return instancetype
 */
- (instancetype _Nonnull)initLocalStreamWithOptions:(nullable NSDictionary *)options
                                         attributes:(nullable NSDictionary *)attributes
                                   videoConstraints:(nullable RTCMediaConstraints *)videoConstraints
                                   audioConstraints:(nullable RTCMediaConstraints *)audioConstraints __attribute__((deprecated("This API is depricated.Use getLocalStream: in EnxRoom")));
/**
 Creates an instace of EnxStream capturing audio/video from the host device
 providing options, attributes.
 
 @param options dictionary. @see kStreamOption for options keys.
 @param attributes dictionary. @see setAttributes.
 
 @see initLocalStream:
 @see initLocalStreamVideoConstraints:audioConstraints:
 @see initLocalStreamWithOptions:attributes:videoConstraints:audioConstraints:
 
 @return instancetype
 */
- (instancetype _Nonnull)initLocalStreamWithOptions:(nullable NSDictionary *)options
                                         attributes:(nullable NSDictionary *)attributes
                                   signalingChannel:(EnxSignalingChannel *_Nonnull)signalingChannel __attribute__((deprecated("This API is depricated.Use getLocalStream: in EnxRoom")));

/**
 Creates an instance of EnxStream capturing audio/video data
 from host device with defaultVideoConstraints and defaultAudioConstraints.
 
 @see initWithLocalStreamVideoConstraints:audioConstraints:
 @see initLocalStreamWithOptions:attributes:videoConstraints:audioConstraints:
 
 @return instancetype
 */
- (instancetype _Nonnull)initLocalStream;

/**
 Creates an instance of EnxStream with a given stream id and signaling channel.
 
 @param streamId Enx stream id for this stream object.
 @param attributes Stream attributes. Attributes will not be sent to the server.
 @param signalingChannel Signaling channel used by EnxRoom that handles the stream.
 
 @return instancetype
 */
- (instancetype _Nonnull)initWithStreamId:(nonnull NSString *)streamId
                               attributes:(nullable NSDictionary *)attributes
                         signalingChannel:(nonnull EnxSignalingChannel *)signalingChannel;

/**
 Attempt to switch between FRONT/REAR camera for the local stream
 being capturated.
 
 @returns NSException.
 */
- (NSException *_Nullable)switchCamera;

/**
 Indicates if the media stream has audio tracks.
 
 If you want to know if the stream was initializated requesting
 audio look into streamOptions dictionary.
 
 @returns Boolean value.
 */
- (BOOL)hasAudio;

/**
 Indicates if the media stream has video tracks.
 
 If you want to know if the stream was initializated requesting
 video look into streamOptions dictionary.
 
 @returns Boolean value.
 */
- (BOOL)hasVideo;

/**
 Indicates if the stream has data activated.
 
 @returns Boolean value.
 */
- (BOOL)hasData;


- (void)hardMuteVideo:(NSString *_Nonnull)clientId;


- (void)hardUnMuteVideo:(NSString *_Nonnull)clientId;

/**
 mute/unmute stream audio.
 
 @param isMuted BOOL to send true to mute or false to unmute.
 @see EnxStreamDelegate:didAudioEvents:data:
 */
- (void)muteSelfAudio:(BOOL)isMuted;

/**
 Unmute Audio tracks for this stream.
 */
//- (NSException *)unmute;

/**
 mute/unmute stream video when user cross over senssor .
 
 @param flag BOOL to send true to mute or false to unmute.
 @see EnxStreamDelegate:didVideoEvents:data:
 */
-(void)muteUnMuteVideowithSenssorUpdate:(BOOL)flag;

/**
 mute/unmute stream video.
 
 @param isMuted BOOL to send true to mute or false to unmute.
 @see EnxStreamDelegate:didVideoEvents:data:
 */
- (void)muteSelfVideo:(BOOL)isMuted;

/**
// stopvideo */
//- (NSException *)stopvideo:(EnxStream *_Nullable)stream ;
//
///**
// startvideo
// */
//- (NSException *)startvideo:(EnxStream *_Nullable)stream ;

/**
 Generates the video tracks for the stream
 */
- (void)generateVideoTracks;

/**
 Generates the audio tracks for the stream
 */
- (void)generateAudioTracks;

/**
 Get attributes of the stream
 */
- (NSDictionary *_Nullable)getAttributes;

/**
 Set attributes of the stream
 
 Notice that this method will replace the whole dictionary.
 
 If the stream doesn't belong to a connected room, the attributes
 will be marked as dirty and they will be sent to the server
 once the stream gets a functional signaling channel.
 
 If the stream is a remote stream it will not submit attributes.
 */
- (void)setAttributes:(NSDictionary *_Nonnull)attributes;

/**
 Send data stream on channel
 
 data Dictionary.
 */
- (NSException *_Nullable)sendData:(NSDictionary *_Nonnull)data;
//
-(void)getReceivedData:(NSDictionary *_Nonnull)data;
//

////Hard video mute
-(void)getHardVideoMute:(NSArray *_Nonnull)data;
//
-(void)getHardVideoUnMute:(NSArray *_Nonnull)data;
//
////P Lisner
-(void)getRecivehardMuteVideo:(NSArray *_Nonnull)data;
//
-(void)getRecivehardUnmuteVideo:(NSArray *_Nonnull)data;

//self video mute/unmute
-(void)getselfVideoMute:(NSArray *_Nonnull)data;
//
-(void)getselfVideoUnmute:(NSArray *_Nonnull)data;

//self audio mute/unmute
-(void)getselfAudioMute:(NSArray *_Nonnull)data;
//
-(void)getselfAudioUnmute:(NSArray *_Nonnull)data;


//Single user mute/unmute

//M
-(void)gethardMuteAudio:(NSArray *_Nonnull)data;

-(void)gethardUnMuteAudio:(NSArray *_Nonnull)data;

//P
-(void)getRecievedHardMutedAudio:(NSArray *_Nonnull)data;

-(void)getRecievedHardUnMutedAudio:(NSArray *_Nonnull)data;

///--------------------- _Nullable--------------
/// @name Properties
///-----------------------------------

/// EnxStreamDelegate were this stream will invoke methods as events.
@property (weak, nonatomic) id <EnxStreamDelegate> _Nullable delegate;


/// RTCMediaStream object that represent the stream a/v data.
@property RTCMediaStream * _Nullable mediaStream;

/// Enx stream id.
@property  NSString * _Nullable streamId;

/// Enx stream attributes for the stream being pubished.
@property (strong, nonatomic) NSDictionary * _Nullable streamAttributes;

/// Indicates attributes hasn't been sent to Enx yet.
@property (readonly) BOOL dirtyAttributes;

/// Enx Lecol stream options.
@property (strong, nonatomic) NSDictionary * _Nullable streamOptions;

/// Enx Remote stream options.
@property (strong, nonatomic) NSDictionary * _Nullable remoteStreamOptions;

/// Factory instance used to access local media.
//@property (strong, nonatomic) RTCPeerConnectionFactory * _Nullable peerFactory;

/// EnxSignalingChannel instance assigned by EnxRoom at the moment
//@property (weak) EnxSignalingChannel * _Nullable signalingChannel;

/// EnxRoom instance at the moment
@property (weak, nonatomic) EnxRoom * _Nullable room;

/// to indicate about localStream object or not.
@property (readonly) BOOL isLocal;

/// Screen share check
@property (assign) BOOL screen;
//
@property (nonatomic) BOOL isSelfMuted;
//
@property (nonatomic) BOOL isSelfVideoMuted;

@property (weak,nonatomic) NSString * _Nullable mediaType;


/// Default video contraints.
@property (readonly,weak) RTCMediaConstraints * _Nullable defaultVideoConstraints;

/// Default audio contraints.
@property (readonly,weak) RTCMediaConstraints * _Nullable defaultAudioConstraints;

@property (nonatomic) RTCCameraVideoCapturer * _Nullable capturer;

@property(weak,readonly,nonatomic) EnxPlayerView * _Nullable enxPlayerView;
@property(strong,nonatomic) NSString * _Nullable clientID;

//@property (strong,nonatomic) NSDictionary * _Nullable receivedDataDictionary;
//
- (void)SelfHardMuteAudio:(BOOL)isMuted;

-(void)stopCapture;
-(void)startCapture;
/**
 mute single audio stream.
 
 Note: Hardmute functionality is only applicable to moderator.
 
 @param clientId is the participant who is being mute to the floor.
 
 @see EnxRoomDelegate:didMutedSingleUser:::data:
 
 */

- (void)hardMuteAudio:(NSString*_Nonnull)clientId;

/**
 unmute single audio stream.
 
 Note: Hardmute functionality is only applicable to moderator.
 
 @param clientId is the participant who is being mute to the floor.
 
 @see EnxRoomDelegate:didUnMutedSingleUser:::data:
 
 */

- (void)hardUnMuteAudio:(NSString*_Nonnull)clientId;

- (void)attachRenderer :(EnxPlayerView*_Nonnull)ObjEnxPlayerView;

- (void)detachRenderer;

-(BOOL)getUserMedia;

@end

