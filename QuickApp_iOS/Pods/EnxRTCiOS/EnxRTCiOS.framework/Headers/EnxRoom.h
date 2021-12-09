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
 Description :- These @enum, use to validate connected room state
 */

typedef NS_ENUM(NSInteger, EnxRoomStatus) {
    EnxRoomStatusReady,
    EnxRoomStatusConnected,
    EnxRoomStatusDisconnected,
    EnxRoomStatusError,
    EnxConferenceExprired,
    EnxDisconnectedByModerator
};
/**
 @enum EnxOutBoundCallState
 Description :- These @enum, use to validate current outbond call state
 */

typedef NS_ENUM(NSInteger, EnxOutBoundCallState) {
    Initiated,
    Ringing,
    Connected,
    Failed,
    Disconnected,
    Timeout
};
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
 @Details This room event will notify to the user, who has subscribed any stream after connected to room
 */
- (void)room:(EnxRoom *_Nullable)room didSubscribeStream:(EnxStream *_Nullable)stream;

/**
 Fired when server has succesfully unsubscribed a stream.
 @param room Instance of the room where event happen.
 @param stream The unSubscribed Stream object.
 @Details This room event will notify to the user, who has unsubscribed any stream after connected to room and has all ready subscribe that stream
 */
- (void)room:(EnxRoom *_Nullable)room didUnSubscribeStream:(EnxStream *_Nullable)stream;

/**
 Fired when server sent the streamId of the published stream.
 @param room Instance of the room where event happen.
 @param stream EnxStream being published.
 @Details This room event will notify to the user, who has published their owne stream after connected to room
 */
- (void)room:(EnxRoom *_Nullable)room didPublishStream:(EnxStream *_Nullable)stream;

/**
 Fired when server ACK to unpublish the requested stream by EnxRoom:unpublish.
 After this method is called the Room will close and nilify the publishing
 client. You need to unreference the publishing stream from your side to let
 the object be deallocated.
 @param room Instance of the room where event happen.
 @param stream The stream being unpublished.
 @Details This room event will notify to the user, who has unpublished their owne stream after connected to room, and has all ready published their stream
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

//Currentaly this delegate method not in used
- (void)room:(EnxRoom *_Nullable)room didStartRecordingStream:(EnxStream *_Nullable)stream withRecordingId:(NSString *_Nullable)recordingId recordingDate:(NSDate *_Nullable)recordingDate;
/**
 Fired when server failed to start recording the stream.
 @param room Instance of the room where event happen.
 @param stream String representing the Id of the stream being recorded.
 @param errorMsg The error message sent by the server.
 */

/**
 Fired when recording start successful
 @param response Instance of the room where event happen.
 @Details This delegate method will notify to all end users, once room recording has started successfully.
 */
-(void)startRecordingEvent:(NSArray *_Nullable)response;

/**
 Fired when recording stop successful
 @param response Instance of the room where event happen.
 @Details This delegate method will notify to all end users, once room recording has stopped successfully.
 */
-(void)stopRecordingEvent:(NSArray *_Nullable)response;

/**
 Fired when signaling channel connected with Enx Room.
 @param room Instance of the room where event happen.
 @param roomMetadata has all information about connected room , Example as below
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
 @Details This delegate method will notify to current user after room connected success ,who is trying to connect with available room. This method will give roomMetadata which has all details of connected room.
 */
- (void)room:(EnxRoom *_Nullable)room didConnect:(NSDictionary *_Nullable)roomMetadata;
/**
 Fired when used will disconnect room by hem self or by modiatore
 @param response has all information about disconnect room
 @Details This delegate method will notify to current user after room disconnect success , Disconnect happen either by self or by modiatore or by room duration exrire.
 Here "response" will carry information about disconnection of room.
 */
- (void)didRoomDisconnect:(NSArray * _Nullable)response;
/**
 Fired each time there is an error with the room.
 It doesn't mean the room has been disconnected. For example you could receive
 this message when one of the streams subscribed did fail for some reason.
 @param room Instance of the room where event happen.
 @param reason Text explaining the error. (Not always available)
 @Details This delegate method will notify to the user , who try to connect with any confrence but its has failed due to any resion like- token not valid, room full, not a valid user to join etc..
 Here reason has details about failure of room connect.
 */
- (void)room:(EnxRoom *_Nullable)room didError:(NSArray *_Nullable)reason;

/**
 Fired each time whenever user will do some action and its failed due to invalide action type or action state.
 @param room Instance of the room where event happen.
 @param reason Its will give an Array of dictonary with cause of failure with their error code.
 @Details This delegate method will notify to the user , who try to consume any EnxRoom API or EnxStream API again and again with invalid sate, as sdk will maintain all event state. For example- User will do video mute and after successfully mute video or in progress to mute video, he is try agin to mute video.
 */
- (void)room:(EnxRoom *_Nullable)room didEventError:(NSArray *_Nullable)reason;

/**
 Fired to pass info related to any event if required.
 @param room Instance of the room where event happen.
 @param infoData Its will give a dictonary with details of events
 @Details This delegate method will notify to the user , who try to consume any EnxRoom API or EnxStream API with details for the events
 */
// called pass info related to any event if required.
- (void)room:(EnxRoom *_Nullable)room didEventInfo:(NSDictionary *_Nullable)infoData;

/**
 Fired to inform user once reconnect happen.
 @param room Instance of the room where event happen.
 @param reason Its will give a NSString with cause of reconnect
 @Details This delegate method will notify to the user, with cause of reconnect, reconnect can happen if network connect got disconnect or ice connection has failed etc....
 */
- (void)room:(EnxRoom *_Nullable)room didReconnect:(NSString *_Nullable)reason;

/**
 Fired each time the room changed his state.
 @param room Instance of the room where event happen.
 @param status EnxRoomStatus value.
 @Details This delegate method will notify to the user with room current state or if state has changed.
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

/**
 Fired when a user did zoom for remote user stream
 @param room Instance of the room where event happen.
 @param data has the information of zoom coordinate
 @Details This delegate method will notify to end user about zoom of their stream, for example- in room there are 4 participent like p1,p2,p3,p4 now p1 try to zoom p4 , so p4 will get a signaling notification and zoom their local scream.
 */
- (void)room:(EnxRoom *_Nullable)room didStreamZoom:(NSDictionary *_Nullable)data;

/**
 Fired when stream attribute updated.
 @param room Instance of the room where event happen.
 @param stream The stream that updated his attributes.
 @Details Look EnxStream:streamAttributes to know which.
 */
- (void)room:(EnxRoom *_Nullable)room didUpdateAttributesOfStream:(EnxStream *_Nullable)stream;


/**
 Fired when user will uplaod their sdk logs to server
 @param data has detail informtion of upload logs
 @Details This delegate method will inform to self user , who is uploading their sdk logs to server.
 */
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
/**
    Fired when any participent  will will request for floor access
    @param Data has detail informtion about floor
    @Details- This delegate method for participent who requested for floor access.
 */
- (void)didFloorRequested:(NSArray *_Nullable)Data;

/**
 This ACK method for Participent , When he/she will cancle their request floor
 @param Data has detail informtion about floor
 @Details This ACK method for Participent only, When he/she will cancle their request floor

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
/**
    Fired when any participent will finished their floor access.
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
 @param Data has detail informtion about floor
 @Details This ACK method for Participent , When he/she will finished their request floor
 after request floor accepted by any modiatore
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
 @param Data has detail informtion about floor
 @Details This delegate invoke when Moderator accepts the floor request.
 */
- (void)didGrantedFloorRequest:(NSArray *_Nullable)Data;
/**
 This delegate methos will invoke , when moderator will diny any participent floor access request
 Here, Data is result form EnxServer on receiving denyFloor event.
 
 Participant Delegate: Denied:
 
 Response:
 
 {
 "msg" : "Floor Request Denied"
 }
 */
- (void)didDeniedFloorRequest:(NSArray *_Nullable)Data;
/**
 This delegate methos will invoke , when moderator will release/finished any participent  whover floor request has approved.
 Here, Data is result form EnxServer on releaseFloor event.
 
 Participant Delegate: Releases:
 
 Here, Data is result form EnxServer on receiving releaseFloor event.
 
 Response:
 
 {
 "msg" : "Floor Released"
 }
 */

- (void)didReleasedFloorRequest:(NSArray *_Nullable)Data;

/**
    Fired when moderator will cancle any floor access , requested by participent
    @param Data has detail informtion about moderator , who has cancle the floor access.
    @Details This delegate method will notify to all available modiatore, Once any participent has cancled there floor request
 */

- (void)didCancelledFloorRequest:(NSArray *_Nullable)Data;

