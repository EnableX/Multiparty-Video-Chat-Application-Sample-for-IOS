//
//  NSDictionary+keySafety.h
//  VCXiOS_SampleApp
//
//  Created by Enablex on 16/01/19.
//  Copyright © 2019 Vcloudx. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSDictionary (keySafety)

-(id)safeObjectForKey:(id)key;

@end

NS_ASSUME_NONNULL_END
