@import WebRTC;
#import <Foundation/Foundation.h>
#import "EnxClient.h"
#import "EnxSignalingChannel.h"
#import "EnxStream.h"
#import "EnxRoomStatsProtocol.h"
@class EnxRoom;
@class Client;

/**
 @enum EnxRoomStatus
 */
typedef NS_ENUM(NSInteger, EnxRoomStatus) {
    EnxRoomStatusReady,
    EnxRoomStatusConnected,
    EnxRoomStatusDisconnected,
    EnxRoomStatusError
};

/**
 @enum EnxRoomErrorStatus
 */
//typedef NS_ENUM(NSInteger, EnxRoomErrorStatus) {
//    EnxRoomErrorUnknown,
//    /// A generic error that comes from an EnxClient
//    EnxRoomErrorClient,
//    EnxRoomErrorClientFailedSDP,
//    /// A generic error that comes from EnxSignalingChannel
//    EnxRoomErrorSignaling
//};

///-----------------------------------
/// @name Protocols
///-----------------------------------

/**
 EnxRoomDelegate
 
 Will fire events related with EnxRoom state change.
 */

@protocol EnxRoomDelegate <NSObject>
@optional
/**
 Fired when server sent the streamId of the subscribed stream.
 
 @param room Instance of the room where event happen.
 @param stream The subscribed Stream object.
 
 */
- (void)room:(EnxRoom *_Nullable)room didSubscribeStream:(EnxStream *_Nullable)stream;

/**
 Fired when server has succesfully unsubscribed a stream.
 
 @param room Instance of the room where event happen.
 @param stream The unSubscribed Stream object.
 
 */
- (void)room:(EnxRoom *_Nullable)room didUnSubscribeStream:(EnxStream *_Nullable)stream;

/**
 Fired when server sent the streamId of the published stream.
 
 @param room Instance of the room where event happen.
 @param stream EnxStream being published.
 
 */
- (void)room:(EnxRoom *_Nullable)room didPublishStream:(EnxStream *_Nullable)stream;

/**
 Fired when server ACK to unpublish the requested stream by EnxRoom:unpublish.
 After this method is called the Room will close and nilify the publishing
 client. You need to unreference the publishing stream from your side to let
 the object be deallocated.
 
 @param stream The stream being unpublished.
 */
- (void)room:(EnxRoom *_Nullable)room didUnpublishStream:(EnxStream *_Nullable)stream;

/**
 Fired when server sent the recordingId of a stream being published and
 recorded.
 
 @param room Instance of the room where event happen.
 @param stream String representing the Id of the stream being recorded.
 @param recordingId String representing the Id of the recording of the stream.
 @param recordingDate moment when the server started to record the stream.
 
 */
- (void)room:(EnxRoom *_Nullable)room didStartRecordingStream:(EnxStream *_Nullable)stream withRecordingId:(NSString *_Nullable)recordingId recordingDate:(NSDate *_Nullable)recordingDate;
/**
 Fired when server failed to start recording the stream.
 
 @param room Instance of the room where event happen.
 @param stream String representing the Id of the stream being recorded.
 @param errorMsg The error message sent by the server.
 
 */
//- (void)room:(EnxRoom *)room didFailStartRecordingStream:(EnxStream *)stream
//withErrorMsg:(NSString *)errorMsg;


//delegate method when recording start successful
-(void)startRecordingEvent:(NSArray *_Nullable)response;

//when recording fails.
//-(void)recordingDidFail:(NSString *)response;

//delegate method when recording stop successful
-(void)stopRecordingEvent:(NSArray *_Nullable)response;

//when recording stop fails.
//-(void)recordingDidStopFail:(NSString *)response;

/**
 Fired when signaling channel connected with Enx Room.
 
 @param room Instance of the room where event happen.
 
 roomMetadata sample:
 {
 defaultVideoBW = 300;
 iceServers = (
 {
 url = "stun:stun.l.google.com:19302";
 },
 {
 credential = secret;
 url = "turn:xxx.xxx.xxx.xxx:443";
 username = me;
 }
 );
 id = 591df649e29e562067143117;
 maxAudioBW = 64;
 maxVideoBW = 300;
 streams =(
 {
 audio = 1;
 id = 208339986973492030;
 video = 1;
 }
 );
 }
 
 */
