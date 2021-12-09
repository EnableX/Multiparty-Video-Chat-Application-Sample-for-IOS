//
//  EnxAudioManager.h
//  VCXiOS_SampleApp
//
//  Created by Enablex on 19/02/19.
//  Copyright © 2019 Enablex. All rights reserved.
//
@import WebRTC;
#import <Foundation/Foundation.h>
#import "EnxRoom.h"
#if TARGET_IPHONE_SIMULATOR
    
#else
    #import <CoreTelephony/CTTelephonyNetworkInfo.h>
#endif
//#import <WebRTC/RTCAudioSession.h>
//NS_ASSUME_NONNULL_BEGIN

#ifdef DEBUG
#define L_DEBUG(f, ...) { \
logThis(LOG_MODE_DEBUG, @(__FILE__), @(__LINE__), f, ##__VA_ARGS__); \
}
#else
#define L_DEBUG(f, ...) {}
#endif

#define L_ERROR(f, ...) { \
logThis(LOG_MODE_ERROR, @(__FILE__), @(__LINE__), f, ##__VA_ARGS__); \
}

#define L_WARNING(f, ...) { \
logThis(LOG_MODE_WARNING, @(__FILE__), @(__LINE__), f, ##__VA_ARGS__); \
}

#define L_INFO(f, ...) { \
logThis(LOG_MODE_INFO, nil, nil, f, ##__VA_ARGS__); \
}

@protocol EnxMediaDeledate <NSObject>

@optional
-(void)newAudioMediaConnected;
-(void)currentConnectedNetwork:(NSString*)networkName;
@end
typedef NS_ENUM(NSInteger, EnxCustomeEventType) {
    Error,
    Info,
    Debug,
    Warning,
    Success,
};

typedef enum {
    LOG_MODE_UNKNOWN,
    LOG_MODE_DEBUG,
    LOG_MODE_INFO,
    LOG_MODE_WARNING,
    LOG_MODE_ERROR
} LOG_MODE;

typedef NS_OPTIONS(NSUInteger, LOG_MODE_MASK) {
    LOG_MODE_DEBUG_MASK         = 1 << LOG_MODE_DEBUG,
    LOG_MODE_INFO_MASK          = 1 << LOG_MODE_INFO,
    LOG_MODE_WARNING_MASK       = 1 << LOG_MODE_WARNING,
    LOG_MODE_ERROR_MASK         = 1 << LOG_MODE_ERROR
};

@class EnxUtilityManager;
@interface EnxUtilityManager : NSObject <RTCAudioSessionDelegate>
@property(nonatomic,weak) id <EnxMediaDeledate> delegate;
@property(nonatomic,strong) NSString *previousNetwork;
@property(nonatomic,readonly,strong) EnxRoom *mRoom;
@property(nonatomic)BOOL isNoNetwork;
@property(nonatomic,readonly,strong) NSString *sdkVersion;
@property(nonatomic,readonly,strong) NSString *osVersion;
@property(nonatomic,readonly,strong) NSString *deviceName;


+(EnxUtilityManager*)shareInstance;
-(BOOL)setAudioDevice:(NSString*)name;
-(NSString*)selectedDevice;
-(NSArray*)connectedDevice;
-(NSString*)currentConnectNetwork;
-(void)setPriviousNetwork;

/*Custome Events*/
-(void)setEventServerURL:(NSString *)serverURL;
-(NSString*)getEventServer;
-(void)setEventToken:(NSString*)token;
-(NSString*)getEventToken;
-(void)sendCustomeEvent;
-(void)setLogId:(NSString*)logId;
-(void)customeEvent:(NSString *)eventLavelName eventType:(EnxCustomeEventType)type eventName:(NSString*)eventname eventData:(NSDictionary*)eventdata;
-(void)checkForStoreRequest;
/*These two variables used in enxclient class only*/
-(NSMutableArray *)getMessageQueue;
-(NSMutableArray *)geticeServers;
-(void)setSDKVersion:(NSString* _Nonnull)version;
-(NSString *_Nonnull)getSDKVersion;
-(NSString*_Nonnull)getOsVersion;
-(NSString*_Nonnull)getDeviceName;

/*PreCall Test API*/
-(void)clientDiagnostics:(NSDictionary *_Nonnull)options;

/* Property And Methods for client Loger */
///-----------------------------------
/// @name Properties
///-----------------------------------
@property (strong,nonatomic) NSString *vcxLogId;
///-----------------------------------
/// @name Methods
///-----------------------------------

- (void)log:(LOG_MODE)mode file:(NSString *)file line:(NSNumber *)line format:(NSString *)format, ...;
- (void)log:(LOG_MODE)mode file:(NSString *)file line:(NSNumber *)line format:(NSString *)format args:(va_list)args;
- (void)logWithModesOverride:(LOG_MODE_MASK)overrideModes mode:(LOG_MODE)mode file:(NSString *)file line:(NSNumber *)line
                      format:(NSString *)format args:(va_list)args;

// To send logs to server.
-(NSString *)getLogsFromFile;
-(BOOL)checkLogsStatus;
-(void)startLog;
-(BOOL)getLoggerStatus;
/*Drain All object once Session closed*/
-(void)drainAllObjectRefrence;
//Thame color
-(void)setIconColor:(UIColor *)color;
-(UIColor*)getThameColor;
- (UIColor *)colorFromHexString:(NSString *)hexString;
-(void)setMRoom:(EnxRoom *)mRoom;
//Start Screen Share
-(void)setAppGroupsName:(NSString * _Nonnull)appGroup withUserKey:(NSString* _Nonnull)key;
-(NSString* _Nonnull)getAppGroupName;
-(NSString* _Nonnull)getUserKey;

@end

FOUNDATION_STATIC_INLINE void logThis(LOG_MODE mode, NSString *file, NSNumber *line, NSString *format, ...) {
    va_list args;
    va_start(args, format);
#ifdef LOG_MODES
    LOG_MODE_MASK overrideModes = LOG_MODES;
    [[Logger sharedInstance] logWithModesOverride:overrideModes mode:mode file:file line:line format:format args:args];
#else
    [[EnxUtilityManager shareInstance] log:mode file:file line:line format:format args:args];
#endif
    va_end(args);
}
//NS_ASSUME_NONNULL_END
