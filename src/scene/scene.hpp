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
	vector<Entity*> fixedEntities;
	vector<Entity*> debugEntities;
	vector<Entity*> controlPointsEntities;
	Renderer* renderer;
	Camera* freeCamera;
	Camera* pathCamera;
	Function* fun;
	vector<vec3>* controls;
	float t;
public:
	Scene(vector<Entity*> fixed, vector<Entity*> debug, vector<Entity*> control, Camera* camera, Function* fun);
	void render();
	void updatePosition();
	Camera* getCamera();
	vector<Entity*> getEntities();
};

class FernetSceneFactory {
public:
	Scene* build();
	vector<Entity*> getDebug(vector<ShaderInfo> shaders);
	Entity* getControlPoint(vector<ShaderInfo> shaders);
};
