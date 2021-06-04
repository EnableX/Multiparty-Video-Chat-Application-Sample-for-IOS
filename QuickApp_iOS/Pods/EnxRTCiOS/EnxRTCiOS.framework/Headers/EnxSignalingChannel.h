
/**
 This class will be responsible for all socket event either for parent room connection or reconnect or breakout room connect,
 To initiate this class -initWithEncodedToken:roomDelegate:clientDelegate:
 @param roomDelegate - is the refrence of EnxRoom instance, which help to delegate with room and inform room through callback methods.
 @param clientDelegate - this is also a refrence of EnxRoom instance, which help to delegate with EnxRoom and from EnxRoom to EnxClient through callback methods.
 */

@import WebRTC;
@import SocketIO;
#import "EnxSignalingMessage.h"
#import "EnxSignalingEvent.h"

#import "EnxClientDelegate.h"



@class EnxSignalingChannel;

///-----------------------------------
/// @protocol EnxSignalingChannelDelegate
/// This delegate method used for EnxClient, its will update to Enxclient with all publisher/subscriber behaviour
///-----------------------------------

@protocol EnxSignalingChannelDelegate <NSObject>
// This variable will carry publisher stream ID
@property NSString * _Nullable streamId;
// This variable will carry socket peer ID
@property NSString * _Nullable peerSocketId;

/**
 Event fired when Enx server has validated our token.
 @param signalingChannel EnxSignalingChannel the channel that emit the message.
 */
- (void)signalingChannelDidOpenChannel:(EnxSignalingChannel * _Nullable)signalingChannel;

/**
 Event fired each time EnxSignalingChannel has received a new EnxSignalingMessage.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param message EnxSignalingMessage received by channel.
 */
- (void)signalingChannel:(EnxSignalingChannel * _Nullable)channel didReceiveMessage:(EnxSignalingMessage * _Nonnull)message mediaConfigurationCodec:(NSString *_Nonnull)mediaConfigurationCodec mediaCodecsInfoInSDP:(NSDictionary *_Nonnull)mediaCodecsInfoInSDP;

/**
 Event fired when Enx is ready to receive a publishing stream.
 @param signalingChannel EnxSignalingChannel the channel that emit the message.
 @param peerSocketId Id of the socket in a p2p publishing without MCU. Pass nil if
        you are not setting a P2P room.
 @param streamId id of the publish stream
 @details this is a callback method for EnxClient to inform about publisher stream has published with their valid stresm ID
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)signalingChannel
  readyToPublishStreamId:(NSString *_Nonnull)streamId
            peerSocketId:(NSString *_Nullable)peerSocketId;

/**
 Event fired when Enx failed to publishing stream.
 @param signalingChannel EnxSignalingChannel the channel that emit the message.
 @details this callback event for EnxClient, to inform about publisher stream getting failed to published due to some reson.
 */
- (void)signalingChannelPublishFailed:(EnxSignalingChannel *_Nullable)signalingChannel;

/**
 Event fired each time EnxSignalingChannel has received a confirmation from the server
 to subscribe a stream.
 This event is fired to let Client know that it can start signaling to subscribe the stream.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId Id of the stream that will be subscribed.
 @param peerSocketId pass nil if is MCU being used.
 @details this is a callback method for EnxClient to inform about subscriber stream bas  subscriped with their valid stresm ID
 */
- (void)signalingChannel:(EnxSignalingChannel * _Nullable)channel
readyToSubscribeStreamId:(NSString *_Nonnull)streamId
            peerSocketId:(NSString *_Nullable)peerSocketId;

#pragma mark-  Canvas portocal
/**
 Event fired when Enx failed to publishing canvas stream.
 @param signalingChannel EnxSignalingChannel the channel that emit the message.
 @details this callback event for EnxClient, to inform about canvas stream getting failed to published due to some reson.
 */
- (void)signalingChannelCanvasPublishFailed:(EnxSignalingChannel * _Nullable)signalingChannel;
/**
 Event fired when Enx is ready to receive a canvas publishing stream.
 @param signalingChannel EnxSignalingChannel the channel that emit the message.
 @param peerSocketId Id of the socket in a p2p publishing without MCU. Pass nil if
        you are not setting a P2P room.
 @param streamId Id of canvas stream that will be publish.
 @details this is a callback method for EnxClient to inform about canvas stream has published with their valid stresm ID
 */
- (void)signalingChannel:(EnxSignalingChannel * _Nullable)signalingChannel
  canvasReadyToPublishStreamId:(NSString *_Nonnull)streamId
            peerSocketId:(NSString *_Nullable)peerSocketId;

#pragma mark-  Share Screen portocal
/**
 Event fired when Enx failed to publishing canvas stream.
 @param signalingChannel EnxSignalingChannel the channel that emit the message.
 @details this callback event for EnxClient, to inform about share screen stream getting failed to published due to some reson.
 */
- (void)signalingChannelShareScreenPublishFailed:(EnxSignalingChannel *_Nullable)signalingChannel;
/**
 Event fired when Enx is ready to receive a canvas publishing stream.
 @param signalingChannel EnxSignalingChannel the channel that emit the message.
 @param peerSocketId Id of the socket in a p2p publishing without MCU. Pass nil if
        you are not setting a P2P room.
 @param streamId Id of share screen stream that will be publish.
 @details this is a callback method for EnxClient to inform about share screen stream has published with their valid stresm ID
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)signalingChannel
  shareScreenReadyToPublishStreamId:(NSString *_Nonnull)streamId
            peerSocketId:(NSString *_Nullable)peerSocketId;
@end

///-----------------------------------
/// @protocol EnxSignalingChannelRoomDelegate
/// /// This delegate method used for EnxRoom, its will update to EnxRoom with all valid behariour of room events
///-----------------------------------

@protocol EnxSignalingChannelRoomDelegate <NSObject>
/**
 This event is fired when a Socket error  was Occured.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param reason  Cause of error returned by the server.
 @details this callback method will inform to EnxRoom that server connection has stablished with valid cause of connection failure.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didError:(NSArray *_Nonnull)reason;
/**
 This event is fired when a socket failed to subscribe/publish for any stream.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param reason  Cause of error returned by the server.
 @param eventType what type of event was like local stream publish canvas publish, screen share publish,annotation publish or remote stream subscribe
 @details this callback method will inform to EnxRoom that server has failed to either publish or subscriber or both has failed.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didEventError:(NSString *_Nonnull)reason type:(NSString *_Nonnull)eventType streamId:(NSString *_Nullable)streamId;

/**
 This event is fired when a Reconnect attamped.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param reason  message for reconnect.
 @details this socket on listener method will inform to EnxRoom about cause of reconnect.
 **/
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didReconnect:(NSString *_Nonnull)reason;
/**
 This event is fired when a Reconnect with same room success attamped.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data  reconnected room meta data.
 @details this is the socket on listener  method for EnxRoom to inform about reconnect has successfully done with room.
 **/
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didConnect:(NSArray *_Nonnull)Data;
/**
 This event is fired when a token was not successfuly used.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param reason  cause for failure of reconnect.
 @details this is the socket emit method for EnxRoom to validate token and its failed.
**/
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel tokenValidationFailed:(NSArray *_Nonnull)reason;

