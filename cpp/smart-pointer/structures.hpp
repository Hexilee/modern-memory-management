/*
 * Created by 李晨曦 on 2019-07-14.
 */

#ifndef CPP_STRUCTURES_HPP
#define CPP_STRUCTURES_HPP
#include <cstdint>
#include <iostream>

class A {
    int data;
  public:
    explicit A(int data) : data(data) {};
    
    auto say() {
        std::cout << "data=" << data << std::endl;
    }
    
    ~A() {
        std::cout << "A destruct, data=" << data << std::endl;
    }
};

template<typename T>
class unique_ptr {
    T *raw_ptr;
  public:
    explicit unique_ptr(T *raw_ptr) : raw_ptr(raw_ptr) {}
    
    unique_ptr(const unique_ptr &) = delete;
    
    unique_ptr(unique_ptr &&other) noexcept : raw_ptr(other.raw_ptr) {
        other.raw_ptr = nullptr;
    };
    
    T *operator->() {
        return raw_ptr;
    }
    
    ~unique_ptr() {
        delete raw_ptr;
    }
};

template<typename T>
class shared_ptr {
    T *raw_ptr;
    uint64_t *counter;
  public:
    explicit shared_ptr(T *raw_ptr) : raw_ptr(raw_ptr), counter(new uint64_t(1)) {}
    
    shared_ptr(const shared_ptr &other) : raw_ptr(other.raw_ptr), counter(other.counter) {
        ++(*counter);
    };
    
    shared_ptr(shared_ptr &&other) noexcept : raw_ptr(other.raw_ptr), counter(other.counter) {
        other.raw_ptr = nullptr;
        other.counter = nullptr;
    };
    
    T *operator->() {
        return raw_ptr;
    }
    
    ~shared_ptr() {
        std::cout << "counter=" << *counter - 1 << std::endl;
        if (--(*counter) == 0) {
            delete raw_ptr;
            delete counter;
        }
    }
};
#endif //CPP_STRUCTURES_HPP
