Rust 自诞生起就以它独特、现代化的内存管理机制闻名于世；而其指定的竞争对手 Cpp 从 C++11 以来在内存管理现代化的道路上下了很大功夫。笔者平时写 Rust 比较多，最近在写 Cpp 便试图给脑中零散的概念做个总结，并使用 Rust 与其作对比，也算是一篇面向 Cpp 用户的 Rust 推销文章吧。

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


#### 没解决的问题

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

### Rust

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

### Cpp

#### 拷贝

Cpp 的拷贝本质上是调用了拷贝构造函数`T(const T& other)`（构造时）或拷贝赋值运算符`T& operator=(const T& other)`（赋值时）。

> `other` 也可以是 `T&`。

一般来说开发者无需自己定义拷贝构造函数或者拷贝赋值运算符，编译器会隐式实现（默认实现是调用所有成员的拷贝构造函数或拷贝赋值运算符），但在一些特殊情况下（比如存在没有拷贝构造函数或拷贝赋值运算符的成员）必须自己实现。具体的隐式实现条件请参考 cppreference，本文不作赘述。

拷贝场景：

```cpp
// [cpp] bazel run //move-or-copy:copy  

class A {
  public:
    A() = default;
    
    A(const A &) {
        std::cout << "call copy constructor" << std::endl;
    }
        
    auto operator=(const A &) -> A & {
        std::cout << "call copy operator=" << std::endl;
        return *this;
    }
    
};

auto set_a(A a) {}

auto get_a() {
    A a;
    return static_cast<A&>(a); // 防止返回值优化
}

int main() {
    A a;
    set_a(a);
    get_a();
    auto copy_a = a;
    a = copy_a;
    return 0;
}
```

打印出

```bash
call copy constructor
call copy constructor
call copy constructor
call copy operator=
```
#### 移动

Cpp 的移动本质上是调用了移动构造函数`T(T&& other)`（构造时）或移动赋值运算符`T& operator=(T&& other)`（赋值时）。

其中 `T&&` 就是 `T` 类型的右值引用。

Cpp 左值引用本质上是变量别名（alias），即与同一对象绑定的多个变量；而右值引用（语义上）则表示某对象没有与任何变量绑定，故可能在两种情况下出现：

 - 直接取自右值
 - 取自左值（使用 `std::move`、`std::forward` 等）

这里重点说一下第二种情况，

`std::move` 仅仅是语义上的 move，用于从左值取出右值引用，表示该对象与原来的所有的左值引用解除绑定，move 过后原来所有的左值引用全部失效，不允许再被使用。

正是因为右值引用有着这样的语义，所以移动构造函数和移动赋值运算符可以放心使用右值引用（无变量绑定，移动不影响其它变量）。

而你一旦使用任何变量接收右值引用，这个变量就变成了左值，因为**右值引用不与任何变量绑定**。如果要保证引用在函数之间传递时能“完美转发”（右值引用不会转成左值），可使用 `std::forward`。

```cpp
// [cpp] bazel run //move-or-copy:move 

class A {
  public:
    A() = default;
    
    A(const A &) {
        std::cout << "call copy constructor" << std::endl;
    }
    
    A(A &&) noexcept {
        std::cout << "call move constructor" << std::endl;
    }
    
    auto operator=(const A &) -> A & {
        std::cout << "call copy operator=" << std::endl;
        return *this;
    }
    
    auto operator=(A &&) noexcept -> A & {
        std::cout << "call move operator=" << std::endl;
        return *this;
    }
};

auto set_a(A a) {}
auto copy_ref(A&& a) {
    A _copy(a); // a 转成了左值
}
auto move_ref(A&& a) {
    A _move(std::forward<A>(a)); // 转发
}
int main() {
    A a;
    set_a(std::move(a));
    set_a(static_cast<A&&>(A())); // 防止构造优化
    copy_ref(A());
    move_ref(A());
    return 0;
}
```

