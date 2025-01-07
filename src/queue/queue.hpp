#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <functional>
#include <vector>

template <typename T> struct QueueElement {
    T value;
    QueueElement<T> *next;
};

template <typename T> class Queue {
  private:
    QueueElement<T> *head;

  public:
    Queue<T>() {
        this->head = nullptr;
    }

    Queue<T>(T value) {
        this->head = new QueueElement<T>;
        this->head->value = value;
        this->head->next = nullptr;
    }

    void append(T value) {
        if(!this->head) {
            this->head = new QueueElement<T>;
            this->head->value = value;
            this->head->next = nullptr;
            return;
        }

        QueueElement<T> *current = this->head;
        while (current->next) {
            current = current->next;
        }

        current->next = new QueueElement<T>;
        current->next->value = value;
        current->next->next = nullptr;
    }

    T pop() {
        QueueElement<T> *old = this->head;
        if (this->head->next) {
            this->head = this->head->next;
        } else {
            this->head = nullptr;
        }
        return old->value;
    }

    void foreach_element(std::function<T> callback) {
        QueueElement<T> current = this->head;
        do {
            callback(current.value);
            current = current.next;
        } while (current);
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