/**
 This event is fired when a token was not successfuly used.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param reason  cause for failure of room metadata.
 @details this is the socket on listener  method for EnxRoom to inform about failure to get room metadate after connect success.
 **/
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel failedToloadRoomMetadata:(NSArray *_Nonnull)reason;

/**
 Event fired as soon a client connect to a room.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param roomMeta Metadata associated to the room that the client just connect.
 @details this is the socket on listener  method for EnxRoom to inform room connection successfully stablished and roommetadat also available
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didConnectToRoom:(NSDictionary *_Nonnull)roomMeta;

/**
 Event fired as soon a client connect to a room.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param roomMeta Metadata associated to the room that the client just connect.
 @details this is the socket on listener  method for EnxRoom to inform room reconnection successfully stablished with same room and roommetadat also available.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didReConnectToRoom:(NSDictionary *_Nonnull)roomMeta;

/**
 Event fired as soon as rtc channels were disconnected and websocket
 connection is about to be closed.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param roomMeta Metadata associated to the room that the client just disconnected.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel roomDidDisconnected:(NSDictionary *_Nonnull)roomMeta;
/**
 Event fired as soon as rtc channels were disconnected and websocket
 connection is about to be closed.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param roomMeta Metadata associated to the room that the client just disconnected.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel roomDidDisconnectedByServer:(NSDictionary *_Nonnull)roomMeta;
/**
 Event fired when a new stream id has been created and server is ready
 to start publishing it.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream that will be published.
 @details this is socket emit method for EnxRoom to inform the local stream has started publish it.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didReceiveStreamIdReadyToPublish:(NSString *_Nonnull)streamId;
/**
 Event Fired when unpublish called
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data NSArray Response of the stream that will be Unpublished.
 @details this is socket emit method for EnxRoom to inform the local stream has started unpublish it.
*/
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didOnUnPublish:(NSArray *_Nonnull)data;

/**
 Event fired when a new stream id has been created and server is ready
 to start subscribing it.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream that will be subscribe.
 @details this is socket emit method for EnxRoom to inform about the remote stream has started subscribe it.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didReceiveStreamIdReadyToSubscribe:(NSString *_Nonnull)streamId;


/**
 Event fired when a recording of a stream has started.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream being recorded.
 @param recordingId NSString id of the recording id on Enx server.
 @param recordingDate NSDate when the server start to recording the stream.
 @details this is socket emit method for EnxRoom to inform about recording for stream has started.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didStartRecordingStreamId:(NSString *_Nonnull)streamId
    withRecordingId:(NSString *_Nonnull)recordingId
    recordingDate:(NSDate *_Nonnull)recordingDate;
/**
 Event fired when a recording of a stream has failed.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream being recorded.
 @param errorMsg Error string sent from the server.
 @details this is socket emit method for EnxRoom to inform about recording for stream has failed to start.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didFailStartRecordingStreamId:(NSString *_Nonnull)streamId
            withErrorMsg:(NSString *_Nonnull)errorMsg;

/**
 Event fired when recording start successful
 @param response about recording start.
 @details this is socket on listener  method for EnxRoom to inform about recording started successful.
 */
-(void)signalingRecordingDidStart:(NSArray *_Nonnull)response;

/**
 Event fired when recording stop successful
 @param response about recording start.
 @details this is socket on listener  method for EnxRoom to inform about recording stop successful.
 */
-(void)signalingRecordingDidStop:(NSArray *_Nonnull)response;

/**
 Event fired when a new StreamId has been added to a room.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString added to the room.
 @param event Event name and data carried
 @details this is the socket emit method for EnxRoom to update about subscribe stream with their stream ID has added
 */
- (void)signalingChannel:(EnxSignalingChannel * _Nullable)channel
    didStreamAddedWithId:(NSString *_Nonnull)streamId
                   event:(EnxSignalingEvent *_Nonnull)event;

/**
 Event fired when a StreamId previously subscribed has been unsubscribed.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString of the unsubscribed stream.
 @details this is the socket emit method for EnxRoom to update about unsubscribe stream with their stream ID has added
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didUnsubscribeStreamWithId:(NSString *_Nonnull)streamId;

/**
Event fired when a StreamId previously subscribed has been failed to unsubscribed.
@param channel EnxSignalingChannel the channel that emit the message.
@param streamId NSString of the unsubscribed stream.
 @details this is the socket emit method for EnxRoom to update about unsubscribe stream with their stream ID has failed
*/
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didUnsubscribeFailedStreamWithId:(NSString *_Nonnull)streamId;

/**
 Event fired when a published stream is being unpublished.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString of the stream being unpublished
 @details this is the socket emit method for EnxRoom to update about unpublished stream with their stream ID has done.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didUnpublishStreamWithId:(NSString *_Nonnull)streamId;

/**
 Event fired when a published stream is faild to unpublished.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString of the stream being unpublished
 @details this is the socket emit method for EnxRoom to update about unpublished stream with their stream ID has failed.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didUnpublishStreamfailed:(NSString *_Nonnull)streamId;

/**
 Method called when the signaling channels needs a new client to operate a connection.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamID Id of stream.
 @returns EnxClientDelegate instance.
 */
