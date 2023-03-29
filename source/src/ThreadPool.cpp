#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>

#include "ThreadPool.hpp"

ThreadPool::ThreadPool (int threads) : shutdown_ (false) {
    //Create the specified number of threads
    threads_.reserve (threads);
    for (int i = 0; i < threads; ++i)
        threads_.emplace_back (std::bind (&ThreadPool::threadEntry, this, i));
}

ThreadPool::ThreadPool() {
    std::cerr << "Thread Pool's default constructor should NOT be used!" << std::endl;
}

ThreadPool::~ThreadPool () {
    {
        //Unblock any threads and tell them to stop
        std::unique_lock <std::mutex> l (lock_);

        shutdown_ = true;
        condVar_.notify_all();
    }

    //Wait for all threads to stop
    //std::cerr << "Joining threads" << std::endl;
    for (auto& thread : threads_)
        thread.join();
}

void ThreadPool::doJob (std::function <void (void)> func) {
        //Place a job on the queu and unblock a thread
        std::unique_lock <std::mutex> l (lock_);

        jobs_.emplace (std::move (func));
        condVar_.notify_one();
    }

void ThreadPool::threadEntry (int i) {
    std::function <void (void)> job;

    while (1) {
        {
            std::unique_lock <std::mutex> l (lock_);

            while (! shutdown_ && jobs_.empty())
                condVar_.wait (l);

            if (jobs_.empty ())
            {
                //No jobs to do and we are shutting down
                //std::cerr << "Thread " << i << " terminates" << std::endl;
                return;
            }

            //std::cerr << "Thread " << i << " does a job" << std::endl;
            job = std::move (jobs_.front ());
            jobs_.pop();
        }

        // Do the job without holding any locks
        job ();
    }

}
