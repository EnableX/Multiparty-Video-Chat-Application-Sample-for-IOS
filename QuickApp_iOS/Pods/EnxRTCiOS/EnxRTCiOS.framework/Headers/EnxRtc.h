#import <Foundation/Foundation.h>
@import WebRTC;
#import "EnxRtc.h"
#import "EnxRoom.h"
#import "EnxStream.h"

@interface EnxRtc : NSObject<EnxRoomDelegate>{
    
}
/**
 To quick start and join the room.
 @param token it is encoded token string received from Enx application server.
 @param delegate <EnxRoomDelegate>). If developer looks to handle the success or failure of the action, then they can implement delegates methods for the same.
 @param publishStreamInfo JSON
 
 @return EnxRoom object
 */

-(EnxStream *_Nullable)joinRoom:(NSString *_Nonnull)token delegate:(id _Nonnull )delegate PublishStreamInfo:(NSDictionary *_Nonnull)publishStreamInfo roomInfo:(NSDictionary * _Nullable)roomInfo advanceOptions:(NSArray *_Nullable)advanceOption;
@end

