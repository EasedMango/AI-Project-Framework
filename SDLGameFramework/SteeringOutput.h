#pragma once
#include <glm/vec3.hpp>


struct SteeringOutput
{
	glm::vec3 linear = {};
	float angular = 0;
	inline const SteeringOutput operator + (const SteeringOutput& v) const {
		return SteeringOutput(linear + v.linear, angular + v.angular);
	}
	inline SteeringOutput& operator += (const SteeringOutput& v) {
		linear += v.linear;
		angular += v.angular;
		return *this;
	}

	SteeringOutput& operator*=(const float rhs)
	{
		linear *= rhs;
		angular *= rhs;
		return *this;
	}

	SteeringOutput* operator*(const float rhs) const
	{
		SteeringOutput result = *this;
		result.linear *= rhs;
		result.angular *= rhs;
		return &result;
	}

	void Print() const
	{
				std::cout << "Linear: " << linear.x << ", " << linear.y << ", " << linear.z << std::endl;
		std::cout << "Angular: " << angular << std::endl;
	}

};
inline SteeringOutput& operator*(const float lhs, const SteeringOutput& rhs)
{
	SteeringOutput result = rhs;
	result.linear *= lhs;
	result.angular *= lhs;
	return result;
};