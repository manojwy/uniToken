//
//  main.m
//  Tokenizer
//
//  Created by Manoj on 11/03/14.
//  Copyright (c) 2014 Manoj. All rights reserved.
//

#import <Foundation/Foundation.h>

void Test();
int sqlite3FtsUnicodeIsalnum(int c);


int main(int argc, const char * argv[])
{

    @autoreleasepool {
        
        // insert code here...
        NSLog(@"Hello, World! - START");

        Test();
        
//        बीबीसी हिंदी डॉटकॉम पर भारत, पाकिस्तान और चीन सहित 
//        NSString *uni2=[[NSString alloc] initWithUTF8String:"दुनिया भर की ताज़ा ख़बरें"];
        NSString *uni2=[[NSString alloc] initWithUTF8String:"Esto es manoj, poniendo a prueba este código"];
        
        unicodeTokens([uni2 cStringUsingEncoding:NSUTF8StringEncoding]);
        
        NSLog(@"Hello, World! - END");
    }
    return 0;
}

