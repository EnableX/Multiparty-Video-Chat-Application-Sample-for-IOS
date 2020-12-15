@import WebRTC;
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "EnxClient.h"
#import "EnxSignalingChannel.h"
#import "EnxStream.h"
#import "EnxPlayerView.h"
@class EnxRoom;
@class Client;

/**
 @enum EnxRoomStatus
 */
typedef NS_ENUM(NSInteger, EnxRoomStatus) {
    EnxRoomStatusReady,
    EnxRoomStatusConnected,
    EnxRoomStatusDisconnected,
    EnxRoomStatusError,
    EnxConferenceExprired,
    EnxDisconnectedByModerator
};
typedef NS_ENUM(NSUInteger, EnxFilePosition) {
  Top,
  Center,
  Bottom
};
typedef NS_ENUM(NSInteger, EnxOutBoundCallState) {
    Initiated,
    Ringing,
    Connected,
    Failed,
    Disconnected
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

- (void)didRoomDisconnect:(NSArray * _Nullable)response;
/**
 Fired each time there is an error with the room.
 It doesn't mean the room has been disconnected. For example you could receive
 this message when one of the streams subscribed did fail for some reason.
 
 @param room Instance of the room where event happen.
 @param reason Text explaining the error. (Not always available)
 
 */
- (void)room:(EnxRoom *_Nullable)room didError:(NSArray *_Nullable)reason;

// called when error on event failure like publish, subscribe etc
- (void)room:(EnxRoom *_Nullable)room didEventError:(NSArray *_Nullable)reason;

// called pass info related to any event if required.
- (void)room:(EnxRoom *_Nullable)room didEventInfo:(NSDictionary *_Nullable)infoData;

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
 Fired when a data stream is received.
 
 @param room Instance of the room where event happen.
 @param stream The EnxStream received from.
 @param data stream message received.
 
 */
- (void)room:(EnxRoom *_Nullable)room didReceiveData:(NSDictionary *_Nullable)data
  fromStream:(EnxStream *_Nullable)stream;


- (void)room:(EnxRoom *_Nullable)room didStreamZoom:(NSDictionary *_Nullable)data;

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


-(void)didLogUpload:(NSArray *_Nullable)data;

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

/*
    This ACK method for Participent , When he/she will cancle their request floor
    Response will be
    {
        msg = Success;
        request =     {
            id = processFloorRequest;
            params =         {
                action = cancelFloor;
                clientId = "ClentID";
            };
        };
        result = 0;
    }
 */
- (void)didFloorCancelled:(NSArray *_Nullable)Data;
/*
    This ACK method for Participent , When he/she will finished their request floor
    after request floor accepted by any modiatore
 Response will be
    {
        msg = Success;
        request =     {
            id = processFloorRequest;
            params =         {
                action = finishFloor;
                clientId = "ClentID";
            };
        };
        result = 0;
    }
 */
- (void)didFloorFinished:(NSArray *_Nullable)Data;

/**
 This delegate invoke when Moderator accepts the floor request.
 
 Participant Delegate:
 
 Here, Data is result form EnxServer on receiving grantFloor event.
 
 Response:
 
 {
 "msg" : "Floor Request Granted"
 }
 
 */
- (void)didGrantedFloorRequest:(NSArray *_Nullable)Data;
/**
 
 Here, Data is result form EnxServer on receiving denyFloor event.
 
 Participant Delegate: Denied:
 
 Response:
 
 {
 "msg" : "Floor Request Denied"
 }
 */
- (void)didDeniedFloorRequest:(NSArray *_Nullable)Data;
/**
 
 Here, Data is result form EnxServer on releaseFloor event.
 
 Participant Delegate: Releases:
 
 Here, Data is result form EnxServer on receiving releaseFloor event.
 
 Response:
 
 {
 "msg" : "Floor Released"
 }
 
 */

- (void)didReleasedFloorRequest:(NSArray *_Nullable)Data;
/*
    This delegate method will notify to all available modiatore, Once any participent has cancled there floor request
 */
- (void)didCancelledFloorRequest:(NSArray *_Nullable)Data;
/*
   This delegate method will notify to all available modiatore, Once any participent has finished there floor request
*/
- (void)didFinishedFloorRequest:(NSArray *_Nullable)Data;

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
- (void)didhardMute:(NSArray *_Nullable)Data;


/**
 Moderator Delegates
 There would be listener for moderator when hardunmute used by moderator. For this delegates are:
 */
- (void)didhardUnMute:(NSArray *_Nullable)Data;


/**
 Paricipant Delegates
 There would be listener for paricipant when hardunmute used by moderator. For this delegates are:

 */
- (void)didHardMuteReceived:(NSArray *_Nullable)Data;


/**
 Paricipant Delegates
 There would be listener for paricipant when hardunmute used by moderator. For this delegates are:
 
 */
- (void)didHardunMuteReceived:(NSArray *_Nullable)Data;


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
//This delegate methods will return list of EnxStream
-(void)room:(EnxRoom *_Nullable)room didActiveTalkerList:(NSArray *_Nullable)Data;
//This delegate methods will return collectionView of EnxStream
-(void)room:(EnxRoom *_Nullable)room didActiveTalkerView:(UIView *_Nullable)view;

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
-(void)room:(EnxRoom *_Nullable)room didScreenShareStarted:(EnxStream *_Nullable)stream;
/**
 A Participant listens to this delegate to know about a screen shared by a user has stopped.
 
 @param room Instance of the room where event happen.
 
 */

-(void)room:(EnxRoom *_Nullable)room didScreenShareStopped:(EnxStream *_Nullable)stream;

//Screen Share Start/Stop ACK Delegate for Self User
/**
 A user will receive this ack delegate, when he/she will start screen share.
 @param room Instance of the room where event happen.
 @param Data ACK list of start Annotation
 
 */
-(void)room:(EnxRoom *_Nullable)room didStartScreenShareACK:(NSArray *_Nullable)Data;
/**
 A user will receive this ack delegate, when he/she will stop screen share.
 
 @param room Instance of the room where event happen.
 @param  Data ACK list of Stop Annotation
 */
-(void)room:(EnxRoom *_Nullable)room didStoppedScreenShareACK:(NSArray *_Nullable)Data;

//Canvas Delegate
/**
 
 A Participant listens to this delegate to know about a Canvas started by a user.
 @param room Instance of the room where event happen.
 @param Data list of required information to display canvas
 
 */
-(void)room:(EnxRoom *_Nullable)room didCanvasStarted:(EnxStream *_Nullable)stream;
/**
 A Participant listens to this delegate to know about that Canvas  has stopped by user.
 
 @param room Instance of the room where event happen.
 @param Data list of required information to remove canvas
 
 */
-(void)room:(EnxRoom *_Nullable)room didCanvasStopped:(EnxStream *_Nullable)stream;

//Annotation Start/Stop Delegate for End User
/**
 A Participant listens to this delegate to know about a Anotation started by a user.
 @param room Instance of the room where event happen.
 @param Data list of required information to display Annotation Stream
 
 */
-(void)room:(EnxRoom *_Nullable)room didAnnotationStarted:(NSArray *_Nullable)Data;
/**
 A Participant listens to this delegate to know about that Annotation  has stopped by user.
 
 @param room Instance of the room where event happen.
 @param Data list of required information to remove Annotation Stream
 */
-(void)room:(EnxRoom *_Nullable)room didAnnotationStopped:(NSArray *_Nullable)Data;

//Annotation Start/Stop Delegate for Self User
/**
 A Participant listens to this delegate to know about a Anotation started by a user.
 @param room Instance of the room where event happen.
 @param Data ACK list of start Annotation
 
 */
-(void)room:(EnxRoom *_Nullable)room didStartAnnotationACK:(NSArray *_Nullable)Data;
/**
 A Participant listens to this delegate to know about that Annotation  has stopped by user.
 
 @param room Instance of the room where event happen.
 @param  Data ACK list of Stop Annotation
 */
-(void)room:(EnxRoom *_Nullable)room didStoppedAnnotationACK:(NSArray *_Nullable)Data;

/*
 Canvas Start/Stop Delegate for Self User

 A Participant listens to this delegate to know about a canvas started by a user.
 @param room Instance of the room where event happen.
 @param Data ACK list of start canvas
 
 */
-(void)room:(EnxRoom *_Nullable)room didStartCanvasACK:(NSArray *_Nullable)Data;
/**
 A Participant listens to this delegate to know about that Canvas  has stopped by user.
 
 @param room Instance of the room where event happen.
 @param  Data ACK list of Stop Canvas
 */
-(void)room:(EnxRoom *_Nullable)room didStoppedCanvasACK:(NSArray *_Nullable)Data;

/**
 Fired when a bandWidth alert received from server.
 
 @param room Instance of the room where event happen.
 @param data bandWidthAlert info on a stream.
 
 */
- (void)room:(EnxRoom *_Nullable)room didBandWidthUpdated:(NSArray *_Nullable)data;


/**
 Fired when a canvas stream state change due to bandwidth change.
 
 @param room Instance of the room where event happen.
 @param data CanvasStateEvent info on a stream.
 
 */
- (void)room:(EnxRoom *_Nullable)room didCanvasStreamEvent:(NSArray *_Nullable)data;


/**
 Fired when a share stream state change due to bandwidth change.

 @param room Instance of the room where event happen.
 @param data ShareStateEvent info on a stream.
 
 */
- (void)room:(EnxRoom *_Nullable)room didShareStreamEvent:(NSArray *_Nullable)data;


/**
 This delegate for response on setting video quality of remote streams.
 */
-(void)room:(EnxRoom *_Nullable)room didSetVideoQuality:(NSArray *_Nullable)data;
/**
 This delegate Method Will Notify app user for any Audio media changes happen recentally(Like :- New device connected/Doisconnected).
 */
-(void)didNotifyDeviceUpdate:(NSString*_Nonnull)updates;

#pragma -mark Network Connection intrupted
/**
 Fired when a bandWidth alert received from server.
 
 @param room Instance of the room where event happen.
 @param data network intruption alert info.
 
 */
-(void)room:(EnxRoom*_Nonnull)room didConnectionInterrupted:(NSArray*_Nonnull)data;

#pragma -mark Network Connection Lost
/**
 Fired when a bandWidth alert received from server after 10sec .
 
 @param room Instance of the room where event happen.
 @param data network Time Out alert info.
 
 */
-(void)room:(EnxRoom*_Nonnull)room didConnectionLost:(NSArray*_Nonnull)data;

#pragma -mark Reconnect Success
/**
 Fired when a bandWidth alert received from server after 10sec .
 
 @param room Instance of the room where event happen.
 @param data Inform user for reconnect success.
 
 */
-(void)room:(EnxRoom*_Nonnull)room didUserReconnectSuccess:(NSDictionary*_Nonnull)data;


/*
    This method will update once stats enable and update to app user for stats
    @param statsData has all stats information.
 */
-(void)didReceiveStats:(NSArray * _Nonnull)statsData;

/*
 This method will update  stats Subscribe and UnSubscribe
 @param statsData has all stats information.
 */
-(void)didAcknowledgeStats:(NSArray * _Nonnull)subUnSubResponse;

/*
 This delegate method called to receive message at room Level.
 */
- (void)room:(EnxRoom *_Nonnull)room didMessageReceived:(NSArray *_Nullable)data;

/*
 This delegate method called to receive custom signaling event message at room Level.
 */
- (void)room:(EnxRoom *_Nonnull)room didUserDataReceived:(NSArray *_Nullable)data;

/*
This delegate method called When any of the user in same room will start sharing file.
*/
- (void)room:(EnxRoom *_Nonnull)room
didFileUploadStarted:(NSArray *_Nullable)data;

/*
This delegate method called When self user will start sharing file.
*/
- (void)room:(EnxRoom *_Nonnull)room
didInitFileUpload:(NSArray *_Nullable)data;

/*
This delegate method called When File available to download.
*/
- (void)room:(EnxRoom *_Nonnull)room
didFileAvailable:(NSArray *_Nullable)data;

/*
This delegate method called upload file is success.
*/
- (void)room:(EnxRoom *_Nonnull)room
didFileUploaded:(NSArray *_Nullable)data;

/*
This delegate method called upload file is failed.
*/
- (void)room:(EnxRoom *_Nonnull)room
didFileUploadFailed:(NSArray *_Nullable)data;

/*
This delegate method called When download of file success.
*/
- (void)room:(EnxRoom *_Nonnull)room
didFileDownloaded:(NSString *_Nullable)data;

/*
This delegate method called When file download failed.
*/
- (void)room:(EnxRoom *_Nonnull)room
didFileDownloadFailed:(NSArray *_Nullable)data;
/*
This delegate method called When file download initiated.
*/
- (void)room:(EnxRoom *_Nonnull)room
didInitFileDownload:(NSArray *_Nullable)data;

/*
This delegate method called When file download Cancel.
*/
- (void)room:(EnxRoom *_Nonnull)room
didFileDownloadCancelled:(NSArray *_Nullable)data;

/*
This delegate method called When file Upload Cancel.
*/
- (void)room:(EnxRoom *_Nonnull)room
didFileUploadCancelled:(NSArray *_Nullable)data;

/*
 This delegate called for advance options updates.
 */
- (void)room:(EnxRoom *_Nullable)room didAcknowledgementAdvanceOption:(NSDictionary *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)room didBatteryUpdates:(NSDictionary *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)room didAspectRatioUpdates:(NSArray *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)room didVideoResolutionUpdates:(NSArray *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)room didGetAdvanceOptions:(NSArray *_Nullable)data;

/*
 Confrence Time Alert
 */
- (void)room:(EnxRoom *_Nullable)room didConferenceRemainingDuration:(NSArray *_Nullable)data;
/* Extend Confrence callback*/
- (void)room:(EnxRoom *_Nullable)room didConferencessExtended:(NSArray *_Nullable)data;
#pragma mark- Switch user role Delegate

- (void)room:(EnxRoom *_Nullable)room didSwitchUserRole:(NSArray *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)room didUserRoleChanged:(NSArray *_Nullable)data;


#pragma mark- Send Data Delegate
- (void)room:(EnxRoom *_Nullable)room didAcknowledgSendData:(NSArray *_Nullable)data;

#pragma mark- Out Bond Call for App User
- (void)room:(EnxRoom *_Nullable)room didOutBoundCallInitiated:(NSArray *_Nullable)data;
- (void)room:(EnxRoom *_Nullable)room didDialStateEvents:(EnxOutBoundCallState)state;

//#pragma mark- Lock/Unlock Room Delegate
- (void)room:(EnxRoom *_Nullable)room didAckLockRoom:(NSArray *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)room didAckUnlockRoom:(NSArray *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)room didLockRoom:(NSArray *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)room didUnlockRoom:(NSArray *_Nullable)data;

//Delegate for drop and destroy
- (void)room:(EnxRoom *_Nullable)room didAckDropUser:(NSArray *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)room didAckDestroy:(NSArray *_Nullable)data;

#pragma mark- Room Live Streaming Events
- (void)room:(EnxRoom *_Nullable)channel didAckStartStreaming:(NSArray *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)channel didAckStopStreaming:(NSArray *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)channel didStreamingNotification:(NSArray *_Nullable)data;
#pragma mark - Acknowledgment for Add/Remove ping user

/**
 Fired when a Modiator request for add Ping user .
 
 @param room Instance of the room where event happen.
 @param data Inform user for pinged  success/failure.
 
 */
- (void)room:(EnxRoom *_Nullable)channel didAckPinUsers:(NSArray *_Nullable)data;
/**
 Fired when a Modiator request for remove Ping user .
 @param room Instance of the room where event happen.
 @param data Inform user for pinged  success/failure.
 */
- (void)room:(EnxRoom *_Nullable)channel didAckUnpinUsers:(NSArray *_Nullable)data;

- (void)room:(EnxRoom *_Nullable)channel didPinnedUsers:(NSArray *_Nullable)data;

@end
@protocol EnxFaceXDelegate <NSObject>
@optional
#pragma mark - FaceX Events
-(void)didPingBack:(NSArray*_Nullable)data;
-(void)didStartedClientUsage:(NSArray*_Nullable)data;
-(void)didStoppedClientUsage:(NSArray*_Nullable)data;
@end

@protocol EnxBroadCastDelegate <NSObject>
@optional
#pragma mark - EnxBroadCast  Events
-(void)didStartBroadCast:(NSArray*_Nullable)data;
-(void)didStoppedBroadCast:(NSArray*_Nullable)data;
-(void)broadCastDisconnected;
-(void)broadCastConnected;
-(void)failedToConnectWithBroadCast:(NSArray*_Nonnull)reason;
-(void)disconnectedByOwner;
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

/// EnxFaceXDelegate were this room will invoke methods as events.
@property (weak, nonatomic) id <EnxBroadCastDelegate> _Nullable broadCastDelegate;

/// EnxFaceXDelegate were this room will invoke methods as events.
@property (weak, nonatomic) id <EnxFaceXDelegate> _Nullable faceXDelegate;




///Here i am storing playerDelegate when join room called and set this playerDelegate to player when we create player internally.
@property (weak, nonatomic) id  _Nullable playerDelegate;

///// EnxRoomStatsDelegate delegate to receive stats.
///// Notice that you should also set *publishingStats* to YES.
//@property (weak, nonatomic) id <EnxRoomStatsDelegate> _Nullable statsDelegate;

/// EnxSignalingChannel signaling delegate instance associtated with this room.
/// Is not required for you to set this property manually.
@property EnxSignalingChannel * _Nullable signalingChannel;

/// The status of this Room.
@property (nonatomic, readonly) EnxRoomStatus status;

/// Enx Local stream options.
@property (readonly, nonatomic) NSDictionary * _Nullable streamOptions;
/// Enx Screen Share/Canvas stream options.
@property (readonly, nonatomic) NSDictionary * _Nullable options_screen;

/// Full response after signalling channel connect the server.
@property (nonatomic, readonly) NSMutableDictionary * _Nullable roomMetadata;

/// The Enx room id for this room instance.
@property (nonatomic, readonly) NSString * _Nullable roomId;

/// NSString stream id of the stream being published
//@property (readonly) NSString * _Nullable publishStreamId;

/// EnxStream referencing the stream being published.
@property (readonly) EnxStream * _Nullable publishStream;

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
//@property NSDictionary * _Nullable defaultSubscribingStreamOptions;

@property (nonatomic,readonly) BOOL moderatorHardMuteActiveState;
@property (nonatomic,readonly) BOOL participantHardMuteActiveState;
@property (nonatomic,readonly) BOOL isHardMuteRoom;
@property (nonatomic, readonly) BOOL isHardMuteUser;
@property (nonatomic,readonly) BOOL isVideoUserHardMute;
@property (nonatomic,readonly) BOOL isAudioOnlyStreams;
@property (nonatomic) BOOL isAudioOnlyRoom;
@property(nonatomic,readonly)BOOL chatOnlyRoom;

@property(nonatomic)BOOL isReconnectingAttampted;
//@property (nonatomic,strong) NSNumber *activeTalkerCount;
// connected clientId
@property (readonly,weak) NSString * _Nullable clientId;
// connected clientName
@property (readonly,weak) NSString * _Nullable clientName;
@property (readonly,nonatomic) NSString * _Nonnull userRole;
@property(readonly) int maxFrameLimits;
@property (readonly) NSDictionary * _Nullable roomInfo;
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

- (void)connect:(NSString *_Nonnull)token roomInfo:(NSDictionary * _Nullable)roomInfo advanceOptions:(NSArray *_Nullable)advanceOption;

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
 This API is only available during Lecture Mode of a Session. Each Participant can cancle their  floor request individually any time after reise hand request success using this API Call. This API calls are only available to users with role “Participant”
 */

- (void)cancelFloor;
/**
 This API is only available during Lecture Mode of a Session. Each Participant can Finish Floor request  individually after floor request accepted by any of the modiatore in same room using this API Call. This API calls are only available to users with role “Participant”
 */

- (void)finishFloor;
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
- (void)hardMute;

/**
 unmute all audio stream.
 
 Note: Hardmute functionality is only applicable to moderator.
 
 @see EnxRoomDelegate:didUnMutedAllUser:data:
 
 */
- (void)hardUnMute;

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

-(EnxStream *_Nullable)getLocalStream:(NSDictionary *_Nonnull)publishStreamInfo;


/**
 Speaker set active or not active
 
 @param state set true or false
 
 */

//- (void)speakerActive :(BOOL)state;

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

-(void)setAudioOnlyMode:(BOOL)audioOnly;

/**
opt which should be "Auto, HD , SD, LD and talker/canvas"
 This API use to request server to set the remote video stream in different quality.
 */
-(void)setReceiveVideoQuality:(NSDictionary*_Nonnull)opt;

/**
 streamType which should be "talker/canvas"
 This API use to return the remote video stream quality.
 */
-(NSString *_Nonnull)getReceiveVideoQuality:(NSString*_Nonnull)streamType;

/** This method for get event from stream if any unauthrozed event get called **/
-(void)getEnxSteamEventError:(NSString *_Nonnull)eventName;
/** This method Will return all list of connected Audio Device**/
-(NSArray*_Nonnull)getDevices;
/** This method Will return Current selected Audio device**/
-(NSString*_Nonnull)getSelectedDevice;
/** This method Will Switch to selected media device**/
-(void)switchMediaDevice:(NSString*_Nonnull)mediaName;


/** Client endpoint will call this method to a mute/unmute remote stream while application in the background. **/
-(void)stopVideoTracksOnApplicationBackground:(BOOL)flag;

/** Client endpoint will call this method to a mute/unmute remote stream while application in the foreground. **/
-(void)startVideoTracksOnApplicationForeground:(BOOL)flag;

/*
    Client End point will call this method to enable/disable stats by passing flag = true/false
 'True' for enable stats and 'False' for desable Stats
 */
-(void)enableStats:(BOOL)flag;

/*
 Client end point use this method to active/inactive audio of all subcribe streams.
 */
-(void)muteSubscribeStreamsAudio:(BOOL)flag;


-(void)sendMessage:(NSString *_Nonnull)message isBroadCast:(BOOL)broadcast recipientIDs:(NSArray *_Nullable)clientIds;

-(void)sendUserData:(NSDictionary *_Nonnull)message isBroadCast:(BOOL)broadcast recipientIDs:(NSArray *_Nullable)clientIds;

/* Client endpoint can set options at room level. */

-(void)setAdvanceOptions:(NSArray *_Nonnull)data;

/* To get Advance options set by client endpoint. */
-(void)getAdvanceOptions;

//Client endpoint can use this method to switch role.
-(void)switchUserRole:(NSString *_Nullable)clientId;
#pragma mark - File upload/download/Cancle
-(void)sendFiles:(BOOL)isBroadcast clientIds:(NSArray *_Nullable)clientIds;
-(void)downloadFile:(NSDictionary *_Nonnull)file autoSave:(BOOL)flag;
-(NSArray*_Nullable)getAvailableFiles;
-(void)cancelUpload:(int)jobID;
-(void)cancelAllUploads;
-(void)cancelDownload:(int)jobID;
-(void)cancelAllDownloads;
/*
    This method for initiating outbound call
 */
-(void)makeOutboundCall:(NSString*_Nonnull)number callerId:(NSString *_Nonnull)callerId;
-(void)setZoomFactor:(CGFloat)value clientId:(NSArray *_Nonnull)clientIds;
/* Annotations*/
-(void)startAnnotation:(EnxStream*_Nonnull)stream;
-(void)stopAnnotation;
/* Canvas*/
-(void)startCanvas:(UIView*_Nonnull)view;
-(void)stopCanvas;
/*
 This metho for Extend Confrence Duration
 */
-(void)extendConferenceDuration;
-(void)updateConfiguration:(NSDictionary *_Nonnull)data;

 // lock/Unlock Room
-(void)lockRoom;
-(void)unlockRoom;

-(void)enableProximitySensor:(BOOL)value;

//Drop user and Destroy Room API
-(void)dropUser:(NSArray *_Nonnull)clientIds;
-(void)destroy;
/* Get all available users in room*/
-(NSArray*_Nonnull)getUserList;
/* this method help to rearrange collection view*/
-(void)adjustLayout;

//Live broadcast Streaming API
-(void)startStreaming:(NSDictionary *_Nonnull)streamingConfig;
-(void)stopStreaming:(NSDictionary *_Nonnull)streamingConfig;
//Start Screen Share
/**
 
 This method is required instance of EnxScreenshareDelegate along with token
 @param token The auth token for room access. See initWithEncodedToken:
 for token composition details.
 */
-(void)connectWithScreenshare:(NSString *_Nonnull)token withScreenDelegate:(id<EnxBroadCastDelegate> _Nonnull)delegate;
-(void)startScreenShare;
-(void)sendVideoBuffer:(CMSampleBufferRef _Nonnull )sampleBuffer;
-(void)stopScreenShare;

//FaceX Methods
/*-(void)pingBack:(id<EnxFaceXDelegate>_Nullable)faceXDelegate;
-(void)startClientUsage:(NSDictionary *_Nonnull)data;
-(void)stopClientUsage:(NSDictionary *_Nonnull)data;*/

//pin user
-(void)pinUsers:(NSArray *_Nonnull)clientIds;
-(void)unpinUsers:(NSArray *_Nonnull)clientIds;
@end