/**
    Fired when moderator will finished  any floor access , requested by participent
    @param Data has detail informtion about moderator , who has finished the floor access.
    @Details This delegate method will notify to all available modiatore, Once any participent has finished there floor request
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

/*
 This is an acknowledgment method for the inviteToFloor to the moderator.
 */
-(void)didACKInviteToFloorRequested:(NSArray *_Nullable)Data;
/*
 This event method will notify to all moderator in the same session (including the owner of the event), that invitation received by participant
 **/
-(void)didInviteToFloorRequested:(NSArray *_Nullable)Data;
/*
 This delegate method for Participant , How or she will receive handover floor access.
 */
-(void)didInvitedForFloorAccess:(NSArray *_Nullable)Data;

/*
 This is an event method for the all  moderator including owner of the API and participant which has received handover floor request .
 */
-(void)didCanceledFloorInvite:(NSArray *_Nullable)Data;

/*
 This is an event method for the all  moderator including owner of the API and participant which has received handover floor request .
 */
-(void)didRejectedInviteFloor:(NSArray *_Nullable)Data;
/*
 This is an event method for the all  moderator including owner of the API and invitee participant,  once invitee has accept the invite floor  .
 */
-(void)didAcceptedFloorInvite:(NSArray *_Nullable)Data;

#pragma mark-  HardMute Delegate

/**
 Fired when any modiatore in confrence will do room mute
@param Data has detail informtion about room mute
 @Details This delegate method for all modiatores in room including owner of hard mute room will got notify*/
/**
 Moderator Delegates
 There would be listener for moderator when hardmute used by moderator. For this delegates are:
 */
- (void)didhardMute:(NSArray *_Nullable)Data;

/**
 Fired when any modiatore in confrence will do room unmute
@param Data has detail informtion about room unmute
 @Details This delegate method for all modiatores in room including owner of hard unmute room will got notify*/
/**
 Moderator Delegates
 There would be listener for moderator when hardunmute used by moderator. For this delegates are:
 */
- (void)didhardUnMute:(NSArray *_Nullable)Data;

/**
 Fired when any modiatore in confrence will do room mute
@param Data has detail informtion about room mute
 @Details This delegate method for all participent in room once any moderator will do room mute in same confrence
 */
/**
 Paricipant Delegates
 There would be listener for paricipant when hardunmute used by moderator. For this delegates are:

 */
- (void)didHardMuteReceived:(NSArray *_Nullable)Data;

/**
 Fired when any modiatore in confrence will do room unmute
@param Data has detail informtion about room unmute
 @Details This delegate method for all participent in room once any moderator will do room unmute in same confrence
 */
/**
 Paricipant Delegates
 There would be listener for paricipant when hardunmute used by moderator. For this delegates are:
 
 */
- (void)didHardunMuteReceived:(NSArray *_Nullable)Data;

/**
 Fired when any modiatore in confrence will do mute to any participent/other modiatore in same room
@param Data has detail informtion about room mute
 @Details This delegate method for all modiatore in room once any moderator will do mute to any participent/moderator in same room
 */
/**
 There would be listener for moderator when hardmute used by moderator. For this delegates are:
 
 Moderator Delegates
 */
- (void)didMutedSingleUser:(NSArray *_Nullable)Data;

/**
 Fired when any modiatore in confrence will do unmute to any participent/other modiatore in same room
@param Data has detail informtion about room mute
 @Details :- This delegate method for all modiatore in room once any moderator will do unmute to any participent/moderator in same room
 */
/**
 There would be listener for moderator when hardmute used by moderator. For this delegates are:
 
 Moderator Delegates
 
 */
- (void)didUnMutedSingleUser:(NSArray *_Nullable)Data;

/**
 Fired when any modiatore in confrence will do mute to any participent/other modiatore in same room
@param Data has detail informtion about room mute
 @Details  This delegate method for all participent in room once any moderator will do mute to any participent/moderator in same room
 */
/**
 There would be listener for paricipant when hardmute used by moderator. For this delegates are:
 Paricipant Delegates
 
 */
- (void)didHardMutedSingleUser:(NSArray *_Nullable)Data;
/**
 Fired when any modiatore in confrence will do unmute to any participent/other modiatore in same room.
@param Data has detail informtion about room mute
 @Details This delegate method for all participent in room once any moderator will do unmute to any participent/moderator in same room
 */
/**
 
 There would be listener for paricipant when hardmute used by moderator. For this delegates are:
 Paricipant Delegates
 
 */
- (void)didHardUnMuteSingleUser:(NSArray *_Nullable)Data;

/**
 This API is designed to return a JSON Structure containing the user information who connected  to the Room.
 @param room Instance of the room where event happen.
 @param Data has full information of participent who has join in same confrence.
 @Details  This delegate method will inform to all partipient/moderator in same confrence with details information about the user who has just join the room.
 */
-(void)room:(EnxRoom *_Nullable)room userDidJoined:(NSArray *_Nullable)Data;

/**
  This API is designed to return a JSON Structure containing the user disconnected to the Room.
    This delegate call when any user leave the room.
 @param room Instance of the room where event happen.
 @param Data has full information of participent who has disconencted from same confrence.
 @Details This delegate method will inform to all partipient/moderator in same confrence with details information about the user who has just disconnected from confrence.
 
 */
-(void)room:(EnxRoom *_Nullable)room userDidDisconnected:(NSArray *_Nullable)Data;

/**
 Moderator get raish hand and approve hand list
 @param room Instance of the room where event happen.\
 @param data has contain current state of room.
 @Details This methods will inform to all user in confrence about current state of floor request
 */
- (void)room:(EnxRoom *_Nullable)room ChairControlStates:(NSMutableDictionary *_Nullable)data;

#pragma mark- recording Participant delegate

/**
 To start Recording Participant delegate
 Paricipant delegate
 @param Data has contain information about recording.
 @Details All particpents/Moderator in room will receive this delegate method once recoding has started in same confrence
 */
-(void)roomRecordOn:(NSArray *_Nullable)Data;

/**
 To stop Recording Participant delegate
 Paricipant delegate
 @param Data has contain information about recording.
 @Details All particpents/Moderator in room will receive this delegate method once recoding has stopped in same confrence
 */

-(void)roomRecordOff:(NSArray *_Nullable)Data;

//Active Talker Delegate

/**
 A Participant get of active talker list
 @param room Instance of the room where event happen.
 @param Data will contain list of EnxStream.
 @param view will contain full view of active talker list
 @Details once any participent successfully completed subscription of all avaialble stream in same confrence will receive available active talker list.
 By default its will give list of EnxStream, if user want complete view of active talker in that case he/she will pass a parameter "activeviews" = @"view" in "roominfo" duringb join room.
 
 */
//This delegate methods will return list of EnxStream
-(void)room:(EnxRoom *_Nullable)room didActiveTalkerList:(NSArray *_Nullable)Data;
//This delegate methods will return collectionView of EnxStream
-(void)room:(EnxRoom *_Nullable)room didActiveTalkerView:(UIView *_Nullable)view;

/**
 
 A Participant get of max number of talker count .
 @param room Instance of the room where event happen.
 @param Data will contain information about max possible talker in same confrence.
 @details this delegate method will give information about max  possible active talker in same confrence. so that user can change any number of active talker which is less than possible active talkers in room. User can request to know max possible AT in room at any time after join room.
 */
-(void)room:(EnxRoom *_Nullable)room didGetMaxTalkers:(NSArray *_Nullable)Data;
/**
 A Participant get of number of talker count .
 @param room Instance of the room where event happen.
 @param Data will contain number of current active talker in room.
 @Details with the help of this delegate methoder, use can check current number of active taker in same confrence.
 */
-(void)room:(EnxRoom *_Nullable)room didGetTalkerCount:(NSArray *_Nullable)Data;

/**
  A Participant set of number of talker count .
  @param room Instance of the room where event happen.
  @param Data will contain information about set active talker
 @details - this delegate method will fired once user will request to set active talker in same confrence.
*/
-(void)room:(EnxRoom *_Nullable)room didSetTalkerCount:(NSArray *_Nullable)Data;

#pragma mark - ACK/start/stop screen share delegates
/**
 A Participant listens to this delegate to know about a screen shared started by a user.
 @param room Instance of the room where event happen.
 @param stream has list of required information to display screen share stream
 @details all participants (Except owner of screen share) in same confrence will get this delegate method, whenever screen share has stared by any participent in same confrence.
 */
-(void)room:(EnxRoom *_Nullable)room didScreenShareStarted:(EnxStream *_Nullable)stream;
/**
 A Participant listens to this delegate to know about a screen shared by a user has stopped.
 @param room Instance of the room where event happen.
 @param stream has list of required information to remove screen share stream
 @details all participants (Except owner of screen share) in same confrence will get this delegate method, whenever screen share has stopped by any participent in same confrence.
 */

-(void)room:(EnxRoom *_Nullable)room didScreenShareStopped:(EnxStream *_Nullable)stream;

