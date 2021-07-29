#pragma once

#include <GL/glew.h>

class FloatTexture
{
    GLuint handle;

  public:
    FloatTexture();
    FloatTexture(int w, int h);
    GLuint GetHandle();
    void Bind();
    ~FloatTexture();

    static void Unbind();

};