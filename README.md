Rust 自诞生起就以它独特、现代化的内存管理机制闻名于世；而其指定的竞争对手 Cpp 从 C++11 以来在内存管理更现代化的道路上下了很大功夫。笔者平时写 Rust 比较多，最近在写 Cpp 便试图给脑中零散的概念做个总结，并使用 Rust 与其作对比，也算是一篇面向 Cpp 用户的 Rust 推销文章吧（本人 Cpp 不够熟练，若有什么地方讲的不对还请赐教）。

本文主要讨论四点内容，引用（reference）、拷贝（copy）、移动（move）和智能指针（smart pointer）。

## 引用

### Cpp

首先讲 Cpp 的引用，我们都知道 Cpp 分为左值（lvalue）引用和右值（rvalue）引用，这一部分我们主要讨论左值引用（右值引用应该放到后面的移动那一部分讲）。

#### 左值引用

Cpp 左值引用的本质是变量别名（alias），即一个已经存在变量的别名（引用同一对象的多个变量），所以可以看到这样的操作：

```cpp
// [cpp] bazel run //reference:lvalue_ref

#include <iostream>

class A {
    int _data;
  public:
    explicit A(int data) : _data(data) {}
    
    auto data() -> int & {
        return _data;
    }
};


int main() {
    A a(0);
    a.data() = 1; // equal to `a._data = 1;`
    std::cout << "a._data = " << a.data() << std::endl;
    return 0;
}
```

或者用一个变量接收这个引用

```cpp
// [cpp] bazel run //reference:lvalue_ref_bind

int main() {
    A a(0);
    auto &data_ref = a.data();
    data_ref = 1;
    std::cout << "a._data = " << a.data() << std::endl;
    return 0;
}
```

使用 C++14 的 `decltype(auto)` 写法

```cpp
// [cpp] bazel run //reference:lvalue_ref_bind_cpp14

int main() {
    A a(0);
    decltype(auto) data_ref = a.data();
    data_ref = 1;
    std::cout << "a._data = " << a.data() << std::endl;
    return 0;
}
```

他们的打印结果都毫无疑问是

```bash
a._data = 1
```

#### 不可变引用

Cpp 的左值引用又可分为可变引用（`T&`）和不可变引用（`const T&`）两种。不可变引用同时约束了引用的绑定不可变和内部不可变：


# 引用、拷贝、移动和智能指针

cpp 的值传递，本质是调用了拷贝构造函数（接收不可变左值引用时）或移动构造函数（接收右值引用时）

> 赋值的时候则调用拷贝赋值函数（接收不可变左值引用时）或移动赋值函数（接收右值引用时）

cpp 左值引用本质上是变量别名（alias），即与同一对象绑定的多个变量；而右值引用则表示某对象没有与任何变量绑定，故可能在两种情况下出现：

 - 直接取自右值
 - 使用 `std::move` 从左值取出右值引用

这里重点说一下第二种情况

`std::move` 仅仅是语义上的 move，用于从左值取出右值引用，表示该对象与原来的所有左值引用解除绑定，move 过后原来所有的左值引用全部失效，不允许再被使用。

而你一旦使用任何变量接收右值引用，这个变量就变成了左值引用，因为**右值引用表示该对象没有与任何变量绑定**。

```cpp

```

正是因为左值引用和右值引用有着这样的约定，所以移动构造函数和移动赋值函数可以放心使用右值引用（无变量绑定，移动不影响其它变量），而拷贝构造函数和拷贝赋值函数只需选择不可变左值引用（绑定了其他变量，不可随便改动对象）。



