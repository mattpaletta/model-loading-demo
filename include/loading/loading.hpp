#pragma once

#include <engine/engine.hpp>
#include <engine/model.hpp>
#include "loading/camera.hpp"

class Loading final : public Game {
private:
	Shader shader;
	std::unique_ptr<Model> model = nullptr;
	Camera camera;
	std::string model_file;

	// Camera positions
	float lastX;
	float lastY;

    float rotation = 0.0f;

public:
	Loading(const ScreenSize& size, const std::string& _model, const std::string& model_name);
	~Loading();

	void Init() override;

	void Update(const double& dt) noexcept override;

	void Render() const noexcept override;
};

void run_demo(const std::string& model_file, const std::string& model_name);
