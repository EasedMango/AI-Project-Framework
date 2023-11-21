#pragma once
#include <glm/vec3.hpp>


struct SteeringOutput
{
	glm::vec3 linear={};
	float angular=0;
	inline const SteeringOutput operator + (const SteeringOutput& v) const {
		return SteeringOutput(linear + v.linear, angular + v.angular);
	}
	inline SteeringOutput& operator += (const SteeringOutput& v) {
		linear += v.linear;
		angular += v.angular;
		return *this;
	}



};
inline SteeringOutput& operator*(const float lhs, const SteeringOutput& rhs) 
{
	SteeringOutput result = rhs;
	result.linear *= lhs;
	result.angular *= lhs;
	return result;
};