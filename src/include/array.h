#pragma once

#include <stdint.h>
#include <memory.h>
#include <panic.h>

namespace nov
{

struct nov_array_container
{
    nov_array_container* next;
    uint32_t elements_in_block;
};

template <typename T>
class nov_array
{
private:
    // pointer to first allocated element slot in the linked list. may or may not actually contain list data
    nov_array_container* first = 0x0;
    // pointer to last allocated element slot in the linked list. may or may not actually contain list data
    nov_array_container* last = 0x0;
    // number of consumed element slots in the linked list
    uint32_t length = 0;
    // number of allocated element slots
    uint32_t capacity = 0;
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
        nov_array_container* first_fresh = (nov_array_container*)memory::malloc(sizeof(nov_array_container) + (extra_capacity * sizeof(T)));
        if (first_fresh == 0x0) panic();

        // check if the array is uninitialised
        if (first == 0x0)
        {
            // if so, set the first and last to point to this block
            first = first_fresh;
            last = first_fresh;
        }
        else
        {
            // otherwise, tell the existing block to point to this new one
            last->next = first_fresh;
            last = first_fresh;
        }

        // start populating this block of memory with array containers (slots for elements)
        last->elements_in_block = extra_capacity;
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
        if (index >= length) panic(); // crashes the kernel
        // step over the linked list until we reach the block which contains the relevant index
        nov_array_container* current = first;
        uint32_t cumulative = 0;
        while (index > cumulative + current->elements_in_block)
        {
            cumulative += current->elements_in_block;
            current = current->next;
        }

        // and get the data from this container
        return ((T*)(current + 1))[index - cumulative];
    }

    /**
     * access element by index from the array
     * 
     * @param index index into the array to access
     * 
     * **/
    inline T operator[](uint32_t index) const
    {
        if (index >= length) panic(); // crashes the kernel
        // step over the linked list until we reach the block which contains the relevant index
        nov_array_container* current = first;
        uint32_t cumulative = 0;
        while (index > cumulative + current->elements_in_block)
        {
            cumulative += current->elements_in_block;
            current = current->next;
        }

        // and get the data from this container
        return ((T*)(current + 1))[index - cumulative];
    }

    /**
     * insert an element into the back of the list
     * 
     * @param element element data to insert
     * 
     * **/
    inline void push(T element)
    {
        // if we're at the limit, resize
        if (length == capacity)
        {
            resize(capacity+4);
        }
        // if we have size now, increase length and insert
        if (capacity > length)
        {
            length++;
            (*this)[length-1] = element;
        }
    }

    /**
     * remove an item from the end of the array, and return its value
     * 
     * @return value of the last item in the array before the pop, or NULL if the array has no elements
     * 
     * **/
    inline T pop()
    {
        if (length == 0) panic(); // crashes the kernel
        T value = (*this)[length-1];
        length--;
        return value;
    }

    /**
     * clear the array of all of its items
     * 
     * **/
    inline void clear()
    {
        length = 0;
    }

    inline uint32_t get_length() const { return length; }
    inline uint32_t get_capacity() const { return capacity; }

    inline constexpr nov_array(uint32_t _capacity)
    {
        // clear all values, then request a resize to the desired capacity
        first = 0x0;
        last = 0x0;
        length = 0;
        capacity = 0;
        resize(_capacity);
    }

    nov_array(const nov_array&) = delete;

    nov_array() : first(0x0), last(0x0), length(0), capacity(0) { };

    inline ~nov_array()
    {
        if (first == 0x0) return;
        // iterate over the containers in the array
        nov_array_container* current = first;
        while (current != 0x0)
        {
            // free the block
            memory::mfree((void*)current);
            // step onto the next
            current = current->next;
        }
        first = 0x0; last = 0x0;
        memory::mconsolidate();
    }
};

}