#include "StdAfx.hpp"
#include "Texture.hpp"

Texture::Texture(const std::string& filename, GLenum format, bool mipmaps)
	: hasMipmaps(mipmaps)
{
	// Load data from file name
}

Texture::Texture(void *data, int colorComponents, size_t width, size_t height,
			GLenum format, GLenum type, bool mipmaps)
	: hasMipmaps(mipmaps)
{
	init(data, colorComponents, width, height, format, type, mipmaps);
}

void Texture::setAsActiveTexture()
{
	glBindTexture(GL_TEXTURE_2D, id);
	for (auto it = intParams.begin(); it != intParams.end(); ++it)
		glTexParameteri(GL_TEXTURE_2D, it->first, it->second);

	for (auto it = floatParams.begin(); it != floatParams.end(); ++it)
		glTexParameterf(GL_TEXTURE_2D, it->first, it->second);
}

void Texture::init(void* data, int colorComponents, size_t width, size_t height,
			GLenum format, GLenum type, bool mipmaps)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	// select modulate to mix texture with color for shading
	// Likely not needed since we plan on using shaders for everything
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	intParams[GL_TEXTURE_MIN_FILTER] = GL_LINEAR_MIPMAP_NEAREST;
	intParams[GL_TEXTURE_MIN_FILTER] = GL_LINEAR;
	intParams[GL_TEXTURE_WRAP_S] = GL_REPEAT;
	intParams[GL_TEXTURE_WRAP_T] = GL_REPEAT;

	if (mipmaps) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, colorComponents, width, height,
				format, type, data);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, colorComponents, width, height,
				0, format, type, data);
	}
}
