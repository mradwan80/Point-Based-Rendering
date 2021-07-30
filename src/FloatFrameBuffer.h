#pragma once

#include <GL/glew.h>
#include "FloatTexture.h"

class FloatFrameBuffer
{
    GLuint handle;

  public:
    FloatFrameBuffer();
    FloatFrameBuffer(int w, int h);
    GLuint GetHandle();
    void AttachtTexture(FloatTexture* tex, int att);
    void Bind();
    ~FloatFrameBuffer();

    static void Unbind();

};