#include "FloatTexture.h"

FloatTexture::FloatTexture()
{
    glGenTextures(1, &handle);
}

FloatTexture::FloatTexture(int w, int h)
{
    glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, 0);
}
GLuint FloatTexture::GetHandle()
{
    return handle;
}
void FloatTexture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, handle);
}

void FloatTexture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

FloatTexture::~FloatTexture()
{

}
