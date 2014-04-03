#include <stdexcept>
#include "benchmark.hpp"

struct node {
    spinlock lock;
    data val;
    node *next;
};

class List {
public:
    node *head;

    List() {
        head = new node;
        head->next = nullptr;
    }
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
        head->lock.lock();
        newitem->next = head->next;
        head->next = newitem;
        head->lock.unlock();
    }

    data pop() {
        head->lock.lock();
        if (head->next == nullptr) {
            head->lock.unlock();
            throw std::runtime_error("Can't pop from an empty list");
        }
        node *oldhead = head->next;
        oldhead->lock.lock();
        data ret = oldhead->val;
        head->next = oldhead->next;
        delete oldhead;
        head->lock.unlock();
        return ret;
    }

    data sum() {
        data s = 0;
        head->lock.lock();
        node *cur = head;
        while (cur->next != nullptr) {
            node *next = cur->next;
            next->lock.lock();
            s += next->val;
            cur->lock.unlock();
            cur = next;
        }
        cur->lock.unlock();
        return s;
    }
};

int main() {
    List l;
    benchmark_list(l);
}
