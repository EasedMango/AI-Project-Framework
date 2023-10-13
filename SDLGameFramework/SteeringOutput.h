#pragma once
#include <glm/vec3.hpp>


struct SteeringOutput
{
	glm::vec3 linear;
	float angular;
	inline const SteeringOutput operator + (const SteeringOutput& v) const {
		return SteeringOutput(linear + v.linear, angular + v.angular);
	}
	inline SteeringOutput& operator += (const SteeringOutput& v) {
		linear += v.linear;
		angular += v.angular;
		return *this;
	}
};
