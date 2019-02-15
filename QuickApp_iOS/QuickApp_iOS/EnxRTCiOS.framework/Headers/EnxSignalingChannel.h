@import WebRTC;

#import "EnxSignalingMessage.h"
#import "EnxSignalingEvent.h"

#import "EnxClientDelegate.h"

@class EnxSignalingChannel;

///-----------------------------------
/// @protocol EnxSignalingChannelDelegate
///-----------------------------------

@protocol EnxSignalingChannelDelegate <NSObject>

@property NSString *streamId;
@property NSString *peerSocketId;

/**
 Event fired when Enx server has validated our token.
 
 @param signalingChannel EnxSignalingChannel the channel that emit the message.
 */
- (void)signalingChannelDidOpenChannel:(EnxSignalingChannel *)signalingChannel;

/**
 Event fired each time EnxSignalingChannel has received a new EnxSignalingMessage.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param message EnxSignalingMessage received by channel.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didReceiveMessage:(EnxSignalingMessage *)message mediaConfigurationCodec:(NSString *)mediaConfigurationCodec mediaCodecsInfoInSDP:(NSDictionary *)mediaCodecsInfoInSDP;

/**
 Event fired when Enx is ready to receive a publishing stream.

 @param signalingChannel EnxSignalingChannel the channel that emit the message.
 @param peerSocketId Id of the socket in a p2p publishing without MCU. Pass nil if
        you are not setting a P2P room.
 */
- (void)signalingChannel:(EnxSignalingChannel *)signalingChannel
  readyToPublishStreamId:(NSString *)streamId
            peerSocketId:(NSString *)peerSocketId;

/**
 Event fired when Enx failed to publishing stream.
 
 @param signalingChannel EnxSignalingChannel the channel that emit the message.
 */
- (void)signalingChannelPublishFailed:(EnxSignalingChannel *)signalingChannel;

/**
 Event fired each time EnxSignalingChannel has received a confirmation from the server
 to subscribe a stream.
 This event is fired to let Client know that it can start signaling to subscribe the stream.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId Id of the stream that will be subscribed.
 @param peerSocketId pass nil if is MCU being used.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel
readyToSubscribeStreamId:(NSString *)streamId
            peerSocketId:(NSString *)peerSocketId;

@end

///-----------------------------------
/// @protocol EnxSignalingChannelRoomDelegate
///-----------------------------------

@protocol EnxSignalingChannelRoomDelegate <NSObject>

/**
 This event is fired when a token was not successfuly used.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param reason String of error returned by the server.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didError:(NSString *)reason;

- (void)signalingChannel:(EnxSignalingChannel *)channel didEventError:(NSString *)reason type:(NSString *)eventType streamId:(NSString *)streamId;


- (void)signalingChannel:(EnxSignalingChannel *)channel didReconnect:(NSString *)reason;

- (void)signalingChannel:(EnxSignalingChannel *)channel didRoomConnected:(NSArray *)Data;


/**
 Event fired as soon a client connect to a room.

 @param channel EnxSignalingChannel the channel that emit the message.
 @param roomMeta Metadata associated to the room that the client just connect.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didConnectToRoom:(NSDictionary *)roomMeta;

/**
 Event fired as soon as rtc channels were disconnected and websocket
 connection is about to be closed.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param roomMeta Metadata associated to the room that the client just connect.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didDisconnectOfRoom:(NSDictionary *)roomMeta;

/**
 Event fired when a new stream id has been created and server is ready
 to start publishing it.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream that will be published.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didReceiveStreamIdReadyToPublish:(NSString *)streamId;


/*(Event fired when a new stream id has been created and server is ready
to start subscribing it. */
- (void)signalingChannel:(EnxSignalingChannel *)channel didReceiveStreamIdReadyToSubscribe:(NSString *)streamId;


