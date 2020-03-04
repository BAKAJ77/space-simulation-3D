#pragma once
#include <glm/glm.hpp>

struct SpotLight
{
	glm::vec3 m_position, m_direction;
	glm::vec3 m_ambient, m_diffuse, m_specular;

	float m_constant, m_linear, m_quadratic;
	float m_innerCutOff, m_outerCutOff;
	bool m_enabled = true;
};