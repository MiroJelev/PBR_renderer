#include "common.h"
#include "Shader.h"
#include "texture.h"
#include "vertex_buffer.h"
#include "vertex_array.h"
#include "element_buffer.h"
#include "camera.h"
#include "model.h"
#include "framebuffer.h"
#include "render_buffer.h"


sf::ContextSettings settings;

sf::Vector2f mouse_movement_handler(const sf::Window& window, const sf::Vector2i& screen_center);




unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = static_cast<unsigned int>(indices.size());

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);           
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);        
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));        
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}










int main(){
	
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.attributeFlags = 1;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	
	sf::Window window(sf::VideoMode(scr_w, scr_h), "OpenGL", sf::Style::Default, settings);
	window.setMouseCursorGrabbed(true);
	window.setMouseCursorVisible(false);
	
	glewInit();
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_FRAMEBUFFER_SRGB);
	/*
	glm::vec3 point_light_positions[]{
		//glm::vec3( 0.7f,  0.2f,  2.0f),
		glm::vec3( 1.0f,  0.0f, 2.0f),
		glm::vec3( 2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3( 0.0f,  0.0f, -3.0f)
	};
	
	
	Model backpack{"assets/backpack/backpack.obj"};
	Model floor{"assets/wood_plane/wood_plane.obj"};
	
	
	std::array<float, 24>screen_quad = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f							
    };
    
    Vertex_buffer quad_vbo{};
    quad_vbo.bind();
	glBufferData(GL_ARRAY_BUFFER, 
				sizeof(screen_quad), 
				&screen_quad, 
				GL_STATIC_DRAW);
	
	Vertex_array quad_vao{};
	quad_vao.bind();
	//position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	//texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	quad_vao.unbind();
	quad_vbo.unbind();
	/////////////////////////////
	
	Framebuffer framebuffer{};
	//framebuffer.attach_depthmap(depth_map.get_ID());
	//framebuffer.remove_color_buffer();
	
	Texture2D color_buffer{};
	color_buffer.init_as_color_buffer();
	color_buffer.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	color_buffer.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	framebuffer.attach_texture(color_buffer.get_ID());
	
	Render_buffer render_buffer{};
	framebuffer.attach_render_buffer(render_buffer.get_ID());
	
*/
	////////////////
	
	Camera camera{};
	camera.face(glm::vec3(0.0, 0.0, -1.0));//face the cubes
	camera.add_yaw(-90.0f);
	
	//view matrix
	glm::mat4 view = glm::mat4(1.0f);
	camera.calculate_view(view);
	
	//projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	camera.calculate_perspective(projection);
	
	//model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	

////////////////////////////////////////////////
	//shaders
