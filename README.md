## Benchmarks
### Nested calls
Testcase:

```C++
static volatile int v;

__attribute__((noinline)) void test_0()
{
    throw 1;
}

__attribute__((noinline)) void test_1()
{
    v = 0;
    test_0();
    v = 1;      // Prevent tail call optimization
}

// ...

__attribute__((noinline)) void test_9()
{
    v = 8;
    test_8();
    v = 9;
}

void test()
{
    try {
        test_9();
    } catch(int) { }
}
```

Measure number of cycles required to unwind the stack, depending on the number of frames being unwound.

### Classes

Testcase:

```C++
static volatile int v;

// Define N empty classes
class class_0 { };
// ...
class class_9 { };

// Define N functions like this:
void test_throw_0()
{
    try {
        throw class_0();
    } catch(class_0 &) {
        v = 0;
    } catch(class_1 &) {
// ...
    } catch(class_9 &) {
        v = 9;
    }
}
```

Measure time required to handle exception depending on total number of classes and
class of the object being thrown.

### Class hierarchy

Testcase:

```C++
static volatile int v;

// Define hierarchy of N classes
class class_0 { };
class class_1 : public class_0 { };
// ...
class class_9 : public class_8 { };

// Define N functions like this:
void test_throw_0()
{
    try {
        throw class_0();
    } catch(class_9 &) {
        v = 9;
    } catch(class_8 &) {
// ...
    } catch(class_0 &) {
        v = 0;
    }
}
```

Measure time required to handle exception depending on total number of classes and
class of the object being thrown.

## References
1. Technical Report on C++ Performance [TR18015](http://www.open-std.org/jtc1/sc22/wg21/docs/TR18015.pdf)