//Screen Share Start/Stop ACK Delegate for Self User
/**
 A user will receive this ack delegate, when he/she will start screen share.
 @param room Instance of the room where event happen.
 @param Data ACK list of start Annotation
 @details This delegate method will receive the owner of screen share , means who has started screen share.
 */
-(void)room:(EnxRoom *_Nullable)room didStartScreenShareACK:(NSArray *_Nullable)Data;
/**
 A user will receive this ack delegate, when he/she will stop screen share.
 @param room Instance of the room where event happen.
 @param  Data ACK list of Stop Annotation
 @details This delegate method will receive the owner of screen share , means who has stopped screen share.
 */
-(void)room:(EnxRoom *_Nullable)room didStoppedScreenShareACK:(NSArray *_Nullable)Data;
/**
 Owner of the method will received this call back
 @param room Instance of the room where event happen.
 @param  Data ACK list of Stop Annotation
 @details This delegate method will Acknowledge  to the owner of screen share , means who has stopped screen share.
 */
-(void)room:(EnxRoom *_Nullable)room didExitScreenShareACK:(NSArray *_Nullable)Data;




#pragma mark - ACK/start/stop canvas delegates

/**
 A Participant listens to this delegate to know about a Canvas started by a user.
 @param room Instance of the room where event happen.
 @param stream has  list of required information to display canvas stream
 @details all participants (Except owner of start canvas) in same confrence will get this delegate method, whenever canvashas  stared by any participent in same confrence.
 */
-(void)room:(EnxRoom *_Nullable)room didCanvasStarted:(EnxStream *_Nullable)stream;
/**
 A Participant listens to this delegate to know about that Canvas  has stopped by user.
 @param room Instance of the room where event happen.
 @param stream list of required information to remove canvas
 @details all participants (Except owner of stop canvas) in same confrence will get this delegate method, whenever canvas has stopped by any participent in same confrence.
 */
-(void)room:(EnxRoom *_Nullable)room didCanvasStopped:(EnxStream *_Nullable)stream;

/**
 Canvas Start/Stop Delegate for Self User
 @param room Instance of the room where event happen.
 @param Data ACK list of start canvas
 @details -  This delegate method will receive the owner of strart canvas , means who has started Canvas only receive this delegate method.
 */
-(void)room:(EnxRoom *_Nullable)room didStartCanvasACK:(NSArray *_Nullable)Data;
/**
 A Participant listens to this delegate to know about that Canvas  has stopped by user.
 @param room Instance of the room where event happen.
 @param  Data ACK list of Stop Canvas
 @details -  This delegate method will receive the owner of stopped canvas , means who has stopped Canvas, only receive this delegate method.
 */
-(void)room:(EnxRoom *_Nullable)room didStoppedCanvasACK:(NSArray *_Nullable)Data;

#pragma mark - ACK/start/stop Annotation delegates
/**
 A Participant listens to this delegate to know about a Anotation started by a user.
 @param room Instance of the room where event happen.
 @param Data list of required information to display Annotation Stream
 @details all participants (Except owner of start annotation) in same confrence will get this delegate method, whenever Annotation has started by any participent in same confrence.
 */
-(void)room:(EnxRoom *_Nullable)room didAnnotationStarted:(NSArray *_Nullable)Data;
/**
 A Participant listens to this delegate to know about that Annotation  has stopped by user.
 
 @param room Instance of the room where event happen.
 @param Data list of required information to remove Annotation Stream
 @details all participants (Except owner of stop annotation) in same confrence will get this delegate method, whenever Annotation has stopped by any participent in same confrence.
 */
-(void)room:(EnxRoom *_Nullable)room didAnnotationStopped:(NSArray *_Nullable)Data;

//Annotation Start/Stop Delegate for Self User
/**
 A Participant listens to this delegate to know about a Anotation started by a user.
 @param room Instance of the room where event happen.
 @param Data ACK list of start Annotation
 @details -  This delegate method will receive the owner of strart annotation , means who has started annotation only receive this delegate method.
 */
-(void)room:(EnxRoom *_Nullable)room didStartAnnotationACK:(NSArray *_Nullable)Data;
/**
 A Participant listens to this delegate to know about that Annotation  has stopped by user.
 
 @param room Instance of the room where event happen.
 @param  Data ACK list of Stop Annotation
 @details -  This delegate method will receive the owner of stopped annotation , means who has stopped annotation, only receive this delegate method.
 */
-(void)room:(EnxRoom *_Nullable)room didStoppedAnnotationACK:(NSArray *_Nullable)Data;

/**
 Fired when a bandWidth alert received from server.
 @param room Instance of the room where event happen.
 @param data bandWidthAlert info on a stream.
 @details this delegate method will inform to all use in confrence, whenever any bandwidth update hapen by server in same room.
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
 @details this delegate method will inform to the current user , share screen state changes due to any cause like low bandwidth.
 */
- (void)room:(EnxRoom *_Nullable)room didShareStreamEvent:(NSArray *_Nullable)data;


/**
 This delegate for response on setting video quality of remote streams.
 @param room Instance of the room where event happen.
 @param data has details about remote stream video resolution information.
 @details this delegate method will update about remote stream video quality.
 */
-(void)room:(EnxRoom *_Nullable)room didSetVideoQuality:(NSArray *_Nullable)data;
/**
 Audio media changed.
 @param updates carry information about current audio media
 @details This delegate Method Will Notify app user for any Audio media changes happen recentally(Like :- New device connected/Doisconnected).
 */
-(void)didNotifyDeviceUpdate:(NSString*_Nonnull)updates;

#pragma -mark Network Connection intrupted
/**
 Fired when a network clunch receive.
 @param room Instance of the room where event happen.
 @param data network intruption alert info.
 @details this delegate method will inform to user, if he/she has network clunch during call.
 */
-(void)room:(EnxRoom*_Nonnull)room didConnectionInterrupted:(NSArray*_Nonnull)data;

#pragma -mark Network Connection Lost
/**
 Fired when a Network lost
 @param room Instance of the room where event happen.
 @param data network Time Out alert info.
 @details this delegate method will call when network will not resume within given time out time.
 */
-(void)room:(EnxRoom*_Nonnull)room didConnectionLost:(NSArray*_Nonnull)data;

#pragma -mark Reconnect Success
/**
 Fired when a reconnect success .
 @param room Instance of the room where event happen.
 @param data Inform user for reconnect success.
 @details this delegate method will get called when reconnect get successfully resume.
 */
-(void)room:(EnxRoom*_Nonnull)room didUserReconnectSuccess:(NSDictionary*_Nonnull)data;

/**
 Fired when a user enable stats
 @param statsData has all stats information.
 @details this delegate method will get called when request for stats and stats receive in room.
 */
-(void)didReceiveStats:(NSArray * _Nonnull)statsData;

/**
 Fired when a user enable/desable stats
 @param subUnSubResponse has all stats information.
 @details this delegate method will get called when request for subscribe/unsubscribe stats in room
 */
-(void)didAcknowledgeStats:(NSArray * _Nonnull)subUnSubResponse;

/**
 Fired when message received in room
 @param room Instance of the room where event happen.
 @param data information of message.
 @details this delegates methods get called, when any participent will exchanges chat message in same room.
 */
- (void)room:(EnxRoom *_Nonnull)room didMessageReceived:(NSArray *_Nullable)data;

/**
 Fired when receive custom signalingmessage in room
 @param room Instance of the room where event happen.
 @param data information of custom signalling message.
 @details This delegate method called to receive custom signaling event message at room Level.
 */
- (void)room:(EnxRoom *_Nonnull)room didUserDataReceived:(NSArray *_Nullable)data;

/**
 Fired when start file sharing
 @param room Instance of the room where event happen.
 @param data information file.
 @details This delegate method called When any of the user in same room will start sharing file.
 */

- (void)room:(EnxRoom *_Nonnull)room
didFileUploadStarted:(NSArray *_Nullable)data;
/**
 Fired when user will initiate file sharing
 @param room Instance of the room where event happen.
 @param data information of  file.
 @details This delegate method called When self user will start sharing file.
 */

- (void)room:(EnxRoom *_Nonnull)room
didInitFileUpload:(NSArray *_Nullable)data;

/**
 Fired when user will isuccessfully upload file
 @param room Instance of the room where event happen.
 @param data information of  file.
 @details This delegate method called When any user will share file successfully and now File available to download for other participent.
 */

- (void)room:(EnxRoom *_Nonnull)room
didFileAvailable:(NSArray *_Nullable)data;

/**
 Fired when user will isuccessfully upload file
 @param room Instance of the room where event happen.
 @param data information of  file.
 @details This delegate method called upload file is success.
 */

- (void)room:(EnxRoom *_Nonnull)room
didFileUploaded:(NSArray *_Nullable)data;

