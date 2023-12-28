#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include<iostream>
int main() {
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 tans = glm::mat4(1.0f);
	tans = glm::translate(tans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = tans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;

	while (true)
	{

	}
	return 0;
}