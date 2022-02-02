#include "scene.hpp"

Scene* FernetSceneFactory::build() {
	ObjectProvider provider = ObjectProvider();
	vector<ShaderInfo> shaders = {
			{ GL_VERTEX_SHADER, "resources/shaders/shader.vert" },
			{ GL_FRAGMENT_SHADER, "resources/shaders/shader.frag" },
			{ GL_NONE, NULL }
	};

	auto p1 = vec3(-5.f, 1.f, -5.f);
	auto p2 = vec3( 3.f, 1.f,  0.f);
	auto p3 = vec3(-5.f, 1.f,  5.f);
	auto p4 = vec3( 4.f, 1.f,  8.f);
	Entity* c1 = getControlPoint(p1, 0.4f, shaders);
	Entity* c2 = getControlPoint(p2, 0.4f, shaders);
	Entity* c3 = getControlPoint(p3, 0.4f, shaders);
	Entity* c4 = getControlPoint(p4, 0.4f, shaders);
	Entity* floor = new Entity(
		provider.get("resources/plane.obj"),
		shaders,
		vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::translate(mat4(1), vec3(0.0f, -2.0f, 0.0f))
	);
	vector<Entity*> entities = { floor, c1, c2, c3, c4 };

	// push path debugging
	auto path = this->getPath(p1, p2, p3, p4, shaders);
	entities.insert(entities.end(), path.begin(), path.end());

	Scene* scene = new Scene(
		entities,
		new Camera(vec3(16.0f, 6.0f, -3.0f), vec3(-0.9f, -0.35f, 0.25f), -195, -20),
		{ p1, p2, p3, p4 },
		new BezierFunction()
	);
	return scene;
}

vector<Entity*> FernetSceneFactory::getPath(vec3 a, vec3 b, vec3 c, vec3 d, vector<ShaderInfo> shaders) {
	ObjectProvider provider = ObjectProvider();
	size_t points = 100;
	auto temp = vector<Entity*>(points);
	for (auto i = 0; i < points; i++) {
		auto t = (float)i / points;
		auto position = BezierFunction().apply(t, { a, b, c, d });
		temp.at(i) = new Entity(
			provider.get("resources/box.obj"),
			shaders,
			vec4(1.0f, 0.40f, 0.40f, 1.0f),
			glm::scale(glm::translate(mat4(1), position), vec3(0.2f))
		);
	}
	return temp;
}

Entity* FernetSceneFactory::getControlPoint(vec3 position, float scale, vector<ShaderInfo> shaders) {
	ObjectProvider provider = ObjectProvider();
	return new Entity(
		provider.get("resources/sphere.obj"),
		shaders,
		vec4(0.f, 0.10f, 0.20f, 1.0f),
		glm::scale(glm::translate(mat4(1), position), vec3(scale))
	);
}
