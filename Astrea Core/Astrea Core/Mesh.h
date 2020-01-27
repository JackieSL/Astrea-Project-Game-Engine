#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <vector>
#include <string>
#include <map>

typedef unsigned short int	u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef signed short int	s16;
typedef signed int			s32;
typedef signed long long	s64;


struct triangle
{
	std::vector<glm::vec4> data;
};
class Mesh
{
private:
	std::vector<glm::vec4>	vertices;
	std::vector<glm::vec2>	normals;
	std::vector<glm::vec2>	uv;


	std::map<u16, glm::vec4> indices;


public:
	Mesh();

	void index()
	{
		for (auto it = vertices.begin(); it != vertices.end(); it++)
		{
			indices[(u16)it]
		}
	}
};