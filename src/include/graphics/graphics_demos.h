#pragma once

#include <graphics/framebuffer.h>
#include <colour.h>

namespace nov
{
namespace graphics
{

void drawStar(Framebuffer buffer, vector::UVector2 uv, Colour foreground, Colour background, bool skip_background = false);
void drawModel(Framebuffer buffer);

}
}