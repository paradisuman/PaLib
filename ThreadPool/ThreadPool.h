#pragma once

#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <pthread.h>
#include <unistd.h>
#include "../Logger/log.h"


namespace ThreadPool {
    class ThreadPool{
    public:
        ThreadPool(size_t, const std::string name = "thread_pool", const int max_queue_size = 1000);
        template<class F, class ... Args>
        auto enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
        template<class Return>
        void concurrent_run(std::vector<std::function<Return()>> &func_list);

        ~ThreadPool();

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        
        int work_num_;
        int queue_size_ = 0;
        int max_queue_size;
        int free_worker_num_ = 0;
    };

    inline ThreadPool::ThreadPool(size_t thread_num, const std::string name, const int max_queue_size) : work_num_(thread_num), max_queue_size(max_queue_size) { 
        Logger::Logger::GetInstance().Log("", "thread pool is start");
        for(size_t i = 0; i < thread_num; ++i){
            char buffer[16];
            sprintf(buffer, "%s_%d", name.c_str(), i);
            std::string thread_name = buffer;
            workers.emplace_back([this, thread_name] {
		// MacOS
    		#if defined(__APPLE__) || defined(__MACH__)
    		pthread_setname_np(thread_name.c_str());
    		// Linux
    		#elif defined(__linux__) || defined(linux) || defined(__linux)
    		pthread_setname_np(pthread_self(), thread_name.c_str());
		#endif
                while(true){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        free_worker_num_++;
                        this->condition.wait(lock, [this] {
                            return !this->tasks.empty();
                        });
                        free_worker_num_--;
                        if(this->tasks.empty()){
                            Logger::Logger::GetInstance().Log("", "thread pool is shutting down");
                            return;
                        }
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                        queue_size_--;
                    }
                    task();
                    Logger::Logger::GetInstance().Log("", "donw work");
                }
            });
        }
    }
    

    template <class F, class ... Args> 
    auto ThreadPool::enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if(queue_size_ >= max_queue_size){
                throw std::runtime_error("Thread pool is full");
            }
            queue_size_++;
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    template <class Return>
    void ThreadPool::concurrent_run(std::vector<std::function<Return()>> &func_list){
        std::vector<std::future<Return>> futures;
        for(auto &func : func_list){
            futures.emplace_back(enqueue(func));
        }
        for(auto &future : futures){
            future.get();
        }    
    }

    ThreadPool::~ThreadPool(){
        {
            // std::unique_lock<std::mutex> lock(queue_mutex);
        }
        condition.notify_all();
        // 等待所有线程结束
        for (std::thread &worker : workers) worker.join();
    }
}

