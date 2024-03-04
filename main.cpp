#include <iostream>
#include <thread>
#include "log.h"
#include "ThreadPool.h"

int main() {
    ThreadPool::ThreadPool pool(8, "test_pool", 1000);
    std::vector<std::function<void()>> func_list;
    for(int i = 0; i < 1000; ++i){
        func_list.emplace_back([] {
            std::cout<<"hello world"<<std::this_thread::get_id()<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }
    pool.concurrent_run<void>(func_list);
}