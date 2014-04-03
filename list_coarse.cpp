#include <stdexcept>
#include "benchmark.hpp"

struct node {
    data val;
    node *next;
};

class List {
public:
    spinlock lock;
    node *head;

    List(): head(nullptr) {}
    ~List() {
        while (head != nullptr) {
            node *next = head->next;
            delete head;
            head = next;
        }
    }

    void push(data item) {
        node *newitem = new node;
        newitem->val = item;
        lock.lock();
        newitem->next = head;
        head = newitem;
        lock.unlock();
    }

    data pop() {
        lock.lock();
        if (head == nullptr) {
            lock.unlock();
            throw std::runtime_error("Can't pop from an empty list");
        }
        data ret = head->val;
        node *oldhead = head;
        head = head->next;
        delete oldhead;
        lock.unlock();
        return ret;
    }

    data sum() {
        data s = 0;
        lock.lock();
        for (node *n = head; n != nullptr; n = n->next) {
            s += n->val;
        }
        lock.unlock();
        return s;
    }
};

int main() {
    List l;
    benchmark_list(l);
}
