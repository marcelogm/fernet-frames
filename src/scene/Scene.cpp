#include "scene.hpp"

Scene::Scene(vector<Entity*> fixed, vector<Entity*> debug, vector<Entity*> control, Camera* camera, Function* fun) {
	this->fixedEntities = fixed;
	this->debugEntities = debug;
	this->controlPointsEntities = control;
	this->pathCamera = new Camera();
	this->freeCamera = camera;
	this->renderer = new Renderer();
	this->fun = fun;
	this->t = 0.f;
}

void Scene::render() {
	KinematicConfiguration* kinematic = Configuration::getInstance()->getKinematic();
	vector<vec3>* controlPoints = kinematic->controlPoints;
	size_t debugPathCount = kinematic->pathFraction;

	// 0.0f to 1.0f interval
	this->updateStep(kinematic->velocity);
	this->updatePathCamera(controlPoints);

	// prepare opengl structures
	this->renderer->clear();
    this->renderer->prepare();
	
	// define current camera
	Camera* currentCamera = kinematic->isFollowing ? pathCamera : freeCamera;

	// render fixed entities
	for (Entity* entity : this->fixedEntities) {
		this->updateAndRender(entity, currentCamera);
	}

	// update control points position
	for (auto i = 0; i < controlPoints->size(); i ++) {
		auto entity = this->controlPointsEntities.at(i);
		entity->setModel(glm::scale(glm::translate(mat4(1), controlPoints->at(i)), vec3(0.5f)));
		this->updateAndRender(entity, currentCamera);
	}

	// create path with boxes
	for (auto i = 0; i < debugPathCount; i++) {
		auto entity = this->debugEntities.at(i);
		auto position = this->fun->apply((i / (float) debugPathCount), *controlPoints);
		entity->setModel(glm::scale(glm::translate(mat4(1), position), vec3(0.2f)));
		this->updateAndRender(entity, currentCamera);
	}
}

void Scene::updatePathCamera(vector<vec3>* controlPoints) {
	auto position = this->fun->apply(t, *controlPoints);
	auto tangent = glm::normalize(this->fun->firstDerivative(t, *controlPoints));
	auto normal = glm::normalize(this->fun->secondDerivative(t, *controlPoints));
	auto binormal = glm::cross(tangent, normal);

	pathCamera->lookAt(position, tangent, binormal);
}

void Scene::updateAndRender(Entity* entity, Camera* camera) {
	entity->update();
	renderer->render(entity, camera, Configuration::getInstance()->getLight());
}

void Scene::updateStep(float velocity) {
	auto delta_t = Frametime::getInstance()->getDelta();
	this->t += 0.05f * velocity * delta_t;
	if (t > 1.0f) {
		t = 0.0f;
	}
}

Camera* Scene::getCamera() {
	return freeCamera;
}

vector<Entity*> Scene::getEntities() {
	return this->fixedEntities;
}

