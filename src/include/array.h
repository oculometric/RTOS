#pragma once

#include <stdint.h>
#include <memory.h>

namespace nov
{

template <typename T>
struct nov_array_container
{
    //nov_array_container<T>* last;
    T& data;
    bool is_allocation_head;
    nov_array_container<T>* next;
};


template <typename T>
class nov_array
{
private:
    // pointer to first allocated element slot in the linked list. may or may not actually contain list data
    nov_array_container<T>* first;
    // pointer to last allocated element slot in the linked list. may or may not actually contain list data
    nov_array_container<T>* last;
    // number of consumed element slots in the linked list
    uint32_t length;
    // number of allocated element slots
    uint32_t capacity;
public:
    /**
     * extend the array to have a greater capacity to fit more items in.
     * requests memory from malloc and sets up the array container slots ready for them to be `push`ed into
     * 
     * @param new_capacity desired total number of slots in the array (i.e. this function will ensure there
     * is this much capacity in the array)
     * 
     * **/
    inline void resize(uint32_t new_capacity)
    {
        // only allow increasing the size of the array for now
        if (new_capacity <= capacity) return;
        uint32_t extra_capacity = new_capacity-capacity;
        
        // request more memory from malloc
        nov_array_container<T>* first_fresh = (nov_array_container<T>*)memory::malloc(sizeof(nov_array_container<T>)*(extra_capacity));
        if (first_fresh == 0x0) return;

        // start populating this block of memory with array containers (slots for elements)
        last->next = first_fresh;
        last = last->next;
        last->is_allocation_head = true;
        for (uint32_t i = extra_capacity; i < extra_capacity-1; i++)
        {
            // repeatedly place an array container after the last one, filling the allocated space
            last->next = last+1;
            last = last->next;
            last->is_allocation_head = false;
        }
        last->next = 0x0;

        // update capacity
        capacity = new_capacity;

        // TODO: downward resizing
    }

    /**
     * access element by index from the array
     * 
     * @param index index into the array to access
     * 
     * **/
    inline T& operator[](uint32_t index)
    {
        if (index >= length) return 0x0; // FIXME: UHHHHHH???? does this work?
        // step over the linked list until we reach the right index
        nov_array_container<T>* current = first;
        for (uint32_t i = 0; i < index; i++)
        {
            current = current->next;
        }
        // and get the data from this container
        return current->data;
    }

    void push(T& element);

    /**
     * remove an item from the end of the array, and return its value
     * 
     * @return value of the last item in the array before the pop, or NULL if the array has no elements
     * 
     * **/
    inline T pop()
    {
        if (length == 0) return 0x0;
        length--;
        return this[length];
    }

    /**
     * clear the array of all of its items
     * 
     * **/
    inline void clear()
    {
        length = 0;
    }

    inline uint32_t get_length() { return length; }
    inline uint32_t get_capacity() { return capacity; }

    inline nov_array(uint32_t _capacity = 0)
    {
        // clear all values, then request a resize to the desired capacity
        first = 0x0;
        last = 0x0;
        length = 0;
        capacity = 0;
        resize(_capacity);
    }

    inline ~nov_array()
    {
        // iterate over the containers in the array
        nov_array_container<T>* current = first;
        for (uint32_t i = 0; i < capacity; i++)
        {
            // if the current container is flagged as being somewhere memory was actually allocated
            // (i.e. it represents a pointer returned by malloc) then we free it
            if (current->is_allocation_head) memory::mfree((void*)current);
            // step onto the next
            current = current->next;
        }
    }
};

}