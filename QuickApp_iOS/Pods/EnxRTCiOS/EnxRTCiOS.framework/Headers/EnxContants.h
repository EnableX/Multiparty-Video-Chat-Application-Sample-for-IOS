//
//  EnxContants.h
//  EnxiOS_SampleApp
//
//  Created by Enablex on 06/07/18.
//  Copyright © 2018 Vcloudx. All rights reserved.
//

#ifndef EnxContants_h
#define EnxContants_h

/* EnxStream */
#define kEnxAudioLabel                @"LCMSa0"
#define kEnxVideoLabel                @"LCMSv0"

/// Video option
#define kStreamOptionVideo            @"video"
/// Audio option
#define kStreamOptionAudio            @"audio"
/// Data option
#define kStreamOptionData             @"data"
/// minVideoBW
#define kStreamOptionMinVideoBW       @"minVideoBW"
/// maxVideoBW
#define kStreamOptionMaxVideoBW       @"maxVideoBW"
/// scheme
#define kStreamOptionScheme           @"scheme"
/// maxHeight
#define kStreamOptionMaxHeight        @"maxHeight"
/// maxWidth
#define kStreamOptionMaxWidth         @"maxWidth"
/// minHeight
#define kStreamOptionMinHeight        @"minHeight"
/// minWidth
#define kStreamOptionMinWidth         @"minWidth"

// Simulcast
#define ksimulcastOption              @"maxVideoLayers"

// AudioOnly
#define kAudioMuted                     @"audioMuted"

//VideoOnly
#define kVideoMuted                  @"videoMuted"




/* EnxRoom */
#define kRTCStatsTypeSSRC                   @"ssrc"
#define kRTCStatsBytesSent                  @"bytesSent"
#define kRTCStatsLastDate                   @"lastDate"
#define kRTCStatsMediaTypeKey               @"mediaType"
#define kRTCStatsTypegoogCandidatePair      @"googCandidatePair"
#define kRTCStatsgoogActiveConnection       @"googActiveConnection"
#define kRTCStatsgoogLocalAddress           @"googLocalAddress"
#define kRTCStatsgoogRemoteAddress          @"googRemoteAddress"
#define kRTCStatsgoogCodecName              @"googCodecName"
#define kRTCStatslocalcandidate             @"localcandidate"
#define kRTCStatsnetworkType                @"networkType"
#define kRTCStatsipAddress                  @"ipAddress"
#define kSteamMuteAudio                     @"streamAudioMute"
#define kSteamUnMuteAudio                   @"streamAudioUnMute"
#define kSteamMuteVideo                     @"streamVideoMute"
#define kSteamUnMuteVideo                   @"streamVideoUnMute"
#define kAudioAccessDeny                    @"audioAccessDeny"
#define kVideoAccessDeny                    @"videoAccessDeny"
#define kAudioVideoAccessDeny               @"AudiovideoAccessDeny"
#define kcameraSwitchDeny                   @"CameraSwitchDeny"
#define kCameraOpenDeny                     @"CameraOpenDeny"
#define kNoNetworkConnection                @"No internet"
#define kwifiNetworkConnection              @"wifi"
#define k4gNetworkConnection                @"4G"
#define k3gNetworkConnection                @"3G"
#define k2gNetworkConnection                @"2G"
#define kswitchedNetwork                    @"NetworkChange"
#define kmaxVideoDeny                       @"MaxVideoDeny"

#define kFrameRate                          @"framerate"
//#define kMinWidthResolution (1280)
//#define kMaxWidthResolution (1280)
//#define kMinHeightResolution (720)
//#define kMaxHeightResolution (720)

#define kMinWidthResolution (180)
#define kMaxWidthResolution (1280)
#define kMinHeightResolution (320)
#define kMaxHeightResolution (720)
#define kMaxBW (1500)
#define kMinBW (150)
//size.width = 1280;
//size.height = 720;
/// Event State Mechanism

//******Publish*****//

    #define kPublish                           @"publish"
    #define kPublish_In_Progress               @"publish_in_progress"
    #define kPublished                         @"published"
//******UnPublish*****//
    #define kUnPublish                         @"unPublish"
    #define kUnPublish_In_Progress             @"unPublish_in_progress"
//******Subscribe*****//

    #define kSubscribe                         @"subscribe"
    #define kSubscribing                       @"subscribing"
    #define kSubscribed                        @"subscribed"
//******UnSubscribe*****//
    #define kUnSubscribe                       @"unSubscribe"
    #define kUnSubscribe_In_Progress           @"unSubscribe_in_progress"

//******Disconnect*****//
    #define kDisconnect                            @"disconnect"
    #define kDisconnect_In_Progress                @"disconnecting"
    #define kDisconnected                          @"disconnected"

//******Recording*****//
    #define kRecord                                @"startrecord"
    #define kRecording_In_Progress                 @"recording_in_progress"
    #define kRecording                             @"recording"
    #define kRecording_Stopping                    @"recordingstopping"

//******Mute/Unmute Room*****//
    #define kMuteRoom                              @"muteroom"
    #define kMuteRoom_In_Progress                  @"muteroom_in_progress"
    #define kMutedRoom                             @"mutedroom"
    #define kUnMuteRoom_In_Progress                @"unmuteroom_in_progress"

//******Floor Request *****//
    #define kRequestFloor                          @"requestfloor"
    #define kRequestFloor_In_Progress              @"requestfloor_in_progress"
    #define kRequestedFloor                        @"requestedfloor"
    #define kRequestFloorAccepted                  @"requestflooraccepted"

