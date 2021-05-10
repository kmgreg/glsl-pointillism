#include "glm/glm.hpp"

class Camera {
private:
	glm::vec4 position; // camera position
	glm::vec4 lookAt; //lookat vector
public:
	Camera();
	Camera(glm::vec4 pos);
	Camera(glm::vec4 pos, glm::vec4 lookat);
};