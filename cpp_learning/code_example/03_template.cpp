#include "common.h"

template<typename T>
class String {
public:
    String() {
        std::cout << "string construct" << std::endl;
    }

};

template<>
class String<float> {
public:
    String() {
        std::cout << "float construct " << std::endl;
    }
};

template<typename T, int size>
class Buffer {
    T v[size];
    const static int sz = size;
public:
    Buffer() {
        std::cout << "size " << sz << std::endl;
    }
};

/*
specialization
*/



int main(int argc, char const *argv[])
{
    String<int> s1;
    Buffer<float, 2> b;
    String<float> s2;
    return 0;
}
