#include "scene.hpp"
#include "scene.hpp"
#include "scene.hpp"
#include "scene.hpp"
#include "scene.hpp"
#include "scene.hpp"
#include <glm/gtx/norm.hpp>

Scene::Scene(vector<Entity*> fixed, vector<Entity*> debug, vector<Entity*> control, Camera* camera, Function* fun) {
	this->fixedEntities = fixed;
	this->debugEntities = debug;
	this->controlPointsEntities = control;
	this->character = new Entity(
		ObjectProvider().get("resources/sphere.obj"),
		{
			{ GL_VERTEX_SHADER, "resources/shaders/shader.vert" },
			{ GL_FRAGMENT_SHADER, "resources/shaders/shader.frag" },
			{ GL_NONE, NULL }
		},
		vec4(1.f, 0.00f, 0.00f, 1.0f),
		mat4(1)
	);
	this->pathCamera = new Camera();
	this->freeCamera = camera;
	this->renderer = new Renderer();
	this->fun = fun;
	this->t = 0.f;
	this->last = 0.f;
}

void Scene::render() {
	// prepare opengl structures
	this->renderer->clear();
	this->renderer->prepare();

	KinematicConfiguration* kinematic = Configuration::getInstance()->getKinematic();
	vector<vec3>* controlPoints = kinematic->controlPoints;
	size_t debugPathCount = kinematic->pathFraction;

	// define current camera
	Camera* currentCamera = kinematic->isFollowing ? pathCamera : freeCamera;

	// create path with boxes
	auto oldPosition = vec3();
	auto distance = 0.0f;
	auto current = 0.01f;

	for (auto i = 0; i < debugPathCount; i++) {
		auto entity = this->debugEntities.at(i);
		auto into = (i / (float)debugPathCount);
		auto position = this->fun->apply(into, *controlPoints);
		entity->setModel(glm::scale(glm::translate(mat4(1), position), vec3(0.2f)));
		this->updateAndRender(entity, currentCamera);

		if (i != 0) {
			auto current_dist = glm::l2Norm(oldPosition - position);
			distance += current_dist;
			if (t > into && t < (into + 0.01f)) {
				// ta aqui
				current = current_dist;
			}
		}
		oldPosition = position;
	}

	// 0.0f to 1.0f interval
	this->updateStep(kinematic->velocity, distance, current);
	this->updatePathCamera(controlPoints);
	this->updateAndRender(this->character, currentCamera);
	
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
}

void Scene::updatePathCamera(vector<vec3>* controlPoints) {
	auto position = this->fun->apply(t, *controlPoints);
	auto tangent = glm::normalize(this->fun->firstDerivative(t, *controlPoints));
	auto normal = glm::normalize(this->fun->secondDerivative(t, *controlPoints));
	auto binormal = glm::cross(tangent, normal);

	pathCamera->lookAt(position, tangent, binormal);
	character->setModel(glm::scale(glm::translate(mat4(1), position), vec3(0.8f)));
}

void Scene::updateAndRender(Entity* entity, Camera* camera) {
	entity->update();
	renderer->render(entity, camera, Configuration::getInstance()->getLight());
}

void Scene::updateStep(float velocity, float distance, float current) {
	auto delta_t = Frametime::getInstance()->getDelta();
	auto step = 0.01f * velocity;
	auto contribution = ((current * 100.f) / distance);
	this->t = last + (step * (1.0f / contribution));
	this->last = this->t;

	if (t > 1.0f) {
		t = 0.0f;
		last = 0.0f;
	}
}

Camera* Scene::getCamera() {
	return freeCamera;
}

vector<Entity*> Scene::getEntities() {
	return this->fixedEntities;
}