/**
 Fired when user will  upload file failed
 @param room Instance of the room where event happen.
 @param data information of  file.
 @details This delegate method called upload file is failed.
 */
- (void)room:(EnxRoom *_Nonnull)room
didFileUploadFailed:(NSArray *_Nullable)data;

/**
 Fired when file download done
 @param room Instance of the room where event happen.
 @param data information of  file.
 @details This delegate method called When download of file success.
 */

- (void)room:(EnxRoom *_Nonnull)room
didFileDownloaded:(NSString *_Nullable)data;

/**
 Fired when file download failed
 @param room Instance of the room where event happen.
 @param data information of  file.
 @details This delegate method called When file download failed.
 */

- (void)room:(EnxRoom *_Nonnull)room
didFileDownloadFailed:(NSArray *_Nullable)data;

/**
 Fired when file download started
 @param room Instance of the room where event happen.
 @param data information of file.
 @details This delegate method called When file download initiated.
 */

- (void)room:(EnxRoom *_Nonnull)room
didInitFileDownload:(NSArray *_Nullable)data;

/**
 Fired when file download Cancle
 @param room Instance of the room where event happen.
 @param data information of file.
 @details This delegate method called When file download Cancel.
 */

- (void)room:(EnxRoom *_Nonnull)room
didFileDownloadCancelled:(NSArray *_Nullable)data;

/**
 Fired when file upload Cancle
 @param room Instance of the room where event happen.
 @param data information of file.
 @details This delegate method called When file Upload Cancel.
 */

- (void)room:(EnxRoom *_Nonnull)room
didFileUploadCancelled:(NSArray *_Nullable)data;

/**
 Fired for ACK advance option
 @param room Instance of the room where event happen.
 @param data ACK  information of advance options.
 @details This delegate called for advance options updates.
 */

- (void)room:(EnxRoom *_Nullable)room didAcknowledgementAdvanceOption:(NSDictionary *_Nullable)data;

/**
 Fired for battery update
 @param room Instance of the room where event happen.
 @param data information of device batter energy.
 @details This delegate called once user will set advance option and after that better energy lable getting low.
 */
- (void)room:(EnxRoom *_Nullable)room didBatteryUpdates:(NSDictionary *_Nullable)data;

/**
 Fired for resolution update
 @param room Instance of the room where event happen.
 @param data information of device stream resolution update.
 @details This delegate called once user will set advance option and request to know about aspect retion update during call.
 */
- (void)room:(EnxRoom *_Nullable)room didAspectRatioUpdates:(NSArray *_Nullable)data;

/**
 Fired for video resolution update
 @param room Instance of the room where event happen.
 @param data information of device stream video resolution update.
 @details This delegate called once user will set advance option and request to know about video resolution update during call.
 */
- (void)room:(EnxRoom *_Nullable)room didVideoResolutionUpdates:(NSArray *_Nullable)data;

/**
 Fired for advance opt update
 @param room Instance of the room where event happen.
 @param data information of advanceoption update.
 @details This delegate called once user will set advance option and request to know about advance option update during call.
 */
- (void)room:(EnxRoom *_Nullable)room didGetAdvanceOptions:(NSArray *_Nullable)data;

/**
 Fired for confrence duration update
 @param room Instance of the room where event happen.
 @param data information about confrence duration
 @details This delegate called for all participent in same confrence, 5mint before confrence duration.
 */

- (void)room:(EnxRoom *_Nullable)room didConferenceRemainingDuration:(NSArray *_Nullable)data;
/**
 Fired for confrence duration extended
 @param room Instance of the room where event happen.
 @param data information about confrence duration extended
 @details This delegate called for all participent in same confrence, once any of moderatore has exted the confrence duration.
 */

- (void)room:(EnxRoom *_Nullable)room didConferencessExtended:(NSArray *_Nullable)data;
#pragma mark- Switch user role Delegate

/**
 Fired when switch user role happen
 @param room Instance of the room where event happen.
 @param data information about user role when switched
 @details This delegate called, when any moderatore has switched their role or hand over their role to any of the participent in same room.
 */
- (void)room:(EnxRoom *_Nullable)room didSwitchUserRole:(NSArray *_Nullable)data;
/**
 Fired when current role changed
 @param room Instance of the room where event happen.
 @param data information about user role
 @details This delegate called, when any moderatore has switched their role or hand over their role to any of the participent in same room.
 */
- (void)room:(EnxRoom *_Nullable)room didUserRoleChanged:(NSArray *_Nullable)data;

#pragma mark- Send Data Delegate

/**
 Fired Acknowledgment for send data
 @param room Instance of the room where event happen.
 @param data information about send data
 @details This is an acknowledgment delegate method for send data, when any of participent will send their data to other participent,
 */
- (void)room:(EnxRoom *_Nullable)room didAcknowledgSendData:(NSArray *_Nullable)data;

#pragma mark- Out Bond Call for App User
/**
 Fired when outbond call initiated
 @param room Instance of the room where event happen.
 @param data information about outbond call
 @details This delegate method will called, when any of user will initiate for botbond call in same room.
 */
- (void)room:(EnxRoom *_Nullable)room didOutBoundCallInitiated:(NSArray *_Nullable)data;
/**
 Fired when outbond call state changed
 @param room Instance of the room where event happen.
 @param state information about outbond call state
 @details This delegate method will called, when any of user will initiate for botbond call and call state has changed.
 */
- (void)room:(EnxRoom *_Nullable)room didDialStateEvents:(EnxOutBoundCallState)state;

/**
 Fired when outbond call started
 @param room Instance of the room where event happen.
 @param dtmfCollected information about DTMF number
 @details This delegate method will called, when any of user will initiate for outbound call and end user will press any DTMF number
 */
- (void)room:(EnxRoom *_Nullable)room didDTMFCollected:(NSString*_Nullable)dtmfCollected;

#pragma mark- Lock/Unlock Room Delegate

/**
 Fired when locked room called
 @param room Instance of the room where event happen.
 @param data information about lookroom event
 @details this is an acklodgment method for lockroom, whenever any moderatore will call lock room method.
 */
- (void)room:(EnxRoom *_Nullable)room didAckLockRoom:(NSArray *_Nullable)data;
/**
 Fired when unlocked room called
 @param room Instance of the room where event happen.
 @param data information about unlookroom event
 @details this is an acklodgment method for unlockroom, whenever any moderatore will call unlock room method.
 */
- (void)room:(EnxRoom *_Nullable)room didAckUnlockRoom:(NSArray *_Nullable)data;
/**
 Fired when room locked
 @param room Instance of the room where event happen.
 @param data information about lookroom event
 @details this delegate method will call, when moderatore will lock the room, other participent will receive this method.
 */
- (void)room:(EnxRoom *_Nullable)room didLockRoom:(NSArray *_Nullable)data;

/**
 Fired when room unlocked
 @param room Instance of the room where event happen.
 @param data information about unlookroom event
 @details this delegate method will call, when moderatore will unlock the room, other participent will receive this method.
 */
- (void)room:(EnxRoom *_Nullable)room didUnlockRoom:(NSArray *_Nullable)data;

#pragma mark- dropuser/destroy Room Delegate

/**
 Fired when drop user called
 @param room Instance of the room where event happen.
 @param data information about drop user event
 @details this is an acklodgment method for dropuser, Moderatore can drop any user at any time.
 */

- (void)room:(EnxRoom *_Nullable)room didAckDropUser:(NSArray *_Nullable)data;

/**
 Fired when destroy room called
 @param room Instance of the room where event happen.
 @param data information about destroy room event
 @details this is an acklodgment method for destroy room, Moderatore can destroy room  at any time.
 */
- (void)room:(EnxRoom *_Nullable)room didAckDestroy:(NSArray *_Nullable)data;

#pragma mark- Room Live Streaming Events

/**
 Fired when start streaning event called
 @param room Instance of the room where event happen.
 @param data information about streaming  event
 @details this is an acklodgment method for streaming in room, Moderatore can start streaming at any time in confrence.
 */
- (void)room:(EnxRoom *_Nullable)room didAckStartStreaming:(NSArray *_Nullable)data;
/**
 Fired when stop streaning event called
 @param room Instance of the room where event happen.
 @param data information about streaming  event
 @details this is an acklodgment method for streaming in room, Moderatore can stop streaming at any time in confrence.
 */
- (void)room:(EnxRoom *_Nullable)room didAckStopStreaming:(NSArray *_Nullable)data;
/**
 Fired when  streaning started in room
 @param room Instance of the room where event happen.
 @param data information about streaming  event
 @details This delegate method will called, when any modiatore will started streaming in room.
 */
- (void)room:(EnxRoom *_Nullable)room didStreamingNotification:(NSArray *_Nullable)data;

