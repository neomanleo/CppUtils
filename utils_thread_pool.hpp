#ifndef UTILS_THREAD_POOL_HPP
#define UTILS_THREAD_POOL_HPP

#include "utils_queue.hpp"
#include <condition_variable>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

class ThreadPool {
public:
  ThreadPool(size_t num_threads) {
    if ((num_threads == 0) || (num_threads > 4) ||
        (std::thread::hardware_concurrency() <= 4)) {
      std::cout << "Too many or too few threads requested.";
      exit(EXIT_FAILURE);
    }
    m_num_threads = num_threads;
  };

  ~ThreadPool() {
    if (!should_terminate) {
      stop();
    }
  };

  void queue_job(const std::function<void()> &job) {
    {
      std::unique_lock<std::mutex> mutex_lock(m_job_queue_mutex);
      m_job_queue.enqueue(job);
    }
    m_job_queue_mutex_condition.notify_one();
  };

  void start() {
    while (m_num_threads--) {
      m_threads.emplace_back(std::thread(&ThreadPool::thread_loop, this));
    }
  };

  void stop() {
    {
      std::unique_lock<std::mutex> mutex_lock(m_job_queue_mutex);
      should_terminate = true;
    }
    m_job_queue_mutex_condition.notify_all();
    for (std::thread &active_thread : m_threads) {
      active_thread.join();
    }
    m_threads.clear();
  };

  bool busy() {
    bool busy_status;
    {
      std::unique_lock<std::mutex> mutex_lock(m_job_queue_mutex);
      busy_status = !m_job_queue.empty();
    }
    return busy_status;
  };

private:
  bool should_terminate = false;

  UtilsQueue<std::function<void()>> m_job_queue;
  std::condition_variable m_job_queue_mutex_condition;
  std::mutex m_job_queue_mutex;

  std::vector<std::thread> m_threads;
  size_t m_num_threads;

  void thread_loop() {
    while (true) {
      std::function<void()> job;
      {
        std::unique_lock<std::mutex> mutex_lock(m_job_queue_mutex);
        m_job_queue_mutex_condition.wait(mutex_lock, [this] {
          return !m_job_queue.empty() || should_terminate;
        });
        if (should_terminate) {
          return;
        }
        job = m_job_queue.dequeue();
      }
      job();
    }
  }
};

#endif
