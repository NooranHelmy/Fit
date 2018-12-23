#pragma once
#define GLM_FORCE_CXX11

#include <glm/glm.hpp>

#include "shape.cpp"
#include <scene.hpp>
#include <shader.hpp>
#include <mesh/mesh.hpp>
#include <textures/texture2d.hpp>
#include <framebuffer.hpp>
#include <camera/camera.hpp>
#include <camera/controllers/fly_camera_controller.hpp>
#include <stack> 
#include "../build/include/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

class RendertoTextureScene : public Scene {
private:
	Shader * shader, *grayScaleShader;
	Mesh* plane;
	/* TEST */
	FrameBuffer* fbo;
	Texture2D *fboTex, *fboDepthTex;
	bool useGrayScale;
	Mesh *quad;
	/*  */

	Mesh* shapes[66];
	int modelIDs[8][8];
	shape *shapeInfo[8];
	int noOfShapes;
	Mesh* stack[2];
	int stackSize;
	int posOfStack;
	Texture2D* textures[24];
	Texture2D* backgroundTex;
	int distToTop;
	glm::vec3 topPosition;
	bool spaceClicked; //True if space is clicked
	bool isHit;	      // True if the player win (ha7tgha 3shan a3rf hanzel l model w atl3o tany wla howa kaseb fa ashela)
	bool goUp;        // True if the model went down (it will tell me when to go up)
	int botID;
	int oldBotID;
	float botY;
	int count;
	bool isFirst;
	double shrinkScale;
	float rotationValueBot;
	float rotationValueTop;
	float currentTime;
	Mesh* background;

	Camera* camera;
	FlyCameraController* controller;
	GLuint mvpLoc, texLoc;
	std::vector <std::string> modelFiles;
	irrklang::ISoundEngine* soundEngine;

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
	void updateStack(int,int);
	int getModelIndex(int myID, bool whole);
};
