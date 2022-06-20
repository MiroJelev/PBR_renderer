#include "common.h"

const float max_pitch = 89.0f;
const float min_pitch = -89.0f;
const float max_fov = 45.0f;
const float min_fov = 1.0f;
const float max_view_distance = 100.0f;
const float min_view_distance = 0.1f;

class Camera{
public:
	Camera();
	void add_fov(const float& value);
	void add_yaw(const float& value);
	void add_pitch(const float& value);
	void add_row(const float& value);
	void set_speed(const float& value);
	
	void move_forward();
	void move_backward();
	void move_left();
	void move_right();
	void face(glm::vec3 direction);
	void update_face_direction();

	void calculate_perspective(glm::mat4& projection);
	void calculate_ortho(glm::mat4& projection);
	void calculate_view(glm::mat4& view);
	
	glm::vec3 get_position()const;
	glm::vec3 get_front()const;
	glm::vec3 get_up()const;
	float get_fov()const;
	float get_speed()const;
	float get_yaw()const;
	float get_pitch()const;
	float get_row()const;
	
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	
	float fov;
	float speed;
	float yaw;
	float pitch;
	float row;
};
