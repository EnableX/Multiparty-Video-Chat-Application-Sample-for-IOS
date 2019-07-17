@import WebRTC;

@interface RTCSessionDescription (JSON)

+ (RTCSessionDescription *)descriptionFromJSONDictionary:(NSDictionary *)dictionary;
- (NSData *)JSONData;

@end
