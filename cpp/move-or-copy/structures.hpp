/*
 * Created by 李晨曦 on 2019-07-14.
 */

#ifndef CPP_STRUCTURES_HPP
#define CPP_STRUCTURES_HPP
#include <iostream>

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
    
    auto first() -> const T & {
        return *header;
    }
};

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

#endif //CPP_STRUCTURES_HPP
