#include <iostream>
#include <cstdlib>
#include <vector>
#include <papi.h>

const size_t num_iter = 1000000;

// Objective C exception hadling
extern "C" void objc_test();

void call_0()
{
    throw 1;
}

void test_call_0()
{
    try {
        throw 1;
    } catch (int) { }
}

// Nested calls
void test_call_1();
void test_call_2();
void test_call_3();
void test_call_4();
void test_call_5();
void test_call_6();
void test_call_7();
void test_call_8();
void test_call_9();

// Classes
void test_classes_0();
void test_classes_10();
void test_classes_20();
void test_classes_30();
void test_classes_40();
void test_classes_49();

typedef void (*pfn_test_t)();
using test_t = std::pair<size_t, pfn_test_t>;
using test_vector_t = std::vector<test_t>;

static const test_vector_t objc_tests {
    {0, &objc_test},
};

static const test_vector_t call_tests {
    {0, &test_call_0},
    {1, &test_call_1},
    {2, &test_call_2},
    {3, &test_call_3},
    {4, &test_call_4},
    {5, &test_call_5},
    {6, &test_call_6},
    {7, &test_call_7},
    {8, &test_call_8},
    {9, &test_call_9},
};

static const test_vector_t classes_tests {
    {0, &test_classes_0},
    {10, &test_classes_10},
    {20, &test_classes_20},
    {30, &test_classes_30},
    {40, &test_classes_40},
    {49, &test_classes_49},
};

static void usage(const char *msg)
{
    std::cerr << msg
              << "\nUsage:\n"
              << "    benchmark {calls | classes | objc}\n";
}

static void fail(const char *msg)
{
    std::cerr << "Error: " << msg << '\n';
    exit(EXIT_FAILURE);
}

static void papi_fail(int rc, const char *msg)
{
    const char *papi_err = PAPI_strerror(rc);
    std::cerr << "Error: " << msg << '\n'
              << papi_err << '\n';
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        usage("Invalid number of arguments");

    std::string arg(argv[1]);
    const test_vector_t *test_vec = nullptr;
    if (arg == "objc")
        test_vec = &objc_tests;
    else if (arg == "calls")
        test_vec = &call_tests;
    else if (arg == "classes")
        test_vec = &classes_tests;
    else
        usage("Invalid argument");

    int event_set = PAPI_NULL;
    int rc = PAPI_library_init(PAPI_VER_CURRENT);
    if (rc != PAPI_VER_CURRENT && rc > 0)
            fail("PAPI library version mismatch");
    else if (rc < 0)
            fail("failed to init PAPI libarary");
    rc = PAPI_is_initialized();
    if (rc != PAPI_LOW_LEVEL_INITED)
            fail("failed to init PAPI libarary");
    rc = PAPI_create_eventset(&event_set);
    if (rc != PAPI_OK)
            papi_fail(rc, "failed to create event set");
    rc = PAPI_add_event(event_set, PAPI_TOT_CYC);
    if (rc != PAPI_OK)
            papi_fail(rc, "failed to add total cycles counter");
    rc = PAPI_add_event(event_set, PAPI_TOT_INS);
    if (rc != PAPI_OK)
            papi_fail(rc, "failed to add total insns counter");

    test_vector_t *test_vector = nullptr;
    std::cout << "test,tot_cyc,tot_insn,iter\n";

    // Warm up
    for (const test_t &test : *test_vec)
        test.second();

    enum counter_t {
        CNT_TOT_CYC = 0,
        CNT_TOT_INS = 1,
        CNT_NUM
    };
    long long counters[CNT_NUM];

    for (const test_t &test : *test_vec) {
        rc = PAPI_start(event_set);
        if (rc != PAPI_OK)
            papi_fail(rc, "failed to start counters");

        for (size_t i = 0; i < num_iter; i++)
            test.second();

        rc = PAPI_stop(event_set, counters);
        if (rc != PAPI_OK)
            papi_fail(rc, "failed to stop counters");
        rc = PAPI_reset(event_set);
        if (rc != PAPI_OK)
            papi_fail(rc, "failed to reset counters");
        std::cout << test.first
                  << ',' << counters[CNT_TOT_CYC]
                  << ',' << counters[CNT_TOT_INS]
                  << ',' << num_iter << '\n';
    }

    rc = PAPI_cleanup_eventset(event_set);
    if (rc != PAPI_OK)
        papi_fail(rc, "failed to cleanup event set");
    rc = PAPI_destroy_eventset(&event_set);
    if (rc != PAPI_OK)
        papi_fail(rc, "failed to destroy event set");
    return 0;
}

