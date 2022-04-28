#include "common.h"

class Table {
public:
    Table() {
        std::cout << "table init" << std::endl;
    }
};

// class X {
// public:
//     static Table memtbl;
// };

class Zlib {
    static void initialize() {
        initialized = true;
    }
public:
    static bool initialized;

    void f() {
        if (!initialized) {
            initialize();
        }
    }
};

bool Zlib::initialized = false;

struct X {
    int z;

    X() {
        z = 2;
    }
};

class Ptr {

public:
    X *x;
    Ptr(): x(new X) {}

    X* operator ->() {
        return x;
    }
};

int main(int argc, char const *argv[])
{
    // std::vector<Table>* tables = new std::vector<Table>(10);
    // Table& t = X::memtbl;
    Ptr p;
    p->z;
    printf("val %d %d\n", p->z, p.operator->()->z);
    Zlib z;
    z.f();
    return 0;
}
