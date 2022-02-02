#include "gui.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

void GuiRenderer::render() {
	auto const config = Configuration::getInstance();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Camera", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Checkbox("Following the path", &config->getKinematic()->isFollowing);
	ImGui::End();

	auto controlPoints = config->getKinematic()->controlPoints;
	for (auto i = 0; i < controlPoints->size(); i++) {
		ImGui::SliderFloat3(("Control Point " + std::to_string(i)).c_str(), value_ptr(controlPoints->at(i)), -20, 20, "%.0f", ImGuiSliderFlags_None);
	}

	ImGui::Begin("Light", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat3("Position", value_ptr(config->getLight()->lightPosition), -20, 20, "%.0f", ImGuiSliderFlags_None);
	ImGui::SliderFloat("Ambient Strength", &config->getLight()->ambientLightStrength, 0, 1, "%.2f", ImGuiSliderFlags_None);
	ImGui::SliderFloat("Difffuse Strength", &config->getLight()->diffuseLightStrength, 0, 1, "%.2f", ImGuiSliderFlags_None);
	ImGui::End();

	ImGui::Begin("Render mode", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::RadioButton("Triangulos", (int *)&config->getOpenGLConfiguration()->mode, 0);
	ImGui::RadioButton("Lines", (int*)&config->getOpenGLConfiguration()->mode, 1);
	ImGui::RadioButton("Points", (int*)&config->getOpenGLConfiguration()->mode, 2);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GuiRenderer::GuiRenderer(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
	ImGui::StyleColorsDark();
}
