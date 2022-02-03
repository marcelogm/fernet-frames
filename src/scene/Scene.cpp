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
	this->currentStep = 0.f;
	this->lastStep = 0.f;
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

	auto oldPosition = vec3();
	auto functionPathNorm = 0.0f;
	auto stepNorm = 0.01f;

	for (auto i = 0; i < debugPathCount; i++) {
		auto entity = this->debugEntities.at(i);
		auto step = (i / (float)debugPathCount);
		auto position = this->fun->apply(step, *controlPoints);

		// create path with boxes
		entity->setModel(glm::scale(glm::translate(mat4(1), position), vec3(0.2f)));
		this->updateAndRender(entity, currentCamera);

		// calculate curve distance
		if (i != 0) {
			auto currentStepNorm = glm::l2Norm(oldPosition - position);
			functionPathNorm += currentStepNorm;

			// current step distance
			if (currentStep > step && currentStep < (step + (1.f / (float)debugPathCount))) {
				stepNorm = currentStepNorm;
			}
		}
		oldPosition = position;
	}

	// 0.0f to 1.0f interval
	this->updateStep(kinematic->velocity, functionPathNorm, stepNorm);
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
	auto position = this->fun->apply(currentStep, *controlPoints);
	auto tangent = glm::normalize(this->fun->firstDerivative(currentStep, *controlPoints));
	auto normal = glm::normalize(this->fun->secondDerivative(currentStep, *controlPoints));
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
	auto step = 0.01f * velocity * delta_t;

	auto contribution = ((current * 100.f) / distance);
	this->currentStep = lastStep + (step * (1.0f / contribution));
	this->lastStep = this->currentStep;

	if (currentStep > 1.0f) {
		currentStep = 0.0f;
		lastStep = 0.0f;
	}
}

Camera* Scene::getCamera() {
	return freeCamera;
}

vector<Entity*> Scene::getEntities() {
	return this->fixedEntities;
}

