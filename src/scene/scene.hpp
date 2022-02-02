#pragma once

#include "../configuration/config.hpp"
#include "../camera/camera.hpp"
#include "../entity/entity.hpp"
#include "../math/math.hpp"

using glm::mat4;
using std::vector;

class Renderer {
private:
	float aspect;
	mat4 projection;
	float lightPosition;
	float ambientLightStrength;
	float diffuseLightStrength;
public:
	void clear();
	void prepare();
	void render(Entity*, Camera*, LightConfiguration*);
};

class Scene {
private:
	vector<Entity*> entities;
	Renderer* renderer;
	Camera* camera;
	Function* fun;
	vector<vec3> controls;
	float t;
public:
	Scene(vector<Entity*> entities, Camera* camera, vector<vec3> controlPoints, Function* function);
	void render();
	void updatePosition();
	Camera* getCamera();
	vector<Entity*> getEntities();
};

class FernetSceneFactory {
public:
	Scene* build();
	vector<Entity*> getPath(vec3 a, vec3 b, vec3 c, vec3 d, vector<ShaderInfo> shaders);
	Entity* getControlPoint(vec3 position, float scale, vector<ShaderInfo> shaders);
};
