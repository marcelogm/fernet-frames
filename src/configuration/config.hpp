#pragma once
#include <vector>
#include <glm/glm.hpp>

using std::vector;
using glm::vec4;
using glm::vec3;

typedef struct {
	int pathFraction;
	bool isFollowing;
	vector<vec3>* controlPoints;
} KinematicConfiguration;

typedef struct {
	float width;
	float height;
} ViewportDimension;

typedef struct {
	vec4 lightPosition;
	float ambientLightStrength;
	float diffuseLightStrength;
} LightConfiguration;

enum class RenderMode {
	TRIANGLES = 0,
	LINES = 1,
	POINTS = 2
};

typedef struct {
	RenderMode mode;
} OpenGLConfiguration;

class Configuration {
private:
	Configuration();
	static Configuration* instance;
	LightConfiguration light;
	ViewportDimension viewport;
	OpenGLConfiguration openGL;
	KinematicConfiguration kinematic;
public:
	static Configuration* getInstance();
	ViewportDimension* getViewport();
	LightConfiguration* getLight();
	OpenGLConfiguration* getOpenGLConfiguration();
	KinematicConfiguration* getKinematic();
};