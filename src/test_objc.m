#import <objc/objc.h>
#import <stdint.h>

void objc_test(size_t num_iter)
{
    for (size_t i = 0; i < num_iter; i++) {
        @try {
            @throw nil;
        } @catch(id ex) { }
    }
}

