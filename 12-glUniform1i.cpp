#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <Shader/Shader.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image/stb_image.h>

const unsigned int screenWidth = 800;
const unsigned int screenHeigth = 600;

void framebuffer_seize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
void mouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

glm::vec3 cameraPos = glm::vec3(0, 0, 3.0f);
glm::vec3 cameraFront = glm::vec3(0, 0, -1.0f);
glm::vec3 cameraUp = glm::vec3(0, 1.0f, -1.0f);


float deltaTime = 0.0f;//当前帧和上一帧时间差
float lastFrame = 0.0f;//上一帧时间

float lastX = 1.0f * screenWidth / 2;
float lastY = 1.0f * screenHeigth / 2;

float yaw = -90.0f;
float pitch = 0.0f;

float fov = 1.0f;

bool firstMouse = true;
int main()
{
	//初始化GLFW
	glfwInit();
	//设置主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//设置次版本号 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置核心包
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeigth, "my GLFWWindow", NULL, NULL);
	if (NULL == window) {
		std::cout << "fail to create window" << std::endl;
		//停止并清理分配的内存
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "fail init gladLoadGlLoader " << std::endl;
		return -1;
	}

	//设置OpenGL 窗口大小
	glViewport(0, 0, 800, 600); //0,0 相当于左下角

	//注册窗口大小更改回调
	glfwSetFramebufferSizeCallback(window, framebuffer_seize_callback);
	//注册鼠标回调
	glfwSetCursorPosCallback(window, mouseCallBack);

	glfwSetScrollCallback(window, mouseScrollCallBack);
	glEnable(GL_DEPTH_TEST);

	//隐藏鼠标
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader ourShader("vertxt.glsl", "fragment.glsl");

	float vertives[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		  glm::vec3(0.0f,  0.0f,  0.0f),
		  glm::vec3(2.0f,  5.0f, -15.0f),
		  glm::vec3(-1.5f, -2.2f, -2.5f),
		  glm::vec3(-3.8f, -2.0f, -12.3f),
		  glm::vec3(2.4f, -0.4f, -3.5f),
		  glm::vec3(-1.7f,  3.0f, -7.5f),
		  glm::vec3(1.3f, -2.0f, -2.5f),
		  glm::vec3(1.5f,  2.0f, -2.5f),
		  glm::vec3(1.5f,  0.2f, -1.5f),
		  glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//生成VBO、VAO、EBO对象
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);


	//绑定VAO
	glBindVertexArray(VAO);
	//绑定缓冲(VBO)类型
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//复制定点数据到缓冲的内存中(VBO)  GL_STATIC_DRAW：数据不会或者几乎不会改变
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertives), vertives, GL_STATIC_DRAW);


	//设置顶点位置属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//设置顶点纹理属性指针
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//生成纹理1
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture1);
	//为当前绑定的纹理设置环绕/过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//图像的输入
	int width, height, nrChannels;
	unsigned char* textureData = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (textureData) {

		//生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failde to load Texture" << std::endl;
	}
	//释放内存
	stbi_image_free(textureData);

	//纹理2
	glGenTextures(1, &texture2);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture2);
	//为当前绑定的纹理设置环绕/过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//图像的输入
	textureData = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);

	if (textureData) {

		//生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failde to load Texture" << std::endl;
	}
	//释放内存
	stbi_image_free(textureData);

	//旋转和缩放箱子
	glm::mat4 trans;
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	ourShader.use();
	ourShader.setInt("ourTexture1", 0);
	ourShader.setInt("ourTexture2", 1);

	//不应该关闭则不关闭
	while (!glfwWindowShouldClose(window))
	{

		//输入
		processInput(window);
		//渲染
		//清屏
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//uniform

		float timeValue = glfwGetTime();

		deltaTime = timeValue - lastFrame;
		lastFrame = timeValue;
		float redValue = cos(timeValue);
		float greenValue = sin(timeValue);
		float blackValue = cos(timeValue);

		//位置偏移量
		float offsetX = (sin(timeValue) * 0.9f);
		float offsety = (cos(timeValue) * 0.5f);
		float offsetz = (sin(timeValue) * 0.5f);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();

		//view矩阵
		glm::mat4 view = glm::mat4(1.0);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//投影矩阵
		glm::mat4 projection = glm::mat4(1.0);
		projection = glm::perspective(glm::radians(fov) * 10.0f, (float)screenWidth / (float)screenHeigth, 0.1f, 100.0f);

		ourShader.setFloat3("tempColor", redValue, greenValue, blackValue);
		ourShader.setFloat3("offset", offsetX, offsety, offsetz);
		ourShader.setMatrix4fv("view", GL_FALSE, glm::value_ptr(view));
		ourShader.setMatrix4fv("projection", GL_FALSE, glm::value_ptr(projection));

		for (unsigned int i = 0; i < 10; i++) {
			//模型矩阵
			glm::mat4 model = glm::mat4(1.0);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i + 1);
			model = glm::rotate(model, timeValue * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
			ourShader.setMatrix4fv("model", GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		glBindVertexArray(VAO);

		//检查并调用事件
		glfwPollEvents();
		//缓冲交换
		glfwSwapBuffers(window);
	}

	// optional: de-allocate all resources once they've outlived their purpose:
   // ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//释放内存
	glfwTerminate();
	return 0;
}


//设置窗口大小回调
void framebuffer_seize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//按下esc 关闭window
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}


	float cameraSpeed = 5.0f * deltaTime;
	//wsad
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	float sensitity = 0.05f;

	float fxpos = static_cast<float>(xpos);
	float fypos = static_cast<float>(ypos);

	if (firstMouse) {
		lastX = fxpos;
		lastY = fypos;
		firstMouse = false;
	}
	float xoffset = (fxpos - lastX) * sensitity;
	float yoffset = (lastY - fypos) * sensitity;
	lastX = fxpos;
	lastY = fypos;

	yaw += xoffset;
	pitch += yoffset;

	float degrees = 89.0f;
	if (pitch > degrees) pitch = degrees;
	if (pitch < -degrees) pitch = -degrees;

	glm::vec3 fornt;
	fornt.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	fornt.y = sin(glm::radians(pitch));
	fornt.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(fornt);
}

void mouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}