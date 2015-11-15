#import <objc/objc.h>

void objc_test()
{
    @try {
        @throw nil;
    } @catch(id ex) { }
}

