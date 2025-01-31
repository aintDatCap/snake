#ifndef LEVELLIST_HPP
#define LEVELLIST_HPP

#include <cstddef>

class Level {
    public:
        int value;
        Level* before;
        Level* next;
        
        Level(int val){
            this->value = val; 
            before = nullptr;
            next = nullptr;
        }   
};

class LevelsList {
    private:
        int element_count;
        Level* head;
        Level* current; //keeps track of current level

    public:
        LevelsList(){ 
            this->head = nullptr;
            this->element_count = 0;
            this->current = nullptr;
        }
        ~LevelsList() { 
            while (head) {
                remove_element_at(0); // Remove elements one by one
            }
        }

        void add_element(int value) {
            if (!head) { //if head still empty
                this->head = new Level(value);
                this->element_count++;
                return;
            }

            Level* new_element = new Level(value);
            Level* current = head;

            while (current->next) { //go to the end of the list...
                current = current->next;
            }
            current->next = new_element;
            new_element->before = current;

            element_count++;
        }

        Level* get_element_at(int index) {
            if (!head) { //empty list
                return nullptr;
            }
            
            Level* current = head;
            int i = 0;
            while (i < index) {
                if (!current->next) { //if could not reach till the end...
                    return nullptr;
                }
                current = current->next;
                i++;
            } 
            return current;
        }

        Level* remove_element_at(int index) {
            if (!this->head) {
                return nullptr;
            }
            Level* current = this->head;
            int i = 0;
            while (i < index) {
                if (!current->next) {
                    return nullptr;
                }
                current = current->next;
                i++;
            }

            if (current == this->head) { //if head removed...
                this->head = this->head->next;
                return nullptr;
            }

            //if got to the right element...
            current->before->next = current->next;
            current->next->before = current->before;

            this->element_count--;
            return current;
        }

        int get_element_count(){
            return this->element_count;
        }

        Level* get_next_level(){
            if(current){
                return current->next;
            }
        }
        Level* get_previous_level(){
            if(current){
                return current->before;
            }
        }
};

#endif // LIST_HPP
