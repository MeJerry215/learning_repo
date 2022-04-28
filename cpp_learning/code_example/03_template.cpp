#include "common.h"

template<typename T>
class String {
public:
    // String() {
        // std::cout << "string construct" << std::endl;
    // }

    String();
};


template<typename T> String<T>::String() {
    std::cout << "string construct" << std::endl;
}


int main(int argc, char const *argv[])
{
    String<int> s;
    return 0;
}
