#import <Foundation/Foundation.h>

@class RTCSessionDescription;

@interface EnxSDPUtils : NSObject

/// Updates the original SDP description to instead prefer the specified video
/// codec. We do this by placing the specified codec at the beginning of the
/// codec list if it exists in the sdp.
//+ (RTCSessionDescription *)descriptionForDescription:(RTCSessionDescription *)description preferredVideoCodec:(NSString *)codec;
+ (RTCSessionDescription *)descriptionForDescription:(RTCSessionDescription *)description preferredVideoCodec:(NSString *)codec forStreamID:(NSString *)streamID withBandWidth:(int)bandWidth;


/// Appends an SDP line after a regex matching existing line.
+ (RTCSessionDescription *)descriptionForDescription:(RTCSessionDescription *)description
                                        appendingLine:(NSString *)line
                                        afterRegexString:(NSString *)regexStr;

/// If `b=` is not defined adds `b=AS:{bandwidthLimit}` for the given media type.
+ (RTCSessionDescription *)descriptionForDescription:(RTCSessionDescription *)description
                                       bandwidthLimit:(NSInteger)bandwidthLimit
                                         forMediaType:(NSString *)mediaType;

/// Replace a matching SDP string regex template with a given new line string.
+ (RTCSessionDescription *)descriptionForDescription:(RTCSessionDescription *)description
                                  matchingPatternStr:(NSString *)matchingPatternStr
                                     replaceWithLine:(NSString *)replacementLine;

+ (RTCSessionDescription *)descriptionForDescription:(RTCSessionDescription *)description
                                       codecMimeType:(const NSString *)codec
                                          fmtpString:(NSString *)fmtpString
                                    preserveExistent:(BOOL)preserveExistent;

+ (RTCSessionDescription *)descriptionForDescription:(RTCSessionDescription *)description
                             mediaConfigurationCodec:(NSString *)mediaConfigurationCodec mediaCodecsInfoInSDP:(NSDictionary *)mediaCodecsInfoInSDP bandWidth:(NSString *)bandWidth;



@end
