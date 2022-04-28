#include "common.h"

/*
异常处理机制提供了处理错误的方式
*/

class Matter {};

class OverFlow: public Matter {
    public:
        operator std::string() {
            return "a";
        }
};

class UnderFlow: public Matter {};


int main(int argc, char const *argv[])
{
    /* code */
    try
    {
        throw  OverFlow();
    }
    catch(OverFlow o)
    {
        std::cout << std::string(o) << std::endl;
    }
    
    return 0;
}
