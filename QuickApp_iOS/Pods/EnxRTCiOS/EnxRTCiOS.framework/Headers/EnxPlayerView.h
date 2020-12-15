#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
@import WebRTC;
#import "EnxStream.h"
#import "EnxContants.h"

@class EnxStream;

//typedef NS_ENUM(BOOL , ViewType) {
//    isLocal = 0 ,
//    isRemote,
//};


/**
 @enum EnxRoomStatus
 */
typedef NS_ENUM(NSInteger, TextPosition) {
    TOP,
    CENTER,
    BOTTOM
};


@protocol EnxPlayerDelegate <NSObject>
@optional
-(void)didPlayerStats:(NSDictionary * _Nonnull)data;
-(void)didCapturedView:(UIImage*_Nonnull)snapShot;
@end


@interface EnxPlayerView : UIView <RTCVideoViewDelegate>
///-----------------------------------
/// @name Initializers
///-----------------------------------
-(instancetype _Nonnull)init UNAVAILABLE_ATTRIBUTE;
-(instancetype _Nonnull )initWithCoder:(NSCoder *_Nonnull)aDecoder UNAVAILABLE_ATTRIBUTE;
- (instancetype _Nonnull)initWithFrame:(CGRect)frame UNAVAILABLE_ATTRIBUTE;

-(instancetype _Nonnull)initLocalView:(CGRect)frame;
-(instancetype _Nonnull)initRemoteView:(CGRect)frame;

// To set UIViewContentMode of playerView
//Note: Works only with devices which supports metal.
-(void)setContentMode:(UIViewContentMode)contentMode;
@property(readonly) BOOL staTsFlag;
@property (nonatomic) BOOL isPingUSer;
/// EnxPlayerDelegate were this Player will invoke methods as events.
@property (weak, nonatomic) id <EnxPlayerDelegate> _Nullable delegate;
-(void)enablePlayerStats:(BOOL)flag;
-(void)setUpdateStats:(NSDictionary * _Nonnull)data;
-(void)captureScreenShot;
-(void)createOverLayView:(NSString *_Nonnull)message;
-(void)removeOverLayView;
-(void)setConfigureOption:(NSDictionary *_Nonnull)options;
-(UIView*_Nullable)getremoteView;
-(UIView*_Nullable)getlocalView;
//-(void)createAvatar:(CGRect)frame;
//-(void)removeAvatar;
-(void)createOverlayIcons:(NSDictionary *_Nullable)playerConfiguartion;
-(void)videoMutedByUser:(NSDictionary *_Nullable)playerConfiguartion;
-(void)videoUnMutedByUser;
-(void)videoMutedByBW:(NSDictionary *_Nullable)playerConfiguartion;
-(void)bandWidthRecovered;
-(void)audioMuted:(NSDictionary *_Nullable)playerConfiguartion;
-(void)audioUnmuted;
-(void)showAndHideStats:(BOOL)flag;
-(void)updateCollectionViewFrame;
-(void)showAndHidePinged:(BOOL)flag;

@end
