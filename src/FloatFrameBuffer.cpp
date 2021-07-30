#include "FloatFrameBuffer.h"

FloatFrameBuffer::FloatFrameBuffer()
{
    glGenFramebuffers(1, &handle);
}

FloatFrameBuffer::FloatFrameBuffer(int w, int h)
{
    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
	GLuint r0;	glGenTextures(1, &r0);
	glBindTexture(GL_TEXTURE_2D, r0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, r0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FloatFrameBuffer::AttachtTexture(FloatTexture* tex, int att)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+att, GL_TEXTURE_2D, tex->GetHandle(), 0);
}	
GLuint FloatFrameBuffer::GetHandle()
{
    return handle;
}

void FloatFrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

FloatFrameBuffer::~FloatFrameBuffer()
{

}

void FloatFrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}