- (void)room:(EnxRoom *_Nullable)room didConnect:(NSDictionary *_Nullable)roomMetadata;

- (void)roomDidDisconnected:(EnxRoomStatus)status;

/**
 Fired each time there is an error with the room.
 It doesn't mean the room has been disconnected. For example you could receive
 this message when one of the streams subscribed did fail for some reason.
 
 @param room Instance of the room where event happen.
 @param reason Text explaining the error. (Not always available)
 
 */
- (void)room:(EnxRoom *_Nullable)room didError:(NSString *_Nullable)reason;


- (void)room:(EnxRoom *_Nullable)room didReconnect:(NSString *_Nullable)reason;



/**
 Fired each time the room changed his state.
 
 @param room Instance of the room where event happen.
 @param status EnxRoomStatus value.
 
 */
- (void)room:(EnxRoom *_Nullable)room didChangeStatus:(EnxRoomStatus)status;

/**
 Event fired once a new stream has been added to the room.
 
 It is up to you to subscribe that stream or not.
 It is worth to notice that your published stream will not be notified
 by this method, use EnxRoomDelegate:didPublishStream: instead.
 
 @param room Instance of the room where event happen.
 @param stream EnxStream object (not subscribed yet), that were just added
 to the room.
 */
- (void)room:(EnxRoom *_Nullable)room didAddedStream:(EnxStream *_Nullable)stream;

/**
 Fired when a stream in a room has been removed, not necessary the
 stream was being consumed/subscribed.
 
 @param room Instance of the room where event happen.
 @param stream The removed stream.
 
 @discusion After this method return the stream will be destroyed.
 
 */
- (void)room:(EnxRoom *_Nullable)room didRemovedStream:(EnxStream *_Nullable)stream;

/**
 Fired when a data stream is received.
 
 @param room Instance of the room where event happen.
 @param stream The EnxStream received from.
 @param data stream message received.
 
 */
- (void)room:(EnxRoom *_Nullable)room didReceiveData:(NSDictionary *_Nullable)data
  fromStream:(EnxStream *_Nullable)stream;



/**
 Fired when stream attribute updated.
 
 @param room Instance of the room where event happen.
 @param stream The stream that updated his attributes.
 
 @discusion Look EnxStream:streamAttributes to know which.
 
 */
- (void)room:(EnxRoom *_Nullable)room didUpdateAttributesOfStream:(EnxStream *_Nullable)stream;

//On Logs upload success
//-(void)logsUploadedSuccess:(NSString *)message;

//On Logs uploaded failure
//-(void)logsUploadedFailure:(NSString *)message;

-(void)didLogUpload:(NSString *_Nullable)message;

#pragma mark- ChairControl
#pragma mark- CC
/**
 
 Here, Data is result form EnxServer on requestFloor event.
 
 Response:
 
 {
 "msg" : "Floor Request Received",
 "result" : 1701
 }
 
 */
- (void)didFloorRequested:(NSArray *_Nullable)Data;
/**
 This delegate invoke when Moderator accepts the floor request.
 
 Participant Delegate:
 
 Here, Data is result form EnxServer on receiving grantFloor event.
 
 Response:
 
 {
 "mess" : "Floor Request Granted"
 }
 
 */
- (void)didGrantFloorRequested:(NSArray *_Nullable)Data;
/**
 
 Here, Data is result form EnxServer on receiving denyFloor event.
 
 Participant Delegate: Denied:
 
 Response:
 
 {
 "mess" : "Floor Request Denied"
 }
 */

- (void)didDenyFloorRequested:(NSArray *_Nullable)Data;
/**
 
 Here, Data is result form EnxServer on releaseFloor event.
 
 Participant Delegate: Releases:
 
 Here, Data is result form EnxServer on receiving releaseFloor event.
 
 Response:
 
 {
 "mess" : "Floor Released"
 }
 
 */

- (void)didReleaseFloorRequested:(NSArray *_Nullable)Data;

