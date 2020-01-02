@import WebRTC;
#import "EnxClientState.h"

@class EnxClient;

///-----------------------------------
/// @protocol EnxClientDelegate Protocol
///-----------------------------------

/**
 @protocol EnxClientDelegate

 Classes that implement this protocol will be called for RTC Client
 event notification.

 */
@protocol EnxClientDelegate <NSObject>

- (void)appClient:(EnxClient *)client didChangeState:(EnxClientState)state;
- (void)appClient:(EnxClient *)client didChangeConnectionState:(RTCIceConnectionState)state;
- (void)appClient:(EnxClient *)client didReceiveRemoteStream:(RTCMediaStream *)remoteStream
                                          withStreamId:(NSString *)streamId;
- (void)appClient:(EnxClient *)client didError:(NSError *)error;
- (RTCMediaStream *)streamToPublishByAppClient:(EnxClient *)client;
- (NSDictionary *)appClientRequestICEServers:(EnxClient *)client;
-(void)ICEConnectionFailed;

@end
