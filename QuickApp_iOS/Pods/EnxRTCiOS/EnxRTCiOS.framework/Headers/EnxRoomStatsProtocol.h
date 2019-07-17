#import <Foundation/Foundation.h>
@import WebRTC;
#import "EnxClient.h"
#import "EnxRoom.h"

@class EnxRoom;

/**
 EnxRoomStatsDelegate

 Will fire events related with publishing/subscribing stats.

 When publishingStats is YES, stats will be collected each 2 seconds.
 To receive calls from this Protocol you should assign a statsDelegate
 to the room instance.
 */
@protocol EnxRoomStatsDelegate <NSObject>
@optional
/**
 Receive a notification with the current bitrate as measured in intervals
 of 2 seconds.

 @param room Instance of the room where event happen.
 @param publishingClient the client who is publishing.
 @param mediaType String representing the mediaType as kRTCMediaStreamTrackKind(Audio/Video).
 @param ssrc String representing the source id of the stream.
 @param kbps Long value representing the current bitrate for the track.

 */
- (void)room:(EnxRoom *)room publishingClient:(EnxClient *)publishingClient
                                   mediaType:(NSString *)mediaType
                                        ssrc:(NSString *)ssrc
                         didPublishingAtKbps:(long)kbps;
/**
 Receive a notification of several stats gathered by libjingle.

 @param room Instance of the room where event happen.
 @param publishingClient the client who is publishing.
 @param mediaType String representing the mediaType as kRTCMediaStreamTrackKind(Audio/Video).
 @param ssrc String representing the source id of the stream.
 @param statsReport RTCLegacyStatsReport object.

 */
- (void)room:(EnxRoom *)room publishingClient:(EnxClient *)publishingClient
                                   mediaType:(NSString *)mediaType
                                        ssrc:(NSString *)ssrc
                                PublishingAtKbps:(long)kbps
                             didReceiveStats:(RTCLegacyStatsReport *)statsReport;

- (void)room:(EnxRoom *)room subscribeClient:(EnxClient *)subscribeClient
   mediaType:(NSString *)mediaType
        ssrc:(NSString *)ssrc
subscribeAtKbps:(long)kbps
didReceiveStats:(RTCLegacyStatsReport *)statsReport;
@end