#pragma mark- ChairControl - Moderator Delegate
/**
 
 There would be listener for participant and moderator when participant request floor. For this delegates are:
 
 Moderator Delegates:
 
 Here, Data is result form EnxServer on receiving requestFloor event.
 
 Response:
 
 {
 "clientId" : "720aa92f-193f-4e9e-a62b-48b72cd196a8",
 "name" : "iOS"
 }
 
 */
- (void)didFloorRequestReceived:(NSArray *_Nullable)Data;
/**
 
 Here, Data is result form EnxServer on grantFloor,releaseFloor and denyFloor event.
 
 Moderator Delegate:
 
 Response:
 
 Here, Data is result form EnxServer on grantFloor event.
 
 {
 "msg" : "Floor Granted",
 "result" : 1708
 }
 
 Here, Data is result form EnxServer on releaseFloor event.
 
 {
 "msg": "Floor Released",
 "result": "1712"
 }
 
 Here, Data is result form EnxServer on denyFloor event.
 {
 "msg": "Floor Denied",
 "result": "1709"
 }
 
 */
- (void)didProcessFloorRequested:(NSArray *_Nullable)Data;


#pragma mark-  HardMute Delegate


/**
 Moderator Delegates
 There would be listener for moderator when hardmute used by moderator. For this delegates are:
 */
- (void)didMutedAllUser:(NSArray *_Nullable)Data;


/**
 Moderator Delegates
 There would be listener for moderator when hardunmute used by moderator. For this delegates are:
 */
- (void)didUnMutedAllUser:(NSArray *_Nullable)Data;


/**
 Paricipant Delegates
 There would be listener for paricipant when hardunmute used by moderator. For this delegates are:

 */
- (void)didHardMutedAll:(NSArray *_Nullable)Data;


/**
 Paricipant Delegates
 There would be listener for paricipant when hardunmute used by moderator. For this delegates are:
 
 */
- (void)didHardUnMuteAllUser:(NSArray *_Nullable)Data;


/**
 
 There would be listener for moderator when hardmute used by moderator. For this delegates are:
 
 Moderator Delegates
 */
- (void)didMutedSingleUser:(NSArray *_Nullable)Data;
/**
 
 There would be listener for moderator when hardmute used by moderator. For this delegates are:
 
 Moderator Delegates
 
 */
- (void)didUnMutedSingleUser:(NSArray *_Nullable)Data;


/**
 
 There would be listener for paricipant when hardmute used by moderator. For this delegates are:
 
 Paricipant Delegates
 
 */
- (void)didHardMutedSingleUser:(NSArray *_Nullable)Data;
/**
 
 There would be listener for paricipant when hardmute used by moderator. For this delegates are:
 Paricipant Delegates
 
 */
- (void)didHardUnMuteSingleUser:(NSArray *_Nullable)Data;

/**
 
 This API is designed to return a JSON Structure containing the user connected to the Room.
 
 
 @param room Instance of the room where event happen.
 
 Note the result for publishers also include associated stream-Ids. Refer "Return JSON" below.
 
 This delegate call when any user join the room.

 */
-(void)room:(EnxRoom *_Nullable)room userDidJoined:(NSArray *_Nullable)Data;

/**
  This API is designed to return a JSON Structure containing the user disconnected to the Room.
 
 This delegate call when any user leave the room.
 
 @param room Instance of the room where event happen.
 
 */
-(void)room:(EnxRoom *_Nullable)room userDidDisconnected:(NSArray *_Nullable)Data;


/**
 
 Moderator get raish hand and approve hand list
 
 @param room Instance of the room where event happen.
 
 */
- (void)room:(EnxRoom *_Nullable)room ChairControlStates:(NSMutableDictionary *_Nullable)data;

#pragma mark- recording Participant delegate

/**
 
 To start Recording Participant delegate
 Paricipant delegate

 */
-(void)roomRecordOn:(NSArray *_Nullable)Data;

/**
 To stop Recording Participant delegate
 Paricipant delegate
 */

-(void)roomRecordOff:(NSArray *_Nullable)Data;

//Active Talker Delegate
/**
 
 A Participant get of active talker list
 @param room Instance of the room where event happen.
 
 */
