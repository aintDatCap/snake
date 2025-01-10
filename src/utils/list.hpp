#ifndef LIST_HPP
#define LIST_HPP
#include <cstdint>

template <typename T> struct ListElement {
    T value;
    T *before;
    T *next;
};

template <typename T> class List {
  private:
    ListElement<T> *head;
    public:
    List() {
        this->head = nullptr;
    }

    void add_element(T value) {
        if(!this->head) {
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
    }

    T get_element_at(uint32_t index) {
        ListElement<T> *current = this->head;
        uint32_t i = 0;
        while ((++i) < index) {
            if(!current->next) {
                return NULL;
            }
            current = current->next;
        }
        return current->value;
    }

    void remove_elements_with(T value);
};

#endif