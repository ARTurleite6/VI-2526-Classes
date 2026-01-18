#pragma once

namespace VI {

class ThreadPool final {
public:
  explicit ThreadPool(size_t num_threads = std::thread::hardware_concurrency());
  ~ThreadPool();

  template <class F, class... Args>
  std::future<typename std::invoke_result_t<F, Args...>>
  Enqueue(F &&f, Args &&...args) {
    using ReturnType = typename std::invoke_result_t<F, Args...>;

    auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<ReturnType> res = task->get_future();
    {
      std::unique_lock<std::mutex> lock{m_QueueMutex};
      assert(!m_Stop && "Enqueue called on stopped ThreadPool");
      m_Tasks.emplace([task]() { (*task)(); });
    }
    m_Condition.notify_one();
    return res;
  }

private:
  std::vector<std::thread> m_Workers;
  std::queue<std::function<void()>> m_Tasks;
  std::mutex m_QueueMutex;
  std::condition_variable m_Condition;
  bool m_Stop{false};
};

} // namespace VI
