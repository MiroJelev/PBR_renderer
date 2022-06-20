#pragma once

//#define GLEW_STATIC
#include <GL/glew.h>

//SFML
#include <SFML/Window.hpp>

//GLM
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

//standard library
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>




const int scr_w = 1920;
const int scr_h = 1080;
const float mouse_sensitivity = 0.3f;
const float delta_game_speed = 20.0f;
//for shadow mapping
const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;