-(id<EnxSignalingChannelDelegate>)clientDelegateRequiredForSignalingChannel:(EnxSignalingChannel *_Nullable)channel forStream:(NSString *_Nonnull)streamID;

/**
 Event fired when data stream received.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream received from.
 @param dataStream NSDictionary having message and timestamp.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel
            fromStreamId:(NSString *_Nonnull)streamId
	  receivedDataStream:(NSDictionary *_Nonnull)dataStream;


/**
 Event fired when data  received room.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data NSarray having message and timestamp.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel
        receivedDataRoom:(NSArray *_Nonnull)data withCallBackEmiter:(SocketAckEmitter * _Nonnull) emitter;

/**
 Event fired when bandwidth update.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data having message and bandwidth.
 @details bandwidth update for publisher  stream.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel
      didBandWidthUpdated:(NSArray *_Nonnull)data;
/**
 Event fired when bandwidth update canvas.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data having message and bandwidth.
 @details bandwidth update for canvas  stream.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel
     didCanvasStreamEvent:(NSArray *_Nonnull)data;
/**
 Event fired when bandwidth update share screen.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data having message and bandwidth.
 @details bandwidth update for screen share  stream.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel
     didShareStreamEvent:(NSArray *_Nonnull)data;

/**
 Event fired when video hard Mute .
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data having message about hard mute.
 @details this is the socket emit method for EnxRoom about video hard unmute
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel
        didHardVideoMute:(NSArray *_Nonnull)data;
/**
 Event fired when video hard unMute .
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data having message about hard unmute.
 @details this is the socket emit method for EnxRoom about video hard unmute
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel
        didHardVideoUnMute:(NSArray *_Nonnull)data;
/**
 Event fired when video hard Mute .
 @param channel EnxSignalingChannel the channel that emit the message.
 @param dataStream having message about hard mute.
 @details this is the socket on listener method for EnxRoom about video hard mute by any of the modeator
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didReciveHardMuteVideo:(NSArray *_Nonnull)dataStream;
/**
 Event fired when video hard unMute .
 @param channel EnxSignalingChannel the channel that emit the message.
 @param dataStream having message about hard unmute.
 @details this is the socket on listener method for EnxRoom about video hard unmute by any of the modeator
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didReciveHardUnmuteVideo:(NSArray *_Nonnull)dataStream;

/**
 Event fired when self mute video  .
 @param channel EnxSignalingChannel the channel that emit the message.
 @param dataStream having message about video mute.
 @details this is the socket emit method for EnxRoom about user has muted their video
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didSelfMuteVideo:(NSArray *_Nonnull)dataStream;
/**
 Event fired when self unmute video  .
 @param channel EnxSignalingChannel the channel that emit the message.
 @param dataStream having message about video unmute.
 @details this is the socket emit method for EnxRoom about user has unmuted their video
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didSelfUnmuteVideo:(NSArray *_Nonnull)dataStream;
/**
 Event fired when self mute audio  .
 @param channel EnxSignalingChannel the channel that emit the message.
 @param dataStream having message about audio mute.
 @details this is the socket emit method for EnxRoom about user has muted their audio
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didSelfMuteAudio:(NSArray *_Nonnull)dataStream;
/**
 Event fired when self unmute audio  .
 @param channel EnxSignalingChannel the channel that emit the message.
 @param dataStream having message about audio unmute.
 @details this is the socket emit method for EnxRoom about user has unmuted their audio
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didSelfUnmuteAudio:(NSArray *_Nonnull)dataStream;

//recive by Moderator
/**
 Event fired when any modeator mute   .
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data having message about  mute.
 @details this is the socket emit method for EnxRoom about modeator has muted any participent
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didResponsesSelfMute:(NSArray *_Nonnull)Data;
/**
 Event fired when any modeator unmute   .
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data having message about  unmute.
 @details this is the socket emit method for EnxRoom about modeator has unmuted any participent
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didResponsesSelfUnMute:(NSArray *_Nonnull)Data;

/**
 Event fired when stream atrribute updated.
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream received from.
 @param attributes NSDictionary having custom attribute.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel
            fromStreamId:(NSString *_Nonnull)streamId
   updateStreamAttributes:(NSDictionary *_Nonnull)attributes;

/**
 Event fired when  Logs  upload
 @param data list of logs
 @details this is the socket emit method for logs upload to server.
 */
-(void)signalingLogsUpLoaded:(NSArray *_Nonnull)data;

#pragma mark - ChairControl
/**
 Event fired for Floor Manage On Event
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data list for floor request
 @details this is the socket emit method for EnxRoom about floor managment
 */
