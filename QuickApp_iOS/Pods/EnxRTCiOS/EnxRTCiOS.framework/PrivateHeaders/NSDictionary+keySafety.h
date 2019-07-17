//
//  NSDictionary+keySafety.h
//  VCXiOS_SampleApp
//
//  Created by Jay Kumar on 16/01/19.
//  Copyright © 2019 Hemrajjhariya. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSDictionary (keySafety)
    -(id)safeObjectForKey:(id)key;
@end

NS_ASSUME_NONNULL_END