-(void)room:(EnxRoom *_Nullable)room activeTalkerList:(NSArray *_Nullable)Data;
/**
 
 A Participant get of max number of talker count .
 @param room Instance of the room where event happen.

 */
-(void)room:(EnxRoom *_Nullable)room didGetMaxTalkers:(NSArray *_Nullable)Data;
/**
 
 A Participant get of number of talker count .
 @param room Instance of the room where event happen.
 
 */
-(void)room:(EnxRoom *_Nullable)room didGetTalkerCount:(NSArray *_Nullable)Data;

/**
  A Participant set of number of talker count .
  @param room Instance of the room where event happen.
*/
-(void)room:(EnxRoom *_Nullable)room didSetTalkerCount:(NSArray *_Nullable)Data;

//Share Screen Delegate
/**
 
 A Participant listens to this delegate to know about a screen shared started by a user.
 @param room Instance of the room where event happen.
 
 */
-(void)room:(EnxRoom *_Nullable)room screenSharedStarted:(NSArray *_Nullable)Data;
/**
 A Participant listens to this delegate to know about a screen shared by a user has stopped.
 
 @param room Instance of the room where event happen.
 
 */
-(void)room:(EnxRoom *_Nullable)room screenShareStopped:(NSArray *_Nullable)Data;

/**
 Fired when a bandWidth alert received from server.
 
 @param room Instance of the room where event happen.
 @param data bandWidthAlert info on a stream.
 
 */
- (void)room:(EnxRoom *_Nullable)room didBandWidthUpdated:(NSArray *_Nullable)data;

@end

///-----------------------------------
/// @name Interface Declaration
///-----------------------------------

/*
 Interface responsable of publshing/consuming streams in a given EnxRoom.
 */
@interface EnxRoom : NSObject <EnxSignalingChannelRoomDelegate, EnxClientDelegate>

///-----------------------------------
/// @name Initializers
///-----------------------------------

/**
 Create an EnxRoom with the given EnxRoomDelegate.
 
 Notice that if initialize EnxRoom like this, you will never be able to
 publish/subscribe streams without first call method connect:
 method.
 @see connect:
 
 @param roomDelegate EnxRoomDelegate instance for this room.
 
 
 @return instancetype
 */
- (instancetype _Nonnull )initWithDelegate:(id<EnxRoomDelegate>_Nonnull)roomDelegate;

///-----------------------------------
/// @name Properties
///-----------------------------------

/// EnxRoomDelegate were this room will invoke methods as events.
@property (weak, nonatomic) id <EnxRoomDelegate> _Nullable delegate;

/// EnxRoomStatsDelegate delegate to receive stats.
/// Notice that you should also set *publishingStats* to YES.
@property (weak, nonatomic) id <EnxRoomStatsDelegate> _Nullable statsDelegate;

/// EnxSignalingChannel signaling delegate instance associtated with this room.
/// Is not required for you to set this property manually.
@property EnxSignalingChannel * _Nullable signalingChannel;

/// The status of this Room.
@property (nonatomic, readonly) EnxRoomStatus status;

/// Full response after signalling channel connect the server.
@property (nonatomic, readonly) NSDictionary * _Nullable roomMetadata;

/// The Enx room id for this room instance.
@property (nonatomic, readonly) NSString * _Nullable roomId;

/// NSString stream id of the stream being published
@property (readonly) NSString * _Nullable publishStreamId;

/// EnxStream referencing the stream being published.
@property (weak, readonly) EnxStream * _Nullable publishStream;

/// EnxStream streams in the room.
@property (readonly) NSMutableDictionary * _Nullable streamsByStreamId;

/// List of remote EnxStream streams available in this room.
/// They might be subscribed or not.
@property (readonly) NSArray * _Nullable remoteStreams;

/// BOOL set/get enable recording of the stream being published.
@property BOOL recordEnabled;

/// BOOL to check active talker enable or not
@property (readonly) BOOL isRoomActiveTalker;

/// BOOL is P2P kind of room.
@property (readonly) BOOL peerToPeerRoom;

/// RTC Factory shared by streams of this room.
@property RTCPeerConnectionFactory * _Nullable peerFactory;

