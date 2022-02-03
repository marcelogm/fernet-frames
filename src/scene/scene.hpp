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
	void updatePathCamera(vector<vec3>* controlPoints);
	void updateAndRender(Entity* entity, Camera* camera);
	void updateStep(float velocity);
	Camera* getCamera();
	vector<Entity*> getEntities();
};

class FernetSceneFactory {
private:
	ObjectProvider provider;
	vector<ShaderInfo> shaders;
public:
	Scene* build();
	vector<Entity*> getDebugPath();
	Entity* getControlPoint();
	FernetSceneFactory();
};

class Frametime {
private:
	static Frametime* instance;
	Frametime();

	double framerate;
	double frametime;

	double lastFrameTime = 0.0;
	double currentFrameTime = 0.0;
	double timeFrameDiff = 0.0;
	size_t frameCounter = 0;
public:
	static Frametime* getInstance();
	void countFrame();
	double getFramerate();
	double getFrametime();
	double getDelta();
};