打印出

```bash
call move constructor
call move constructor
call copy constructor
call move constructor
```

#### 为什么需要拷贝和移动

拷贝和移动本质上都是为了保证变量与其绑定的对象生命期一致，这是它们与引用本质上的目的区别，用额外的内存开销换取内存安全。

有时这点额外内存开销是可以忽略不计的，但不是所有时候都这样。减少内存开销的常见做法是堆分配，但堆分配带来的新问题是可能会内存泄漏。

如果使用堆内存分配和拷贝，就需要想一套方案来决定什么时候回收内存。常见的思路是引用计数或者 [GC](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science))。

但我们可以发现，移动是天然符合 [RAII](https://zh.wikipedia.org/wiki/RAII) 的：堆内存分配，堆内存生命期与栈对象一致（在栈对象析构函数中回收内存）。

比如我们来造一个 `Vector` 

```cpp
// [cpp] bazel run //move-or-copy:vector 

#include <cstdlib>
#include <iostream>

template<typename T>
class Vector {
    T *header;
    size_t capacity;
    size_t length;
  public:
    explicit Vector(size_t cap = 0) : header(new T[cap]), capacity(cap), length(0) {};
    Vector(const Vector &) = delete;
    
    Vector(Vector &&other) noexcept : header(other.header), capacity(other.capacity), length(other.length) {
        other.header = nullptr;
        other.capacity = 0;
        other.length = 0;
    }
    
    ~Vector() {
        std::cout << "destruct Vector(header=" << header << ")" << std::endl;
        delete[] header;
    }
    
    auto first() -> const T& {
        return *header;
    }
};


auto product_vector() {
    auto v = Vector<int>(10);
    return std::move(v);
}

auto consume_vector(Vector<int> _v) {

}

int main() {
    auto v = product_vector();
    consume_vector(std::move(v));
    return 0;
}
```

> 我们在这里显式删除了拷贝构造函数，导致它只能移动而无法拷贝，你也可以实现它。

打印出

```cpp
destruct Vector(header=0x0)
destruct Vector(header=0x7fd361c02a10)
destruct Vector(header=0x0)
```

从析构顺序来看只有 `consume_vector` 中的 `v` delete 了真正的 `header`，`product_vector` 和 `main` 中的 `v` 再析构之前被 move 从而被“掏空”，`header` 变成了 `nullptr`。

#### 不足之处

虽然 Cpp 的拷贝和移动机制已经很完善了，但依然存在一些缺陷，最主要的问题就是语义上的 move 并没有静态分析的支持。

- 虽然 move 了，但后面可能还会不小心用到。当然这种情况现代编辑器和编译器一般都会给个 warning。
- 虽然 move 了，但之前的引用还在被使用，这种情况编辑器和编译器很难发觉。

```cpp
// [cpp] bazel run //move-or-copy:object_moved

int main() {
    auto v = Vector<int>(10);
    auto& ref_v = v;
    std::cout << ref_v.first() << std::endl;
    auto moved = std::move(v);
    std::cout << ref_v.first() << std::endl;
}
```

没有任何 warning，运行时 segmentation fault。

### Rust

#### 移动

与 Cpp 不同的是，Rust 所有类型默认都是移动的，除非它实现了 `trait Copy`，所以我们先来讲移动。

Rust 无法像 Cpp 那样自定义移动操作，移动事实上只是一次 memory copy。但是，变量的所有权（`ownership`）已经移交出去了，你永远不能再使用这个变量，除非你再给它所有权。

> 移交所有权就像 Cpp 里的取右值引用

```rust
// [rust] cargo run --example ownership_moved 

fn main() {
    let a = String::from("hello, world");
    println!("{}", &a);
    let b = a;
    println!("{}", &a);
}
```

编译失败：`borrow of moved value: 'a'`。

如果它正在被引用，就不能被移动

```rust
// [rust] cargo run --example ownership_borrowed 
fn main() {
    let a = String::from("hello, world");
    let ref_a = &a;
    println!("{}", ref_a);
    let b = a;
    println!("{}", ref_a);
}
```

编译失败：` cannot move out of 'a' because it is borrowed`。

所以 Rust 的移动跟 Cpp 的移动在语义上是完全一致的，但 Rust 的静态分析可以保证：

- 不能对已移交所有权的变量取引用。
- 在其任意引用的生命期内变量不能移交所有权。

说到这，还剩下的一个问题就是，Rust 怎样在不自定义移动操作的情况下控制资源的回收（堆内存的释放）呢？

比如，如果 Cpp 的移动也只能是 memory copy，那么：

```cpp
// [cpp] bazel run //move-or-copy:move_by_memcopy 

class B {
    int *_data;
  public:
    explicit B(int data) : _data(new int(data)) {}
    
    B(const B &) = delete;
    
    B(B &&other) noexcept : _data(other._data) {}
    
    ~B() {
        std::cout << "delete " << _data << std::endl;
        delete _data;
    }
};

auto get_b() {
    auto b = B(1);
    return std::move(b); // 防止返回值优化，强制移动
}

int main() {
    auto b = get_b();
    return 0;
}
```

运行

```bash
delete 0x7ffa04402a10
delete 0x7ffa04402a10
move_by_memcopy(58846,0x10af085c0) malloc: *** error for object 0x7ffa04402a10: pointer being freed was not allocated
move_by_memcopy(58846,0x10af085c0) malloc: *** set a breakpoint in malloc_error_break to debug
[1]    58846 abort      bazel run //move-or-copy:move_by_memcopy
```

`get_b` 和 `main` 两个函数执行完后 `delete` 了同一个指针。

但在 Rust 中

```rust
// [rust] cargo run --example drop

#![feature(ptr_internals)]

use core::ptr::{self, Unique};
use core::mem;
use std::alloc::{dealloc, Layout, alloc};

struct B {
    data: Unique<i32>
}

static I32_LAYOUT: Layout = Layout::from_size_align_unchecked(mem::size_of::<i32>(), mem::align_of::<i32>());

impl B {
    fn new(value: i32) -> Self {
        unsafe {
            let raw_ptr = alloc(I32_LAYOUT) as *mut i32;
            ptr::write(raw_ptr, value);
            Self { data: Unique::new_unchecked(raw_ptr) }
        }
    }

    fn as_ref(&self) -> &i32 {
        unsafe {
            self.data.as_ref()
        }
    }
}

impl Drop for B {
    fn drop(&mut self) {
        println!("drop raw ptr: {:?}", self.data);
        unsafe {
            dealloc(self.data.as_ptr() as *mut u8, I32_LAYOUT);
        }
    }
}

fn boxed(value: i32) -> B {
    let b = B::new(value);
    return b;
}

fn main() {
    let b = boxed(1);
    println!("b = {}", b.as_ref());
}
```

> 手动 alloc、dealloc 和操作裸指针是不安全的行为，大多数场景下应使用已封装好的组件（如 `Box`）来替换裸指针，以上代码仅供参考。

运行，打印结果是

```bash
b = 1
drop raw ptr: 0x7ff42ec02c40
```

你会发现这个叫作 `Drop`，看起来像是析构函数的 `trait` 跟 Cpp 析构函数还是有明显差别的：`Drop::drop` 只会在所有权被 drop 的时候被调用。

函数 `boxed` 中的 `b` 返回之后已经把所有权移交给了 `main` 里的 `b`，故 `boxed` 里 `b` 在函数调用结束被回收时仅仅回收了它在栈上占用的内存，而不会调用 `Drop`。

所以，一个变量交出了所有权之后就没有任何被访问的可能性（甚至没有析构函数之类的东西可调用）。Rust 就是以如此简洁的方式完美地实现了移动！