#pragma mark - BreakOut Room Delegates

/**
 Fired when create breakout room event called
 @param room Instance of the room where event happen.
 @param data information about create breakout room  event
 @details this is an acklodgment method for create breakout  room, Moderatore/Participent can create a breakout room at any time in running confrence.
 */
- (void)room:(EnxRoom *_Nullable)room didAckCreateBreakOutRoom:(NSArray *_Nullable)data;
/**
 Fired when create and nvite breakout room event called
 @param room Instance of the room where event happen.
 @param data information about createand invite breakout room  event
 @details this is an acklodgment method for create room, Moderatore/Participent can create and invite a breakout room at any time in running confrence.
 */
- (void)room:(EnxRoom *_Nullable)room didAckCreateAndInviteBreakOutRoom:(NSArray *_Nullable)data;
/**
 Fired when join breakout room failed
 @param room Instance of the room where event happen.
 @param data information about failure to join breakoutroom
 @details this delegate method will called when any user will try to join breakout room and failed.
 */
- (void)room:(EnxRoom *_Nullable)room didFailedJoinBreakOutRoom:(NSArray *_Nullable)data;
/**
 Fired when  invite breakout room event called
 @param room Instance of the room where event happen.
 @param data information about  invite breakout room  event
 @details this is an acklodgment method for invite brealout room, Moderatore/Participent can intite to any user to join any breakout room.
 */
- (void)room:(EnxRoom *_Nullable)room didAckInviteBreakOutRoom:(NSArray *_Nullable)data;
/**
 Fired when   breakout room connected
 @param room Instance of the room where event happen.
 @param roomMetadata information of room after join breakout room
 @details this delegate method will called, when user joined breakout room successfully.
 */
- (void)room:(EnxRoom *_Nullable)room didConnectedBreakoutRoom:(NSDictionary *_Nullable)roomMetadata;

/**
 Fired when user pause room
 @param room Instance of the room where event happen.
 @param data information of room pause event
 @details this is an ackniwledgment method for pause room, Any participent in room can pause the main room after join breakout room.
 */

- (void)room:(EnxRoom *_Nullable)room didAckPause:(NSArray *_Nullable)data;
/**
 Fired when user resume room
 @param room Instance of the room where event happen.
 @param data information of room resume event
 @details this is an ackniwledgment method for resume room, Any participent in room can resume a paused main room after join/disconnect breakout room.
 */
- (void)room:(EnxRoom *_Nullable)room didAckResume:(NSArray *_Nullable)data;
/**
 Fired when user mute room
 @param room Instance of the room where event happen.
 @param data information of room mute event
 @details this is an ackniwledgment method for mute room, Any participent can mute an individua behaviour of room like audio,video after join breakout room.
 */
- (void)room:(EnxRoom *_Nullable)room didAckMuteRoom:(NSArray *_Nullable)data;
/**
 Fired when user unmute room
 @param room Instance of the room where event happen.
 @param data information of room unmute event
 @details this is an ackniwledgment method for unmute room, Any participent can unmute an individua behaviour of room like audio,video after mute such bahaviour after join/disconnect from breakout room
 */
- (void)room:(EnxRoom *_Nullable)room didAckUnmuteRoom:(NSArray *_Nullable)data;
/**
 Fired when user disconnect from breakout room
 @param room Instance of the room where event happen.
 @param respons information of breakout room disconnection
 @details This delegate method will called, when user will disconnected from breakout room either self or due to any cause.
 */
- (void)room:(EnxRoom *_Nullable)room didDisconnectedBreakoutRoom:(NSArray *_Nullable)respons;
/**
 Fired when user joined from breakout room
 @param room Instance of the room where event happen.
 @param data information of user who has joined breakout room.
 @details This delegate method will called, when user will joined any breakout room.
 */
- (void)room:(EnxRoom *_Nullable)room didUserJoinedBreakoutRoom:(NSArray *_Nullable)data;
/**
 Fired when  joined from breakout room
 @param room Instance of the room where event happen.
 @param data information of user for join breakout room
 @details This delegate method will called, when user will joined any breakout room.
 */
- (void)room:(EnxRoom *_Nullable)room didInvitationForBreakoutRoom:(NSArray *_Nullable)data;

/**
 Event fired when user will disconnect from breakout room
 @param room Instance of the room where event happen.
 @param data details user , which got disconnected from breakout room.
 @details this is the Room on listrener method, Which will inform to owner of breakout room that all user has disconencted and room get destroied.
 */
- (void)room:(EnxRoom *_Nullable)room didDestroyedBreakoutRoom:(NSArray *_Nullable)data;

/**
 Event fired when user will disconnect from breakout room
 @param room Instance of the room where event happen.
 @param data details user , which got disconnected from breakout room.
 @details this is the socket on listrener method for all use in parents room will get notify about user disconnected from breakout room resently.
 */
- (void)room:(EnxRoom *_Nullable)room didUserDisconnectedFromBreakoutRoom:(NSArray *_Nullable)data;
/**
 Event fired when invitee user will reject breakout room invitation
 @param room Instance of the room where event happen.
 @param data details user , acknoledgmet details for reject breakout room invitation
 @details this is the socket emit acknowledgment listrener method for the user who has rejected to join breakout room.
 */

- (void)room:(EnxRoom *_Nullable)room didAckRejectBreakoutRoom:(NSArray *_Nullable)data;

/**
 Event fired for all available moderator in room
 @param room Instance of the room where event happen.
 @param data details of breakout room ,
 @details this is the socket on room managment listrener method for all moderator , once breakout room created.
 */
- (void)room:(EnxRoom *_Nullable)room didBreakoutRoomCreated:(NSArray *_Nullable)data;
/**
 Event fired for all available moderator in room
 @param room Instance of the room where event happen.
 @param data details of user who has intivet in breakout room
 @details this is the socket on room managment listrener method, will be sent to all the moderators about a participant being invited to breakout room
 */
- (void)room:(EnxRoom *_Nullable)room didBreakoutRoomInvited:(NSArray *_Nullable)data;

/**
 Event fired for all available moderator in room
 @param room Instance of the room where event happen.
 @param data details user , How has rejected for breakout room invitation.
 @details this is the socket on room managment listrener method, event will be sent to the invitee and all the moderator if the breakout invite is rejected. The message will contain the room_id and rejected client id.
 */
- (void)room:(EnxRoom *_Nullable)room didBreakoutRoomInviteRejected:(NSArray *_Nullable)data;

#pragma mark - Acknowledgment for Add/Remove ping user
/**
 Fired when a Moderator request for add Ping user .
 @param room Instance of the room where event happen.
 @param data Inform user for pinged  success/failure.
 @details this is an acknowledgment method for pinUser events done by any modiatore.
 */
- (void)room:(EnxRoom *_Nullable)room didAckPinUsers:(NSArray *_Nullable)data;
/**
 Fired when a Moderator request for remove Ping user .
 @param room Instance of the room where event happen.
 @param data Inform user for pinged  success/failure.
 @details this is an acknowledgment method for UnpinUsers events done by any modiatore.
 */
- (void)room:(EnxRoom *_Nullable)room didAckUnpinUsers:(NSArray *_Nullable)data;
/**
 Fired when a any user has pinned by any moderatore .
 @param room Instance of the room where event happen.
 @param data Inform user for pinged  user list.
 @details this delegate method will update the list of pinned user list in same confrence.
 */
- (void)room:(EnxRoom *_Nullable)room didPinnedUsers:(NSArray *_Nullable)data;

#pragma mark - Knock-Knock Room /wait for moderator
/**
 Fired when your have join knock knoc room or wait for moderatore enable room.
 @param room Instance of the room where event happen.
 @param reason Inform user regarding why room waited
 @details this delegate method will inform to a participent , who is going to join a knock-knock room or wait for moderatore enable room.
 This API will inform either its knock-knoc room or wait for moderatore room.
 This API only for participent.
 */
-(void)room:(EnxRoom *_Nullable)room didRoomAwated:(NSArray *_Nullable)reason;
/**
 Fired when a modiatore will approve any awaited user in knock - kock room
 @param room Instance of the room where event happen.
 @param data Inform about approved awated user.
 @details this acknowledgment method for modeator, when  he/she will approved awated user in knock knock room.
 */
-(void)room:(EnxRoom *_Nullable)room didAckForApproveAwaitedUser:(NSArray *_Nullable)data;

/**
 Fired when a any user has pinned by any moderatore .
 @param room Instance of the room where event happen.
 @param data  Inform about deny awated user.
 @details this acknowledgment method for modeator, when he/she will deny  awated user in knock knock room.
 */
-(void)room:(EnxRoom *_Nullable)room didAckForDenyAwaitedUser:(NSArray *_Nullable)data;

