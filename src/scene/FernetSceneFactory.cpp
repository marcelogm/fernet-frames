#include "scene.hpp"

Scene* FernetSceneFactory::build() {
	ObjectProvider provider = ObjectProvider();
	vector<ShaderInfo> shaders = {
			{ GL_VERTEX_SHADER, "resources/shaders/shader.vert" },
			{ GL_FRAGMENT_SHADER, "resources/shaders/shader.frag" },
			{ GL_NONE, NULL }
	};
	Configuration::getInstance()->getSimulationParams()->windModifier = 0.00;
	auto f1 = vec3(-0.1f, -0.1f, 2.f);
	auto f2 = vec3(-0.1f,  0.1f, 2.f);
	auto f3 = vec3( 0.1f, -0.1f, 2.f);
	auto f4 = vec3( 0.1f,  0.1f, 2.f);
	Entity* b1 = getDebugBox(f1, 0.2, shaders);
	Entity* b2 = getDebugBox(f2, 0.2, shaders);
	Entity* b3 = getDebugBox(f3, 0.2, shaders);
	Entity* b4 = getDebugBox(f4, 0.2, shaders);
	Entity* bar = new Entity(
		provider.get("resources/bar.obj"),
		shaders,
		vec4(0.39f, 0.83f, 0.71f, 1.0f),
		mat4(1),
		true,
		1.0f,
		false
	);
	Entity* floor = new Entity(
		provider.get("resources/plane.obj"),
		shaders,
		vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::translate(mat4(1), vec3(0.0f, -2.0f, 0.0f)),
		false,
		0.0f,
		true
	);
	vector<Entity*> entities = { floor, bar, b1, b2, b3, b4 };
	Scene* scene = new Scene(
		entities,
		new Camera(vec3(3.9f, 0.0f, 0.0f), vec3(-1.f, 0.0f, 8.7), -180, 0)
	);
	return scene;
}

Entity* FernetSceneFactory::getDebugBox(vec3 position, float scale, vector<ShaderInfo> shaders) {
	ObjectProvider provider = ObjectProvider();
	return new Entity(
		provider.get("resources/box.obj"),
		shaders,
		vec4(0.f, 0.10f, 0.20f, 1.0f),
		glm::scale(glm::translate(mat4(1), position), vec3(scale)),
		false,
		1.0f,
		false
	);
}
