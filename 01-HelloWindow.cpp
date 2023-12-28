#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

	//不应该关闭则不关闭
	while (!glfwWindowShouldClose(window))
	{
		//输入
		processInput(window);
		//渲染
		//清屏
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


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