//******Floor Grant *****//
    #define kGrant_Floor_Request                   @"grantfloorrequest"
    #define kGrant_Floor_Request_In_Progress       @"grantingfloorrequest"
    #define kGranted_Floor_Request                 @"grantedfloorrequest"

//******Floor Deny *****//
//    #define kDeny_Floor_Request                        @"denyfloorrequest"
//    #define kDeny_Floor_Request_In_Progress            @"deningfloorrequest"
//    #define kDenied_Floor_Request                      @"deniedfloorrequest"

//******Floor Release *****//
   // #define kRelease_Floor_Request_In_Progress         @"deningfloorrequest"


//******Floor Cancle *****//
    #define kCancle_Floor_Request                        @"cancelfloor"
    #define kCancle_Floor_Request_In_Progress            @"cancelfloor_in_progress"
    
//******Floor Finished *****//
    #define kFinish_Floor_Request                        @"finishfloor"
    #define kFinish_Floor_Request_In_Progress            @"finishfloor_in_progress"
//******Change to audio only*****//

    #define kChange_To_Audio                            @"change_to_audio"
    #define kChange_To_Audio_In_Progress                @"change_to_audio_in_progress"
    #define kChanged_To_Audio                           @"changed_to_audio"
    #define kChanged_To_AudioVideo_In_Progress          @"changed_to_audiovideo_in_progress"

//******Set active talker*****//

    #define kSet_Active_Talker                           @"set_active_talker"
    #define kSet_Active_Talker_In_Progress               @"activetalker_in_progress"

//******Send Logs*****//
    #define kSendLog                                     @"sendlogs"
    #define kSendLog_In_Progress                         @"sendlogs_in_progress"

//******Self Mute Audio*****//
    #define kSelf_Mute_Audio                             @"self_mute_audio"
    #define kSelf_Mute_Audio_Request_In_Progress         @"self_mute_audio_in_progress"
    #define kSelf_Muted_Audio                            @"self_muted_audio"
    #define kSelf_Not_muted                             @"selfnotmuted"
    #define kHARD_MUTE_AUDIO                             @"hard_mute_audio"

//******Self UnMute Audio*****//
    #define kSelf_UnMute_Audio                             @"self_unmute_audio"
    #define kSelf_UnMute_Audio_Request_In_Progress         @"self_unmute_audio_in_progress"
    #define kSelf_UnMuted_Audio                            @"self_unmuted_audio"


//******Self Mute Video*****//
    #define kSelf_Mute_Video                             @"selfmutevideo"
    #define kSelf_Mute_Video_Request_In_Progress         @"self_mute_video_in_progress"
    #define kSelf_Muted_Video                            @"self_muted_video"
    #define kSelf_Not_muted_Video                        @"selfnotmutedVideo"
    #define kHARD_MUTE_Video                             @"single_hardmute_video"
    #define  kAudioOnly_Room                            @"AudioOnlyRoom"

//******Self UnMute Audio*****//
    #define kSelf_UnMute_Video                             @"self_unmute_video"
    #define kSelf_UnMute_Video_Request_In_Progress         @"self_unmute_video_in_progress"
    #define kSelf_UnMuted_Video                            @"self_unmuted_video"

//******Canvas Observer*****//

#define kIsCanvasStarted                                   @"IsCanvasStarted"
#define kTextEditDone                                      @"EditDone"
#define kTextOption                                         @"DoenSelection"
#define kreannotatePoint                                    @"reannotatePoint"
#define kfinishreannotate                                  @"finishreannotate"
#define kSingleUndo                                    @"singleAnnotationundo"
#define kfinishedUndo                                  @"finishedUndo"
#define kUndoAll                                        @"undoAll"
#define kfinishedAllUndo                                  @"finishedAllUndo"

//******Player Observer*****//

#define kIsStatsUnable                                      @"isStatsUnable"

/*** Stats State ***/

#define kSubscribeStats                                      @"subscribeStats"
#define kSubscribeStatsinProgress                            @"subscribeStatsInProgress"
#define kSubscribedStats                                     @"subscribedStats"
#define kStopSubscribeStatsinProgress                        @"StopsubsubscribeStatsInProgress"

/*** Reconnect State ***/
#define kReconnect                                           @"reconnect"
#define kReconnectInProgress                                 @"reconnectInProgress"


/*** Reconnect State ***/
#define kCaptchaOn                                           @"startcaptcha"
#define kCaptchaOff                                           @"stopcaptcha"

/*
    Outbound Call
 */
#define kStartOutBoundCall                                  @"startCall"
#define kOutBoundCallInProgress                             @"callInProgress"
#define kOutBoundCallEnd                                    @"callEnd"
/* Annotation*/
#define kStartAnnotation                         @"StartAnnotation"
#define kStartAnnotationInProgress               @"StartAnnotationInProgress"
#define kStartAnnotationStartted                 @"StartAnnotationStarted"

//******Lock/Unlock Room*****//
#define kLockRoom                              @"lockroom"
#define kLockRoom_In_Progress                  @"Lockroom_in_progress"
#define kLockdRoom                             @"Lockdroom"
#define kUnLockRoom_In_Progress                @"unLockroom_in_progress"

/* Canvas*/
#define kStartCanvas                         @"StartCanvas"
#define kStartCanvasInProgress               @"StartCanvasInProgress"
#define kStartCanvasStartted                 @"StartCanvasStarted"

#endif /* EnxContants_h */