/**
 Fired when a any participent waiting in lobby of knock knock room
 @param room Instance of the room where event happen.
 @param data  Inform about waited user
 @details this callback method for modeartor , when any user user join after modeartor join and waiting in lobby.
 */
-(void)room:(EnxRoom *_Nullable)room diduserAwaited:(NSArray *_Nullable)data;

/**
 Fired for ACK for Talker subscribe notification option
 @param room Instance of the room where event happen.
 @param data ACK  information of Talker notification.
 @details This delegate called for Talker notification subscribe
 */

- (void)room:(EnxRoom *_Nullable)room didAckSubscribeTalkerNotification:(NSArray *_Nullable)data;
/**
 Fired for ACK for Talker unsubscribe notification option
 @param room Instance of the room where event happen.
 @param data ACK  information of Talker notification.
 @details This delegate called for Talker notification unsubscribe updates.
 */
- (void)room:(EnxRoom *_Nullable)room didAckUnsubscribeTalkerNotification:(NSArray *_Nullable)data;
/**
 Fired for giving all list of active speaker details
 @param room Instance of the room where event happen.
 @param data  information of active speker.
 @details This delegate called for Talker notification subscribe/unsubscribeupdates.
 */

-(void)room:(EnxRoom *_Nullable)room didTalkerNotification:(NSArray *_Nullable)data;

/**
 Fired to notify user for his/her subscriber bandwidth alart
 @param room Instance of the room where event happen.
 @param data  information about subscriber bandwidth.
 @details This delegate notify to the user incase of subscriber bandwidth goes low
 */

-(void)room:(EnxRoom *_Nullable)room didRoomBandwidthAlert:(NSArray *_Nullable)data;


#pragma mark - delegate for Add/Remove sportLight user
/**
 Fired when a Moderator request for add Spotlight user .
 @param room Instance of the room where event happen.
 @param data Inform user for Spotlight  success/failure.
 @details this is an acknowledgment method for add Spotlight events done by any modiatore.
 */
- (void)room:(EnxRoom *_Nullable)room didAckAddSpotlightUsers:(NSArray *_Nullable)data;
/**
 Fired when a Moderator request for remove Spotlight user .
 @param room Instance of the room where event happen.
 @param data Inform user for Spotlight  success/failure.
 @details this is an acknowledgment method for remove Spotlight events done by any modiatore.
 */
- (void)room:(EnxRoom *_Nullable)room didAckRemoveSpotlightUsers:(NSArray *_Nullable)data;
/**
 Fired when a any user has Spotlight by any moderatore .
 @param room Instance of the room where event happen.
 @param data Inform user for pinged  user list.
 @details this delegate method will update the list of Spotlight user list in same confrence.
 */
- (void)room:(EnxRoom *_Nullable)room didUpdatedSpotlightUsers:(NSArray *_Nullable)data;

#pragma mark - delegate for Switch Room Mode

/**
 This is an acknolagement for switch room
 
 @param room Instance of the room where event happen.
 @param data Acknowldgement details.
 @details this is an acknowladgement owner of the method  .
 */


- (void)room:(EnxRoom *_Nullable)room didAckSwitchedRoom:(NSArray *_Nullable)data;

/**
 This is  notification  for switch room
 
 @param room Instance of the room where event happen.
 @param data details of the new room details.
 @details this is notification for all participants or moderator   .
 */

-(void)room:(EnxRoom *_Nullable)room didRoomModeSwitched:(NSArray *_Nullable)data;

@end
@protocol EnxRoomFaceXDelegate <NSObject>
@optional
#pragma mark - FaceX Events

/**
 Fired when a pingBack event called
 @param data Information pingback event
 @details this delegate method when faceX process will start.
 */
-(void)didPingBack:(NSArray*_Nullable)data;
/**
 Fired when a StartedClientUsage event called
 @param data Information StartClientUsage event
 @details this delegate method when faceX process will start and StartedClientUsage fired.
 */
-(void)didStartedClientUsage:(NSArray*_Nullable)data;
/**
 Fired when a StoppedClientUsage event called
 @param data Information StoppedClientUsage event
 @details this delegate method when faceX process will start and StoppedClientUsage fired.
 */
-(void)didStoppedClientUsage:(NSArray*_Nullable)data;
@end

@protocol EnxBroadCastDelegate <NSObject>
@optional
#pragma mark - EnxBroadCast  Delegates
/**
 Fired when a broadcased started
 @param data Information broadcast  event
 @details This delegate method will called when user will add replaykit breadcast and start shreen shared
 */
-(void)didStartBroadCast:(NSArray*_Nullable)data;
/**
 Fired when a broadcased stopped
 @param data Information broadcast  event
 @details This delegate method will called when user will add replaykit breadcast and stopped shreen shared
 */
-(void)didStoppedBroadCast:(NSArray*_Nullable)data;
/**
 Fired when a broadcased disconnected
 @details This delegate method will called when user will stopped screen shared
 */
-(void)broadCastDisconnected;
/**
 Fired when a broadcased disconnected
 @details This delegate method will called when user will start screen shared
 */
-(void)broadCastConnected;
/**
 Fired when a broadcased disconnected
 @param reason Information about failure to start screen share
 @details This delegate method will called when user will start screen and failed
 */
-(void)failedToConnectWithBroadCast:(NSArray*_Nonnull)reason;
/**
 Fired when disconnectedByOwner
 @details This delegate method will called when user will start screen shared and main room disconnected.
 */
-(void)disconnectedByOwner;
/**
 The chield will get notify to exist from breakout room
 @param  Data ACK list of Stop Annotation
 @details This delegate method will notify  to the child client .
 */
-(void)didRequestedExitRoom:(NSArray *_Nullable)Data;
@end

#pragma mark- Breakout Internal ClassUpdate
//This delegate class used for Internal Update to EnxBreakOutRoomClass
@protocol EnxBreakoutInternalClassDelegate <NSObject>
-(void)room:(EnxRoom *_Nullable)room didBreackoutRoomConnected:(NSDictionary*_Nullable)roomMetadata;
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
 @details this method is used to create a refrence of room and set room delegate
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
@property (weak, nonatomic) id <EnxRoomFaceXDelegate> _Nullable faceXRoomDelegate;
/// EnxBreakoutInternalClassDelegate were this room will invoke methods as events. This protocal method for sdk internal used.
@property(weak,nonatomic) id<EnxBreakoutInternalClassDelegate> _Nullable breakOutDelegate;
///Here i am storing playerDelegate when join room called and set this playerDelegate to player when we create player internally.
@property (weak, nonatomic) id  _Nullable playerDelegate;
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

//Mantain a flag when moderatore will do room mute/unmute
@property (nonatomic,readonly) BOOL moderatorHardMuteActiveState;
//Mantain a flag when moderatore will do single participent mute/unmute
@property (nonatomic,readonly) BOOL participantHardMuteActiveState;
//Mantain a flag when room has all ready hard muted by any modiatore.
@property (nonatomic,readonly) BOOL isHardMuteRoom;
//Mantain a flag when participent has all ready hard muted by any modiatore.
@property (nonatomic, readonly) BOOL isHardMuteUser;
//Mantain a flag when video muted in room.
@property (nonatomic,readonly) BOOL isVideoUserHardMute;
//Mantain a flag when stream has audio only.
@property (nonatomic,readonly) BOOL isAudioOnlyStreams;
//Mantain a flag when room has audio only.
@property (nonatomic) BOOL isAudioOnlyRoom;
//Mantain a flag when room has Chat only.
@property(nonatomic,readonly)BOOL chatOnlyRoom;
//Mantain a flag to check if reconnect all ready in progress.
@property(nonatomic)BOOL isReconnectingAttampted;
// connected clientId
@property (readonly,weak) NSString * _Nullable clientId;
// connected clientName
@property (readonly,weak) NSString * _Nullable clientName;
// connected user role
@property (readonly,nonatomic) NSString * _Nonnull userRole;
// max frame limit
@property(readonly) int maxFrameLimits;
// NSDictonary who carry all room information.
@property (readonly) NSDictionary * _Nullable roomInfo;
//Mantain a flag to check joined room as parents room or breakout room.
@property (nonatomic)BOOL isBreakOutRoom;
//store delegate refrence of parents room.
@property(nonatomic) id _Nullable delegateRef;
@property(nonatomic)BOOL isLobby;
///-----------------------------------
/// @name Public Methods
///-----------------------------------

/**
 This method is required if you have instanciated EnxRoom class without providing a token.
 @param token The auth token for room access. See initWithEncodedToken:
 for token composition details.
 @param roomInfo will a required argument, this will contain about room delais, for example - is reconnect allowed, number of attamped, time out, palayer icons , Activetalker type etc....
 @advanceOption this is an optional argument, this will contain information about subscription of advance option during join room
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
 @see EnxRoomDelegate:didRoomDisconnect:
 */
