#include "scene.hpp"
#include "scene.hpp"
#include "scene.hpp"
#include "scene.hpp"

Scene* FernetSceneFactory::build() {
	ObjectProvider provider = ObjectProvider();
	vector<ShaderInfo> shaders = {
			{ GL_VERTEX_SHADER, "resources/shaders/shader.vert" },
			{ GL_FRAGMENT_SHADER, "resources/shaders/shader.frag" },
			{ GL_NONE, NULL }
	};
	Entity* floor = new Entity(
		provider.get("resources/plane.obj"),
		shaders,
		vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::translate(mat4(1), vec3(0.0f, -2.0f, 0.0f))
	);
	vector<Entity*> fixed = { floor };

	vector<Entity*> control = {};
	KinematicConfiguration* config = Configuration::getInstance()->getKinematic();
	config->controlPoints->push_back(vec3(-5.f, 1.f, -5.f));
	config->controlPoints->push_back(vec3(3.f, 1.f, 0.f));
	config->controlPoints->push_back(vec3(-5.f, 1.f, 5.f));
	config->controlPoints->push_back(vec3(4.f, 1.f, 8.f));
	for (vec3 contrlPoint : *config->controlPoints) {
		control.push_back(getControlPoint(shaders));
	}

	Scene* scene = new Scene(
		fixed,
		getDebug(shaders),
		control,
		new Camera(vec3(16.0f, 6.0f, -3.0f), vec3(-0.9f, -0.35f, 0.25f), -195, -20),
		new BezierFunction()
	);
	return scene;
}

vector<Entity*> FernetSceneFactory::getDebug(vector<ShaderInfo> shaders) {
	auto provider = ObjectProvider();
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

Entity* FernetSceneFactory::getControlPoint(vector<ShaderInfo> shaders) {
	ObjectProvider provider = ObjectProvider();
	return new Entity(
		provider.get("resources/sphere.obj"),
		shaders,
		vec4(0.f, 0.10f, 0.20f, 1.0f),
		mat4(1)
	);
}
