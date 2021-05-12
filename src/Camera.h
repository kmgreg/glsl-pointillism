/**
@author = Lauren Cole. 
Basic camera class that allows panning in 3d
TODO: allow looking around (camera rotation)
*/

#include "glm/glm.hpp"
#include "Shader.h"


class Camera {
private:
	glm::vec3 position; // camera position
	glm::vec3 lookAt; //lookat vector
	float cameraSpeed;

	#define LEFT    bounds[0]
	#define RIGHT   bounds[1]
	#define TOP     bounds[2]
	#define BOTTOM  bounds[3]
	#define NEAR    bounds[4]
	#define FAR     bounds[5]

public:

	Camera();
	Camera(glm::vec3 pos);
	Camera(glm::vec3 pos, glm::vec3 lookat);
	void setPos(glm::vec3 pos);
	void moveCamera(glm::vec3 translation);
	glm::vec3 getPos();
	void onUpdate(Shader shader);
	void setFrustum(Shader shader);
	void setViewing(Shader shader);
};