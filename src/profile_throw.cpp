// Profile routine which throws an exception

int main(int, char**)
{
    for (long i = 0; i < 10000000; i++) {
        try {
            throw 1;
        } catch (int) { }
    }
    return 0;
}

