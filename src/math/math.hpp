#pragma once
#include <vector>
#include <glm/glm.hpp>

using glm::vec3;
using std::vector;

class Function {
public:
	Function() {};
	virtual vec3 apply(float t, vector<vec3>) = 0;
	virtual vec3 firstDerivative(float t, vector<vec3> control) = 0;
	virtual vec3 secondDerivative(float t, vector<vec3> control) = 0;
};

class BezierFunction: public Function {
public:
	BezierFunction() : Function() {};
	vec3 apply(float t, vector<vec3> control);
	vec3 firstDerivative(float t, vector<vec3> control);
	vec3 secondDerivative(float t, vector<vec3> control);
};

