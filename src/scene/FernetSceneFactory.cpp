#include "scene.hpp"

Scene* FernetSceneFactory::build() {
	ObjectProvider provider = ObjectProvider();
	vector<ShaderInfo> shaders = {
			{ GL_VERTEX_SHADER, "resources/shaders/shader.vert" },
			{ GL_FRAGMENT_SHADER, "resources/shaders/shader.frag" },
			{ GL_NONE, NULL }
	};
	auto f1 = vec3(-0.1f, -0.1f, 2.f);
	Entity* b1 = getDebugBox(f1, 0.2, shaders);
	Entity* floor = new Entity(
		provider.get("resources/plane.obj"),
		shaders,
		vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::translate(mat4(1), vec3(0.0f, -2.0f, 0.0f))
	);
	vector<Entity*> entities = { floor, b1 };
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
		glm::scale(glm::translate(mat4(1), position), vec3(scale))
	);
}
