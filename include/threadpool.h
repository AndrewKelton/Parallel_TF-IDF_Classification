#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>


class ThreadPool {
    
    public:

        ThreadPool(size_t num_threads) {
            for (size_t i = 0; i < num_threads; ++i) {
                workers.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(queue_mutex);
                            condition.wait(lock, [this] { return !tasks.empty() || stop; });

                            if (stop && tasks.empty()) return;

                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();  // Execute the task
                    }
                });
            }
        }

        void enqueue(std::function<void()> task) {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                tasks.push(std::move(task));
            }
            condition.notify_one();
        }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();

            for (std::thread& worker : workers)
                worker.join();
        }

    private:

        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop = false;
};