-(void)signalingChannel:(EnxSignalingChannel*_Nullable)channel didFloorManagmentEvents:(NSArray *_Nonnull)Data;
/**
 Event fired for Floor process Emit Event
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data list for floor request
 @details this is the socket emit method for EnxRoom about process floor request
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didProcessFloorRequest:(NSArray *_Nonnull)Data;

/**
 Event fired for Moderator who give direct access to any participent
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about floor access event
 @details this is the socket emit method for EnxRoom about mederator giving direct access to any participent with floor request
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didACKHandOverFloorRequest:(NSArray *_Nonnull)Data;

/**
 Event fired hard mute
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about hard mute
 @details this is the socket on listener method for EnxRoom about hard muted by any of modeator
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didResponseMuteAllUser:(NSArray *_Nonnull)Data;
/**
 Event fired hard Unmute
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about hard Unmute
 @details this is the socket on listener method for EnxRoom about hard Unmuted by any of modeator
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didResponseUnMuteAllUser:(NSArray *_Nonnull)Data;
/**
 Event fired single user mute
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about single user mute
 @details this is the socket on listener method for EnxRoom about single user mute by any of modeator
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didResponseMuteSingleUser:(NSArray *_Nonnull)Data;
/**
 Event fired single user unmute
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about single user unmute
 @details this is the socket on listener method for EnxRoom about single user unmute by any of modeator
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didResponseUnMuteSingleUser:(NSArray *_Nonnull)Data;

//lisner
/**
 Event fired alll user mute
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  alll user mute
 @details this is the socket on listener method for EnxRoom about all user mute by any of modeator
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel receivedHardMuteAll:(NSArray *_Nonnull)Data;
/**
 Event fired alll user unmute
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  alll user unmute
 @details this is the socket on listener method for EnxRoom about all user unmute by any of modeator
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel receivedHardUnmuteAll:(NSArray *_Nonnull)Data;
/**
 Event fired alll user mute
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  alll user unmute
 @details this is the socket on listener method for EnxRoom about all user mute by any of modeator
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel receivedHardMuteUser:(NSArray *_Nonnull)Data;
/**
 Event fired alll user unmute
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  alll user unmute
 @details this is the socket on listener method for EnxRoom about all user unmute by any of modeator
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel receivedHardUnMuteUser:(NSArray *_Nonnull)Data;
/**
 Event fired when any user has joined
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  new user
 @details this is the socket on listener method for EnxRoom ,which inform whenever any new user has joined to the room.
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel userDidJoined:(NSArray *_Nonnull)Data;
/**
 Event fired when any user has disconnected
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  disconnected user
 @details this is the socket on listener method for EnxRoom ,which inform whenever any new user has disconnected from room.
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel userDidDisconnected:(NSArray *_Nonnull)Data;

   #pragma mark- recording Participant
/**
 Event fired when recording has started in room
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  start recording
 @details this is the socket on listener method for EnxRoom ,which inform to room whenever recording has started in room.
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel roomRecordOn:(NSArray *_Nonnull)Data;
/**
 Event fired when recording has stopped in room
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  stopped recording
 @details this is the socket on listener method for EnxRoom ,which inform to room whenever recording has stopped in room.
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel roomRecordOff:(NSArray *_Nonnull)Data;

/**
 Event fired for Active talker
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  list of active talker
 @details this is the socket on listener method for EnxRoom ,which inform to room whenever active talket list change
 */

-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel activeTalkerList:(NSArray *_Nonnull)Data;
/**
 Event fired for Max talker
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  max talker
 @details this is the socket on listener method for EnxRoom ,which inform to room about what will be the possible max talker in room.
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didGetMaxTalkersFromSignaling:(NSArray *_Nonnull)Data;
/**
 Event fired to get available taker count
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  available talker
 @details this is the socket on listener method for EnxRoom ,which inform to room about current active takler in room
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didGetTalkerCountFromSignaling:(NSArray *_Nonnull)Data;
/**
 Event fired to set talker count
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  set talker
 @details this is the socket on listener method for EnxRoom ,which inform to room about set takler in room
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didSetTalkerCountFromSignaling:(NSArray *_Nonnull)Data;
/**
 Event fired when screen share started
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  screen shared
 @details this is the socket on listener method for EnxRoom ,which inform to room about start screen shared
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel screenShareStartedSignaling:(NSArray *_Nonnull)Data;
/**
 Event fired when screen share stopped
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  screen shared
 @details this is the socket on listener method for EnxRoom ,which inform to room about stopped screen shared
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel screenShareStoppedSignaling:(NSArray *_Nonnull)Data;

//Canvas Delegate
/**
 Event fired when Canvas started
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  Canvas
 @details this is the socket on listener method for EnxRoom ,which inform to room about start Canvas
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel canvasStartedSignaling:(NSArray *_Nonnull)Data;
/**
 Event fired when Canvas stopped
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details about  Canvas
 @details this is the socket on listener method for EnxRoom ,which inform to room about stopped Canvas
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel canvasStoppedSignaling:(NSArray *_Nonnull)Data;
/**
 Event fired when publisher codec change
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details publisher codec
 @details this is the socket on listener method for EnxRoom ,which inform to room about publisher codec
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel publishMediaCodecChange:(NSArray *_Nonnull)Data;
/**
 Event fired when publisher Bandwidth
 @param channel EnxSignalingChannel the channel that emit the message.
 @param Data details publisher bandwidth
 @details this is the socket on listener method for EnxRoom ,which inform to room about publisher bandwidth
 */
-(void)signalingChannel:(EnxSignalingChannel *_Nullable)channel publisherBandWidthAlert:(NSArray *_Nonnull)Data;
//SimulCast
/**
 Event fired when remote stream video quality update
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details SimulCast
 @details this is the socket on listener method for EnxRoom ,which inform to room about  SimulCast upate
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didSignallingVideoQualityUpdated:(NSArray *_Nonnull)data;

//delegate method when to get connectedIp
-(void)signalingGetConnectedIp:(NSString *_Nonnull)connectedIp;

#pragma mark- Stats Degegate
//on Enable/diasable  Stats
/**
 Event fired when rstats enable
 @param data details about stats
 @details this is the socket emit  method for EnxRoom ,which inform troom stats.
 */
-(void)signalingEnableDesableStats:(NSArray *_Nonnull)data;
/**
 Event fired when rstats enable
 @param statsData details about stats
 @details this is the socket emit  method for EnxRoom ,which inform troom stats.
 */
-(void)signalingReciveStastData:(NSArray *_Nonnull)statsData;

