#pragma once 
#include <queue>
#include <future>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/ranges.h>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : m_numThreads(numThreads) {
        for (size_t i = 0; i < m_numThreads; ++i) {
            m_threads.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(m_mutex);
                        m_condition.wait(lock, [this]() { return!m_tasks.empty() || m_stop; });
                        if (m_stop && m_tasks.empty()) {
                            return;
                        }
                        task = std::move(m_tasks.front());
                        m_tasks.pop();  
                    }
                    task();
                }
            });
        }
    }

    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using ReturnType = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<ReturnType> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_stop) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            m_tasks.emplace([task]() { (*task)(); });
        }
        m_condition.notify_one();
        return res;
    }

    void stop() {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_stop = true;
        }
        m_condition.notify_all();
        for (auto& thread : m_threads) {
            thread.join();
        }
    }

private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    size_t m_numThreads;
    bool m_stop = false;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};