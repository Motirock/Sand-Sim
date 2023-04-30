#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "ShaderClass.h"

class Texture
{
public:
	Texture(const char *image, const char *texType, GLuint slot, GLenum format, GLenum pixelType);

	// Assigns a texture unit to a texture
	void textureUnit(Shader &shader, const char* uniform, GLuint unit);
	// Binds a texture
	void bind();
	// Unbinds a texture
	void unbind();
	// Deletes a texture
	void deleteTexture();

	GLuint ID;
	const char *type;
	GLuint unit;
};
#endif