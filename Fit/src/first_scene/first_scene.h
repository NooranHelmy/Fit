#pragma once
#define GLM_FORCE_CXX11

#include <glm/glm.hpp>

#include <scene.hpp>
#include <shader.hpp>
#include <mesh/mesh.hpp>
#include <textures/texture2d.hpp>
#include <framebuffer.hpp>
#include <camera/camera.hpp>
#include <camera/controllers/fly_camera_controller.hpp>
#include <stack> 

class RendertoTextureScene : public Scene {
private:
	Shader * shader;
	Mesh* plane;

	Mesh* shapes[16];
	int noOfShapes;
	Mesh* stack[3];
	int stackSize;
	int posOfStack;
	Texture2D* textures[24];
	int distToTop;
	glm::vec3 topPosition;

	Camera* camera;
	FlyCameraController* controller;
	GLuint mvpLoc, texLoc;
	std::vector <std::string> modelFiles;

	float lightYaw, lightPitch;
	GLuint mLoc, mitLoc, vpLoc, camPosLoc;
	struct {
		GLuint diffuse, specular, ambient, shininess;
	} materialVars;
	struct {
		GLuint diffuse, specular, ambient, direction;
	} lightVars;

	glm::mat4 frameVPMatrix;
	bool frameVPInitialized = false;

public:
	RendertoTextureScene(Application* app) : Scene(app) {}

	void Initialize() override;
	void Update(double delta_time) override;
	void Draw() override;
	void Finalize() override;
	std::vector<std::string> getAllFilesinDir(const std::string &dirPath, const std::vector<std::string> dirSkipList = {});
};