/**
 Event fired when a recording of a stream has started.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream being recorded.
 @param recordingId NSString id of the recording id on Enx server.
 @param recordingDate NSDate when the server start to recording the stream.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didStartRecordingStreamId:(NSString *)streamId
                                                                 withRecordingId:(NSString *)recordingId
                                                                       recordingDate:(NSDate *)recordingDate;
/**
 Event fired when a recording of a stream has failed.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream being recorded.
 @param errorMsg Error string sent from the server.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didFailStartRecordingStreamId:(NSString *)streamId
            withErrorMsg:(NSString *)errorMsg;


//delegate method when recording start successful
-(void)signalingRecordingDidStart:(NSArray *)response;

//when recording fails.
//-(void)signalingRecordingDidFail:(NSString *)response;

//delegate method when recording stop successful
-(void)signalingRecordingDidStop:(NSArray *)response;

//when recording stop fails.
//-(void)signalingRecordingDidStopFail:(NSString *)response;

/**
 Event fired when a new StreamId has been added to a room.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString added to the room.
 @param event Event name and data carried
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel
    didStreamAddedWithId:(NSString *)streamId
                   event:(EnxSignalingEvent *)event;

/**
 Event fired when a StreamId has been removed from a room, not necessary this
 stream has been consumed.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString of the removed stream.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didRemovedStreamId:(NSString *)streamId;

/**
 Event fired when a StreamId previously subscribed has been unsubscribed.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString of the unsubscribed stream.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didUnsubscribeStreamWithId:(NSString *)streamId;

/**
 Event fired when a published stream is being unpublished.

 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString of the stream being unpublished
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didUnpublishStreamWithId:(NSString *)streamId;

/**
 Event fired when some peer request to subscribe to a given stream.

 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString of the unsubscribed stream.
 @param peerSocketId String that identifies the peer connection for the stream.

 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didRequestPublishP2PStreamWithId:(NSString *)streamId
                                                                        peerSocketId:(NSString *)peerSocketId;

/**
 Method called when the signaling channels needs a new client to operate a connection.

 @param channel EnxSignalingChannel the channel that emit the message.

 @returns EnxClientDelegate instance.
 */
- (id<EnxSignalingChannelDelegate>)clientDelegateRequiredForSignalingChannel:(EnxSignalingChannel *)channel;

/**
 Event fired when data stream received.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream received from.
 @param dataStream NSDictionary having message and timestamp.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel
            fromStreamId:(NSString *)streamId
	  receivedDataStream:(NSDictionary *)dataStream;

/**
 Event fired when data stream received.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream received from.
 @param dataStream NSDictionary having message and timestamp.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel
            fromStreamId:(NSString *)streamId
      didBandWidthUpdated:(NSArray *)dataStream;


- (void)signalingChannel:(EnxSignalingChannel *)channel
        didHardVideoMute:(NSArray *)data;
    
- (void)signalingChannel:(EnxSignalingChannel *)channel
        didHardVideoUnMute:(NSArray *)data;

- (void)signalingChannel:(EnxSignalingChannel *)channel didReciveHardMuteVideo:(NSArray *)dataStream;

- (void)signalingChannel:(EnxSignalingChannel *)channel didReciveHardUnmuteVideo:(NSArray *)dataStream;


//self Video mute/unmute Participant lisner
- (void)signalingChannel:(EnxSignalingChannel *)channel didSelfMuteVideo:(NSArray *)dataStream;

- (void)signalingChannel:(EnxSignalingChannel *)channel didSelfUnmuteVideo:(NSArray *)dataStream;

//self Audio mute/unmute Participant lisner
- (void)signalingChannel:(EnxSignalingChannel *)channel didSelfMuteAudio:(NSArray *)dataStream;

- (void)signalingChannel:(EnxSignalingChannel *)channel didSelfUnmuteAudio:(NSArray *)dataStream;

//recive by Moderator
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponcesSelfMute:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponcesSelfUnMute:(NSArray *)Data;

/**
 Event fired when stream atrribute updated.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream received from.
 @param attributes NSDictionary having custom attribute.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel
            fromStreamId:(NSString *)streamId
   updateStreamAttributes:(NSDictionary *)attributes;

//on Logs  upload
-(void)signalingLogsUpLoaded:(NSArray *)data;
//on Logs Failure upload
//-(void)signalingLogsFailure:(NSString *)message;

#pragma mark - ChairControl
//For Participant
- (void)signalingChannel:(EnxSignalingChannel *)channel didReciveFloorRequest:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didReciveFloorGranted:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didFloorReleased:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didFloorDeny:(NSArray *)Data;
//For Moserator
- (void)signalingChannel:(EnxSignalingChannel *)channel didFloorRequested:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didProcessFloorRequest:(NSArray *)Data;

//H
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponceMuteAllUser:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponceUnMuteAllUser:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponceMuteSingleUser:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponceUnMuteSingleUser:(NSArray *)Data;

//lisner

- (void)signalingChannel:(EnxSignalingChannel *)channel recievedHardMuteAll:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel recievedHardUnmuteAll:(NSArray *)Data;

- (void)signalingChannel:(EnxSignalingChannel *)channel recievedHardMuteUser:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel recievedHardUnMuteUser:(NSArray *)Data;
//
-(void)signalingChannel:(EnxSignalingChannel *)channel userDidJoined:(NSArray *)Data;
-(void)signalingChannel:(EnxSignalingChannel *)channel userDidDisconnected:(NSArray *)Data;

   #pragma mark- recording Participant
-(void)signalingChannel:(EnxSignalingChannel *)channel roomRecordOn:(NSArray *)Data;
-(void)signalingChannel:(EnxSignalingChannel *)channel roomRecordOff:(NSArray *)Data;

//ActiveTalker Delegate
-(void)signalingChannel:(EnxSignalingChannel *)channel activeTalkerList:(NSArray *)Data;
-(void)signalingChannel:(EnxSignalingChannel *)channel didGetMaxTalkersFromSignaling:(NSArray *)Data;
-(void)signalingChannel:(EnxSignalingChannel *)channel didGetTalkerCountFromSignaling:(NSArray *)Data;
-(void)signalingChannel:(EnxSignalingChannel *)channel didSetTalkerCountFromSignaling:(NSArray *)Data;

//Share screen Delegate
-(void)signalingChannel:(EnxSignalingChannel *)channel screenShareStartedSignaling:(NSArray *)Data;
-(void)signalingChannel:(EnxSignalingChannel *)channel screenShareStoppedSignaling:(NSArray *)Data;

-(void)signalingChannel:(EnxSignalingChannel *)channel publishMediaCodecChange:(NSArray *)Data;

-(void)signalingChannel:(EnxSignalingChannel *)channel publisherBandWidthAlert:(NSArray *)Data;

//SimulCast
- (void)signalingChannel:(EnxSignalingChannel *)channel didSignallingVideoQualityUpdated:(NSArray *)data;

//delegate method when to get connectedIp
-(void)signalingGetConnectedIp:(NSString *)connectedIp;

@end

/**
 @interface EnxSignalingChannel
 
 */
