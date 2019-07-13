/*
 * Created by 李晨曦 on 2019-07-14.
 */

#ifndef CPP_STRUCTURES_HPP
#define CPP_STRUCTURES_HPP

class A {
    int _data;
  public:
    explicit A(int data) : _data(data) {}
    
    auto data() -> int & {
        return _data;
    }
};

#endif //CPP_STRUCTURES_HPP
