#pragma once

#include <vector.h>
#include <font.h>
#include <string.h>

namespace nov
{
namespace gui
{

struct IMButtonResult
{
    bool is_down = false;
    bool is_focussed = false;
    bool did_change = false;
};

struct IMTextBoxResult
{
    String text;
    bool is_focussed = false;
    bool did_change = false;
    bool is_confirmed = false;
};

void drawIMLabel(String text, vector::IVector2 offset, vector::IVector2 size, Font* font);
void drawIMButton(String text, IMButtonResult& result, vector::IVector2 offset, vector::IVector2 size, bool enabled, Font* font);
void drawIMTextBox(IMTextBoxResult& result, vector::IVector2 offset, vector::IVector2 size, bool enabled, Font* font);

}
}