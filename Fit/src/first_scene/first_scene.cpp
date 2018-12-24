#pragma once
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
	soundEngine = irrklang::createIrrKlangDevice();
	if (!soundEngine)
		std::cout << "Error: Sound Engine could not be created" << endl;

	soundEngine->play2D("CrashBandicoot.mp3");

	shader = new Shader();
	shader->attach("../assets/shaders/texture.vert", GL_VERTEX_SHADER);
	shader->attach("../assets/shaders/texture.frag", GL_FRAGMENT_SHADER);
	shader->link();

	grayScaleShader = new Shader();
	grayScaleShader->attach("../assets/shaders/grayscale.vert", GL_VERTEX_SHADER);
	grayScaleShader->attach("../assets/shaders/grayscale.frag", GL_FRAGMENT_SHADER);
	grayScaleShader->link();

	spaceClicked = false;
	goUp = false;

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

	textures[0] = TextureUtils::Load2DTextureFromFile("../assets/textures/blue.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	backgroundTex = TextureUtils::Load2DTextureFromFile("../assets/textures/dots.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	noOfShapes = 66;
	stackSize = 2;
	posOfStack = 0;
	distToTop = 4;
	topPosition = { 5.0f,distToTop,0.0f };
	botY = 0.0f;
	botID = 1;
	count = 0;
	shrinkScale = 0.3f;



	modelFiles = getAllFilesinDir("../assets/models/");

	modelIDs[0][0] = 7;
	modelIDs[0][1] = 1;
	modelIDs[0][2] = 3;
	modelIDs[0][3] = 4;
	modelIDs[0][4] = 5;
	modelIDs[0][5] = 6;
	modelIDs[0][6] = 7;
	modelIDs[0][7] = 8;

	modelIDs[1][0] = 7;
	modelIDs[1][1] = 2;
	modelIDs[1][2] = 3;
	modelIDs[1][3] = 4;
	modelIDs[1][4] = 5;
	modelIDs[1][5] = 6;
	modelIDs[1][6] = 7;
	modelIDs[1][7] = 8;

	modelIDs[2][0] = 7;
	modelIDs[2][1] = 1;
	modelIDs[2][2] = 3;
	modelIDs[2][3] = 4;
	modelIDs[2][4] = 5;
	modelIDs[2][5] = 6;
	modelIDs[2][6] = 7;
	modelIDs[2][7] = 8;



	modelIDs[3][0] = 7;
	modelIDs[3][1] = 1;
	modelIDs[3][2] = 3;
	modelIDs[3][3] = 4;
	modelIDs[3][4] = 5;
	modelIDs[3][5] = 6;
	modelIDs[3][6] = 7;
	modelIDs[3][7] = 8;

	modelIDs[4][0] = 8;
	modelIDs[5][0] = 8;
	modelIDs[6][0] = 8;

	modelIDs[7][0] = 6;
	modelIDs[7][1] = 3;
	modelIDs[7][2] = 4;
	modelIDs[7][3] = 5;
	modelIDs[7][4] = 6;
	modelIDs[7][5] = 7;
	modelIDs[7][6] = 8;


	shapeInfo[0] = new shape(1, 2, "A");
	shapeInfo[1] = new shape(2, 3, "B");
	shapeInfo[2] = new shape(3, 4, "C");
	shapeInfo[3] = new shape(4, 4, "D");
	shapeInfo[4] = new shape(5, 5, "E");
	shapeInfo[5] = new shape(6, 5, "F");
	shapeInfo[6] = new shape(7, 5, "G");
	shapeInfo[7] = new shape(8, 6, "H");

	for (int i = 0; i < noOfShapes; i++)
	{
		shapes[i] = MeshUtils::LoadObj(modelFiles[i].c_str());
		std::cout << modelFiles[i] << std::endl;
	}

	isFirst = true;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, noOfShapes - 1); // define the range
	int indexRand;

	//initalizeStack
	stack[0] = shapes[botID - 1];
	//stack[1] = shapes[1];

	int indexModel = getModelIndex(botID, false);
	stack[1] = shapes[indexModel];
	//std::cout << "Index Model: " << indexModel << std::endl;

	camera = new Camera();
	glm::ivec2 windowSize = getApplication()->getWindowSize();
	camera->setupPerspective(glm::pi<float>() / 2, (float)windowSize.x / windowSize.y, 0.1f, 100.0f);
	camera->setUp({ 0, 1, 0 });
	camera->setPosition({ 1.33088f,6.5f, 0.0f }); //   2.0f,5.0f, 0.0f 
	camera->setTarget({ 5.0f, 2.5f, 0.0f });

	/* TEST */
	// Create our additional frame buffer
	fbo = new FrameBuffer();

	unsigned int width = getApplication()->getWindowSize().x;
	unsigned int height = getApplication()->getWindowSize().y;

	// Create our render target
	fboTex = new Texture2D();

	fboTex->bind();

	fboTex->setup(GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	fbo->attach(fboTex, GL_COLOR_ATTACHMENT0);

	fboDepthTex = new Texture2D();
	fboDepthTex->bind();
	fboDepthTex->setup(GL_DEPTH24_STENCIL8, width, height, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	fbo->attach(fboDepthTex, GL_DEPTH_STENCIL_ATTACHMENT);

	if (fbo->isComplete())
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	useGrayScale = true;

	grayScaleShader->set("factors1", { 0.393,0.769,0.189 });
	grayScaleShader->set("factors2", { 0.349,0.686,0.168 });
	grayScaleShader->set("factors3", { 0.272,0.534,0.131 });
	// Create quad to render fboTex on
	quad = new Mesh();
	quad->setup<Vertex>({
		{ { -1, -1,  0 },{ 1, 1, 1, 1 },{ 0, 0 },{ 0,0,1 } },
		{ { 1, -1,  0 },{ 1, 1, 1, 1 },{ 1, 0 },{ 0,0,1 } },
		{ { 1,  1,  0 },{ 1, 1, 1, 1 },{ 1, 1 },{ 0,0,1 } },
		{ { -1,  1,  0 },{ 1, 1, 1, 1 },{ 0, 1 },{ 0,0,1 } },
		}, {
			0, 1, 2, 2, 3, 0
		});


	background = MeshUtils::Plane({ 0,0 }, { 5,5 });
	/*  */

	lightYaw = lightPitch = glm::quarter_pi<float>();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
}

void RendertoTextureScene::Update(double delta_time)
{
	if (!soundEngine->isCurrentlyPlaying("CrashBandicoot.mp3"))
		soundEngine->play2D("CrashBandicoot.mp3");

	Keyboard* kb = getKeyboard();

	if (kb->justPressed(GLFW_KEY_SPACE) && !goUp)
	{
		spaceClicked = true;
		float shapeAngle = (int)glm::degrees(rotationValueTop) % 360;
		shapeAngle += 360;
		float holeAngle = (int)glm::degrees(rotationValueBot) % 360;
		isHit = shapeInfo[oldBotID - 1]->onClick(shapeAngle, holeAngle);
		if (!isHit)
		{
			soundEngine->play2D("click1.mp3");
			currentTime = glfwGetTime();
			useGrayScale = true;
		}
		else
		{
			soundEngine->play2D("bell.wav");
			useGrayScale = false;
		}

		std::cout << isHit << " " << shapeAngle << ",{}" << holeAngle << std::endl;
	}


	if (lightPitch < -glm::half_pi<float>()) lightPitch = -glm::half_pi<float>();
	if (lightPitch > glm::half_pi<float>()) lightPitch = glm::half_pi<float>();
	lightYaw = glm::wrapAngle(lightYaw);

	grayScaleShader->set("factors1", { 0.393,0.769,0.189 });
	grayScaleShader->set("factors2", { 0.349,0.686,0.168 });
	grayScaleShader->set("factors3", { 0.272,0.534,0.131 });
}

void RendertoTextureScene::Draw()
{
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	if (useGrayScale) fbo->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 VP = camera->getVPMatrix();

	glm::vec3 cam_pos = camera->getPosition();
	glm::vec3 light_dir = -(glm::vec3(glm::cos(lightYaw), 0, glm::sin(lightYaw)) * glm::cos(lightPitch) + glm::vec3(0, glm::sin(lightPitch), 0));

	shader->use();
	float scaleFactor = 6.0;
	float upwardsValue = 2.0;
	float xValue = 0;
	float rotationValue;
	float rotationValueDegrees;
	glm::mat4 trans;
	glm::vec3 translateToCenter;
	glm::vec3 scaleVector;
	glm::vec3 upwardsVector;

	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(VP));
	glUniform3f(camPosLoc, cam_pos.x, cam_pos.y, cam_pos.z);
	glUniform3f(lightVars.diffuse, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightVars.specular, 0.8f, 0.8f, 0.8f);
	glUniform3f(lightVars.ambient, 0.68f, 0.68f, 0.68f);
	//glUniform3f(lightVars.diffuse, 0.9f, 0.8f, 0.6f);
	//glUniform3f(lightVars.specular, 0.9f, 0.8f, 0.6f);
	//glUniform3f(lightVars.ambient, 0.88f, 0.68f, 0.15f);
	glUniform3f(lightVars.direction, light_dir.x, light_dir.y, light_dir.z);

	glUniform3f(materialVars.diffuse, 0.9f, 0.9f, 0.9f);
	glUniform3f(materialVars.specular, 0.2f, 0.2f, 0.2f);
	glUniform3f(materialVars.ambient, 0.5f, 0.5f, 0.5f);
	glUniform1f(materialVars.shininess, 50);

	//glUniform3f(materialVars.diffuse, 0.7f, 0.2f, 0.1f);
	//glUniform3f(materialVars.ambient, 0.7f, 0.2f, 0.1f);
	//float specular = 5 / 10.0f;
	//glUniform3f(materialVars.specular, specular, specular, specular);
	//glUniform1f(materialVars.shininess, 3 * 5.0f + 1.0f);

	backgroundTex->bind();
	//1.33088f, 6.5f, 0.0f
	//30
	//* glm::rotate(glm::mat4(1), 1.57f, glm::vec3(1.0f, 0.0f, .0f))
	//* glm::rotate(glm::mat4(1), 1.57f, glm::vec3(0.0f, 0.0f, 1.0f))

	//glm::mat4 background_mat = glm::translate(glm::mat4(), { 30.0f, 6.5f, 0.0f })
	//	* glm::rotate(glm::mat4(1), 1.57f, glm::vec3(1.0f, 0.0f, .0f))
	//	* glm::rotate(glm::mat4(1), -0.7f, glm::vec3(0.0f, 1.0f, .0f))
	//	* glm::rotate(glm::mat4(1), 1.57f, glm::vec3(0.0f, 0.0f, 1.0f))
	//	* glm::scale(glm::mat4(1), { 50.0, 50.0, 50.0 });
	//glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(VP * background_mat));
	//background->draw();
	//glm::mat4 background_mat = glm::translate(glm::mat4(), { 30.0f, 6.5f, 0.0f })
	//	* glm::rotate(glm::mat4(1), 1.57f, glm::vec3(1.0f, 0.0f, .0f))
	//	* glm::rotate(glm::mat4(1), -0.7f, glm::vec3(0.0f, 1.0f, .0f))
	//	* glm::rotate(glm::mat4(1), 1.57f, glm::vec3(0.0f, 0.0f, 1.0f))
	//	* glm::scale(glm::mat4(1), { 50.0, 50.0, 50.0 });
	//glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(VP * background_mat));
	//background->draw();

	for (int i = 0; i < stackSize; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		textures[0]->bind();

		if (i == 0) //first item in stack 
		{
			//	upwardsVector = glm::vec3(0.0f, 0.5f, 0.0f);
			translateToCenter = topPosition;
			scaleVector = glm::vec3(shrinkScale * scaleFactor, shrinkScale * 3.0f, shrinkScale * scaleFactor);//0.3f * scaleFactor, 0.3f * 3.0f, 0.3f * scaleFactor
			rotationValue = -(float)glfwGetTime();
			rotationValueDegrees = glm::degrees(rotationValue);
			rotationValueTop = rotationValue;
			int temp = glm::degrees(rotationValue);
			//std::cout <<"rotation value:" << temp%360 << std::endl;
		}
		else
		{
			//	upwardsVector = glm::vec3(xValue, upwardsValue, 0.0f);
			translateToCenter = glm::vec3(5.0f, botY, 0.0f); //0.1 y
		//	scaleFactor /= 1.5;
			scaleVector = glm::vec3(scaleFactor, 3.0f, scaleFactor);
			rotationValue = 0.5 * (float)glfwGetTime();
			rotationValueBot = glm::degrees(rotationValue);
		}

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
		stack[i]->draw();

		upwardsValue -= 2.2; //0.5
		xValue += 1;

	}
	if (spaceClicked)
	{
		if (isHit)
		{
			if (topPosition.y >= 1.3)
				topPosition.y -= 0.01;
			else
			{
				spaceClicked = false;
				isHit = false;
				goUp = true;

				//updateStack(getModelIndex(4)); //TO BE CHANGED
				int oldBotID = botID;
				updateStack(getModelIndex(botID, false), oldBotID); //TEST

			}
		}
		else
		{
			if (topPosition.y >= 1.7)//1.7
			{
				topPosition.y -= 0.01;
			}
			else
			{
				spaceClicked = false;
				goUp = true;
				//	shader->set("colorFactors", {1.5,0.2,0.2});  //TEST
					//int oldBotID = botID;
					//updateStack(getModelIndex(botID, false),oldBotID); //TEST
			}

		}
	}
	if (goUp && topPosition.y < distToTop)
		topPosition.y += 2.7 / 800;
	else
		goUp = false;

	if (botY < 0)
	{
		if (botY < -50.0f)
			botY += 0.5f;
		else
			botY += 0.1f;
		count++;
	}

	if (shrinkScale > 0.3)
		shrinkScale -= 0.7 / 800;

	if (useGrayScale) {
		// Switch back to default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		grayScaleShader->use();
		fboTex->bind();
		quad->draw();
		if (isFirst) {
			useGrayScale = false;
			isFirst = false;
		}
	}
}

