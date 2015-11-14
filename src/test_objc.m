#import <objc/objc.h>
#import <stdint.h>

static volatile int glob;

__attribute__((noinline)) void objc_test_throw(id obj)
{
    glob = 0;
    @throw obj;
    glob = 1;
}

void objc_test_catch(id obj)
{
    glob = 2;
    @try {
        objc_test_throw(obj);
    } @catch(id ex) {
    }
    glob = 3;
}

void objc_test(size_t num_iter)
{
    objc_test_catch(0);
}
