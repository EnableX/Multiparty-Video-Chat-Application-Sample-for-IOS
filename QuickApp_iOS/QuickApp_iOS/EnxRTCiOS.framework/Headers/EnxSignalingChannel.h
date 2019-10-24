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
 This event is fired when a Socket error  was Occured.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param reason  Cause of error returned by the server.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didError:(NSArray *)reason;

- (void)signalingChannel:(EnxSignalingChannel *)channel didEventError:(NSString *)reason type:(NSString *)eventType streamId:(NSString *)streamId;

/**
 This event is fired when a Reconnect attamped.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param reason  message for reconnect.
 **/
- (void)signalingChannel:(EnxSignalingChannel *)channel didReconnect:(NSString *)reason;
/**
 This event is fired when a Reconnect with same room success attamped.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data  reconnected room meta data.
 **/
- (void)signalingChannel:(EnxSignalingChannel *)channel didConnect:(NSArray *)Data;
/**
 This event is fired when a token was not successfuly used.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param reason  cause for failure of reconnect.
**/
- (void)signalingChannel:(EnxSignalingChannel *)channel tokenValidationFailed:(NSArray *)reason;

/**
 This event is fired when a token was not successfuly used.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param reason  cause for failure of room metadata.
 **/
- (void)signalingChannel:(EnxSignalingChannel *)channel failedToloadRoomMetadata:(NSArray *)reason;


/**
 Event fired as soon a client connect to a room.

 @param channel EnxSignalingChannel the channel that emit the message.
 @param roomMeta Metadata associated to the room that the client just connect.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didConnectToRoom:(NSDictionary *)roomMeta;

/**
 Event fired as soon a client connect to a room.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param roomMeta Metadata associated to the room that the client just connect.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didReConnectToRoom:(NSDictionary *)roomMeta;

/**
 Event fired as soon as rtc channels were disconnected and websocket
 connection is about to be closed.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param roomMeta Metadata associated to the room that the client just connect.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel roomDidDisconnected:(NSDictionary *)roomMeta;

/**
 Event fired when a new stream id has been created and server is ready
 to start publishing it.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream that will be published.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel didReceiveStreamIdReadyToPublish:(NSString *)streamId;
/**
 Event Fired when unpublish called
@param channel EnxSignalingChannel the channel that emit the message.
@param data NSArray Response of the stream that will be Unpublished.
*/
- (void)signalingChannel:(EnxSignalingChannel *)channel didOnUnPublish:(NSArray *)data;


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
//Now Not in used
//- (void)signalingChannel:(EnxSignalingChannel *)channel didRequestPublishP2PStreamWithId:(NSString *)streamId
//                                                                        peerSocketId:(NSString *)peerSocketId;

/**
 Method called when the signaling channels needs a new client to operate a connection.

 @param channel EnxSignalingChannel the channel that emit the message.

 @returns EnxClientDelegate instance.
 */
- (id<EnxSignalingChannelDelegate>)clientDelegateRequiredForSignalingChannel:(EnxSignalingChannel *)channel forStream:(NSString *)streamID;

/**
 Event fired when data stream received.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream received from.
 @param dataStream NSDictionary having message and timestamp.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel
            fromStreamId:(NSString *)streamId
	  receivedDataStream:(NSDictionary *)dataStream;



- (void)signalingChannel:(EnxSignalingChannel *)channel
               receivedDataRoom:(NSArray *)data;

/**
 Event fired when bandwidth update.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data having message and timestamp.
 */
- (void)signalingChannel:(EnxSignalingChannel *)channel
      didBandWidthUpdated:(NSArray *)data;

- (void)signalingChannel:(EnxSignalingChannel *)channel
     didCanvasStreamEvent:(NSArray *)data;

- (void)signalingChannel:(EnxSignalingChannel *)channel
     didShareStreamEvent:(NSArray *)data;


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
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponsesSelfMute:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponsesSelfUnMute:(NSArray *)Data;

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
- (void)signalingChannel:(EnxSignalingChannel *)channel didReleaseFloorRequested:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didDenyFloorRequested:(NSArray *)Data;
//For Moserator
- (void)signalingChannel:(EnxSignalingChannel *)channel didFloorRequested:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didProcessFloorRequest:(NSArray *)Data;

//H
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponseMuteAllUser:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponseUnMuteAllUser:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponseMuteSingleUser:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel didResponseUnMuteSingleUser:(NSArray *)Data;

//lisner

- (void)signalingChannel:(EnxSignalingChannel *)channel receivedHardMuteAll:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel receivedHardUnmuteAll:(NSArray *)Data;

- (void)signalingChannel:(EnxSignalingChannel *)channel receivedHardMuteUser:(NSArray *)Data;
- (void)signalingChannel:(EnxSignalingChannel *)channel receivedHardUnMuteUser:(NSArray *)Data;
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

//Canvas Delegate
-(void)signalingChannel:(EnxSignalingChannel *)channel canvasStartedSignaling:(NSArray *)Data;
-(void)signalingChannel:(EnxSignalingChannel *)channel canvasStoppedSignaling:(NSArray *)Data;

-(void)signalingChannel:(EnxSignalingChannel *)channel publishMediaCodecChange:(NSArray *)Data;

-(void)signalingChannel:(EnxSignalingChannel *)channel publisherBandWidthAlert:(NSArray *)Data;

//SimulCast
- (void)signalingChannel:(EnxSignalingChannel *)channel didSignallingVideoQualityUpdated:(NSArray *)data;

//delegate method when to get connectedIp
-(void)signalingGetConnectedIp:(NSString *)connectedIp;

#pragma mark- Stats Degegate
//on Enable/diasable  Stats
-(void)signalingEnableDesableStats:(NSArray *)data;
-(void)signalingReciveStastData:(NSArray *)statsData;

#pragma mark- Advance Options Degegate
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAcknowledgementAdvanceOption:(NSArray *_Nullable)data;

- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didGenericEvents:(NSArray *_Nullable)data;


- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didGetAdvanceOptions:(NSArray *_Nullable)data;

#pragma mark- Switch user role Degegate
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didSwitchUserRole:(NSArray *_Nullable)data;

- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didUserRoleChanged:(NSArray *_Nullable)data;

#pragma mark- Send Data Delegate
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAcknowledgSendData:(NSArray *_Nullable)data;


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
@property(nonatomic) int reconnectAttamped;
@property (nonatomic,copy,readonly) NSString *roomSigMediaConfiguration;

///-----------------------------------
/// @name Public Methods
///-----------------------------------

- (void)connect:(NSArray *)advanceOptions;
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
-(void)sendMessage:(NSDictionary *)dataDict;
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
-(void)setTalkerCountFromSignaling:(NSInteger)number callbackCheck:(BOOL)callbackCheck;

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

#pragma mark- Reconnect
-(void)reconnectAttempt:(NSString* _Nonnull)token numberOfAttamped:(NSDictionary*_Nullable)reconnectInfo;
#pragma mark- Stats
-(void)setEnableDesiableStats:(NSDictionary*_Nonnull)info;

#pragma mark- To Set Advance Options.
-(void)setAdvanceOptions:(NSArray *_Nullable)advanceOptions;

-(void)getSignallingAdvanceOptions;

// To Switch user role.
-(void)switchUserRole:(NSString *)clientId;

@end