- (void)disconnect;
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
/**
 This API is only available during Lecture Mode of a Session and for the moderator. where moderator can handover the floor request to any random participent and that participent will either accept or deny.
 Once participant will accept will pubish their stream.
 
 @param clientId  It’s the Client ID for the participant to whom giving floor access.
 
  Delegate (Optional). If you need to handle the success or failure of the action, then pass a function name here to which you look to receive call back.
 
 Moderator delegate
 @see EnxRoomDelegate:didInviteToFloorRequested:data:
 */
-(void)inviteToFloor:(NSString* _Nonnull)clientId;

/**
 This API is only available during Lecture Mode of a Session and for the participant. where moderator can handover the floor request to any random participent, Now its chooice of participent either he is accepting or deny
 
 @param clientId  It’s the Client ID for the participant to whom giving floor access.
 
  Delegate (Optional). If you need to handle the success or failure of the action, then pass a function name here to which you look to receive call back.
 
 Moderator delegate
 @see EnxRoomDelegate:didProcessFloorRequested:data:
 */
-(void)acceptInviteFloorRequest:(NSString* _Nonnull)clientId;

/**
 This API is only available during Lecture Mode of a Session and for the moderator. where moderator can handover the floor request to any random participent and he/she can cancle that request at any time before accept.
 
 @param clientId  It’s the Client ID for the participant to whom giving floor access.
 
  Delegate (Optional). If you need to handle the success or failure of the action, then pass a function name here to which you look to receive call back.
 
 Moderator delegate
 @see EnxRoomDelegate:didProcessFloorRequested:data:
 */
-(void)cancelFloorInvite:(NSString* _Nonnull)clientId;

/**
 This API is only available during Lecture Mode of a Session and for the participant. where moderator can invite the floor request to any random participent, Now its chooice of participent either he is accepting or deny
 
 @param clientId  It’s the Client ID for the participant to whom giving floor access.
 
  Delegate (Optional). If you need to handle the success or failure of the action, then pass a function name here to which you look to receive call back.
 
 Moderator delegate
 @see EnxRoomDelegate:didProcessFloorRequested:data:
 */

-(void)rejectInviteFloor:(NSString* _Nonnull)clientId;


#pragma mark- Hard Mute
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

/**
 This methgod will used to switchged audiovideo call to audio only call.
 @param audioOnly is a flag to switch over audiovideo to audioonly or vise versa
 */
-(void)setAudioOnlyMode:(BOOL)audioOnly;

/**
 This API use to request server to set the remote video stream in different quality.
 @param opt which should be "Auto, HD , SD, LD and talker/canvas"

 */
-(void)setReceiveVideoQuality:(NSDictionary*_Nonnull)opt;

/**
 This API use to return the remote video stream quality.
 @param streamType which should be "talker/canvas"
 @return its will return as NSString which has stream video quality information like "Auto, HD , SD, LD"
 */
-(NSString *_Nonnull)getReceiveVideoQuality:(NSString*_Nonnull)streamType;

/**
 This method for get event from stream if any unauthrozed event get called
    @param eventName will carry name of enent.
 */

-(void)getEnxSteamEventError:(NSString *_Nonnull)eventName;

/**
 This method Will return all list of connected Audio Device
 @return list of audio device
 **/
-(NSArray*_Nonnull)getDevices;

/**
 This method Will return Current selected Audio device
    @return as NSString of current stelected audio device.
 **/
-(NSString*_Nonnull)getSelectedDevice;

/**
 This method Will Switch to selected media device
 @param mediaName as NSString of new audio media name on which we overwride the audio media
 **/
-(void)switchMediaDevice:(NSString*_Nonnull)mediaName;


/**
 Client endpoint will call this method to a mute/unmute remote stream while application in the background.
 @param flag will carry a flag value for mute/unmute remote stream during background
 */
-(void)stopVideoTracksOnApplicationBackground:(BOOL)flag;

/**
 Client endpoint will call this method to a mute/unmute remote stream while application in the foreground.
 @param flag will carry a flag value for mute/unmute remote stream during forground
 */
-(void)startVideoTracksOnApplicationForeground:(BOOL)flag;

/**
    Client End point will call this method to enable/disable stats by passing flag = true/false
 @param flag true/false
 'True' for enable stats and 'False' for desable Stats
 */
-(void)enableStats:(BOOL)flag;

/**
 Client end point use this method to active/inactive audio of all subcribe streams.
 @param flag true/false
 */
-(void)muteSubscribeStreamsAudio:(BOOL)flag;
/**
    client end point will used this method for sending chat message in same confrent either private or publically.
 @param message contain actual chat message. its can't be null
 @param broadcast true/false if breadcast true means this message will sent to all user in confrence false mesan only listed participent
 @param clientIds here will maintain list of client id who going to receive chat message. it can be nil if broadcast true.
 */

-(void)sendMessage:(NSString *_Nonnull)message isBroadCast:(BOOL)broadcast recipientIDs:(NSArray *_Nullable)clientIds;
/**
    client end point will used this method for sending custome event message in same confrent either private or publically.
 @param message contain actual chat message. its can't be null
 @param broadcast true/false if breadcast true means this message will sent to all user in confrence false mesan only listed participent
 @param clientIds here will maintain list of client id who going to receive chat message. it can be nil if broadcast true.
 */
-(void)sendUserData:(NSDictionary *_Nonnull)message isBroadCast:(BOOL)broadcast recipientIDs:(NSArray *_Nullable)clientIds;

/**
 Client endpoint can set options at room level. to set adavance option after join room
@param data contain list of advance option going to subscribe, data can't be nill
 */
-(void)setAdvanceOptions:(NSArray *_Nonnull)data;

/**
 To get list Advance options set by client endpoint. */
-(void)getAdvanceOptions;

/**
 Client endpoint can use this method to switch their role or handover their authority to any of the participent
 @param clientId here user will pass a string of clientID, this clientId must not be nill.
 */
-(void)switchUserRole:(NSString *_Nullable)clientId;
#pragma mark - File upload/download/Cancle
/**
 Client endpoint can use this method to send any type of file in confrence.
 @param isBroadcast true/false true for public share and false fro private share
 @param clientIds list of client id who going to receive a private files, its is a required only if user will send a private file.
 */
-(void)sendFiles:(BOOL)isBroadcast clientIds:(NSArray *_Nullable)clientIds;
/**
 Client endpoint can use this method to download any file shared in confrence.
 @param file It is a NSDictonary with contaion all information about files, its is a required property.
 @param flag true/false, if true the file will auto save phone galary and if false file will not save
 */
-(void)downloadFile:(NSDictionary *_Nonnull)file autoSave:(BOOL)flag;
/**
 Client endpoint can use this method to know all list of available files which he/she can download.
 @return its will return list of available file to be download for the participent
 */
-(NSArray*_Nullable)getAvailableFiles;
/**
 Client endpoint can use this method to Cancle on going upload file.
 @param jobID a Int value which contain cancle file jobID, it is a required property.
 */
-(void)cancelUpload:(int)jobID;
/**
 Client endpoint can use this method to Cancle all on going upload files.
 */
-(void)cancelAllUploads;
/**
 Client endpoint can use this method to Cancle on going download file.
 @param jobID a Int value which contain cancle file jobID, it is a required property.
 */
-(void)cancelDownload:(int)jobID;
/**
 Client endpoint can use this method to Cancle all on going downlod file.
 */
-(void)cancelAllDownloads;
#pragma mark- outbound call
 /**
  Client endpoint can use this method to connect through outbond call,
  @param number it is a NSString value which contain end user mobile number, its is a required property which can't be nil.
  @param callerId this is a NSString which carry callerID,  its is a required property which can't be nil.
    This method for initiating outbound call
 */
-(void)makeOutboundCall:(NSString*_Nonnull)number callerId:(NSString *_Nonnull)callerId;

#pragma mark- zoonin/zoomout on remote stream
/**
 Client endpoint can use this method to zoom remote stream,
 @param value it is a CGFloat value which contain range form 0.1 to 1.0 scall to zoom
 @param clientIds this is a list of clientids which streams are going to zoom. its is a required property , its can't be nil.
*/
-(void)setZoomFactor:(CGFloat)value clientId:(NSArray *_Nonnull)clientIds;

#pragma mark- annotation/canvas start/stop methods
/**
 Client endpoint can use this method to start Annotations on given stream.
 @param stream it is a EnxStream on which annotation going to be done. its is a required property which can't be nil.
*/

-(void)startAnnotation:(EnxStream*_Nonnull)stream;
/**
 Client endpoint can use this method to stop ongoing Annotations , this method only excute if all ready annotation is running in confrence and only owner of annotation can stop annotation.

*/
-(void)stopAnnotation;
/**
 Client endpoint can use this method to start Canvas on given UIview.
 @param view it is a UIview on which canvas is running. its is a required property which can't be nil.
*/
-(void)startCanvas:(UIView*_Nonnull)view;

