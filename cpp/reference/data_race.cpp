/*
 * Created by 李晨曦 on 2019-07-18.
 */

#include <thread>
#include <iostream>
#include <random>
#include <chrono>

int main() {
    auto str = "hello, world";
    std::uniform_int_distribution<> dist{10, 100};
    auto handler = std::thread([&str, &dist]() {
        std::mt19937_64 eng{std::random_device{}()};
        std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
        str = nullptr;
    });
    std::mt19937_64 eng{std::random_device{}()};
    std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
    std::cout << str << std::endl;
    handler.join();
    return 0;
}