#pragma mark- Advance Options Degegate
/**
 Event fired when emit advance option
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data ACK about advance option
 @details this is the socket emit method for advance option
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAcknowledgementAdvanceOption:(NSArray *_Nonnull)data;
/**
 Event fired when generic event for socket
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about generic events
 @details this is the socket on listener method for generic events
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didGenericEvents:(NSArray *_Nonnull)data;

/**
 Event fired when advance option set
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about advance option
 @details this is the socket emit method for advance option
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didGetAdvanceOptions:(NSArray *_Nonnull)data;

#pragma mark- Switch user role Degegate
/**
 Event fired for Switch User Role
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about Switch User
 @details this is the socket emit method for switch user
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didSwitchUserRole:(NSArray *_Nonnull)data;
/**
 Event fired for  Role changed
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about role changed
 @details this is the socket emit method for role changed.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didUserRoleChanged:(NSArray *_Nonnull)data;

#pragma mark- Send Data Delegate
/**
 Event fired for send data acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about send data
 @details this is the socket emit Acknowledgment method for send data.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAcknowledgSendData:(NSArray *_Nonnull)data;

#pragma mark - OutBound Call
/**
 Event fired for outbound call
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about outbound call
 @details this is the socket emit  method for outbound call.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didOutBoundCallInitiated:(NSArray *_Nonnull)data;
/**
 Event fired to update state about outbound call
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about outbound call state
 @details this is the socket on listener  method for outbound call state update.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didDialStateEvents:(NSArray *_Nonnull)data;


#pragma mark- Canvas Room Delegate
/**
 Event fired when a new stream id has been created and server is ready
 to start publishing it.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream that will be published.
 */
- (void)signalingChannel:(EnxSignalingChannel * _Nullable)channel didReceiveCanvasStreamIdReadyToPublish:(NSString *_Nonnull)streamId;

/**
 Event fired when a Canvas published stream is being unpublished.

 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString of the stream being unpublished
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didCanvasUnpublishStreamWithId:(NSString *_Nonnull)streamId;

#pragma mark- Screen Share Room Delegate
/**
 Event fired when a new stream id has been created and server is ready
 to start publishing it.
 
 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString id of the stream that will be published.
 */
- (void)signalingChannel:(EnxSignalingChannel * _Nullable)channel didReceiveScreenShareStreamIdReadyToPublish:(NSString *_Nonnull)streamId;
/**
 Event fired when a Canvas published stream is being unpublished.

 @param channel EnxSignalingChannel the channel that emit the message.
 @param streamId NSString of the stream being unpublished
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didScreenShareUnpublishStreamWithId:(NSString *_Nonnull)streamId;

#pragma mark- Room Expire Events
/**
 Event fired for confrence duration update
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data about confrence duration
 @details this is the socket on listner method for confrence duration update
 */
//Timing Alert
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didRoomManagmentEvent:(NSArray *_Nonnull)data;
//TimeExtend
/**
 Event fired for confrence duration update
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data about confrence duration extend
 @details this is the socket emit method for confrence duration extend
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didConferencessDurationExtend:(NSArray *_Nonnull)data;

//Lock/Unlock Delegates
/**
 Event fired for locked room
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about lock room
 @details this is the socket on listner method for locked room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAckLockRoom:(NSArray *_Nonnull)data;
/**
 Event fired for locked room Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about lock room acknowledgment
 @details this is the socket emit acknowledgment method for locked room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAckUnlockRoom:(NSArray *_Nullable)data;
/**
 Event fired for drop user
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about ldrop user
 @details this is the socket emit acknowledgment method for drop user.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAckDropUser:(NSArray *_Nonnull)data;
/**
 Event fired for destroy room
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about destroy room
 @details this is the socket emit acknowledgment method for destroy room.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAckDestroy:(NSArray *_Nonnull)data;

#pragma mark- Room Live Streaming Events
/**
 Event fired for streaming
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about streaming
 @details this is the socket emit acknowledgment method for start streaming.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAckStartStreaming:(NSArray *_Nonnull)data;
/**
 Event fired for streaming
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about streaming
 @details this is the socket emit acknowledgment method for stop streaming.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAckStopStreaming:(NSArray *_Nonnull)data;
/**
 Event fired for streaming
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about streaming
 @details this is the socket on listiner notification method for stop streaming.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didStreamingNotification :(NSArray *_Nonnull)data;

#pragma mark- Room FaceX  Events
/**
 Event fired for streaming
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about PingBack
 @details this is the socket emit method for faceX
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAcknowledgePingBackInRoom:(NSArray *_Nonnull)data;
/**
 Event fired for FaceX Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about faceX
 @details this is the socket emit acknowledgment method for Start faceX
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAcknowledgeStartClientUsageInRoom:(NSArray *_Nonnull)data;
/**
 Event fired for FaceX Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about faceX
 @details this is the socket emit acknowledgment method for Stopped faceX
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAcknowledgeStopClientUsageInRoom:(NSArray *_Nonnull)data;

#pragma mark- BreakOut Room
/**
 Event fired for creat breakout  room Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details breakout room
 @details this is the socket emit acknowledgment method for create breakout room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didACKForCreateBreakOutRoom:(NSArray *_Nonnull)data;
/**
 Event fired for join breakout  room Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about join breakout room
 @details this is the socket emit acknowledgment method for join breakout room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didACKForJoinBreakOutRoom:(NSArray *_Nonnull)data;
/**
 Event fired for Invite breakout  room Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about Invite breakout room
 @details this is the socket emit acknowledgment method for Invite breakout room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didACKForInviteBreakOutRoom:(NSArray *_Nonnull)data;
/**
 Event fired for mute  room Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about mute room
 @details this is the socket emit acknowledgment method for mute room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didACKForMuteRoom:(NSArray *_Nonnull)data;
/**
 Event fired for unmute  room Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details about unmute room
 @details this is the socket emit acknowledgment method for unmute room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didACKForUnMuteRoom:(NSArray *_Nonnull)data;
/**
 Event fired for creat and invite breakout  room Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details create and invite breakout room
 @details this is the socket emit acknowledgment method for create and invite breakout room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didACKForCreateAndInviteBreakOutRoom:(NSArray *_Nonnull)data;
/**
 Event fired for pause room Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details pause room
 @details this is the socket emit acknowledgment method for pause room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didACKForPauseRoom:(NSArray *_Nonnull)data;
/**
 Event fired for resume room Acknowledgment
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details resume room
 @details this is the socket emit acknowledgment method for resume room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didACKForResumeRoom:(NSArray *_Nonnull)data;
/**
 Event fired for join breakout room
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details join break room
 @details this is the socket emit acknowledgment method for join breakout room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didJoinBreakoutRoom:(NSArray *_Nonnull)data;
/**
 Event fired for user joined breakout room
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details joined break room
 @details this is the socket on listrener method for user joined breakout room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didUserJoinedBreakoutRoom:(NSArray *_Nonnull)data;

/**
 Event fired when all user disconnect from breakoutroom and breakout room will Destroyed
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details breakout Room
 @details this is the socket on listrener method, Which will inform to owner of breakout room that all user has disconencted and room get Destroyed
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didBreakoutRoomDestroyed:(NSArray *_Nonnull)data;

/**
 Event fired when user will disconnect from breakout room
 @param channel EnxSignalingChannel the channel that emit the message.
 @param data details user , which got disconnected from breakout room.
 @details this is the socket on listrener method for all use in parents room will get notify about user disconnected from breakout room resently.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didUserDisconnectedFromBreakoutRoom:(NSArray *_Nonnull)data;

#pragma mark- knock-knock room or wait for modeator
/**
 Event fired when any modeator will approved and user from knock knock room.
 @param channel EnxSignalingChannel _Nullable the channel that emit the message.
 @param data details of allowed awaited user _Nonnull
 @details this is the socket on listrener method for modeator , who going to allowed and user in knock knock room
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAckForApproveAwaitedUser:(NSArray * _Nonnull)data;
/**
 Event fired when any modeat_Nonnullor will deny and user from knock knock room.
 @param channel EnxSignalingChannel__Nullable the channel that emit the message.
 @param data details for deny awaied _Nonnull
 @details this is the socket on listrener method for modeator , who going to deny and user in knock knock room
 */

- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didAckForDenyAwaitedUser:(NSArray *_Nonnull)data;

/**
 Event fired for modeartor when any participent join a konok knock room
 @param channel EnxSignalingChannel _Nullable the channel that emit the message.
 @param data details about user who knocking the room _Nonnull
 @details this is the socket on listrener method for modeartor , this event will fair when any user knocking a room.
 */
- (void)signalingChannel:(EnxSignalingChannel *_Nullable)channel didUserAwated:(NSArray *_Nonnull)data;


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
@property (strong, nonatomic) id<EnxSignalingChannelRoomDelegate> _Nullable roomDelegate;
@property(nonatomic) int reconnectAttamped;
@property (nonatomic,copy,readonly) NSString * _Nonnull roomSigMediaConfiguration;
@property(strong,nonatomic) SocketIOClient * _Nonnull socketIO;

///-----------------------------------
/// @name Public Methods
///-----------------------------------
/**
 Socket API to connect with main room
 @param advanceOptions this is the list of advance option seting during room connect
                this is an option parameter.
 @details EnxRoom will used this method for connect user with main room.
 */
- (void)connect:(NSArray *_Nullable)advanceOptions;
/**
 Socket API to disconnect with main room
 @details EnxRoom will used this method to disconnect user from main room.
 */
- (void)disconnect;
/**
 Socket API to get signaling message
 @param message details about EnxSignalingMessage
 @details EnxClient will used this method to send signaling message to socket after connected from room.
 */
- (void)enqueueSignalingMessage:(EnxSignalingMessage *_Nonnull)message;
/**
 Socket API to get signaling message
 @param message details about EnxSignalingMessage
 @details EnxClient will used this method to send signaling message to socket after connected from room.
 */
- (void)sendSignalingMessage:(EnxSignalingMessage *_Nonnull)message;
/**
 Socket API to get Drain message queue
 @param streamId stream ID which going to drain
 @param peerSocketId which going to drain
 @details EnxClient will used this method to drain message queue with streamID and peerSocketID
 */
- (void)drainMessageQueueForStreamId:(NSString *_Nonnull)streamId
                        peerSocketId:(NSString *_Nonnull)peerSocketId;
/**
    Socket API to publish local stream
    @param options publisher stream option
    @param delegate EnxClient Refrence to update callback
    @details EnxRoom will used this method to publish local stream
 */
- (void)publish:(NSDictionary *_Nonnull)options
  signalingChannelDelegate:(id<EnxSignalingChannelDelegate>)delegate;
/**
    Socket API to unpublish local stream
    @param streamId publisher stream ID
    @param delegate EnxClient Refrence to update callback
    @details EnxRoom will used this method to unpublish local stream
 */
- (void)unpublish:(NSString *_Nonnull)streamId
            signalingChannelDelegate:(id<EnxSignalingChannelDelegate>)delegate;
/**
    Socket API to publish local to peerID
    @param peerSocketId publisher stream peerID
    @param delegate EnxClient Refrence to update callback
    @details EnxRoom will used this method to publish local stream with peerSocketId
 */

- (void)publishToPeerID:(NSString *_Nonnull)peerSocketId
            signalingChannelDelegate:(id<EnxSignalingChannelDelegate>)delegate;
/**
    Socket API to subscribe remote stream
    @param streamId id of subscrib stream
    @param streamOptions stream option for subscribe stream
    @param delegate EnxClient Refrence to update callback
    @details EnxRoom will used this method to subscribe  remote Stream
 */
- (void)subscribe:(NSString *_Nonnull)streamId
    streamOptions:(NSDictionary *_Nonnull)streamOptions
signalingChannelDelegate:(id<EnxSignalingChannelDelegate>)delegate;
/**
    Socket API to unsubscribe remote stream
    @param streamId id of subscrib stream
    @details EnxRoom will used this method to unsubscribe  remote Stream
 */
- (void)unsubscribe:(NSString *_Nonnull)streamId;
/**
    Socket API to start recording
    @param streamId id for start recording
    @details EnxRoom will used this method to start recording
 */
- (void)startRecording:(NSString *_Nonnull)streamId;
/**
    Socket API to stop recording
    @param streamId id for start recording
    @details EnxRoom will used this method to stop recording
 */
- (void)stopRecording:(NSString *_Nonnull)streamId;
/**
    Socket API to Send Data on stream
    @param message details about EnxSignalingMessage
    @details EnxRoom will used this method to send stream data
 */
- (void)sendDataStream:(EnxSignalingMessage *_Nonnull)message;
/**
    Socket API to Send message
    @param dataDict details about message
    @details EnxRoom will used this method to send message
 */
