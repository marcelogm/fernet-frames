#include "scene.hpp"
#include "scene.hpp"
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
	vector<vec3>* controlPoints = Configuration::getInstance()->getKinematic()->controlPoints;

	this->renderer->clear();
    this->renderer->prepare();
	
	Camera* currentCamera;
	if (Configuration::getInstance()->getKinematic()->isFollowing) {
		this->updatePosition();
		auto position = this->fun->apply(t, *controlPoints);
		auto tangent = glm::normalize(this->fun->firstDerivative(t, *controlPoints));
		auto normal = glm::normalize(this->fun->secondDerivative(t, *controlPoints));
		auto binormal = glm::cross(tangent, normal);
		currentCamera = pathCamera;
		currentCamera->lookAt(position, tangent, binormal);
	} else {
		currentCamera = freeCamera;
	}

	for (auto i = 0; i < controlPoints->size(); i ++) {
		auto entity = this->controlPointsEntities.at(i);
		auto position = controlPoints->at(i);
		entity->setModel(glm::scale(glm::translate(mat4(1), position), vec3(0.5f)));
		entity->update();
		this->renderer->render(entity, currentCamera, Configuration::getInstance()->getLight());
	}

	for (Entity* entity : this->fixedEntities) {
		entity->update();
		this->renderer->render(entity, currentCamera, Configuration::getInstance()->getLight());
	}

	for (auto i = 0; i < 100; i++) {
		auto d = (float)i / 100;
		auto position = this->fun->apply(d, *controlPoints);
		auto entity = this->debugEntities.at(i);
		entity->setModel(glm::scale(glm::translate(mat4(1), position), vec3(0.2f)));
		entity->update();
		this->renderer->render(entity, currentCamera, Configuration::getInstance()->getLight());
	}

}

void Scene::updatePosition() {
	this->t += 0.001f;
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

