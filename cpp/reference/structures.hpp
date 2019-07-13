/*
 * Created by 李晨曦 on 2019-07-14.
 */

#ifndef CPP_STRUCTURES_HPP
#define CPP_STRUCTURES_HPP
#include <iostream>

class A {
    int _data;
  public:
    explicit A(int data) : _data(data) {}
    
    auto data() -> int & {
        return _data;
    }
    
    auto const_data() const -> const int & {
        return _data;
    }
};

class B {
    A _a;
  public:
    explicit B(int data) : _a(data) {}
    
    auto a() -> const A & {
        return _a;
    }
};

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

#endif //CPP_STRUCTURES_HPP
