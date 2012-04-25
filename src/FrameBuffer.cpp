#include "StdAfx.hpp"
#include "FrameBuffer.hpp"

#include "Exceptions.hpp"
#include "Texture.hpp"

using namespace std;

FrameBuffer::FrameBuffer(size_t width, size_t height)
	: width(width), height(height)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Generate depth buffer and bind it to the FBO
	glGenRenderbuffers(1, &depthBuff);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuff);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	                          GL_RENDERBUFFER, depthBuff);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteRenderbuffers(1, &depthBuff);
	glDeleteFramebuffers(1, &fbo);
}

void FrameBuffer::attachTexture(const shared_ptr<Texture>& tex)
{
	if (tex->getWidth() != width || tex->getHeight() != height) {
		throw Exceptions::ArgumentException("A texture must be the same"
		                                    " dimensions as the FBO to be"
		                                    " attached to it.",
		                                    __FUNCTION__);
	}
	// Bind the texture to the frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
	                       tex->getID(), 0);
	// Check to make sure we're good to go
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw Exceptions::Exception("Frame buffer object is not complete.",
		                            __FUNCTION__);
	}
}

void FrameBuffer::setupRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, width, height);
}

void FrameBuffer::cleanupRender()
{
	glPopAttrib();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
