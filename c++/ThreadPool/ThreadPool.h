/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreadPool.h
 * Author: rohitsharma
 *
 * Created on 2 July, 2018, 3:23 PM
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>       // std::cout
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future> 
#include <queue>
#include <mutex>

template <typename T, int MAX_SIZE = 10 >
class ThreadPool {
private:
    std::condition_variable empty;
    std::condition_variable full;
    std::mutex mtx;
    int no_threads;

public:
    typedef std::function< T() > Work;
    typedef std::promise<T> Promise;

    std::queue<std::pair< Work, Promise > > works;

    ThreadPool(int threads) : no_threads(threads) {
    }

    ~ThreadPool() {
    }

    void start_pool() {
        for (int i = 0; i < no_threads; i++) {
            std::thread([this]() {
                run();
            });
        }
    }

    void run() {
        std::thread::id id = std::this_thread::get_id();
        
        std::pair<Work, Promise> wrk;
        {
            std::unique_lock<std::mutex> lock(mtx);
            while (works.empty()) {
                empty.wait(lock);
            }
            std::cout<< "de-queuing from thread id = " << id;
            wrk = std::move(works.front());
        }

        full.notify_all();
        Work f = wrk.first;
        Promise& prom = wrk.second;
        T ret = f();
        prom.set_value(ret);
    }

    bool isFull() {
        return works.size() > MAX_SIZE;
    }

    std::future<T> submit_task(Work work) {
        std::thread::id id = std::this_thread::get_id();
        
        typename ThreadPool<T>::Promise prom;
        std::future<T> fut = prom.get_future();

        {
            std::unique_lock<std::mutex> lock(mtx);
            while (isFull()) {
                full.wait(lock);
            }
            std::cout<< "en-queuing from thread id = " << id;
            works.emplace(work, std::move(prom));
        }

        empty.notify_all();
        return fut;
    }

};




#endif /* THREADPOOL_H */

