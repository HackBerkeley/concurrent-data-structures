import java.util.concurrent.ConcurrentHashMap;

import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;

// template <class H>
// void insert_thread(H& h, int start, int end) {
//     for (int i = start; i < end; i++) {
//         h.insert(i);
//     }
//     for (int i = start; i < end; i++) {
//         h.remove(i);
//     }
// }

// template <class H>
// void find_thread(H& h, int start, int end) {
//     int val;
//     for (; start < end; start++) {
//         h.find(start);
//     }
// }

// template <class H>
// void benchmark_hashtable(H& h) {
//     const size_t numthreads = std::thread::hardware_concurrency();
//     const size_t items_per_thread = hashtable_elms / numthreads * 2;
//     std::vector<std::thread> threads;
//     auto t1 = std::chrono::high_resolution_clock::now();
//     for (size_t i = 0; i < numthreads / 2; i++) {
//         threads.emplace_back(insert_thread<H>, std::ref(h), i*items_per_thread, (i+1)*items_per_thread);
//         threads.emplace_back(find_thread<H>, std::ref(h), i*items_per_thread, (i+1)*items_per_thread);
//     }
//     for (size_t i = 0; i < threads.size(); i++) {
//         threads[i].join();
//     }
//     auto t2 = std::chrono::high_resolution_clock::now();
//     std::cout << "benchmark call took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << " ms" << std::endl;
// }


class HashtableJava {
    public static final int hashtable_elms = 10000000;
    public static ConcurrentHashMap<Integer, Integer> h = new ConcurrentHashMap<Integer, Integer>();
    static class Inserter implements Runnable {
        int mStart;
        int mEnd;
        Inserter(int start, int end) {
            mStart = start;
            mEnd = end;
        }
        public void run() {
            for (int i = mStart; i < mEnd; i++) {
                h.put(i, 0);
            }
            for (int i = mStart; i < mEnd; i++) {
                h.remove(i);
            }
        }
    }

    static class Finder implements Runnable {
        int mStart;
        int mEnd;
        Finder(int start, int end) {
            mStart = start;
            mEnd = end;
        }
        public void run() {
            for (int i = mStart; i < mEnd; i++) {
                h.get(i);
            }
        }
    }

    public static void main(String[] args) throws Exception {
        ExecutorService executor = Executors.newCachedThreadPool();
        int threadnum = Runtime.getRuntime().availableProcessors();
        int items_per_thread = hashtable_elms / threadnum * 2;
        Future<?>[] futures = new Future<?>[threadnum];
        long sMs = System.currentTimeMillis();
        for (int i = 0; i < threadnum/2; i ++) {
            futures[i] = executor.submit(new Inserter(i*items_per_thread, (i+1)*items_per_thread));
            futures[i+1] = executor.submit(new Finder(i*items_per_thread, (i+1)*items_per_thread));
        }
        for (Future<?> f : futures) {
            if (f != null) {
                f.get();
            }
        }
        executor.shutdown();
        System.out.println(System.currentTimeMillis() - sMs + " ms");
    }
}
