#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>

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

    void enqueue(T value) {
        if (!this->head) {
            this->head = new QueueElement<T>;
            this->head->value = value;
            this->head->next = nullptr;
            return;
        }

        QueueElement<T> *new_elem = new QueueElement<T>;
        new_elem->value = value;
        new_elem->next = this->head;
        this->head = new_elem;
    }

    QueueElement<T> *dequeue() {
        if (!this->head) {
            return nullptr;
        }

        if (!this->head->next) {
            QueueElement<T> *elem = this->head;
            this->head = nullptr;
            return elem;
        }

        QueueElement<T> *current = this->head;
        while (current->next->next) {
            current = current->next;
        }
        QueueElement<T> *elem = current->next;
        current->next = nullptr;
        return elem;
    }

    // Method to get the size of the queue
    size_t size() const {
        if (!this->head) {
            return 0;
        }

        size_t count = 0;
        QueueElement<T> *current = this->head;
        while (current) {
            ++count;
            current = current->next;
        }
        return count;
    }

    // Method to get an element at a specific index in the queue
    QueueElement<T> *get_element_at(size_t index) {
        if (!this->head) {
            return nullptr;
        }

        QueueElement<T> *current = this->head;
        size_t current_index = 0;

        while (current) {
            if (current_index == index) {
                return current;
            }
            ++current_index;
            current = current->next;
        }
        return nullptr; // Return nullptr if index is out of bounds
    }
};

#endif