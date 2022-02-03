#include "config.hpp"

Configuration::Configuration() {
	this->viewport = { 1200.0f, 800.0f };
	this->light = { vec4(5.0f, 10.0f, 2.0f, 1.0f), 0.5f, 0.1f };
	this->openGL = { RenderMode::TRIANGLES };
	this->kinematic = { 100, false, new vector<vec3>(), 1.0f };
};

Configuration* Configuration::getInstance() {
	if (instance == nullptr) {
		instance = new Configuration();
	}
	return instance;
}

Configuration* Configuration::instance = nullptr;

KinematicConfiguration* Configuration::getKinematic() {
	return &this->kinematic;
}

ViewportDimension* Configuration::getViewport() {
	return &this->viewport;
}

LightConfiguration* Configuration::getLight() {
	return &this->light;
}

OpenGLConfiguration* Configuration::getOpenGLConfiguration() {
	return &this->openGL;
}
