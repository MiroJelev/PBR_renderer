#include "camera.h"

Camera::Camera() : position(0.0f, 0.0f, 3.0f),
				   front(0.0f, 0.0f, 0.0f),
				   up(0.0f, 1.0f, 0.0f),
				   fov(45.0f),
				   speed(0.05f),
				   yaw(0.0f),
				   pitch(0.0f),
				   row(0.0f){	}

void Camera::add_fov(const float& value){
	fov -= value;
	if(fov > max_fov)
		fov = max_fov;
	else if(fov < min_fov)
		fov = min_fov;
}
void Camera::add_yaw(const float& value){
	yaw += value;
}
void Camera::add_pitch(const float& value){
	pitch -= value;
	if(pitch > max_pitch)
		pitch = max_pitch;
	if(pitch < min_pitch)
		pitch = min_pitch;
}
void Camera::add_row(const float& value){
	row += value;
}

void Camera::set_speed(const float& value){
	speed = value;
}



void Camera::move_forward(){
	position += speed * front;
}
void Camera::move_backward(){
	position -= speed * front;
}
void Camera::move_left(){
	position -= speed * glm::normalize(glm::cross(front, up));
}
void Camera::move_right(){
	position += speed * glm::normalize(glm::cross(front, up));
}
void Camera::face(glm::vec3 direction){
	front = glm::normalize(direction);
}
void Camera::update_face_direction(){
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	face(direction);
}


void Camera::calculate_perspective(glm::mat4& projection){
	projection = glm::perspective(glm::radians(fov), (float)scr_w / (float)scr_h, min_view_distance, max_view_distance);
}
void Camera::calculate_ortho(glm::mat4& projection){
	float max_left = -30.6f;			//temporary use of variables
	float max_right = 30.6f;			//not sure if the names encompass the 
	float max_up = 20.0f;			//right meaning of the values
	float max_down = -20.0f;
	projection = glm::ortho(max_left, max_right, max_up, max_down, min_view_distance, max_view_distance);
}
void Camera::calculate_view(glm::mat4& view){
	view = glm::lookAt(position, position + front, up);
}


glm::vec3 Camera::get_position()const{
	return position;
}
glm::vec3 Camera::get_front()const{
	return front;
}
glm::vec3 Camera::get_up()const{
	return up;
}
float Camera::get_fov()const{
	return fov;
}
float Camera::get_speed()const{
	return speed;
}
float Camera::get_yaw()const{
	return yaw;
}
float Camera::get_pitch()const{
	return pitch;
}
float Camera::get_row()const{
	return row;
}
