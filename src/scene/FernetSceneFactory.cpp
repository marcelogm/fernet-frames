#include "scene.hpp"

Scene* FernetSceneFactory::build() {
	vector<Entity*> fixed = { 
		// floor
		new Entity(
			provider.get("resources/plane.obj"),
			shaders,
			vec4(1.0f, 1.0f, 1.0f, 1.0f),
			glm::translate(mat4(1), vec3(0.0f, -2.0f, 0.0f))
		)
	};

	KinematicConfiguration* config = Configuration::getInstance()->getKinematic();
	vector<Entity*> control = vector<Entity*>();
	config->controlPoints->push_back(vec3(0.f, 0.f, 0.f));
	config->controlPoints->push_back(vec3(1.f, 1.f, 1.f));
	config->controlPoints->push_back(vec3(2.f, -8.f, 2.f));
	config->controlPoints->push_back(vec3(25.f, 25.f, 25.f));
	for (vec3 contrlPoint : *config->controlPoints) {
		control.push_back(getControlPoint());
	}

	return new Scene(
		fixed,
		getDebugPath(),
		control,
		new Camera(vec3(16.0f, 6.0f, -3.0f), vec3(-0.9f, -0.35f, 0.25f), -195, -20),
		new BezierFunction()
	);
}

vector<Entity*> FernetSceneFactory::getDebugPath() {
	auto points = Configuration::getInstance()->getKinematic()->pathFraction;
	auto temp = vector<Entity*>(points);
	for (auto i = 0; i < points; i++) {
		temp.at(i) = new Entity(
			provider.get("resources/box.obj"),
			shaders,
			vec4(1.0f, 0.40f, 0.40f, 1.0f),
			mat4(1)
		);
	}
	return temp;
}

Entity* FernetSceneFactory::getControlPoint() {
	return new Entity(
		provider.get("resources/sphere.obj"),
		shaders,
		vec4(0.f, 0.10f, 0.20f, 1.0f),
		mat4(1)
	);
}

FernetSceneFactory::FernetSceneFactory() {
	provider = ObjectProvider();
	shaders = {
			{ GL_VERTEX_SHADER, "resources/shaders/shader.vert" },
			{ GL_FRAGMENT_SHADER, "resources/shaders/shader.frag" },
			{ GL_NONE, NULL }
	};
}
