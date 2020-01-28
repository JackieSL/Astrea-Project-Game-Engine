#pragma once
#include <loguru.hpp>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <stb_image.h>
#include "Model.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Shader.h"

namespace Astrea
{
	namespace Helper
	{
		static unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false)
		{
			std::string filename = std::string(path);
			filename = directory + '/' + filename;

			unsigned int textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				GLenum format;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else
			{
				LOG_F(ERROR, "Failed to load texture: Path: %s Directory: %s", path, directory.c_str());
				stbi_image_free(data);
			}

			return textureID;
		}
	}

	namespace Assets
	{
		struct Vertex;
		struct Texture;
		class Mesh;
		class Model;
		class Camera;

	}
}