-(void)sendMessage:(NSDictionary *_Nonnull)dataDict;
/**
    Socket API to update stream attribute
    @param message details about EnxSignalingMessage
    @details EnxRoom will used this method to update stream attribute
 */
- (void)updateStreamAttributes:(EnxSignalingMessage *_Nonnull)message;
/**
    Socket API to send logs on server
    @param logId a generic vaule to identify logs
    @param logText details logs in NSString
    @details EnxRoom will used this method to send sdk logs on server
 */
-(void)sendLogsToServer:(NSString *_Nonnull)logId logText:(NSString *_Nonnull)logText;

#pragma mark- Canvas
/**
    Socket API to publish canvas stream
    @param options NSDictonary with carry publish information about canvas
    @param delegate EnxClient Refrence to update callback
    @details EnxRoom will used this method to publish canvas stream
 */
- (void)publishCanvas:(NSDictionary *_Nonnull)options
signalingChannelDelegate:(id<EnxSignalingChannelDelegate>_Nullable)delegate;
/**
    Socket API to unpublish canvas stream
    @param streamId id of canvas stream to unpublish
    @param delegate EnxClient Refrence to update callback
    @details EnxRoom will used this method to unpublish canvas stream
 */
- (void)unpublishCanvas:(NSString * _Nonnull)streamId signalingChannelDelegate:(id<EnxSignalingChannelDelegate> _Nullable)delegate;

#pragma mark- shareScreen
/**
    Socket API to publish Screen share  stream
    @param options NSDictonary with carry publish information about screen share stream
    @param delegate EnxClient Refrence to update callback
    @details EnxRoom will used this method to publish screen share stream
 */
- (void)publishShareScreen:(NSDictionary *_Nonnull)options
signalingChannelDelegate:(id<EnxSignalingChannelDelegate>_Nullable)delegate;
/**
    Socket API to unpublish screen share stream
    @param streamId id of screen share stream to unpublish
    @param delegate EnxClient Refrence to update callback
    @details EnxRoom will used this method to unpublish screen share stream
 */
- (void)unpublishShareScreen:(NSString * _Nonnull)streamId signalingChannelDelegate:(id<EnxSignalingChannelDelegate> _Nullable)delegate;

#pragma mark- CC

/**
    Socket API for floor access request
    @details EnxRoom will used this method to to create a request for floor access
 */
- (void)RequestFlloor;
/**
    Socket API for process floor  request
    @param clientId client id for remote strem which floor request going to be process.
    @param status of floor request
    @details EnxRoom will used this method to process any time of floor request (approve,reject,cancle,finished,deny etc...)
 */
- (void)ProcessFloorRequest:(NSString *_Nonnull)clientId status:(NSString *_Nonnull)status;
/**
    Socket API for handover Floor request
    @param clientId client id for remote participent to whom moderator want to give floor access.
    @details EnxRoom will used this method for moderator, to give floor access to any participant , withour requesting to floor access from participent.
 */
-(void)handOverFloorRequest:(NSString* _Nonnull)clientId;

#pragma mark-
/**
    Socket API to mute all users in same room
    @details EnxRoom will used this method to mute all users in room
 */
- (void)muteAllUser;
/**
    Socket API to unmute all users in same room
    @details EnxRoom will used this method to unmute all users in room
 */
- (void)unMuteAllUser;
/**
    Socket API to mute single user in same room
    @details EnxRoom will used this method to mute single user in room
 */
- (void)muteSingleUser:(NSString*_Nonnull)clientId;
/**
    Socket API to mute single user in same room
    @details EnxRoom will used this method to mute single user in room
 */
- (void)unMuteSingleUser:(NSString*_Nonnull)clientId;


#pragma mark- AT
/**
    Socket API to get max possble taker in room
    @details EnxRoom will used this method to know the max talker cound in room.
 */
-(void)getMaxTalkersFromSignaling;
/**
    Socket API to get current  taker in room
    @details EnxRoom will used this method to know the current talker cound in room.
 */
-(void)getTalkerCountFromSignaling;
/**
    Socket API to set   taker in room
    @param takerValue details about set talker count
    @details EnxRoom will used this method to set talker count  in room.
 */
-(void)setTalkerCountFromSignaling:(NSDictionary* _Nonnull)takerValue callbackCheck:(BOOL)callbackCheck;

#pragma mark-
/**
    Socket API tosingle user video mute
    @param clientId for participent for video mute
    @details EnxRoom will used this method to mute single user video
 */
- (void)hardMuteVideo:(NSString*_Nonnull)clientId;
/**
    Socket API tosingle user video unmute
    @param clientId for participent for video unmute
    @details EnxRoom will used this method to unmute single user video
 */
- (void)hardUnmuteVideo:(NSString*_Nonnull)clientId;
#pragma mark- self video Mute/unmute
/**
    Socket API to self video unmute
    @details EnxRoom will used this method to unmute self video
 */
- (void)SelfUnmuteVideo;
/**
    Socket API to self video mute
    @details EnxRoom will used this method to mute self video
 */
- (void)SelfMuteVideo;
#pragma mark- self Audio Mute/unmute
/**
    Socket API to self audio unmute
    @details EnxRoom will used this method to unmute self audio
 */
- (void)SelfUnmuteAudio;
/**
    Socket API to self audio mute
    @details EnxRoom will used this method to mute self audio
 */
- (void)SelfMuteAudio;

/**
    Socket API to mute subscriber video
    @param StreamId of partifipent which video will mute
    @details EnxRoom will used this method to mute subscriber video
 */
-(void)subscriberVideoMute :(NSString *_Nonnull)StreamId;

//SimulCast:  This API use to request server to set the remote video stream in different quality.
-(void)signallingSetReceiveVideoQuality:(NSString*_Nonnull)videoQuality streamId:(NSString * _Nonnull )streamId;

// This method use to send log event data to server.
-(void)socketEnxLogEmit:(NSDictionary *_Nonnull)dictionary;

#pragma mark- Reconnect
/**
    Socket API to reconnect with same room
    @param token to join same room
    @param reconnectInfo information about reconnect
    @details EnxRoom will used this method to reconnect with same room
 */
