#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <utility>
#include <atomic>

#pragma once

typedef size_t data;

const size_t pushes = 1000000;
const size_t sums = 20000;

class spinlock {
    std::atomic_flag lock_;
public:
    spinlock() {
        lock_.clear();
    }

    inline void lock() {
        while (lock_.test_and_set(std::memory_order_acquire));
    }

    inline void unlock() {
        lock_.clear(std::memory_order_release);
    }

    inline bool try_lock() {
        return !lock_.test_and_set(std::memory_order_acquire);
    }

} __attribute__((aligned(64)));

template <class L>
void push_pop_thread(L& l) {
    for (size_t i = 0; i < pushes; i++) {
        l.push(i);
    }
    for (size_t i = 0; i < pushes; i++) {
        l.pop();
    }
}

template <class L>
void sum_thread(L& l) {
    for (size_t i = 0; i < sums; i++) {
        l.sum();
    }
}

template <class L>
void benchmark_list(L& l) {
    const size_t numthreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numthreads / 2; i++) {
        threads.emplace_back(push_pop_thread<L>, std::ref(l));
        threads.emplace_back(sum_thread<L>, std::ref(l));
    }
    for (size_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "benchmark call took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << " ms" << std::endl;
}

const size_t hashtable_elms = 10000000;

template <class H>
void insert_thread(H& h, data start, data end) {
    for (data i = start; i < end; i++) {
        h.insert(i);
    }
    for (data i = start; i < end; i++) {
        h.remove(i);
    }
}

template <class H>
void find_thread(H& h, data start, data end) {
    data val;
    for (; start < end; start++) {
        h.find(start);
    }
}

template <class H>
void benchmark_hashtable(H& h) {
    const size_t numthreads = std::thread::hardware_concurrency();
    const size_t items_per_thread = hashtable_elms / numthreads * 2;
    std::vector<std::thread> threads;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numthreads / 2; i++) {
        threads.emplace_back(insert_thread<H>, std::ref(h), i*items_per_thread, (i+1)*items_per_thread);
        threads.emplace_back(find_thread<H>, std::ref(h), i*items_per_thread, (i+1)*items_per_thread);
    }
    for (size_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "benchmark call took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << " ms" << std::endl;
}
