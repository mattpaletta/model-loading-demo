#include "loading/loading.hpp"

// Helper Function
void run_demo(const std::string& model_file, const std::string& model_name) {
	const std::string local_dir = "../../";
	const ScreenSize size { 800, 600 };

	const auto game_inst = std::make_shared<Loading>(size, local_dir + model_file, model_name);
	Engine e{ game_inst };

	e.run();
}

// CLASS START
//////////////
Loading::Loading(const ScreenSize& size, const std::string& _model, const std::string& model_name) : Game(size, "Loading Demo: " + model_name), lastX(size.WIDTH / 2.0f), lastY(size.HEIGHT / 2.0f), model_file(_model) {
    // Offset cameras for different models
    if (model_name == "nanosuit") {
        this->camera = glm::vec3(0.0f, 8.0f, 20.0f);
    } else if (model_name == "backpack") {
        this->camera = glm::vec3(0.0f, 0.8f, 8.0f);
    } else if (model_name == "audi") {
        this->camera = glm::vec3(0.0f, 1.5f, 10.0f);
    } else {
        std::cout << "ERROR::Loader: Unknown model name." << std::endl;
        std::terminate();
    }
}
Loading::~Loading() {}

void Loading::Init() {
	std::cout << "Initializing Game" << std::endl;
	this->model = std::make_unique<Model>(this->engine, this->model_file);
	for (std::size_t i = 0; i < this->model->numMeshes(); ++i) {
		auto& mesh = this->model->getMesh(i);
		mesh.autoCreateShader();
		std::cout << mesh.description() << std::endl;
	}
	this->model->Init();
	std::cout << "start Rendering Game" << std::endl;
}

void Loading::Update(const double& dt) noexcept {
	const glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(this->window_size.WIDTH) / static_cast<float>(this->window_size.HEIGHT), 0.1f, 100.0f);
	const glm::mat4 view = camera.GetViewMatrix();
	this->engine->get3DRenderer()->setProjectionMatrix(projection);
	this->engine->get3DRenderer()->setViewMatrix(view);
    this->engine->get3DRenderer()->setCameraPosition(this->camera.Position);

	// TODO: only when we move the camera.
	model->UpdatePerspective(this->engine->get3DRenderer());

    const float rotation_speed = 9.0;
    this->rotation += (dt * rotation_speed);
}

void Loading::Render() const noexcept {
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    model = glm::rotate(model, glm::radians(this->rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	this->model->Draw(model);
}
