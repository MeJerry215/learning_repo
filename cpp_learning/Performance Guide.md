# Performance Guide

## Arithmetic Operators

#### cases type conversion:
1. assign/initialize a value of arithmetic type to another arithmetic type
2. combine mixed types
3. convert values when pass argments to functions.

intgral promotions: bool, char, unsigned char, signed char, short -> int


## polymorphism

### Overloading
multi-function proto with differenent args type ans num,  and same function name.



## template

- instantiation(implicit instantiation/explicit instantiation)
- specialization
- partial specialization

specialization overrides the regular template, non-templete overrides both.

```cpp
template void Swap(int, int);          // explicit instantiation

template <> void Swap(int &, int &);   // explicit specialization
```