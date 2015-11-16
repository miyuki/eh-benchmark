#import <objc/objc.h>

void objc_test(void)
{
    @try {
        @throw nil;
    } @catch(id ex) { }
}

