#import <Foundation/Foundation.h>


/**
 @interface EnxSignalingEvent
 */
@interface EnxSignalingEvent : NSObject

@property NSString *name;
@property NSDictionary *message;
@property NSString *streamId;
@property NSString *peerSocketId;
@property NSDictionary *attributes;
@property NSDictionary *updatedAttributes;
@property NSDictionary *dataStream;
@property BOOL audio;
@property BOOL video;
@property BOOL data;
@property BOOL screen;

- (instancetype)initWithName:(NSString *)name
                     message:(NSDictionary *)message;
@end
