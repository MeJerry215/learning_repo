#include <iostream>
#include <stdio.h>
#include <vector>

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