/*	Shader lighting_shaders{"shaders/advanced/shader.vs", "shaders/advanced/blinn-phong.fs"};
	lighting_shaders.use();
	lighting_shaders.set_mat4("model", model);
	lighting_shaders.set_mat4("view", view);
	lighting_shaders.set_mat4("projection", projection);
	
	
	float mat_shininess = 32.0f;//32
	lighting_shaders.set_float("material.shininess", mat_shininess);
	
	glm::vec3 light_ambient(0.2f);//0.2
	glm::vec3 light_diffuse(1.0f);
	glm::vec3 light_specular(1.0f);
	
	//spot light
	lighting_shaders.set_vec3("spot_light.ambient", light_ambient);
	lighting_shaders.set_vec3("spot_light.diffuse", light_diffuse);
	lighting_shaders.set_vec3("spot_light.specular", light_specular);
	
	lighting_shaders.set_vec3("spot_light.position", camera.get_position());
	lighting_shaders.set_vec3("spot_light.direction", camera.get_front());
	lighting_shaders.set_float("spot_light.cut_off", glm::cos(glm::radians(12.5f)));
	lighting_shaders.set_float("spot_light.outer_cut_off", glm::cos(glm::radians(17.5f)));
	
	lighting_shaders.set_float("spot_light.constant", 1.0f);
	lighting_shaders.set_float("spot_light.linear", 0.09f);
	lighting_shaders.set_float("spot_light.quadratic", 0.032f);
	//
	//directional light
	//glm::vec3 dir_light_direction(1.0f, 2.0f, -6.0f);
	glm::vec3 dir_light_direction(-1.0f, -2.0f, 6.0f);
	lighting_shaders.set_vec3("directional_light.direction", dir_light_direction);
	lighting_shaders.set_vec3("directional_light.ambient", light_ambient);
	lighting_shaders.set_vec3("directional_light.diffuse", light_diffuse);
	lighting_shaders.set_vec3("directional_light.specular", light_specular);
	//
	//point lights
	//[0]
	lighting_shaders.set_vec3("point_lights[0].position", point_light_positions[0]);
	
	lighting_shaders.set_float("point_lights[0].constant", 1.0f);
	lighting_shaders.set_float("point_lights[0].linear", 0.09f);
	lighting_shaders.set_float("point_lights[0].quadratic", 0.032f);
	
	lighting_shaders.set_vec3("point_lights[0].ambient", light_ambient);
	lighting_shaders.set_vec3("point_lights[0].diffuse", light_diffuse);
	lighting_shaders.set_vec3("point_lights[0].specular", light_specular);
	
	
	Shader light_source_shaders{"shaders/advanced/shader.vs", "shaders/advanced/light_source_shader.fs"};
	light_source_shaders.use();
	//vertex
	light_source_shaders.set_mat4("model", model);
	light_source_shaders.set_mat4("view", view);
	light_source_shaders.set_mat4("projection", projection);
	//fragment
	light_source_shaders.set_vec3("light_source_color", light_diffuse);
	//
	
	
	Shader framebuffer_shader{"shaders/advanced/framebuffer.vs", "shaders/advanced/framebuffer.fs"};
	framebuffer_shader.use();
	framebuffer_shader.set_int("screen_texture", 0);
	*/
	
	std::string albedo_map_path("assets/pbr/rusted_iron/rustediron2_basecolor.png");
	std::string normal_map_path("assets/pbr/rusted_iron/rustediron2_normal.png");
	std::string metallic_map_path("assets/pbr/rusted_iron/rustediron2_metallic.png");
	std::string roughness_map_path("assets/pbr/rusted_iron/rustediron2_roughness.png");
	std::string ao_map_path("assets/pbr/rusted_iron/ao.png");
	
	
	Texture2D albedo_map{};
	albedo_map.load_from_file(albedo_map_path);
	albedo_map.set_parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	albedo_map.set_parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	albedo_map.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	albedo_map.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture2D normal_map{};
	normal_map.load_from_file(normal_map_path);
	normal_map.set_parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	normal_map.set_parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	normal_map.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	normal_map.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	Texture2D metallic_map{};
	metallic_map.load_from_file(metallic_map_path);
	metallic_map.set_parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	metallic_map.set_parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	metallic_map.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	metallic_map.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	Texture2D roughness_map{};
	roughness_map.load_from_file(roughness_map_path);
	roughness_map.set_parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	roughness_map.set_parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	roughness_map.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	roughness_map.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	Texture2D ao_map{};
	ao_map.load_from_file(ao_map_path);
	ao_map.set_parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	ao_map.set_parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	ao_map.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ao_map.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	
	Shader lighting{"shaders/PBR/pbr_lighting.vs", "shaders/PBR/pbr_lighting.fs"};
	lighting.use();
	//vertex
	lighting.set_mat4("model", model);
	lighting.set_mat4("view", view);
	lighting.set_mat4("projection", projection);
	//fragment
	lighting.set_vec3("camera_position", camera.get_position());
	
	//lighting.set_vec3("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	//lighting.set_float("ao", 1.0f);
	
	
	lighting.use();
    lighting.set_int("albedo_map", 0);
    lighting.set_int("normal_map", 1);
    lighting.set_int("metallic_map", 2);
    lighting.set_int("roughness_map", 3);
    lighting.set_int("ao_map", 4);


	/*glm::vec3 light_positions[] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3( 10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3( 10.0f, -10.0f, 10.0f),
    };
    glm::vec3 light_colors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };*/
	glm::vec3 light_positions[] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3( 10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3( 10.0f, -10.0f, 10.0f),
    };
    glm::vec3 light_colors[] = {
        glm::vec3(150.0f, 150.0f, 150.0f),
        glm::vec3(150.0f, 150.0f, 150.0f),
        glm::vec3(150.0f, 150.0f, 150.0f),
        glm::vec3(150.0f, 150.0f, 150.0f)
    };
    
    int nrRows    = 7;
    int nrColumns = 7;
    float spacing = 2.5;
	
	
	
	