/**
 Client endpoint can use this method to stop ongoing canvas , this method only excute if all ready canvas is running in confrence and only owner of annotation can stop annotation.
*/
-(void)stopCanvas;

#pragma mark- extend confrence and update configuration
/*
 Client endpoint can use this method to Extend Confrence Duration.
 */
-(void)extendConferenceDuration;
/**
 Client endpoint can use this method to update Configuration in ongoing confrence ,
 @param data it is a NSDictonary which contaion information about update configuration, its is a required property ,its can't be nil.
*/
-(void)updateConfiguration:(NSDictionary *_Nonnull)data;

#pragma mark- lock/unlock room
/**
 Client endpoint can use this method to lock the ongoing confrence.
*/
-(void)lockRoom;
/**
 Client endpoint can use this method to unlock the ongoing confrence.
*/
-(void)unlockRoom;
/**
 Client endpoint can use this method to enable/disable proximitry Sensor ,
 @param value true/false , ture for unable proximitry Sensor and false for diasable proximitry Sensor
*/
-(void)enableProximitySensor:(BOOL)value;

#pragma mark- drop user or destroy room
/**
 Client endpoint can use this method to drop any user from ongoing confrence,
 @param clientIds its contain list of clientID who going to be dropped from ongoing confrence, Its is a required property, can't be nil.
*/
-(void)dropUser:(NSArray *_Nonnull)clientIds;
/**
 Client endpoint can use this method to destroy  ongoing confrence at any time,
*/
-(void)destroy;
/**
 Client endpoint can use this method to get list of update available users in room.
 @return its will return list of available users in room.
*/
-(NSArray*_Nonnull)getUserList;
/**
 Client endpoint can use this method to adjust Layout of activetalker View, This method will valid only if user has requested for complete view of activetalker
*/
-(void)adjustLayout;

#pragma mark- start/stop steaming
/**
 Client endpoint can use this method to start startStreaming in ongoing confrence.
 @param streamingConfig it is a NSDictonary which contain information about streaming, it is a required parameter, its can't be nil
*/

//Live broadcast Streaming API
-(void)startStreaming:(NSDictionary *_Nonnull)streamingConfig;
/**
 Client endpoint can use this method to start stopStreaming in ongoing confrence.
 @param streamingConfig it is a NSDictonary which contain information about streaming, it is a required parameter, its can't be nil
*/
-(void)stopStreaming:(NSDictionary *_Nonnull)streamingConfig;
#pragma mark- strat/stop screen share
//Start Screen Share
/**
 This method is required instance of EnxScreenshareDelegate along with token
 @param token The auth token for room access. See initWithEncodedToken:
 for token composition details.
 */
-(void)connectWithScreenshare:(NSString *_Nonnull)token withScreenDelegate:(id<EnxBroadCastDelegate> _Nonnull)delegate;
/**
 Client endpoint can use this method to start screen share in ongoing confrence.
*/
-(void)startScreenShare;
/**
 Client endpoint can use this method to send video buffer after screen shared started.
 @param sampleBuffer it is a required property, can't be nil.
*/
-(void)sendVideoBuffer:(CVPixelBufferRef _Nonnull )sampleBuffer withTimeStamp:(int64_t)timeStampNs;
/**
 Client endpoint can use this method to stop screen share, this method only work when screen shared all ready running.
*/
-(void)stopScreenShare;
/**
 Client endpoint can use this method to infoirm chiled user to exit from room, this method only work when screen shared all ready running.
*/
-(void)exitScreenShare;

#pragma mark- FaceX Methods
//FaceX Methods
//Currentally this method not in used, will be implement this method later.
-(void)initFaceX:(NSDictionary * _Nonnull)roomMeta room:(EnxRoom * _Nonnull)room stream:(EnxStream *)stream delegate:(id _Nonnull )delegate;

-(void)pingBack:(id<EnxRoomFaceXDelegate>_Nullable)faceXDelegate;
-(void)startClientUsage:(NSDictionary *_Nonnull)data;
-(void)stopClientUsage:(NSDictionary *_Nonnull)data;
 
#pragma mark- Breakout room
//BreakOut Room API
/**
 Client endpoint can use this method to create breakout room after join parents room only.
 @param data - this is a NSDictionary which contain information about create breakout room.
 this is a required property , can't be nil.
*/
-(void)createBreakOutRoom:(NSDictionary* _Nonnull)data;
/**
 Client endpoint can use this method to create and invite breakout room after join parents room only.
 @param data - this is a NSDictionary which contain information about create breakout room.
 this is a required property , can't be nil.
*/
-(void)createAndInviteBreakoutRoom:(NSDictionary* _Nonnull)data;
/**
 Client endpoint can use this method to join breakout room after join parents room only.
 @param data - this is a NSDictionary which contain information about create breakout room.
 this is a required property , can't be nil.
 @param streamInfo this is a NSDictionary which contain information about stream like video : true/false, audio : true/false etc.....
*/
-(void)joinBreakOutRoom:(NSDictionary* _Nonnull)data withStreamInfo:(NSDictionary* _Nullable)streamInfo;
/**
 Client endpoint can use this method to invite any participent which has connected in parent room
 @param data - this is a NSDictionary which contain information about create breakout room.
 this is a required property , can't be nil.
*/
-(void)inviteToBreakOutRoom:(NSDictionary * _Nonnull)data;
/**
 Client endpoint can use this method to pause the parent room after join breakout room
*/
-(void)pause;
/**
 Client endpoint can use this method to resume the parent room after disconnect from breakout room
*/
-(void)resume;
/**
 Client endpoint can use this method to mute the parent room after connected to breakout room.
 @param data its a NSDictonary which contain information about mute room , for example audio : true/false, video true/false
*/
-(void)muteRoom:(NSDictionary * _Nonnull)data;
/**
 Client endpoint can use this method to unmute the parent room after disconnect from breakout room.
 @param data its a NSDictonary which contain information about mute room , for example audio : true/false, video true/false
*/
-(void)unmuteRoom:(NSDictionary * _Nonnull)data;

/**
 Client endpoint can use this method to reject/cancel to join   breakout room.
 @param roomId its a NSString which contain information about reject room
*/
-(void)rejectBreakOutRoom:(NSString *_Nonnull)roomId;

#pragma mark- Pin/unpin user
/**
 Client endpoint can use this method to pin any of the available user in room , while pass their clientIDs
 @param clientIds - this is list of clientID who going to be pinged
 this is a required property , can't be nil.
*/
//pin user
-(void)pinUsers:(NSArray *_Nonnull)clientIds;
/**
 Client endpoint can use this method to unpin any of the pingged user in room , while pass their clientIDs of pinned user
 @param clientIds - this is list of pingged clientID who going to be unpinned
 this is a required property , can't be nil.
*/
-(void)unpinUsers:(NSArray *_Nonnull)clientIds;


#pragma mark- Knock- knock room
/**
    Room API to allowed user in knock knock room
    @param clientId  - NSString  _Nonnull  client Id for the participent who going to get approved in knock- knock room
    @details App user will used this method to allowed  to get in any user in knock -knock based room
 */
-(void)approveAwaitedUser:(NSString *_Nonnull)clientId;
/**
    Room API to deny user in knock knock room
    @param clientId  - NSString  _Nonnull  client Id for the participent who going to get deny in knock- knock room
    @details App user will used this method to deny  to get in any user in knock -knock based room
 */
-(void)denyAwaitedUser:(NSString *_Nonnull)clientId;


/**
 Client endpoint can set options at room level. to set Talker Event option after join room
@param enable contain the details information for either subscribe and unsubicribe Talker Events
 */

-(void)subscribeForTalkerNotification:(BOOL)enable;


#pragma mark- add/remove spotlight
/**
 Client endpoint can use this method to add Spotlight for any of the available user in room , while pass their clientIDs
 @param clientIds - this is list of clientID who going to be pinged
 this is a required property , can't be nil.
*/
//pin user
-(void)addSpotlightUsers:(NSArray *_Nonnull)clientIds;
/**
 Client endpoint can use this method to remove  Spotlight any of the added Spotlight user in room , while pass their clientIDs of pinned user
 @param clientIds - this is list of pingged clientID who going to be unpinned
 this is a required property , can't be nil.
*/
-(void)removeSpotlightUsers:(NSArray *_Nonnull)clientIds;


#pragma mark- switch room mode

/**
 Client endpoint can use this method to switch room mode  for any of the available user in room , while pass their room mode
 @param roomMode - this is room mode lecture /group
 this is a required property , can't be nil.
*/

-(void)switchRoomMode:(NSString *_Nonnull)roomMode;

@end

