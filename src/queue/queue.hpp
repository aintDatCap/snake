#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <vector>
template <typename T> struct QueueElement {
    T value;
    QueueElement<T> *next;
};

template <typename T> class Queue {
  private:
    QueueElement<T> head;

  public:
    Queue<T>(T value) {
        this->head.value = value;
    }

    void append(T value) {
        QueueElement<T> elem;
        elem.value = value;

        QueueElement<T> current = this->head;
        while (current.next) {
            current = current.next;
        }
        current.next = &elem;
    }

    void pop() {
        QueueElement<T> old = this->head;
        if (this->head.next) {
            this->head = this->head.next;
        }
    }

    std::vector<T> to_vector() {
        std::vector<T> result;
        QueueElement<T> current = this->head;
        do {
            result.push_back(current.value);
            current = current.next;
        } while (current);
    }
};

#endif