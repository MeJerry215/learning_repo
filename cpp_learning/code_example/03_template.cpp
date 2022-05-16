#include "common.h"

template <typename T>
class String
{
public:
    // String() {
    // std::cout << "string construct" << std::endl;
    // }

    String();
};

template <typename T>
String<T>::String()
{
    std::cout << "string construct" << std::endl;
}

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
    return 0;
}
