#include <functional>
#include <vector>
#include "benchmark.hpp"

class Hashtable {
public:
    std::vector<std::vector<data> > buckets;
    std::vector<spinlock> locks;
    std::hash<data> hashfn;

    Hashtable(): buckets(hashtable_elms / 10), locks(hashtable_elms / 10) {}
    
    void insert(data item) {
        size_t bucket = hashfn(item) % buckets.size();
        locks[bucket].lock();
        buckets[bucket].push_back(item);
        locks[bucket].unlock();
    }

    bool remove(data item) {
        size_t bucket = hashfn(item) % buckets.size();
        locks[bucket].lock();
        for (auto it = buckets[bucket].begin(); it != buckets[bucket].end(); it++) {
            if (*it == item) {
                buckets[bucket].erase(it);
                locks[bucket].unlock();
                return true;
            }
        }
        locks[bucket].unlock();
        return false;
    }

    bool find(data item) {
        size_t bucket = hashfn(item) % buckets.size();
        locks[bucket].lock();
        for (auto it = buckets[bucket].begin(); it != buckets[bucket].end(); it++) {
            if (*it == item) {
                locks[bucket].unlock();
                return true;
            }
        }
        locks[bucket].unlock();
        return false;
    }
};

int main() {
    Hashtable h;
    benchmark_hashtable(h);
}
