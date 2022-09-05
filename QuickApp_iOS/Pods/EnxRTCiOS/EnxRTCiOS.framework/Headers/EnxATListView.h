//
//  EnxATListView.h
//  EnxRTCiOS
//
//  Created by VCX-LP-11 on 27/05/20.
//  Copyright © 2020 Enablex. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EnxStream.h"

NS_ASSUME_NONNULL_BEGIN

@protocol EnxTalkerStreamDelegate <NSObject>
    -(void)didSelectedStreamAtIndex:(EnxStream*)stream;
@end
@interface EnxATListView : UIView
@property (weak, nonatomic) id playerDelegate;
@property(weak,nonatomic) id<EnxTalkerStreamDelegate> delegate;

-(id)initWithFrame:(CGRect)frame;
-(void)updateView:(NSArray* _Nonnull)array;
-(void)preapareATView:(NSArray * _Nonnull)list;
-(void)updateLoayout;
-(void)switchATView:(BOOL)isList;
-(void)forceUpdate;
-(EnxStream*)getStream:(NSString* _Nonnull)clientID;
-(void)highlightBorderColorForClient:(NSArray*_Nonnull)clientIDs;
-(void)chnageBGColorForClient:(NSArray*_Nonnull)clientIDs withColor:(UIColor* _Nonnull)color;


@end

NS_ASSUME_NONNULL_END
