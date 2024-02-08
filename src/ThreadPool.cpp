#include "ThreadPool.hpp"

namespace particle_life {

ThreadPool::ThreadPool(uint32_t numberOfThreads)
    : m_numberOfThreads(numberOfThreads)
    , m_shutdownRequested(false)
{
    for(uint32_t i = 0; i < m_numberOfThreads; ++i)
        m_threads.emplace_back(std::thread(&ThreadPool::threadLoop, this));
}

ThreadPool::~ThreadPool()
{
    shutdown();
}

void ThreadPool::shutdown()
{
    {
        std::lock_guard lk(m_mutex);
        m_shutdownRequested = true;
    }

    m_conditionVariable.notify_all();

    for(auto& thread : m_threads)
        thread.join();
}

void ThreadPool::threadLoop()
{
    std::unique_lock lk(m_mutex);
    while(!m_shutdownRequested || !m_queue.empty())
    {
        m_conditionVariable.wait(lk, [this] {
            return this->m_shutdownRequested || !this->m_queue.empty();
        });

        if(!m_queue.empty())
        {
            auto func = m_queue.front();
            m_queue.pop();

            lk.unlock();
            func();
            lk.lock();
        }
    }
}

uint32_t ThreadPool::getNumberOfThreads() const
{
    return m_numberOfThreads;
}

}