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

#### Cpp 引用没解决的问题

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

#### 引用的生命期

下面是 Rust 引用的用例

```rust
// [rust] cargo run --example mutable_ref 

struct A {
    _data: i32,
}

impl A {
    fn new(data: i32) -> Self {
        A { _data: data }
    }

    fn data(&self) -> &i32{
        &self._data
    }

    fn mut_data(&mut self) -> &mut i32{
        &mut self._data
    }
}

fn main() {
    let mut a = A::new(0);
    let data = a.mut_data();
    *data = 1;
    println!("a._data={}", data);
}
```

> Rust 中对象和引用默认都是不可变的，需要用 `mut` 限定词来使其可变，这与 Cpp 刚好相反。

Rust 的引用同样也分可变引用和不可变引用，Cpp 中对可变引用的约束规则 Rust 也全部涵盖了；并且我们可以注意到，不同于 Cpp 中作为变量别名的引用，Rust 中的引用更像是指针，很多场景下都需要显式地取引用（`&`）和解引用（`*`）。

> Rust 中不能直接拿到裸指针。

看完这个例子后我们继续刚才话题， 第一个问题，Rust 的引用可能比对象本身活得更长吗？答案是不能（在不使用 Unsafe Rust 的情况下）。只要程序过了编译，Rust 能永远保证引用有效。

比如这个程序：

```rust
// [rust] cargo run --example ref_dangling

fn get_data() -> &i32 {
    let a = A::new(0);
    a.data()
}

fn main() {
    println!("a._data={}", get_data());
}
```

编译失败 `missing lifetime specifier`，这是因为 Rust 的引用都有自己的生命期（lifetime）。在一般情况下，Rust 编译器能自己推导出生命期，比如当参数和返回值各只有一个生命期时，编译器会认为返回值的生命期与参数一致（返回引用依赖参数引用）：

```rust
// [rust] cargo run --example lifetime_infer

fn get_data(a: &A) -> &i32 {
    a.data()
}

fn main() {
    println!("a._data={}", get_data(&A::new(0)));
}
```

编译器会认为 `get_data` 返回的 `&i32` 依赖参数 `a`；同时对于 `data` 方法

```rust
struct A {
    _data: i32,
}

impl A {
    fn data(&self) -> &i32{
        &self._data
    }
}
```

返回的 `&i32` 依赖参数 `self`，所以编译器认为这个依赖链没问题，编译通过。

但如果依赖链有问题的话，比如

```rust
// [rust] cargo run --example failed_infer

fn get_data(a: &A) -> &i32 {
    let data = 1;
    &data
}

fn main() {
    println!("a._data={}", get_data(&A::new(0)));
}
```

`get_data` 返回的 `&i32` 和参数 `a` 并没有关系，引用栈变量，拒绝编译。

当然对于更复杂的情况有其它的推导规则，有时也需要手动标记，有兴趣的读者可参考 Rust 的官方文档。

#### 对可变引用的约束

Rust 的引用声明周期可以解决之前提到的第一个问题，那第二个问题呢？

我们前文说过 “Cpp 中对可变引用的约束规则 Rust 也全部涵盖了”，言外之意就是 Rust 对可变引用还有更多的约束：**一个可变引用不能与其他引用同时存在**。

不能同时存在是什么意思呢？就是生命期不能重叠，比如

```rust
// [rust] cargo run --example mut_ref_conflict 

fn main() {
    let mut data = 0;
    let ref_data = &data;
    let mut mut_ref = &mut data;
    println!("data={}", ref_data);
}
```

编译失败。

由于 [NLL](https://github.com/rust-lang/rfcs/blob/master/text/2094-nll.md) 的存在，Rust 引用的生命期从它定义的地方持续到它最后一次被使用的地方而非作用域结尾（注意主语是生命期，存在依赖关系的引用拥有同一个生命期）。

所以这样是 OK 的：

```rust
// [rust] cargo run --example mut_ref

fn main() {
    let mut data = 0;
    let ref_data = &data;
    let mut mut_ref = &mut data;
}
```

这个编译不过：

```rust
// [rust] cargo run --example dep_ref_conflict

fn main() {
    let mut a = A::new(0);
    let data = a.data();
    let mut mut_a = &mut a;
    println!("data={}", data);
}
```

那么，这个约束对线程安全有什么帮助呢？**一个可变引用不能与其他引用同时存在**，再加上后面会提到的**对象在被引用时不能移动**，这就意味着在理想情况下是绝对不会出现数据竞争的。

当然这只是在理想情况下，事实上，由于这个约束过强，很多时候必须使用一些基于 Unsafe Rust 的组件（在更强的约束上开洞而非在更弱的约束上不断修修补补也算 Rust 的设计哲学吧，首要考虑安全性）。

对 Rust 线程安全有兴趣的读者可以自行参阅官方文档，本文也无法讨论太多了；有一定 Rust 基础的读者还可以看看这篇文章作为拓展阅读[《如何理解 rust 中的 Sync、Send？》](https://zhuanlan.zhihu.com/p/64699643)。


## 拷贝和移动

cpp 的拷贝和移动，本质是调用了拷贝构造函数（接收左值引用）和移动构造函数（接收右值引用）

> 赋值的时候则调用拷贝赋值函数（接收左值引用）和移动赋值函数（接收右值引用）

cpp 左值引用本质上是变量别名（alias），即与同一对象绑定的多个变量；而右值引用则表示某对象没有与任何变量绑定，故可能在两种情况下出现：

 - 直接取自右值
 - 使用 `std::move` 从左值取出右值引用

这里重点说一下第二种情况

`std::move` 仅仅是语义上的 move，用于从左值取出右值引用，表示该对象与原来的所有左值引用解除绑定，move 过后原来所有的左值引用全部失效，不允许再被使用。

而你一旦使用任何变量接收右值引用，这个变量就变成了左值引用，因为**右值引用表示该对象没有与任何变量绑定**。

```cpp

```

正是因为左值引用和右值引用有着这样的约定，所以移动构造函数和移动赋值函数可以放心使用右值引用（无变量绑定，移动不影响其它变量），而拷贝构造函数和拷贝赋值函数只需选择不可变左值引用（绑定了其他变量，不可随便改动对象）。



