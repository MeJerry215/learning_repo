#include "common.h"

int main(int argc, char const *argv[])
{
    /**
     * @brief 指针是指向某个数据类型的的地址
     * 
     */
    char c = 'a';
    char *p = &c;
    std::cout << *p << std::endl;
    char **ppc = &p;     // 指向字符指针的指针
    std::cout << **ppc << std::endl;
    int (*fp)(int, char const *[]) = main;
    printf("main func address %p\n", fp);
    int *ap[15];        // 15个数组的int指针
    int v1[] = {1, 2, 3, 5};
    debug_print_array(v1, SIZE(v1, int));
    float v2[8] = {1, 2, 3, 5};
    debug_print_array(v2, SIZE(v2, float));
    char* strs[] = {"hello", "world"};
    debug_print_array(strs, SIZE(strs, char *));
    int& uc = use_count();
    uc = 2;
    printf("use cont %d\n", use_count());
    return 0;
}
