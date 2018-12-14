#import <UIKit/UIKit.h>
@import WebRTC;
#import "EnxStream.h"
@class EnxStream;

@interface EnxPlayerView : UIView <RTCVideoViewDelegate>

///-----------------------------------
/// @name Initializers
///-----------------------------------

//@property (strong, nonatomic) EnxStream *stream;

- (instancetype)initWithFrame:(CGRect)frame;
// To set UIViewContentMode of playerView
//Note: Works only with devices which supports metal.
-(void)setContentMode:(UIViewContentMode)contentMode;
@property(nonatomic, readonly) __kindof UIView<RTCVideoRenderer> *remoteVideoView;

@property(nonatomic, strong) RTCCameraPreviewView *localVideoView;



//- (void)RemoveView;

//- (void)AddView;


@end
