#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

template <typename T> struct ListElement {
    T value;
    ListElement<T> *before;
    ListElement<T> *next;
};

template <typename T> class List {
  private:
    ListElement<T> *head;
    size_t element_count;

  public:
    List() {
        this->head = nullptr;
        this->element_count = 0;
    }

    void clear() {
        while(head != nullptr) {
            ListElement<T>* temp = head;
            head = head->next;
            delete temp;
        }
        element_count = 0;
    }    

    void add_element(T value) {
        if (!this->head) {
            this->head = new ListElement<T>;
            this->head->value = value;
            this->head->before = nullptr;
            this->head->next = nullptr;
            return;
        }

        ListElement<T> *new_element = new ListElement<T>;
        new_element->value = value;

        ListElement<T> *current = this->head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_element;
        new_element->before = current;

        this->element_count++;
    }

    ListElement<T> *get_element_at(size_t index) {
        if (!this->head) {
            return nullptr;
        }
        ListElement<T> *current = this->head;
        size_t i = 0;
        while (i < index) {
            if (!current->next) {
                return nullptr;
            }
            current = current->next;
            i++;
        }
        return current;
    }
    
    const ListElement<T>* get_element_at(size_t index) const {
        ListElement<T>* current = head;
        for(size_t i = 0; i < index && current != nullptr; i++) {
            current = current->next;
        }
        return current;
    }    

    ListElement<T> *remove_element_at(size_t index) {
        if (!this->head) {
            return nullptr;
        }
        ListElement<T> *current = this->head;
        size_t i = 0;
        while (i < index) {
            if (!current->next) {
                return nullptr;
            }
            current = current->next;
            i++;
        }

        if (current == this->head) {
            this->head = this->head->next;
            return nullptr;
        }

        current->before->next = current->next;
        current->next->before = current->before;

        this->element_count--;
        return current;
    }

    size_t get_element_count() const {
        return this->element_count;
    }
};

#endif