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

//	for (std::size_t i = 0; i < this->model->numMeshes(); ++i) {
//		auto& mesh = this->model->getMesh(i);
//		std::cout << mesh.description() << std::endl;
//	}
	std::cout << "start Rendering Game" << std::endl;

    DirLight d;
    d.ambient = glm::vec3(0.2, 0.0, 0.0);
    d.diffuse = glm::vec3(0.0);
    d.specular = glm::vec3(0.0);
    d.direction = glm::vec3(10.0, 0, 0);
    this->engine->getLightManager()->AddDirectionLight(d);

    // Create a single point light
    PointLight p;
    p.position = glm::vec3(1.2f, 1.0f, 2.0f);
    p.ambient = glm::vec3(0.1f);
    p.diffuse = glm::vec3(0.8f);
    p.specular = glm::vec3(1.0f);
    p.constant = 1.0f;
    p.linear = 0.09f;
    p.quadratic = 0.032f;
    this->engine->getLightManager()->AddPointLight(p);


    FlashLight f;
    f.position = glm::vec3(1.2f, 1.0f, 2.0f);
    f.ambient = glm::vec3(0.8f/*0.1f*/);
    f.diffuse = glm::vec3(0.9f);
    f.specular = glm::vec3(1.0f);
    f.constant = 1.0f;
    f.linear = 0.09f;
    f.quadratic = 0.032f;
    f.position = this->camera.Position;
    f.direction = this->camera.Front;
    f.cutOff = glm::cos(glm::radians(12.5f));
    f.outerCutOff = glm::cos(glm::radians(17.5));

    this->engine->getLightManager()->AddSpotLight(f);

    this->model->Init(this->engine);
}

void Loading::Update(const double& dt) noexcept {
	const glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(this->window_size.WIDTH) / static_cast<float>(this->window_size.HEIGHT), 0.1f, 100.0f);
	const glm::mat4 view = camera.GetViewMatrix();
	this->engine->get3DRenderer()->setProjectionMatrix(projection);
	this->engine->get3DRenderer()->setViewMatrix(view);
    this->engine->get3DRenderer()->setCameraPosition(this->camera.Position);

	// TODO: only when we move the camera.
	model->UpdatePerspective(this->engine);

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
