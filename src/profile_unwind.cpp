// Profile routine which unwinds the stack

void call_0()
{
    throw 1;
}

void call_99();

int main(int, char**)
{
    for (long i = 0; i < 100000; i++) {
        try {
            call_99();
        } catch (int) { }
    }
    return 0;
}

