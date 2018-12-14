#import "EnxClient.h"
#import "EnxSignalingChannel.h"
#import "EnxSignalingMessage.h"

@interface EnxClient () <EnxSignalingChannelDelegate, RTCPeerConnectionDelegate>

@property(nonatomic, strong) RTCPeerConnection *peerConnection;
@property(nonatomic, strong) RTCPeerConnectionFactory *factory;
@property(nonatomic, strong) NSMutableArray *messageQueue;
@property(nonatomic, assign) BOOL hasReceivedSdp;
@property(nonatomic, strong) EnxSignalingChannel *signalingChannel;
@property(nonatomic, assign) BOOL isInitiator;
@property(nonatomic, strong) NSMutableArray *iceServers;
@property(nonatomic, strong) RTCMediaConstraints *defaultPeerConnectionConstraints;


@end
