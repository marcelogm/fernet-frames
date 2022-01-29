#pragma once
#include <glm/glm.hpp>

using glm::vec4;

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
public:
	static Configuration* getInstance();
	ViewportDimension* getViewport();
	LightConfiguration* getLight();
	OpenGLConfiguration* getOpenGLConfiguration();
};