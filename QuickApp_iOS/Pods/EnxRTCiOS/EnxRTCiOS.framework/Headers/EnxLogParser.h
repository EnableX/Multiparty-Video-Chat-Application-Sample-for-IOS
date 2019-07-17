//
//  EnxLogParser.h
//  EnxRTCiOS
//
//  Created by Enablex on 09/01/19.
//  Copyright © 2019 Vcloudx. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EnxRoom.h"
#import "EnxStream.h"
NS_ASSUME_NONNULL_BEGIN

@interface EnxLogParser : NSObject

+(NSDictionary *)connectionSuccessLog:(EnxRoom *)room logId:(NSString *)logId ipAddress:(NSString *)ipAddress;
+(NSDictionary *)connectionFailureLog:(EnxRoom *)room logId:(NSString *)logId error:(NSString *)errorMessage;

+(NSDictionary *)publishStreamSuccessLog:(EnxRoom *)room logId:(NSString *)logId streamId:(NSString*)streamId negotiatedCodec:(NSDictionary *)negotiatedCodec selectedCandidateDict:(NSDictionary *)selectedCandidateDict ipAddress:(NSString *)ipAddress;
+(NSDictionary *)publishStreamFailureLog:(EnxRoom *)room logId:(NSString *)logId error:(NSString *)errorMessage negotiatedCodec:(NSDictionary *)negotiatedCodec selectedCandidateDict:(NSDictionary *)selectedCandidateDict;

+(NSDictionary *)subscribeStreamSuccessLog:(EnxRoom *)room logId:(NSString *)logId streamId:(NSString*)streamId negotiatedCodec:(NSDictionary *)negotiatedCodec selectedCandidateDict:(NSDictionary *)selectedCandidateDict ipAddress:(NSString *)ipAddress;
+(NSDictionary *)subscribeStreamFailureLog:(EnxRoom *)room logId:(NSString *)logId error:(NSString *)errorMessage negotiatedCodec:(NSDictionary *)negotiatedCodec selectedCandidateDict:(NSDictionary *)selectedCandidateDict forSteamId:(NSString*)streamID;


@end

NS_ASSUME_NONNULL_END
