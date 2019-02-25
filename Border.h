#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Border
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateBorder(std::string name, glm::vec3 center, float length, float width, float halfZ, glm::vec3 color, glm::vec3 color2, bool fill = false);

}

#pragma once
