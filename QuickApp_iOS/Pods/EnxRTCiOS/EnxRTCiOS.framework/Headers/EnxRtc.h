#import <Foundation/Foundation.h>
@import WebRTC;
#import "EnxRtc.h"
#import "EnxRoom.h"
#import "EnxStream.h"

@protocol EnxTroubleShooterDelegate <NSObject>
-(void)didClientDiagnosisFinished:(NSArray * _Nullable)data;
-(void)didClientDiagnosisFailed:(NSArray * _Nullable)data;
-(void)didClientDiagnosisStatus:(NSArray * _Nullable)data;
-(void)didClientDiagnosisStopped:(NSArray * _Nullable)data;
@end

@interface EnxRtc : NSObject<EnxRoomDelegate>{
    
}
@property(nonatomic,weak) id<EnxTroubleShooterDelegate> _Nullable delegate;
/**
 To quick start and join the room.
 @param token it is encoded token string received from Enx application server.
 @param delegate <EnxRoomDelegate>). If developer looks to handle the success or failure of the action, then they can implement delegates methods for the same.
 @param publishStreamInfo JSON
 
 @return EnxRoom object
 */

-(EnxStream *_Nullable)joinRoom:(NSString *_Nonnull)token delegate:(id _Nonnull )delegate PublishStreamInfo:(NSDictionary *_Nonnull)publishStreamInfo roomInfo:(NSDictionary * _Nullable)roomInfo advanceOptions:(NSArray *_Nullable)advanceOption;


/*PreCall Test API*/
-(void)clientDiagnostics:(NSDictionary *_Nonnull)options;

@end

