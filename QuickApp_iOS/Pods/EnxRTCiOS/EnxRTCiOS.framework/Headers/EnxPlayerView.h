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
/*
    EnxPlayer delegate methods for user.
 these method will help you get stats for individual EnxPlayer, and capture image for individual EnxPlayer.
 */

@protocol EnxPlayerDelegate <NSObject>
@optional
/**
 This delegate method for getting stats for the EnxPlayer
    @param data will carry player stats details.
 @details To get the stats for the individual player , User need to set EnxPlayerDelegate player delegates and listen their -didPlayerStats:  call back method. didPlayerStats: will keep notify about stats of that player.
 */
-(void)didPlayerStats:(NSDictionary * _Nonnull)data;
/**
 This delegate method for give player image to user
    @param snapShot will carry image of player.
 @details To get thesnap shot for the individual player , User need to set EnxPlayerDelegate player delegates and listen their -didCapturedView:  call back method. didCapturedView: will keep notify about snapshot image whenever user demand.
 */
-(void)didCapturedView:(UIImage*_Nonnull)snapShot;
@end


@interface EnxPlayerView : UIView <RTCVideoViewDelegate>
///-----------------------------------
/// @name Initializers
///-----------------------------------
/*
    These method has blocked , so that client can not call these method to create instance of EnxPlayer through these method, Enxplayer instance will possible only wth SDK define method.
 */
-(instancetype _Nonnull)init UNAVAILABLE_ATTRIBUTE;
-(instancetype _Nonnull )initWithCoder:(NSCoder *_Nonnull)aDecoder UNAVAILABLE_ATTRIBUTE;
- (instancetype _Nonnull)initWithFrame:(CGRect)frame UNAVAILABLE_ATTRIBUTE;
/**
    This API will help client to creale local player, to display self video.
    @param frame is the preedefine CGRect value to set x,y coordiante and width,height of local player.
 @return its will give instance of local player
 */
-(instancetype _Nonnull)initLocalView:(CGRect)frame;
/**
    This API will help client to creale remote player, to display remote video.
    @param frame is the preedefine CGRect value to set x,y coordiante and width,height of remote player.
 @return its will give instance of remote player
 */
-(instancetype _Nonnull)initRemoteView:(CGRect)frame;

// To set UIViewContentMode of playerView
//Note: Works only with devices which supports metal.
-(void)setContentMode:(UIViewContentMode)contentMode;
//This property is used to set a flag to check if all ready stats on/off
@property(readonly) BOOL staTsFlag;
//This property is used to on/off pin icon on player.
@property (nonatomic) BOOL isPinUSer;
/// EnxPlayerDelegate were this Player will invoke methods as events.
@property (weak, nonatomic) id <EnxPlayerDelegate> _Nullable delegate;
/**
 Enable player Stats.
 @param flag its a bool value to enable/disable stats
 @details this API will help customer to enable/disable  player stats, while sending true/false value.
 trur for enable , false for desable.
 */
-(void)enablePlayerStats:(BOOL)flag;
/**
 update  Stats.
 @param data will have stats information
 @details this API will used By EnxRoom class to update latest stats value after stats enable, and new stats value arrive in EnxRoom Class.
 */
-(void)setUpdateStats:(NSDictionary * _Nonnull)data;
/**
 Snap shot
 @details this API will used By customer, its will help them to take snap shot of player (Either Local or remote)
 */
-(void)captureScreenShot;
/**
 Over lay
 @param message will has information which user wanted to display on player
 @details this API will used By customer, its will help them to create an overlay on player and show some randome text.
 */
-(void)createOverLayView:(NSString *_Nonnull)message;
/**
 remove over lay
 @details this API will used By customer, its will help them to remove created overlay from player.
 */
-(void)removeOverLayView;
/**
 Configure option
 @param options will has information about player overlay
 @details this  API used by customer , where customer can set configuration about overlay
 */
-(void)setConfigureOption:(NSDictionary *_Nonnull)options;
/**
 get remote view
 @return UIView of remote player
 @details this  API used by self/EnxStream to get all ready created remote player. so that sdk will not required to create new if needed by public.
 */
-(UIView*_Nullable)getremoteView;
/**
 get local view
 @return UIView of local player
 @details this  API used by self/EnxStream to get all ready created local player. so that sdk will not required to create new if needed by public.
 */
-(UIView*_Nullable)getlocalView;
/**
 overlay icon
 @param playerConfiguartion has information about overlay Icon
 @details this  API used by customer/ENxroom to set playerConfiguration like mute/unmute icon details,avatra icon details etc...
 */
-(void)createOverlayIcons:(NSDictionary *_Nullable)playerConfiguartion;
/**
 show video mute icon
 @param playerConfiguartion has information about video icon
 @details this  API used by customer/Enxroom to show video icon on playerView if video muted by user.
 */
-(void)videoMutedByUser:(NSDictionary *_Nullable)playerConfiguartion;
/**
 hide/remove video mute icon
 @details this  API used by customer/Enxroom to hide/remove video icon on playerView.
 */
-(void)videoUnMutedByUser;
/**
  video mute due to low bandwidth
 @param playerConfiguartion has information about video icon
 @details this  API used by Enxroom to show video icon on playerView if video muted or low bandwidth icon due to low bandwidth.
 */
-(void)videoMutedByBW:(NSDictionary *_Nullable)playerConfiguartion;
/**
 bandwidth recover
 @details this  API used by Enxroom to to update enxplayer about bandwidth recover, so that bandwidth icon or if video mute due to this will remove
 */
-(void)bandWidthRecovered;
/**
 show audio mute icon
 @param playerConfiguartion has information about audio icon
 @details this  API used by customer/Enxroom to show audio icon on playerView if audio muted by user.
 */
-(void)audioMuted:(NSDictionary *_Nullable)playerConfiguartion;
/**
 hide/remove audio mute icon
 @details this  API used by Enxroom to hide/remove audio icon on playerView.
 */
-(void)audioUnmuted;
/**
 show/hide stats icon
 @param flag true/false
 @details this  API used by customer/Enxroom to show/hide stats icon based on flag.
 */
-(void)showAndHideStats:(BOOL)flag;
/**
 collection video flar
 @details this  API used by EnxPlayer/EnxATListView to rearrange collection view size based on enxPlayer size
 */
-(void)updateCollectionViewFrame;
/**
 show/hide Pin icon
 @param flag true/false
 @details this  API used by customer/Enxroom to show/hide Pin icon based on flag.
 */
-(void)showAndHidePinged:(BOOL)flag;

/**
 show/hide Speaker Notification icon
 @param flag true/false
 @details this  API used by customer/Enxroom to show/hide Speaker Notification icon based on flag.
 */
-(void)showAndHideSpeakerNotification:(BOOL)flag;

@end
