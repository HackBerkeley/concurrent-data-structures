#include <functional>
#include <vector>
#include "benchmark.hpp"

class Hashtable {
public:
    spinlock lock;
    std::vector<std::vector<data> > buckets;
    std::hash<data> hashfn;

    Hashtable(): buckets(hashtable_elms / 10) {}
    
    void insert(data item) {
        size_t bucket = hashfn(item) % buckets.size();
        lock.lock();
        buckets[bucket].push_back(item);
        lock.unlock();
    }

    bool remove(data item) {
        size_t bucket = hashfn(item) % buckets.size();
        lock.lock();
        for (auto it = buckets[bucket].begin(); it != buckets[bucket].end(); it++) {
            if (*it == item) {
                buckets[bucket].erase(it);
                lock.unlock();
                return true;
            }
        }
        lock.unlock();
        return false;
    }

    bool find(data item) {
        size_t bucket = hashfn(item) % buckets.size();
        lock.lock();
        for (auto it = buckets[bucket].begin(); it != buckets[bucket].end(); it++) {
            if (*it == item) {
                lock.unlock();
                return true;
            }
        }
        lock.unlock();
        return false;
    }
};

int main() {
    Hashtable h;
    benchmark_hashtable(h);
}