@interface EnxSignalingChannel : NSObject

///-----------------------------------
/// @name Initializers
///-----------------------------------

/**
 Creates an instance of EnxSignalingChannel.
 
 @param token The encoded token to access a room.
 @param roomDelegate EnxSignalingChannelRoomDelegate instance that will receive
        events related to stream addition, recording started, etc.
 
 @return instancetype
 */
- (instancetype)initWithEncodedToken:(NSString *)token
                        roomDelegate:(id<EnxSignalingChannelRoomDelegate>)roomDelegate
                      clientDelegate:(id<EnxClientDelegate>)clientDelegate;

///-----------------------------------
/// @name Properties
///-----------------------------------

/// EnxSignalingChannelRoomDelegate reference
@property (weak, nonatomic) id<EnxSignalingChannelRoomDelegate> roomDelegate;


///-----------------------------------
/// @name Public Methods
///-----------------------------------

- (void)connect;
- (void)disconnect;
- (void)enqueueSignalingMessage:(EnxSignalingMessage *)message;
- (void)sendSignalingMessage:(EnxSignalingMessage *)message;
- (void)drainMessageQueueForStreamId:(NSString *)streamId
                        peerSocketId:(NSString *)peerSocketId;
- (void)publish:(NSDictionary *)options
            signalingChannelDelegate:(id<EnxSignalingChannelDelegate>)delegate;
- (void)unpublish:(NSString *)streamId
            signalingChannelDelegate:(id<EnxSignalingChannelDelegate>)delegate;
- (void)publishToPeerID:(NSString *)peerSocketId
            signalingChannelDelegate:(id<EnxSignalingChannelDelegate>)delegate;
- (void)subscribe:(NSString *)streamId
    streamOptions:(NSDictionary *)streamOptions
signalingChannelDelegate:(id<EnxSignalingChannelDelegate>)delegate;
- (void)unsubscribe:(NSString *)streamId;
- (void)startRecording:(NSString *)streamId;
- (void)stopRecording:(NSString *)streamId;
    
- (void)sendDataStream:(EnxSignalingMessage *)message;
- (void)updateStreamAttributes:(EnxSignalingMessage *)message;
-(void)sendLogsToServer:(NSString *)logId logText:(NSString *)logText;

#pragma mark- CC
- (void)RequestFlloor;
- (void)ProcessFloorRequest:(NSString *)clientId status:(NSString *)status;

#pragma mark-
- (void)muteAllUser;
- (void)unMuteAllUser;
- (void)muteSingleUser:(NSString*)clientId;
- (void)unMuteSingleUser:(NSString*)clientId;


#pragma mark- AT
-(void)getMaxTalkersFromSignaling;
-(void)getTalkerCountFromSignaling;
-(void)setTalkerCountFromSignaling:(NSInteger)number;

#pragma mark-
- (void)hardMuteVideo:(NSString*)clientId;
- (void)hardUnmuteVideo:(NSString*)clientId;
#pragma mark- self video Mute/unmute
- (void)SelfUnmuteVideo;
- (void)SelfMuteVideo;
#pragma mark- self Audio Mute/unmute
- (void)SelfUnmuteAudio;
- (void)SelfMuteAudio;


-(void)subscriberVideoMute :(NSString *)StreamId;

//SimulCast:  This API use to request server to set the remote video stream in different quality.
-(void)signallingSetReceiveVideoQuality:(NSString*_Nonnull)videoQuality streamId:(NSString * _Nonnull )streamId;

// This method use to send log event data to server.
-(void)socketEnxLogEmit:(NSDictionary *_Nonnull)dictionary;
@end
