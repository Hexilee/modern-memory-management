/*
 * Created by 李晨曦 on 2019-07-18.
 */

#include <iostream>

class Boy;

class Girl;

class Boy {
    std::shared_ptr<Girl> girl_friend;
  public:
    explicit Boy() : girl_friend(nullptr) {};
    auto set(std::shared_ptr<Girl> &girl) {
        girl_friend = girl;
    };
    ~Boy() {
        std::cout << "boy destruct" << std::endl;
    }
};

class Girl {
    std::shared_ptr<Boy> boy_friend;
  public:
    explicit Girl() : boy_friend(nullptr) {};
    
    auto set(std::shared_ptr<Boy> &boy) {
        boy_friend = boy;
    };
    
    ~Girl() {
        std::cout << "girl destruct" << std::endl;
    }
};


int main() {
    auto boy = std::make_shared<Boy>();
    auto girl = std::make_shared<Girl>();
    boy->set(girl);
    girl->set(boy);
    return 0;
}
