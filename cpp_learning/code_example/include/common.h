#include <iostream>
#include <stdio.h>
#include <vector>   // 数组
#include <list>     // 双向链表
#include <deque>    // 双端队列
#include <queue>    // 队列
#include <stack>    // 堆栈
#include <map>      // 关联数组
#include <set>      // 集合
#include <utility>  // 运算符和对偶
#include <functional>   // 函数对象
#include <memory>   // 容器用的分配器
#include <ctime>    // c风格时间和日期
#include <iterator> // 迭代器
#include <algorithm>    //  通用算法
#include <exception>    // 异常
#include <string>   // 字符串
#include <cassert>  // assert
#include <limits>   // 数值范围

#include <cstdlib>


#define SIZE(arr, T) (sizeof(arr)/sizeof(T))

template<typename T>
void debug_print_array(T *arr, size_t size){
    auto& tid = typeid(T);
    for(size_t i = 0 ; i < size; i++) {
        if (tid == typeid(int)) {
            printf("%d ", arr[i]);
        } else if (tid == typeid(char)) {
            printf("%c ", arr[i]);
        } else if (tid == typeid(float) || tid == typeid(double)) {
            printf("%f ", arr[i]);
        } else if (tid == typeid(char *)) {
            printf("%s ", arr[i]);
        }

    }
    printf("\n");
}


int& use_count() {
    static int uc = 0;
    return uc;
}
