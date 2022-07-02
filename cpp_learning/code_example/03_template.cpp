#include "common.h"

template <typename T>
class String
{
public:
    // String() {
    // std::cout << "string construct" << std::endl;
    // }

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

=======
template <typename T>
String<T>::String()
{
    std::cout << "string construct" << std::endl;
}
>>>>>>> 22ddc20962f516e5109fe898b8781828eb4fb50b

template <typename T>
class Stack
{
private:
    std::vector<T> elems;

public:
    void push(T const &);
    void pop();
    T top();
    bool empty() const
    {
        return elems.empty();
    }
};

template <typename T>
void Stack<T>::push(T const &elem)
{
    elems.push_back(elem);
}

template <typename T>
T Stack<T>::top()
{
    if (elems.empty())
    {
        throw std::out_of_range("Stack<>::pop: empty stack");
    }
    return elems.back();
}

template <typename T>
void Stack<T>::pop()
{
    if (elems.empty())
    {
        throw std::out_of_range("Stack<>::pop: empty stack");
    }
    elems.pop_back();
}

int main(int argc, char const *argv[])
{
<<<<<<< HEAD
    String<int> s1;
    Buffer<float, 2> b;
    String<float> s2;
=======
    String<int> s;
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    std::cout << stack.top() << std::endl;
    stack.pop();
    stack.pop();
    std::cout << stack.top() << std::endl;
    stack.push(5);
    std::cout << stack.top() << std::endl;
>>>>>>> 22ddc20962f516e5109fe898b8781828eb4fb50b
    return 0;
}
