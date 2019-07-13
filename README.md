Rust 自诞生起就以它独特、现代化的内存管理机制闻名于世；而其指定的竞争对手 Cpp 从 C++11 以来在内存管理更现代化的道路上下了很大功夫。笔者平时写 Rust 比较多，最近在写 Cpp 便试图给脑中零散的概念做个总结，并使用 Rust 与其作对比，也算是一篇面向 Cpp 用户的 Rust 推销文章吧（本人 Cpp 不够熟练，若有什么地方讲的不对还请赐教）。

本文主要讨论四点内容，引用（reference）、拷贝（copy）、移动（move）和智能指针（smart pointer）。

## 引用

### Cpp

首先讲 Cpp 的引用，我们都知道 Cpp 里引用分为左值（lvalue）引用和右值（rvalue）引用，在这一部分我们主要讨论左值引用（右值引用应该放到后面的移动那一部分讲）。

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

Cpp 的左值引用又可分为可变引用（`T&`）和不可变引用（`const T&`）两种。不可变引用约束了引用的内部不可变：

```cpp
// [cpp] bazel run //reference:const_ref

class B {
    A _a;
  public:
    explicit B(int data) : _a(data) {}
    
    auto a() -> const A & {
        return _a;
    }
};

int main() {
    B b(0);
    auto & ref_a = b.a();
    ref_a = A(1);
    auto & ref_data = ref_a.data();
    return 0;
}
```

编译会得到两个错误：一个是  `no viable overloaded '='` ，因为 `ref_a` 的类型是 `const A&` 而 `A` 默认的移动赋值函数的 `this` 并没有标记为 `const`；第二个错误也是差不多的原因：`A` 的方法 `auto data() -> int &` 的 `this` 并没有标记为 `const`.

我们再给 `A` 加一个 `const_data 方法`:

```cpp
auto const_data() const -> const int & {
    return _data;
}
```

> 两个 `const` 分别标记 `this` 和返回引用的类型

我们现在可以也只能拿到 `_data` 的不可变引用

```cpp
// [cpp] bazel run //reference:const_ref_and_data

int main() {
    B b(0);
    auto & ref_data = b.a().const_data();
    ref_data = 1;
    return 0;
}
```

这段代码会炸在赋值的地方。

### Rust

#### Cpp 没解决的问题

Cpp 的引用固然在减少拷贝、控制可变性上做得很不错，但依旧存在两个明显的问题

- 引用可能比对象本身活得更长。
- 可能同时持有对同一对象的多个可变引用，不能静态检查到潜在的数据竞争。

第一个问题的例子：

```cpp
// [cpp] bazel run //reference:ref_dangling 
class C {
    int _id;
  public:
    explicit C(int id) : _id(id) {}
    
    ~C() {
        std::cout << "C(id=" << _id << ") destructed" << std::endl;
    }
    
    auto id() -> int & {
        return _id;
    }
};

decltype(auto) get_data() {
    return C(0).id();
}

int main() {
    decltype(auto) ref_data = get_data();
    std::cout << "id=" << ref_data << std::endl;
}
```

打印出来的结果

```bash
C(id=0) destructed
id=-1840470160
```

第二个问题的例子（略）

#### 引用的生命周期

#### 引用的可变性约束

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



