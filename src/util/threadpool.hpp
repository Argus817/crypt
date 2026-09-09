#pragma once

#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>

using Task = std::function<void()>;

class Threadpool {
private:
    std::vector <std::thread> workers;
    std::queue<Task> _tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;

public:
    Threadpool(size_t num);
    ~Threadpool();

    template <typename F, typename... Args>
    auto submit(
        F&& f,
        Args&&... args
    ) -> std::future<std::invoke_result_t<F, Args...>> {
        using Return_Type = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<Return_Type()>>(
            bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<Return_Type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(mtx);
            if (stop) {
                throw std::runtime_error("submit on stopped Threadpool");
            }
            _tasks.emplace([task]() {
                (*task)();
            });
        }

        cv.notify_one();
        return res;
    }

    Threadpool(const Threadpool& other) = delete;
    Threadpool& operator=(const Threadpool& other) = delete;
};