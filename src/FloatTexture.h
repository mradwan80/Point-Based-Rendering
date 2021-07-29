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
    void AttachImageTexture(int im);
    ~FloatTexture();

    static void Unbind();

};