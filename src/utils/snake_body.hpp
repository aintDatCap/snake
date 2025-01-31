#ifndef SNAKEBODY_HPP
#define SNAKEBODY_HPP
#include <cstddef>

class SnakePart {
    public:
        int value;
        SnakePart* next;
        
        SnakePart(int val){
            this->value = val; 
            this->next = nullptr;
        }   
};

class SnakeBody {
    private:
        SnakePart* head;

    public:
        //constructors
        SnakeBody(){
            this->head = nullptr;
        }
        SnakeBody(int value) {
            head = new SnakePart(value);
        }
        ~SnakeBody() {
            while (head) {
                SnakePart* current = head;
                head = head->next;
                delete current;
            }
        }

        void enqueue(int value) {
            SnakePart* new_elem = new SnakePart(value);
            if (!head) {
                head = new_elem;
                return;
            }
            // Insert at the end of the queue
            SnakePart* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = new_elem;
        }

        SnakePart* dequeue() {
            if (!head) {
                return nullptr; // Queue is empty
            }

            SnakePart* elem = head;
            head = head->next; // Move head to the next element
            return elem; // Return the dequeued element
        }

        int size() {
            int count = 0;
            SnakePart* current = head;
            while (current) {
                ++count;
                current = current->next;
            }
            return count;
        }

        // Method to get an element at a specific index in the queue
        SnakePart* get_element_at(int index) {
            if (!head) {
                return nullptr; // Empty queue
            }

            SnakePart* current = head;
            int current_index = 0;

            while (current) {
                if (current_index == index) {
                    return current; // Return element at specified index
                }
                ++current_index;
                current = current->next;
            }
            return nullptr; // Return nullptr if index is out of bounds
        }
};
#endif //SNAKEBODY_HPP