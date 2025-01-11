#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <vector>

template <typename T> struct QueueElement {
    T value;
    QueueElement<T> *next;
};

template <typename T> class Queue {
  private:
    QueueElement<T> *head;

  public:
    Queue() {
        this->head = nullptr;
    }

    Queue(T value) {
        this->head = new QueueElement<T>;
        this->head->value = value;
        this->head->next = nullptr;
    }

    ~Queue() {
        if (!this->head) {
            return;
        }

        QueueElement<T> *current = this->head;
        do {
            QueueElement<T> *previous = current;
            current = current->next;
            delete previous;
        } while (current->next);
    }

    void append(T value) {
        if (!this->head) {
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

    std::vector<T> to_vector() {
        std::vector<T> result;
        QueueElement<T> *current = this->head;
        do {
            result.push_back(current->value);
            current = current->next;
        } while (current);

        return result;
    }
};

#endif