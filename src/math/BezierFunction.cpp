#include "math.hpp"

vec3 BezierFunction::apply(float t, vector<vec3> control) {
	return (pow(1.f - t, 3.f) * control.at(0)) +
			(3 * t * pow(1.f - t, 2.f) * control.at(1)) +
			(3 * pow(t, 2.f) * (1 - t) * control.at(2)) +
			(pow(t, 3.f) * control.at(3));
}

vec3 BezierFunction::firstDerivative(float t, vector<vec3> control) {
	return 3 * pow(1 - t, 2.f) * (control.at(1) - control.at(0)) + 
		6 * (1 - t) * t * (control.at(2) - control.at(1)) +
		3 * pow(t, 2.f) * (control.at(3) - control.at(2));
}

vec3 BezierFunction::secondDerivative(float t, vector<vec3> control) {
	return 6 * (1 - t) * (control.at(2) - (2.f * control.at(1)) + control.at(0)) +
		6 * t * (control.at(3) - (2.f * control.at(2)) + control.at(1));
}