////////////////////////////////////////////////

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//time between frames
	float delta_time = 0.0f; //time between current frame and last frame
	float last_frame = 0.0f; //time of last frame
	
	//mouse//
	sf::Vector2i screen_center{scr_w/2, scr_h/2};
	sf::Mouse::setPosition(screen_center, window);
	////////
	
	sf::Clock clock;
	glViewport(0, 0, scr_w, scr_h);
	bool running = true;
	while(running){
		sf::Event event;
		while(window.pollEvent(event)){
			if (event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed){
					running = false;
				}
				else if(event.type == sf::Event::Resized){
					glViewport(0, 0, event.size.width, event.size.height);
				}
			}
			if(event.type == sf::Event::MouseWheelScrolled){
				if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel){
					camera.add_fov(event.mouseWheelScroll.delta);
					
					camera.calculate_perspective(projection);
					
					lighting.use();
					lighting.set_mat4("projection", projection);
				}
			}
			
		}
		
		
		//constant speed on all hardware
		sf::Time elapsed_frame = clock.getElapsedTime();
		float current_frame = elapsed_frame.asSeconds();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		camera.set_speed(delta_game_speed * delta_time);

		//
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			camera.move_forward();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			camera.move_backward();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			camera.move_right();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
			camera.move_left();
		}
		//mouse
		sf::Vector2f offset = mouse_movement_handler(window, screen_center);
		//camera
		camera.add_yaw(offset.x);
		camera.add_pitch(offset.y);
		camera.update_face_direction();
		camera.calculate_view(view);
		
		
		lighting.use();
		lighting.set_mat4("view", view);

		
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		albedo_map.bind();
		glActiveTexture(GL_TEXTURE1);
		normal_map.bind();
		glActiveTexture(GL_TEXTURE2);
		metallic_map.bind();
		glActiveTexture(GL_TEXTURE3);
		roughness_map.bind();
		glActiveTexture(GL_TEXTURE4);
		ao_map.bind();
		
		
		lighting.use();
		lighting.set_vec3("camera_position", camera.get_position());
		model = glm::mat4(1.0f);
		 for (int row = 0; row < nrRows; ++row) 
        {
            //lighting.set_float("metallic", (float)row / (float)nrRows);
            for (int col = 0; col < nrColumns; ++col) 
            {
				// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                //lighting.set_float("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
                
                 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(
                    (col - (nrColumns / 2)) * spacing, 
                    (row - (nrRows / 2)) * spacing, 
                    0.0f
                ));
                lighting.set_mat4("model", model);
                renderSphere();
            }
        }
        for (unsigned int i = 0; i < sizeof(light_positions) / sizeof(light_positions[0]); ++i)
        {
			sf::Time elapsed = clock.getElapsedTime();
            glm::vec3 newPos = light_positions[i] + glm::vec3(sin(elapsed.asSeconds() * 5.0) * 5.0, 0.0, 0.0);
            //newPos = light_positions[i];
            lighting.set_vec3("light_positions[" + std::to_string(i) + "]", newPos);
            lighting.set_vec3("light_colors[" + std::to_string(i) + "]", light_colors[i]);

            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            lighting.set_mat4("model", model);
            renderSphere();
        }
        
        
		
		
		
		
		/*
		framebuffer.bind();
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			
			for(int i = 0; i < 1; i++){
				model = glm::mat4(1.0f);
				//model = glm::translate(model, point_light_positions[i]);
				model = glm::translate(model, -dir_light_direction);
				model = glm::scale(model, glm::vec3(0.2f));
				
				light_source_shaders.use();
				light_source_shaders.set_mat4("model", model);
				backpack.draw(light_source_shaders);
			}
			
			model = glm::mat4(1.0f);
			lighting_shaders.use();
			lighting_shaders.set_mat4("model", model);
			backpack.draw(lighting_shaders);
			
			
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
			model = glm::scale(model, glm::vec3(4.0f, 1.0f, 4.0f));
			lighting_shaders.use();
			lighting_shaders.set_mat4("model", model);
			floor.draw(lighting_shaders);
		framebuffer.unbind();
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);
			
			framebuffer_shader.use();
			quad_vao.bind();
			color_buffer.bind();
			
			glDrawArrays(GL_TRIANGLES, 0, 6);
			
			color_buffer.unbind();
			quad_vao.unbind();
		*/
		window.display();
	}
	
}

sf::Vector2f mouse_movement_handler(const sf::Window& window, const sf::Vector2i& screen_center){
	sf::Vector2f offset;
	sf::Vector2f mouse_position = (sf::Vector2f)sf::Mouse::getPosition(window);
	offset.x = mouse_position.x - screen_center.x;
	offset.y = mouse_position.y - screen_center.y;
	sf::Mouse::setPosition(screen_center, window);
	
	offset.x *= mouse_sensitivity;
	offset.y *= mouse_sensitivity;
	return offset;
}

