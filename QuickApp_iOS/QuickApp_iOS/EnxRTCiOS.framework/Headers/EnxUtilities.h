#import <Foundation/Foundation.h>

@interface NSDictionary (EnxUtilities)

+ (NSDictionary *)dictionaryWithJSONString:(NSString *)jsonString;
+ (NSDictionary *)dictionaryWithJSONData:(NSData *)jsonData;

@end
