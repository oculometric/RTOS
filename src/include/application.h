#pragma once

#include <stdint.h>

enum ApplicationPermission
{
    KERNEL = 0,
    DRIVER,
    USER
};

enum ApplicationPriority
{
    MAXIMUM,
    HIGH,
    NORMAL,
    LOW,
    BACKGROUND
};

enum ApplicationState
{
    DORMANT,
    WAITING,
    ACTIVE
};

struct RegisterStore
{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t eip;
    uint32_t esp;
    // TODO: etc
};

class ApplicationContainer
{
private:
    uint32_t app_id;
    ApplicationPermission permisison;
    ApplicationPriority priority;

    void* entry_point;
    // TODO: mapped memory block info

    ApplicationState current_state;
    void* stack_base;
    void* stack_pointer;
    RegisterStore saved_state;
};