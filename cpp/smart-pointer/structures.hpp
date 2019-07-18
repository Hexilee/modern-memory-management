/*
 * Created by 李晨曦 on 2019-07-14.
 */

#ifndef CPP_STRUCTURES_HPP
#define CPP_STRUCTURES_HPP

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

#endif //CPP_STRUCTURES_HPP
