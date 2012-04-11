#pragma once

#include <Windows.h>
// Needed because Windows is stupid and defines macros for these things
#undef min
#undef max
#undef near
#undef far
#include <GL/gl.h>

#include <string>
#include <map>

// Represents a 2D texture loaded into OpenGL
class Texture
{
public:
	Texture(const std::string& filename, GLenum format, bool mipmaps = true);
	
	Texture(void *data, int colorComponents, size_t width, size_t height,
			GLenum format, GLenum type, bool mipmaps = true);

	//! \todo We'll have to change this when we support multitexturing
	void setAsActiveTexture();

	//! Gets the OpenGL ID of this texture
	unsigned int getID() { return id; }

	std::map<GLenum, int>& getIntParams() { return intParams; }

	std::map<GLenum, float>& getFloatParams() { return floatParams; }
	
private:
	void init(void* data, int colorComponents, size_t width, size_t height,
			GLenum format, GLenum type, bool mipmaps);
	
	unsigned int id;
	bool hasMipmaps;
	std::map<GLenum, int> intParams;
	std::map<GLenum, float> floatParams;
};