void RendertoTextureScene::Finalize()
{
	delete camera;
	delete shader;
}

std::vector<std::string> RendertoTextureScene::getAllFilesinDir(const std::string &dirPath, const std::vector<std::string> dirSkipList)
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
void RendertoTextureScene::updateStack(int index, int fullID)
{
	for (int i = 1; i < stackSize; i++)
	{
		stack[i - 1] = stack[i];
	}
	//std::cout << "index = " << index << std::endl;
	stack[stackSize - 1] = shapes[index];

	stack[0] = shapes[getModelIndex(fullID, true)];
	botY = -200.0f;
	shrinkScale = 1.0f;
}

int RendertoTextureScene::getModelIndex(int myID, bool whole)//ID not Index
{
	oldBotID = botID;
	int rand;
	if (whole)
	{
		rand = 0;
	}
	else
	{
		int range = modelIDs[myID - 1][0];
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 eng(rd()); // seed the generator
		std::uniform_int_distribution<> distr(1, range); // define the range
		rand = distr(eng);
		/*if (whole)
		{
			rand = 0;
		}*/
		int holeID;
		if (range == 8)
		{
			holeID = rand;
		}
		else
		{
			holeID = modelIDs[myID - 1][rand];
		}
		botID = holeID;
		//std::cout << "Hole id: " << holeID << std::endl;
	}

	int conductor = 0;
	for (int i = 0; i < myID; i++)
	{
		if (i == myID - 1)
		{
			/*if (whole)
			{
				botID = oldBot;
			}*/
			conductor += rand;
			break;
			//Final Round
		}
		else
		{
			conductor += modelIDs[i][0] + 1;
		}
	}
	return conductor;
}