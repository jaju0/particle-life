#ifndef PARTICLE_LIFE_THREAD_POOL_HPP_
#define PARTICLE_LIFE_THREAD_POOL_HPP_

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <thread>
#include <vector>
#include <queue>
#include <functional>

namespace particle_life {

class ThreadPool
{
public:
    ThreadPool(uint32_t numberOfThreads);
    ~ThreadPool();

    void shutdown();

    template<typename F, typename... Args>
    auto addTask(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto taskPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
        auto taskLambda = [taskPtr] { (*taskPtr)(); };

        {
            std::lock_guard lk(m_mutex);
            m_queue.push(taskLambda);
            m_conditionVariable.notify_one();
        }

        return taskPtr->get_future();
    }

    uint32_t getNumberOfThreads() const;

private:
    std::mutex m_mutex;
    std::condition_variable m_conditionVariable;

    std::vector<std::thread> m_threads;
    uint32_t m_numberOfThreads;
    bool m_shutdownRequested;

    std::queue<std::function<void()>> m_queue;

    void threadLoop();
};

}

#endif