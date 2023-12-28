#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <Shader/Shader.h>
#include<stb_image/stb_image.h>

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

	GLFWwindow* window = glfwCreateWindow(800, 600, "my GLFWWindow", NULL, NULL);
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
		// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};


	//生成VBO、VAO、EBO对象
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	//绑定VAO
	glBindVertexArray(VAO);
	//绑定缓冲(VBO)类型
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//复制定点数据到缓冲的内存中(VBO)  GL_STATIC_DRAW：数据不会或者几乎不会改变
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertives), vertives, GL_STATIC_DRAW);

	//绑定EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


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


		ourShader.use();
		ourShader.setFloat3("tempColor", redValue, greenValue, blackValue);
		ourShader.setFloat3("offset", offsetX, offsety, offsetz);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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