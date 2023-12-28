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


	Shader ourShader("vertxt.glsl", "fragment.glsl");

	float vertives[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	//生成VBO、VAO、EBO对象
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//生成纹理
	unsigned int texture;
	glGenTextures(1, &texture);

	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture);


	//为当前绑定的纹理设置环绕/过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//绑定VAO
	glBindVertexArray(VAO);
	//绑定缓冲(VBO)类型
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//复制定点数据到缓冲的内存中(VBO)  GL_STATIC_DRAW：数据不会或者几乎不会改变
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertives), vertives, GL_STATIC_DRAW);

	//绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//设置顶点位置属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//设置顶点颜色属性指针
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//设置顶点纹理属性指针
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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

	//旋转和缩放箱子
	glm::mat4 trans;
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));


	//模型矩阵
	glm::mat4 model = glm::mat4(1.0);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
	//view矩阵
	glm::mat4 view = glm::mat4(1.0);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));
	//投影矩阵
	glm::mat4 projection = glm::mat4(1.0);
	projection = glm::perspective(glm::radians(30.0f), (float)screenWidth / (float)screenHeigth, 0.1f, 100.0f);

	//不应该关闭则不关闭
	while (!glfwWindowShouldClose(window))
	{
		//输入
		processInput(window);
		//渲染
		//清屏
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//uniform

		float timeValue = glfwGetTime();
		float redValue = cos(timeValue);
		float greenValue = sin(timeValue);
		float blackValue = cos(timeValue);

		//位置偏移量
		float offsetX = (sin(timeValue) * 0.5f);
		float offsety = (cos(timeValue) * 0.5f);
		float offsetz = (sin(timeValue) * 0.5f);

		//trans = glm::rotate(trans, 0.01f, glm::vec3(0.0, 0.0, 1.0));

		ourShader.use();
		ourShader.setFloat3("tempColor", redValue, greenValue, blackValue);
		ourShader.setFloat3("offset", offsetX, offsety, offsetz);
		ourShader.setMatrix4fv("model", GL_FALSE, glm::value_ptr(model));
		ourShader.setMatrix4fv("view", GL_FALSE, glm::value_ptr(view));
		ourShader.setMatrix4fv("projection", GL_FALSE, glm::value_ptr(projection));


		//绑定纹理
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//检查并调用事件
		glfwPollEvents();
		//缓冲交换
		glfwSwapBuffers(window);
	}

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
}