#include "Border.h"

#include <Core/Engine.h>

Mesh* Border::CreateBorder(std::string name, glm::vec3 center, float halfLength, float halfWidth, float halfZ, glm::vec3 color, glm::vec3 color2, bool fill)
{
	glm::vec3 corner = center;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(center.x - halfLength, center.y + halfWidth, center.z + halfZ), color2),
		VertexFormat(glm::vec3(center.x + halfLength, center.y + halfWidth, center.z + halfZ),  color2),
		VertexFormat(glm::vec3(center.x + halfLength, center.y - halfWidth, center.z + halfZ),  color2),
		VertexFormat(glm::vec3(center.x - halfLength, center.y - halfWidth, center.z + halfZ), color2),

		// back face
		VertexFormat(glm::vec3(center.x - halfLength, center.y + halfWidth, center.z - halfZ), color), // top left
		VertexFormat(glm::vec3(center.x + halfLength, center.y + halfWidth, center.z - halfZ), color), // top right
		VertexFormat(glm::vec3(center.x + halfLength, center.y - halfWidth, center.z - halfZ),color), // bottom right
		VertexFormat(glm::vec3(center.x - halfLength, center.y - halfWidth, center.z - halfZ), color), // bottom left

		// left face
		VertexFormat(glm::vec3(center.x - halfLength, center.y + halfWidth, center.z + halfZ),color),
		VertexFormat(glm::vec3(center.x - halfLength, center.y + halfWidth, center.z - halfZ),color),
		VertexFormat(glm::vec3(center.x - halfLength, center.y - halfWidth, center.z - halfZ),color),
		VertexFormat(glm::vec3(center.x - halfLength, center.y - halfWidth, center.z + halfZ),color),


		// right face
		VertexFormat(glm::vec3(center.x + halfLength, center.y + halfWidth, center.z + halfZ),color),
		VertexFormat(glm::vec3(center.x + halfLength, center.y + halfWidth, center.z - halfZ),color),
		VertexFormat(glm::vec3(center.x + halfLength, center.y - halfWidth, center.z - halfZ),color),
		VertexFormat(glm::vec3(center.x + halfLength, center.y - halfWidth, center.z + halfZ),color),



		VertexFormat(glm::vec3(center.x - halfLength, center.y + halfWidth, center.z + halfZ),color),
		VertexFormat(glm::vec3(center.x - halfLength, center.y + halfWidth, center.z - halfZ),color),
		VertexFormat(glm::vec3(center.x + halfLength, center.y + halfWidth, center.z - halfZ),color),
		VertexFormat(glm::vec3(center.x + halfLength, center.y + halfWidth, center.z + halfZ),color),

		VertexFormat(glm::vec3(center.x - halfLength, center.y - halfWidth, center.z + halfZ),color),
		VertexFormat(glm::vec3(center.x - halfLength, center.y - halfWidth, center.z - halfZ),color),
		VertexFormat(glm::vec3(center.x + halfLength, center.y - halfWidth, center.z - halfZ),color),
		VertexFormat(glm::vec3(center.x + halfLength, center.y - halfWidth, center.z + halfZ),color)


	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0 , 2,
											4, 5, 6, 7 ,4 ,6,
											8,9,10,11,8,10,
											12,13,14,15,12,14,
											16,17,18,19,16,18,
											20,21,22,23,20,22
	};

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}


	square->InitFromData(vertices, indices);
	return square;
}