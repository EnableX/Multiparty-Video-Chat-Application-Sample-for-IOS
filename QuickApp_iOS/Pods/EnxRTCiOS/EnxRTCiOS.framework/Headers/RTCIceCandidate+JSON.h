@import WebRTC;

@interface RTCIceCandidate (JSON)

+ (RTCIceCandidate *)candidateFromJSONDictionary:(NSDictionary *)dictionary;
- (NSData *)JSONData;

@end
