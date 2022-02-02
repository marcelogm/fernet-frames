#include "scene.hpp"
#include "scene.hpp"
#include "scene.hpp"

Scene::Scene(vector<Entity*> entities, Camera* camera, vector<vec3> controlPoints, Function* fun) {
	this->entities = entities;
	this->camera = camera;
	this->renderer = new Renderer();
	this->controls = controlPoints;
	this->fun = fun;
	this->t = 0.f;
}

void Scene::render() {
	this->renderer->clear();
    this->renderer->prepare();
	
	this->updatePosition();
	auto position = this->fun->apply(t, this->controls);
	auto tangent = glm::normalize(this->fun->firstDerivative(t, this->controls));
	auto normal = glm::normalize(this->fun->secondDerivative(t, this->controls));
	auto binormal = glm::cross(tangent, normal);

	this->camera->lookAt(position, tangent, binormal);

	for (Entity* entity : this->entities) {
		entity->update();
        this->renderer->render(entity, this->camera, Configuration::getInstance()->getLight());
	}
}

void Scene::updatePosition() {
	this->t += 0.001f;
	if (t > 1.0f) {
		t = 0.0f;
	}
}

Camera* Scene::getCamera() {
	return camera;
}

vector<Entity*> Scene::getEntities() {
	return this->entities;
}

