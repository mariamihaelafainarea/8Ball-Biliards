#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Ractangle
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateRactangle(std::string name, glm::vec3 center, float length, float width,float halfZ, glm::vec3 color,glm::vec3 color2, bool fill = false);

}