-(void)reconnectAttempt:(NSString* _Nonnull)token numberOfAttamped:(NSDictionary*_Nullable)reconnectInfo;
#pragma mark- Stats
/**
    Socket API to enable and desiable stats
    @param info about stats
    @details EnxRoom will used this method to enable/desiable stats
 */
-(void)setEnableDesiableStats:(NSDictionary*_Nonnull)info;

#pragma mark- To Set Advance Options.
/**
    Socket API to set advance option
    @param advanceOptions list of option
    @details EnxRoom will used this method to set advance option
 */
-(void)setAdvanceOptions:(NSArray *_Nullable)advanceOptions;
/**
    Socket API to get list of advance option
    @details EnxRoom will used this method to know, what are the option has setted.
 */
-(void)getSignallingAdvanceOptions;

// To Switch user role.
/**
    Socket API to switched user current role
    @param clientId of user , who role going to switched
    @details EnxRoom will used this method to switch user current role.
 */
-(void)switchUserRole:(NSString *_Nonnull)clientId;

// To initiate outbound call.
/**
    Socket API to make outbond call
    @param number to whome try to call
    @param callerId , its a uniq number of caller
    @details EnxRoom will used this method to make out bond call.
 */
-(void)startOutBoundCall:(NSString* _Nonnull)number callerId:(NSString *_Nonnull)callerId;
/**
    Socket API to extend confrence time
    @details EnxRoom will used this method to extend confrence time
 */
-(void)extendConfrenceTime;
/**
    Socket API to lock room
    @details EnxRoom will used this method to lock current room
 */
-(void)signalingLockRoom;
/**
    Socket API to unlock room
    @details EnxRoom will used this method to unlock current room
 */
-(void)signalingUnlockRoom;

//Drop user and Destroy Room API
/**
    Socket API to Drop user
    @details EnxRoom will used this method to any user from confrence
 */
-(void)signalingDropUser:(NSDictionary *_Nonnull)data;
/**
    Socket API to Destroy room
    @details EnxRoom will used this method to Destroy confrence
 */
-(void)signalingDestroy:(NSDictionary *_Nonnull)data;

//Live broadcast Streaming API
/**
    Socket API to start streaming
    @param streamingConfig information about start streaming
    @details EnxRoom will used this method to start Streaming
 */
-(void)signalingStartStreaming:(NSDictionary *_Nonnull)streamingConfig;
/**
    Socket API to stop streaming
    @param streamingConfig information about stop streaming
    @details EnxRoom will used this method to stop Streaming
 */
-(void)signalingStopStreaming:(NSDictionary *_Nonnull)streamingConfig;

//FaceX APIs
/**
    Socket API to PingBack
    @details EnxRoom will used this method to pingBack
 */
-(void)pingBack;
/**
    Socket API to start FaceX
        @param data details about start FaceX
    @details EnxRoom will used this method to start FaceX
 */
-(void)startClientUsage:(NSDictionary* _Nonnull)data;
/**
    Socket API to stop FaceX
        @param data details about stop FaceX
    @details EnxRoom will used this method to stop FaceX
 */
-(void)stopClientUsage:(NSDictionary* _Nonnull)data;

#pragma mark- ScreenShare
/**
    Socket API to connect with same room for  screen share
    @details EnxRoom will used this method to connect with same room for screen share publish
 */
- (void)connectScreenShare;

#pragma mark- BreakOut room
/**
    Socket API to create brealout room
    @param data information about breakout room
    @details EnxRoom will used this method to create breakout from from existing room
 */
-(void)requestForCreateBreakOutRoom:(NSDictionary * _Nonnull)data;
/**
    Socket API to create and invite for brealout room
    @param data information about breakout room
    @details EnxRoom will used this method to create and invite users for breakout from  existing room while using this method server will automatically invite user for join breakout room after created.
 */
-(void)requestForCreateAndInviteBreakOutRoom:(NSDictionary * _Nonnull)data;
/**
    Socket API to join breakout room
    @param data information about  join breakout room
    @details EnxRoom will used this method to join breakout room from existing room.
 */
-(void)requestTojoinBreakOutRoom:(NSDictionary * _Nonnull)data;
/**
    Socket API to invite participent
    @param data information about  invite participent
    @details EnxRoom will used this method to invite end user participent to join breakout room.
 */
-(void)inviteBreakOutRoom:(NSDictionary * _Nonnull)data;
/**
    Socket API to mute room
    @param data information about  mute
    @details EnxRoom will used this method to mute parentes room after join breakout room
 */
-(void)muteRoom:(NSDictionary * _Nonnull)data;
/**
    Socket API to unmute room
    @param data information about  unmute
    @details EnxRoom will used this method to unmute parentes room after join breakout room
 */
-(void)unMuteRoom:(NSDictionary * _Nonnull)data;
/**
    Socket API to connect with breakout room
    @details EnxRoom will used this method to connect with breakout room
 */
-(void)connectWithBreakOutRoom;
/**
    Socket API to pause room
    @param data information about  pause parents room
    @details EnxRoom will used this method to pause parentes room after join breakout room
 */
-(void)pauseRoom:(NSDictionary * _Nonnull)data;
/**
    Socket API to pause resume
    @param data information about  resume parents room
    @details EnxRoom will used this method to resume parentes room after join breakout room
 */
-(void)resumeRoom:(NSDictionary * _Nonnull)data;


#pragma mark- Knock - Knock Room

/**
    Socket API to allowed user in knock knock room
    @param clientId  - NSString  _Nonnull  client Id for the participent who going to get approved in knock- knock room
    @details EnxRoom will used this method to allowed  to get in any user in knock -knock based room
 */
- (void)approveAwaitedUser:(NSString*_Nonnull)clientId;

/**
    Socket API to deny user in knock knock room
    @param clientId  - NSString  _Nonnull  client Id for the participent who going to get deny in knock- knock room
    @details EnxRoom will used this method to deny  to get in any user in knock -knock based room
 */
- (void)denyAwaitedUser:(NSString*_Nonnull)clientId;

@end