/// BOOL enable/disable log publishing stats.
/// Stats are collected each 2 seconds max, having this flag on produces
/// console output, take a look to EnxRoomStatsDelegate to being able
/// to receive events when stats are collected.
@property BOOL publishingStats;

/// Represent a dictionary with the default values that will be sent at the
/// moment of subscribe an EnxStream.
@property NSMutableDictionary * _Nullable defaultSubscribingStreamOptions;

@property (nonatomic) BOOL moderatorHardMuteActiveState;
@property (nonatomic) BOOL participantHardMuteActiveState;
@property (nonatomic) BOOL isHardMuteRoom;
@property (nonatomic) BOOL isHardMuteUser;
@property (nonatomic) BOOL isVideoUserHardMute;
//@property (nonatomic,strong) NSNumber *activeTalkerCount;

@property (nonatomic,strong) NSString * _Nullable clientId;
@property (nonatomic,strong) NSArray * _Nullable userList;

///-----------------------------------
/// @name Public Methods
///-----------------------------------

/**
 
 This method is required if you have instanciated EnxRoom class without providing a token.
 
 @param token The auth token for room access. See initWithEncodedToken:
 for token composition details.
 
 @see EnxRoomDelegate:room:didConnect:
 @see EnxRoomDelegate:room:didError:
 */

- (void)connect:(NSString *_Nonnull)token;

/**
 Publishes a given EnxStream with given options.
 
 @param stream The stream from where we will be publishing.
 
 @see EnxRoomDelegate:room:didPublishStream:
 */
- (void)publish:(EnxStream *_Nonnull)stream;

/**
 Un-Publish the stream being published.
 */
- (void)unpublish;

/**
 Subscribe to a remote stream.
 
 @param stream EnxStream object containing a valid streamId.
 
 You should be connected to the room before subscribing to a stream.
 To know how to get streams ids take a look at the following methods:
 @see EnxRoomDelegate:room:didAddedStream:
 
 @returns Boolean indicating if started to signaling to subscribe the
 given stream.
 */
- (BOOL)subscribe:(EnxStream *_Nonnull)stream;

/**
 Unsubscribe from a remote stream.
 
 @param stream The stream you want to unsubscribe.
 @see EnxRoomDelegate:room:didUnSubscribeStream:
 */
- (void)unsubscribe:(EnxStream *_Nonnull)stream;

/**
 Leave the room.
 
 RTC and WS connections will be closed.
 
 To close the Enx and WC connection use method:
 
 */
- (void)disconnect;

/**
 
 To Forcefully disconnect other users(participant)
 
 */
-(void)disconnectUsers:(NSArray *_Nonnull)clientIds role:(NSArray *_Nonnull)role;

/**
 
 To post Enx client SDK logs to server use the below method.
 
 Note: To post client logs, first developer needs to enable the logs.
 
 @see EnxRoomDelegate:room:logsUploadedSuccess:
 @see EnxRoomDelegate:room:logsUploadedFailure:
 
 
 */
-(void)postClientLogs;

/**
 
 To start Recording
 
 @see EnxRoomDelegate:room:startRecordingEvent:
 
 
 */
-(void)startRecord;

/**
 
 To stop Recording
 
 @see EnxRoomDelegate:room:stopRecordingEvent:
 
 */
-(void)stopRecord;

#pragma mark - CC
/**
 This API is only available during Lecture Mode of a Session. Each Participant Raise hand Control can individually be asked to Join the floor using this API Call. This API calls are only available to users with role “Participant”
 */

- (void)requestFloor;
/**
 
 This API is only available during Lecture Mode. Each Participant requested Floor Control can individually be granted access to the Floor using this API Call. These API calls are only available to users with role “Moderator”.
 
 @param clientId It’s the Client ID for the participant whom access is being granted.
 
  Delegate (Optional). If you need to handle the success or failure of the action, then pass a function name here to which you look to receive call back.
 
 
 Moderator delegate
 @see EnxRoomDelegate:didProcessFloorRequested:data:
 
 Paricipant delegate
 @see EnxRoomDelegate:didGrantFloorRequested:data:
 
 */
