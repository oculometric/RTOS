#pragma once

#include <vector.h>
#include <font.h>
#include <string.h>
#include <colour.h>
#include <gui/compositor.h>

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

void drawIMLabel(String text, vector::IVector2 offset, vector::IVector2 size, ProtectedFramebuffer* framebuffer, Font* font);
void drawIMButton(String text, IMButtonResult& result, vector::IVector2 offset, vector::IVector2 size, bool enabled, Font* font);
void drawIMTextBox(IMTextBoxResult& result, vector::IVector2 offset, vector::IVector2 size, bool enabled, Font* font);

}
}