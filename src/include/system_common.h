#pragma once

namespace nov
{

// CPU privilege level. lower is more privileged
enum Privilege
{
    LEVEL_0 = 0,
    LEVEL_1 = 1,
    LEVEL_2 = 2,
    LEVEL_3 = 3
};

}