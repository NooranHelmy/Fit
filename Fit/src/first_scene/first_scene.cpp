#include "first_scene.h"

#include <mesh/mesh_utils.hpp>
#include <textures/texture_utils.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <application.hpp>
#include <experimental/filesystem>
namespace filesys = std::experimental::filesystem;
#include <random>

const int WIDTH = 400;
const int HEIGHT = 600;

void RendertoTextureScene::Initialize()
{
	shader = new Shader();
	shader->attach("../assets/shaders/texture.vert", GL_VERTEX_SHADER);
	shader->attach("../assets/shaders/texture.frag", GL_FRAGMENT_SHADER);
	shader->link();

	mLoc = glGetUniformLocation(shader->getID(), "M");
	mitLoc = glGetUniformLocation(shader->getID(), "M_it");
	vpLoc = glGetUniformLocation(shader->getID(), "VP");
	camPosLoc = glGetUniformLocation(shader->getID(), "cam_pos");

	materialVars.diffuse = glGetUniformLocation(shader->getID(), "material.diffuse");
	materialVars.specular = glGetUniformLocation(shader->getID(), "material.specular");
	materialVars.ambient = glGetUniformLocation(shader->getID(), "material.ambient");
	materialVars.shininess = glGetUniformLocation(shader->getID(), "material.shininess");

	lightVars.diffuse = glGetUniformLocation(shader->getID(), "light.diffuse");
	lightVars.specular = glGetUniformLocation(shader->getID(), "light.specular");
	lightVars.ambient = glGetUniformLocation(shader->getID(), "light.ambient");
	lightVars.direction = glGetUniformLocation(shader->getID(), "light.direction");
	
	mvpLoc = glGetUniformLocation(shader->getID(), "MVP");
	texLoc = glGetUniformLocation(shader->getID(), "tex");

	textures[0] = TextureUtils::Load2DTextureFromFile("../assets/textures/mntn_white_d.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	noOfShapes = 16;
	stackSize = 3;
	posOfStack = 0;
	distToTop = 4;
	topPosition = {5.0f,distToTop,0.0f};

	plane = MeshUtils::Plane({ 0,0 }, { 10,10 });
	modelFiles = getAllFilesinDir("../assets/models/");

	

	for (int i = 0; i < noOfShapes; i++)
	{
		shapes[i] = MeshUtils::LoadObj(modelFiles[i].c_str());
		std::cout<< modelFiles[i] <<std::endl;
	}

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, noOfShapes - 1); // define the range
	int indexRand;
	
	stack[0] = MeshUtils::LoadObj("../assets/models/Seg_3.obj");
	//initalizeStack
	for (int i = 1; i < stackSize; i++)
	{
		indexRand = distr(eng);
		stack[i] = shapes[i]; 
		std::cout << indexRand << std::endl;
	}
	
	camera = new Camera();
	glm::ivec2 windowSize = getApplication()->getWindowSize();
	camera->setupPerspective(glm::pi<float>() / 2, (float)windowSize.x / windowSize.y, 0.1f, 1000.0f);
	camera->setUp({ 0, 1, 0 });

	controller = new FlyCameraController(this, camera);
	controller->setYaw(-glm::half_pi<float>());
	controller->setPitch(-glm::quarter_pi<float>());
	controller->setPosition({ 20, 10, 50 });
	   		
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void RendertoTextureScene::Update(double delta_time)
{
	controller->update(delta_time);

	Keyboard* kb = getKeyboard();
	
	if (kb->isPressed(GLFW_KEY_SPACE)) 
	{
		//function Nourhan ely bt2ol 7asal collision wala la2 - TODO
		if (true) //collision has occured
		{
			while (topPosition.y >= 2)
			{
				topPosition.y -= (float)delta_time;
			}
			while (topPosition.y <= distToTop)
			{
				topPosition.y += (float)delta_time;
			}
		}
		else
		{
			//add ely fo2 ma3 ely ba3do 
			//hanrsm ely msh ma5room fo2 PLUS mets8r
			//stack.pop();
			//hand5l wa7ed gdeed
		}
	}
	
	if (lightPitch < -glm::half_pi<float>()) lightPitch = -glm::half_pi<float>();
	if (lightPitch > glm::half_pi<float>()) lightPitch = glm::half_pi<float>();
	lightYaw = glm::wrapAngle(lightYaw);
}

void RendertoTextureScene::Draw()
{
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 VP = camera->getVPMatrix();

	glm::vec3 cam_pos = camera->getPosition();
	glm::vec3 light_dir = -(glm::vec3(glm::cos(lightYaw), 0, glm::sin(lightYaw)) * glm::cos(lightPitch) + glm::vec3(0, glm::sin(lightPitch), 0));

	shader->use();
	float scaleFactor = 4.0;
	float upwardsValue = 1.0;
	float rotationValue;
	glm::mat4 trans;
	glm::vec3 translateToCenter; 
	glm::vec3 scaleVector;
	glm::vec3 upwardsVector;

	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(VP));
	glUniform3f(camPosLoc, cam_pos.x, cam_pos.y, cam_pos.z);
	glUniform3f(lightVars.diffuse, 0.9f, 0.8f, 0.6f);
	glUniform3f(lightVars.specular, 0.9f, 0.8f, 0.6f);
	glUniform3f(lightVars.ambient, 0.88f, 0.68f, 0.15f);
	glUniform3f(lightVars.direction, light_dir.x, light_dir.y, light_dir.z);

	glUniform3f(materialVars.diffuse, 0.7f, 0.2f, 0.1f);
	glUniform3f(materialVars.specular, 0.2f, 0.2f, 0.2f);
	glUniform3f(materialVars.ambient, 0.7f, 0.2f, 0.1f);
	glUniform1f(materialVars.shininess, 50);

	for (int i = 0; i < stackSize; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		textures[0]->bind();

		if (i == 0) //Last item in stack 
		{
			upwardsVector = glm::vec3(0.0f, 0.0f, 0.0f);
			translateToCenter = topPosition;
			scaleVector = glm::vec3(1.5f, 2.0f, 1.5f);
			rotationValue = -(float)glfwGetTime();
		}
		else
		{
			upwardsVector = glm::vec3(0.0f, upwardsValue, 0.0f);
		    translateToCenter = glm::vec3(5.0f, 0.1f, 0.0f); //0.1 y
		//	scaleFactor /= 1.5;
			scaleVector = glm::vec3(scaleFactor, 2.0f, scaleFactor);
			rotationValue = (float)glfwGetTime();
		}

		//if (i == 1) //Item before last in stack
		//{
		//	
		//}

		trans = glm::translate(glm::mat4(), translateToCenter);
		trans = glm::translate(trans, upwardsVector);
		trans = glm::rotate(trans, rotationValue, glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::scale(trans, scaleVector);

		glm::mat4 model_mat = trans;
		glm::mat4 model_mat_it = glm::transpose(glm::inverse(model_mat));
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(VP * trans));
		glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model_mat));
		glUniformMatrix4fv(mitLoc, 1, GL_FALSE, glm::value_ptr(model_mat_it));
		float specular = i / 10.0f;
		glUniform3f(materialVars.specular, specular, specular, specular);
		glUniform1f(materialVars.shininess, i * 5.0f + 1.0f);
		stack[i]->draw();
		
		upwardsValue += 1; //0.5

	}
}

void RendertoTextureScene::Finalize()
{
	delete controller;
	delete camera;

	delete plane;
	delete shader;
}	

std::vector<std::string> RendertoTextureScene::getAllFilesinDir(const std::string &dirPath, const std::vector<std::string> dirSkipList )
{
	std::vector<std::string> listOfFiles;
	try
	{
		if (filesys::exists(dirPath) && filesys::is_directory(dirPath))
		{
			filesys::recursive_directory_iterator iter(dirPath);
			filesys::recursive_directory_iterator end;
			while (iter != end)
			{
				if (filesys::is_directory(iter->path()) && (std::find(dirSkipList.begin(), dirSkipList.end(), iter->path().filename()) != dirSkipList.end()))
				{
					iter.disable_recursion_pending();
				}
				else
				{
					listOfFiles.push_back(iter->path().string());
				}
				std::error_code ec;
				iter.increment(ec);
				if (ec)
				{
					std::cerr << "Exception :: " << iter->path().string() << " :: " << ec.message() << "\n";
				}
			}

		}
	}
	catch (std::system_error &e)
	{
		std::cerr << "Exception :: " << e.what();
	}
	return listOfFiles;
}