-(void)grantFloor:(NSString *_Nonnull)clientId;

/**
 
 This API is only available during Lecture Mode of a Session. Each Participant granted Floor Control can individually be asked to release the floor Control using this API Call. This API calls are only available to users with role “Moderator”.
 
 @param clientId It’s the Client ID for the participant who is being denied access to the floor.
 
  Status  Developer pass status as a "releaseFloor".
 
  Delegate (Optional). If you need to handle the success or failure of the action, then pass a function name here to which you look to receive call back.
 
 
 Moderator delegate
 @see EnxRoomDelegate:didProcessFloorRequested:data:
 
 Paricipant delegate
 @see EnxRoomDelegate:didReleaseFloorRequested:data:
  */

-(void)releaseFloor:(NSString *_Nonnull)clientId;
/**
 
 This API is only available during Lecture Mode of a Session. Each Participant requested Floor Control can individually be denied access to the Floor using this API Call. This API calls are only available to users with role “Moderator”.
 
 @param clientId  It’s the Client ID for the participant who is being denied access to the floor.
 
 
  Delegate (Optional). If you need to handle the success or failure of the action, then pass a function name here to which you look to receive call back.
 
 
 Moderator delegate
 @see EnxRoomDelegate:didProcessFloorRequested:data:
 
 Paricipant delegate
 @see EnxRoomDelegate:didDenyFloorRequested:data:
 
 */
-(void)denyFloor:(NSString *_Nonnull)clientId;




#pragma mark-
/**
 mute all audio stream.
 
 Note: Hardmute functionality is only applicable to moderator.
 
 @see EnxRoomDelegate:didMutedAllUser::data:
 
 */
- (void)muteAllUser;

/**
 unmute all audio stream.
 
 Note: Hardmute functionality is only applicable to moderator.
 
 @see EnxRoomDelegate:didUnMutedAllUser:data:
 
 */
- (void)unMuteAllUser;

/**
 mute single audio stream.
 
 Note: Hardmute functionality is only applicable to moderator.
 
 @param clientId is the participant who is being mute to the floor.
 
 @see EnxRoomDelegate:didMutedSingleUser:::data:
 
 */

- (void)muteSingleUser:(NSString*_Nonnull)clientId;

/**
 unmute single audio stream.
 
 Note: Hardmute functionality is only applicable to moderator.
 
 @param clientId is the participant who is being mute to the floor.
 
 @see EnxRoomDelegate:didUnMutedSingleUser:::data:
 
 */

- (void)unMuteSingleUser:(NSString*_Nonnull)clientId;

/**
 
 A Stream carries different type of media . Audio, Video and/or Data. This stream gets transferred towards remote end points through EnableX Media Servers where it’s played and interacted with.
 
 @param publishStreamInfo
 
 Where publishStreamInfo is:
 {
 "audio": true,
 "video": true,
 "data": true,
 "name": "iOS",
 "type" : "public"
 
 }
 
 @returns EnxStream.
 
 */
-(EnxStream *_Nullable)initlocalStream:(NSDictionary *_Nonnull)publishStreamInfo;

/**
 Speaker set active or not active
 
 @param state set true or false
 
 */

- (void)speakerActive :(BOOL)state;

/**
 This method returns user-meta information about the user connected on a End-POint.
 
 @returns NSDictionary.
 
 */

-(NSDictionary *_Nullable)Whoami;

#pragma mark- AT
/**
 This method is available for all users during Active Talker Mode. Using this method, you can get maximum number of allowed Active Talkers in the room.
 
 @see EnxRoomDelegate:didGetMaxTalkers:data:
 
 */
-(void)getMaxTalkers;
/**
 
 The getTalkerCount method is used to know opted streams in Active Talker.
 
 @see EnxRoomDelegate:didGetTalkerCount:data:
 
 
 */
-(void)getTalkerCount;
/**
 
 The setTalkerCount method is used to opt total number of streams to receive at a Client End point in Active Talkers.
 
 @param number to set total number of streams opted to receive in Active Talker.
 
 @see EnxRoomDelegate:didSetTalkerCount:data:
 
 */
-(void)setTalkerCount:(NSInteger)number;

@end
