#pragma once

#include <stdint.h>
#include <panic.h>

// TODO: comments in this file

namespace nov
{

template <typename T>
class LinkedList
{
private:
    struct LinkedListElement
    {
        LinkedListElement* next = nullptr;
        LinkedListElement* prev = nullptr;
        T value;
    };

    LinkedListElement* head = nullptr;
    LinkedListElement* tail = nullptr;

    uint32_t length = 0;
public:
    LinkedList()
    {
    }

    T& get(uint32_t index)
    {
        if (index >= length) panic("linked list index out of range!");
        if (index < (length / 2))
        {
            LinkedListElement* start = head;
            uint32_t current = 0;
            while (current != index)
            {
                current++;
                start = head->next;
            }
            return start->value;
        }
        else
        {
            LinkedListElement* start = tail;
            uint32_t current = length - 1;
            while (current != index)
            {
                current--;
                start = head->prev;
            }
            return start->value;
        }
    }
    
    T& operator[](uint32_t index)
    {
        return get(index);
    }

    uint32_t find(T value)
    {
        LinkedListElement* element = head;
        uint32_t index = 0;

        while (index < length)
        {
            if (element->value == value)
                break;
            index++;
            element = element->next;
        }

        return index;
    }

    uint32_t move(T value, uint32_t index)
    {
        uint32_t old_index = remove(value);
        insert(value, index);

        return old_index;
    }

    uint32_t remove(T value)
    {
        LinkedListElement* element = head;
        uint32_t index = 0;

        while (index < length)
        {
            if (element->value == value)
                break;
            index++;
            element = element->next;
        }

        if (index < length)
        {
            if (element == head)
                head = element->next;
            else
                element->prev->next = element->next;
            
            if (element == tail)
                tail = element->prev;
            else
                element->next->prev = element->prev;

            delete element;
        }

        length--;
        return index;
    }

    T pop()
    {
        T value;
        if (length == 0)
            panic("linked list dry as fuck!");

        value = tail->value;

        tail = tail->prev;
        delete tail->next;
        tail->next = nullptr;

        length--;
        return value;
    }

    uint32_t insert(T value, uint32_t index = -1)
    {
        if (index >= length)
        {
            return append(value);
        }
        else if (index == 0)
        {
            head->prev = new LinkedListElement{ head, nullptr, value };
            head = head->prev;
        }
        else
        {
            uint32_t current = 0;
            LinkedListElement* element = head;
            while (current != index)
            {
                current++;
                element = element->next;
            }
            LinkedListElement* new_el = new LinkedListElement{ element, element->prev, value };
            element->prev->next = new_el;
            element->prev = new_el;
        }

        length++;
        return index;
    }

    uint32_t append(T value)
    {
        LinkedListElement* new_el = new LinkedListElement{ nullptr, tail, value };
        length++;
        if (length > 1)
            tail->next = new_el;
        else
            head = new_el;
        tail = new_el;

        return length - 1;
    }

    uint32_t getLength() { return length; }

    LinkedList(LinkedList& other) = delete;
    LinkedList(LinkedList&& other) = delete;
    LinkedList operator=(LinkedList& other) = delete;
    LinkedList operator=(LinkedList&& other) = delete;

    ~LinkedList()
    {
        LinkedListElement* element = head;
        while (element)
        {
            LinkedListElement* next = element->next;
            delete element;
            element = next;
        }
        head = nullptr;
        tail = nullptr;
        length = 0;
    }
};

}