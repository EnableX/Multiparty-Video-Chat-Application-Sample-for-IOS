@import WebRTC;

@interface RTCIceCandidate (JSON)

+ (RTCIceCandidate *)candidateFromJSONDictionary:(NSDictionary *)dictionary forStream:(NSString*)streamID roomSubscription:(BOOL)isSubscription isAudioOnly:(BOOL)audioOnly;
- (NSData *)JSONData;

@end
