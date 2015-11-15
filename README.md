## Benchmarks
### Nested calls
Testcase:

```C++
static volatile int v;

__attribute__((noinline)) void call_0()
{
    throw 1;
}

__attribute__((noinline)) void call_1()
{
    v = 0;
    test_0();
    v = 1;      // Prevent tail call optimization
}

// ...

__attribute__((noinline)) void call_9()
{
    v = 8;
    test_8();
    v = 9;
}

void test_call_9()
{
    try {
        test_8();
    } catch(int) { }
}
```

Measure number of cycles required to unwind the stack, depending on the number of frames being unwound.

### Classes

Testcase:

```C++
static volatile int v;

// Define N empty classes
namespace {
class class_0 { };
// ...
class class_9 { };
} // anon. namespace

// Define N functions like this:
void test_throw_0()
{
    try {
        throw class_0();
    } catch(class_0&) {
        v = 0;
    } catch(class_1&) {
// ...
    } catch(class_9&) {
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
namespace {
class class_0 { };
class class_1 : public class_0 { };
// ...
class class_9 : public class_8 { };
} // anon. namespace

// Define N functions like this:
void test_throw_0()
{
    try {
        throw class_0();
    } catch(class_9&) {
        v = 9;
    } catch(class_8&) {
// ...
    } catch(class_0&) {
        v = 0;
    }
}
```

Measure time required to handle exception depending on total number of classes and
class of the object being thrown.

## References
1. Technical Report on C++ Performance [TR18015](http://www.open-std.org/jtc1/sc22/wg21/docs/TR18015.pdf)
2. [Itanium C++ ABI: Exception Handling](https://mentorembedded.github.io/cxx-abi/abi-eh.html)
3. [System V Application Binary Interface. AMD64 Architecture Processor Supplement](http://www.x86-64.org/documentation/abi.pdf),
[v. 0.99.7](http://www.x86-64.org/documentation/abi-0.99.7.pdf)
4. [DWARF Debugging Information Format. Version 4](http://dwarfstd.org/doc/DWARF4.pdf)
5. Ian Lance Tayolor's blog posts
 - [.eh_frame](http://www.airs.com/blog/archives/460)
 - [.eh_frame_hdr](http://www.airs.com/blog/archives/462)
 - [.gcc_except_table](http://www.airs.com/blog/archives/464)
6. [Exception Handling in LLVM](http://llvm.org/docs/ExceptionHandling.html)
