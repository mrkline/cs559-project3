#pragma once

#include <GL/gl.h>

#include <string>
#include <map>

// Represents a 2D texture loaded into OpenGL
class Texture
{
public:
	Texture(const char* filename, GLenum format = GL_RGBA, bool mipmaps = true);

	Texture(const void* data, int colorComponents, size_t width, size_t height,
	        GLenum format, GLenum type, bool mipmaps = true);

	~Texture();

	//! \todo We'll have to change this when we support multitexturing
	void setAsActiveTexture();

	//! Gets the OpenGL ID of this texture
	unsigned int getID() { return id; }

	std::map<GLenum, int>& getIntParams() { return intParams; }

	std::map<GLenum, float>& getFloatParams() { return floatParams; }

private:
	void init(const void* data, int colorComponents,
	          size_t width, size_t height,
	          GLenum format, GLenum type, bool mipmaps);

	unsigned int id;
	bool hasMipmaps;
	std::map<GLenum, int> intParams;
	std::map<GLenum, float> floatParams;
};
