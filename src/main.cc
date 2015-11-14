#include <papi.h>
#include <iostream>
#include <cassert>
#include <cstdlib>

//bool USE_PAPI;
#define USE_PAPI 0

#define NOINLINE __attribute__((noinline,noclone))

const size_t num_iter = 1000000;

extern "C" void objc_test();

void papi_fail(int rc, const char *msg)
{
    std::cerr << "Error: " << msg << '\n';
    if (USE_PAPI && rc != 0) {
        const char *papi_err = PAPI_strerror(rc);
        std::cerr <<papi_err << '\n';
    }
    exit(EXIT_FAILURE);
}

__attribute__((noinline))
void test_throw()
{
    throw 1;
}

template<void (*pfn)(void)>
void test_catch()
{
    for (size_t i = 0; i < num_iter; i++) {
        try {
            pfn();
        } catch (int) { }
    }
}

static volatile int glob;

template<void (*pfn)(void), size_t N>
struct test_catch_nested
{
    __attribute__((noinline))
    static void run()
    {
        glob = N;
        test_catch_nested<pfn, N-1>::run();
        glob = N+1;
    }

    __attribute__((noinline))
    static void test()
    {
        try {
            run();
        } catch (int) { }
    }
};

template<void (*pfn)(void)>
struct test_catch_nested<pfn, 0>
{
    NOINLINE static void run()
    {
        glob = 0;
        pfn();
        glob = 1;
    }

    NOINLINE static void test()
    {
        try {
            run();
        } catch (int) { }
    }
};

int main(int, char**)
{
    typedef void (*pfn_t)();
    pfn_t test[7] = {
//                &objc_test,
            &test_catch_nested<test_throw, 0>::test,
            &test_catch_nested<test_throw, 1>::test,
            &test_catch_nested<test_throw, 2>::test,
            &test_catch_nested<test_throw, 3>::test,
            &test_catch_nested<test_throw, 4>::test,
            &test_catch_nested<test_throw, 5>::test,
            &test_catch_nested<test_throw, 6>::test
    };
    int event_set = PAPI_NULL;

    if (USE_PAPI) {
        int rc = PAPI_library_init(PAPI_VER_CURRENT);
        if (rc != PAPI_VER_CURRENT && rc > 0)
                papi_fail(0, "PAPI library version mismatch");
        else if (rc < 0)
                papi_fail(0, "failed to init PAPI libarary");
        rc = PAPI_is_initialized();
        if (rc != PAPI_LOW_LEVEL_INITED)
                papi_fail(0, "failed to init PAPI libarary");
        rc = PAPI_create_eventset(&event_set);
        if (rc != PAPI_OK)
                papi_fail(rc, "failed to create event set");
        rc = PAPI_add_event(event_set, PAPI_TOT_CYC);
        if (rc != PAPI_OK)
                papi_fail(rc, "failed to add total cycles counter");
        rc = PAPI_add_event(event_set, PAPI_TOT_INS);
        if (rc != PAPI_OK)
                papi_fail(rc, "failed to add total insns counter");
        std::cout << "depth,tot_cyc,tot_insn,iter\n";
    }

    // Warm up
    for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); i++)
        test[i]();

    enum counter_t {
        CNT_TOT_CYC = 0,
        CNT_TOT_INS = 1,
        CNT_NUM
    };
    long long counters[CNT_NUM];

    for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); i++) {
        if (USE_PAPI) {
            int rc = PAPI_start(event_set);
            if (rc != PAPI_OK)
                papi_fail(rc, "failed to start counters");
        }

        for (size_t iter = 0; iter < num_iter; iter++)
            test[i]();

        if (USE_PAPI) {
            int rc = PAPI_stop(event_set, counters);
            if (rc != PAPI_OK)
                papi_fail(rc, "failed to stop counters");
            rc = PAPI_reset(event_set);
            if (rc != PAPI_OK)
                papi_fail(rc, "failed to reset counters");
            std::cout << i + 1 << ',' << counters[CNT_TOT_CYC]
                            << ',' << counters[CNT_TOT_INS]
                            << ',' << num_iter << '\n';
        }
    }

    if (USE_PAPI) {
        int rc = PAPI_cleanup_eventset(event_set);
        if (rc != PAPI_OK)
            papi_fail(rc, "failed to cleanup event set");
        rc = PAPI_destroy_eventset(&event_set);
        if (rc != PAPI_OK)
            papi_fail(rc, "failed to destroy event set");
    }
    return 0;
}

