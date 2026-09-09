#include <mutex>
#include <iostream>

#include "threadpool.hpp"

using namespace std;

Threadpool::Threadpool(size_t num) : stop { false } {
    for (size_t i { 0 }; i < num; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                Task task;

                {
                    unique_lock<mutex> lock(mtx);
                    cv.wait(lock, [this]() {
                        return stop || !_tasks.empty();
                    });

                    if (stop && _tasks.empty()) return;

                    task = move(_tasks.front());
                    _tasks.pop();
                }

                try {
                    task();
                }
                catch (const exception &e) {
                    cerr << "Exception in thread: " << e.what() << endl;
                }
            }
        });
    }
}

Threadpool::~Threadpool() {
    {
        unique_lock<mutex> lock(mtx);
        stop = true;
    }

    cv.notify_all();

    for (thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}