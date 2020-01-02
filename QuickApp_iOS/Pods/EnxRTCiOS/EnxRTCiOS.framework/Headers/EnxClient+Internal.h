#import "EnxClient.h"
#import "EnxSignalingMessage.h"
#import "EnxRoom.h"
@interface EnxClient () <EnxSignalingChannelDelegate, RTCPeerConnectionDelegate>

@property(nonatomic, strong) RTCPeerConnection *peerConnection;
@property(nonatomic, assign) BOOL hasReceivedSdp;
@property(nonatomic, assign) BOOL isInitiator;


@end

