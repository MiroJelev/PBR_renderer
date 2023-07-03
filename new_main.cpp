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

#include "ImGui/imgui.h"
#include "ImGui/imgui-SFML.h"
#include "ImGui/imgui_impl_opengl3.h"


#include <btBulletDynamicsCommon.h>


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

void render_gui(sf::Window& window, sf::Clock& gui_delta_clock, float& fps, long long int& frame_count, bool& rotation, bool& calc_perspective, std::array<glm::vec3, 4>& light_colors){
	//IM GUI
	ImGui::SFML::Update(sf::Mouse::getPosition(window),static_cast<sf::Vector2f>(window.getSize()),gui_delta_clock.restart());
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Begin("Lights");
	ImGui::Text("Select light to be manipulated!");
	ImGui::Text("FPS: %f", floor(fps));
	ImGui::Text("Frame count: %i", frame_count);
	
	const char * items[] {"top_left", "top_right", "bottom_left", "bottom_right"};
	static int selectedItem = 0;
	ImGui::ListBox("lights", &selectedItem, items, 4);
	
	static float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	ImGui::ColorEdit3("color", color);
	glm::vec3 col(round(color[0] * 255.0), round(color[1]* 255.0), round(color[2]* 255.0));
	
	if(ImGui::Button("Rotation")){
		rotation = !rotation;
	}
	if(ImGui::Button("Perspective/Orthogonal")){
		calc_perspective = !calc_perspective;
	}

	light_colors[selectedItem] = col;
	
	ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	///////////////////////
}

glm::vec3 btv3_to_glmv3(const btVector3& btvect){
	return { float(btvect.getX()), float(btvect.getY()), float(btvect.getZ()) };
}

glm::vec3 btv3_to_glmquat3(const btQuaternion& btquater){
	return { float(btquater.getX()), float(btquater.getY()), float(btquater.getZ()+1) }; //+1 to show object but the obj is rotated!!
}

int main(){
	
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.attributeFlags = 1;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	
	sf::Window window(sf::VideoMode(scr_w, scr_h), "OpenGL", sf::Style::Default, settings);
	//window.setMouseCursorGrabbed(true);
	//window.setMouseCursorVisible(false);
	window.setFramerateLimit(60);
	
	GLenum error = glewInit();
	if(error != GLEW_OK){
		std::cerr << "Error: glewInit() error" << std::endl;
		return -1;
	}
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	
	ImGui::SFML::Init(window, static_cast<sf::Vector2f>(window.getSize()));
	ImGui_ImplOpenGL3_Init("#version 330");


	///-----initialization_start-----
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	///-----initialization_end-----

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	
	

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
	
    btTransform groundTransform;
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(20.), btScalar(1.), btScalar(20.)));//50.

		collisionShapes.push_back(groundShape);

		// btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, 0));//0,-56,0

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		body->setFriction(1.f);
		body->setRollingFriction(.1);
		body->setSpinningFriction(0.1);
		body->setAnisotropicFriction(groundShape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(5.));//1
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(0, 100, 0));//2,100,0

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		body->setFriction(1.f);
		body->setRollingFriction(.1);
		body->setSpinningFriction(0.1);
		body->setAnisotropicFriction(colShape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

		dynamicsWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		// btCollisionShape* colShape = new btSphereShape(btScalar(1.));//1
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(3, 5, 0));// 3, 50, 0//2,100,0

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		body->setFriction(1.f);
		body->setRollingFriction(.1);
		body->setSpinningFriction(0.1);
		body->setAnisotropicFriction(colShape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

		dynamicsWorld->addRigidBody(body);
	}

	// {
	// 	//create a dynamic rigidbody

	// 	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	// 	btCollisionShape* colShape = new btConeShape(2, 3);//1
	// 	collisionShapes.push_back(colShape);

	// 	/// Create Dynamic Objects
	// 	btTransform startTransform;
	// 	startTransform.setIdentity();

	// 	btScalar mass(1.f);

	// 	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	// 	bool isDynamic = (mass != 0.f);

	// 	btVector3 localInertia(0, 0, 0);
	// 	if (isDynamic)
	// 		colShape->calculateLocalInertia(mass, localInertia);

	// 	startTransform.setOrigin(btVector3(2, 25, 0));//2,100,0

	// 	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	// 	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	// 	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	// 	btRigidBody* body = new btRigidBody(rbInfo);

	// 	dynamicsWorld->addRigidBody(body);
	// }




	std::string albedo_map_path("assets/pbr/Football_Helmet/albedo.jpg");
	std::string normal_map_path("assets/pbr/Football_Helmet/normal.jpg");
	std::string metallic_map_path("assets/pbr/Football_Helmet/metallic.jpg");
	std::string roughness_map_path("assets/pbr/Football_Helmet/roughness.jpg");
	std::string ao_map_path("assets/pbr/Football_Helmet/ao.jpg");
	std::vector<std::string> helmet_textures_paths{
		albedo_map_path,
		normal_map_path,
		metallic_map_path,
		roughness_map_path,
		ao_map_path
	};

	albedo_map_path =("assets/pbr/cube/raw_plank_wall_textures/raw_plank_wall_diff_1k.png");
	normal_map_path =("assets/pbr/cube/raw_plank_wall_textures/raw_plank_wall_nor_gl_1k.png");
	metallic_map_path =("assets/pbr/cube/raw_plank_wall_textures/raw_plank_wall_arm_1k.png");
	roughness_map_path =("assets/pbr/cube/raw_plank_wall_textures/raw_plank_wall_rough_1k.png");
	ao_map_path =("assets/pbr/cube/raw_plank_wall_textures/raw_plank_wall_ao_1k.png");
	std::vector<std::string> floor_textures_paths{
		albedo_map_path,
		normal_map_path,
		metallic_map_path,
		roughness_map_path,
		ao_map_path
	};

	albedo_map_path =("assets/pbr/cube/pavement/pavement_02_diff_1k.png");
	normal_map_path =("assets/pbr/cube/pavement/pavement_02_nor_gl_1k.png");
	metallic_map_path =("assets/pbr/cube/pavement/pavement_02_arm_1k.png");
	roughness_map_path =("assets/pbr/cube/pavement/pavement_02_rough_1k.png");
	ao_map_path =("assets/pbr/cube/pavement/pavement_02_ao_1k.png");
	std::vector<std::string> sphere_textures_paths{
		albedo_map_path,
		normal_map_path,
		metallic_map_path,
		roughness_map_path,
		ao_map_path
	};
	

	//Load assets
	Model helmet{"assets/pbr/Football_Helmet/Football_Helmet.obj", helmet_textures_paths};
	Model helmet_decomp{"assets/pbr/Football_Helmet/decomp.obj"};
    Model cube{"assets/pbr/cube/cube.obj", floor_textures_paths};
	Model cube_test{"assets/pbr/cube/cube.obj", floor_textures_paths};
	Model sphere{"assets/pbr/cube/sphere.obj", sphere_textures_paths};
	// Model cone{"assets/pbr/cube/cone.obj", sphere_textures_paths};


	//HELMET//////////////////////////////////////////////////////////////////////////
	{
		//create a dynamic rigidbody
		//btConvexHullShape* colShape = new btConvexHullShape();//1
		btCompoundShape* colShape = new btCompoundShape();

		for(Mesh i : helmet_decomp.meshes){
			btConvexHullShape* convShape = new btConvexHullShape();
			btTransform startTransform1;
			startTransform1.setIdentity();
			for(int j = 0; j < i.vertices.size(); j++){
				if(j % 50 == 0)
					convShape->addPoint(btVector3(i.vertices[j].position.x, i.vertices[j].position.y, i.vertices[j].position.z));
			}
			colShape->addChildShape(startTransform1, convShape);
		}
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);
		// colShape->calculatePrincipalAxisTransform(&mass, startTransform, localInertia);

		startTransform.setOrigin(btVector3(2, 200, 0));//2,100,0

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	}
	//HELMET//////////////////////////////////////////////////////////////////////////


    /////////////


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


	
	std::array<glm::vec3, 4> light_positions {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3( 10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3( 10.0f, -10.0f, 10.0f),
    };
    std::array<glm::vec3, 4> light_colors {
        glm::vec3(150.0f, 150.0f, 150.0f),
        glm::vec3(150.0f, 150.0f, 150.0f),
        glm::vec3(150.0f, 0.0f, 0.0f),
        glm::vec3(150.0f, 150.0f, 150.0f)
    };
    
    int nrRows    = 7;
    int nrColumns = 7;
    float spacing = 2.5;
    
    
	
////////////////////////////////////////////////

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//time between frames
	float delta_time = 0.0f; //time between current frame and last frame
	float last_frame = 0.0f; //time of last frame
	
	//mouse//
	sf::Vector2i screen_center{scr_w/2, scr_h/2};
	sf::Mouse::setPosition(screen_center, window);
	////////
	
	float fps = 0.0;
	long long int frame_count = 0;
	
	bool rotation = true;
	bool calc_perspective = true;
	glm::mat4 model_saved = glm::mat4(1.0f);
	
	sf::Clock clock;
	sf::Clock gui_delta_clock;
	glViewport(0, 0, scr_w, scr_h);
	bool running = true;
	while(running){
		//constant speed on all hardware
		sf::Time elapsed_frame = clock.getElapsedTime();
		float current_frame = elapsed_frame.asSeconds();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		camera.set_speed(delta_game_speed * delta_time);
		//
		//frame count and fps
		if(frame_count >= sizeof(unsigned long long) * sizeof(unsigned long long)){
			frame_count = 0;
		}
		frame_count++;
		if(frame_count % 10 == 0){
			fps = 1.0f / delta_time;
		}
		//
		
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
					if(calc_perspective){
						camera.calculate_perspective(projection);
					}else{
						camera.calculate_ortho(projection);
					}
					
					lighting.use();
					lighting.set_mat4("projection", projection);
				}
			}
			
		}
		//movement
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
		
		sf::Vector2f offset;
		//look around with keyboard
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
			offset.y = 1;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
			offset.y = -1;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			offset.x = 1;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			offset.x = -1;
		}
		//mouse
		//offset = mouse_movement_handler(window, screen_center);
		//camera
		camera.add_yaw(offset.x);
		camera.add_pitch(offset.y);
		camera.update_face_direction();
		camera.calculate_view(view);
		
		lighting.use();
		lighting.set_mat4("view", view);


		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		// for(int i = 0; i < helmet_textures.size(); i++){
		// 	glActiveTexture(GL_TEXTURE0 + i);
		// 	helmet_textures[i].bind();
		// }
/*
		//objects
		lighting.use();
		lighting.set_vec3("camera_position", camera.get_position());
		model = glm::mat4(1.0f);
		for (int row = 0; row < nrRows; ++row) 
        {
            //lighting.set_float("metallic", (float)row / (float)nrRows);
            for (int col = 0; col < nrColumns; ++col) 
            {
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
        //
        */

        lighting.use();
		lighting.set_vec3("camera_position", camera.get_position());

		helmet.model = glm::mat4(1.0f);
		cube.model = glm::mat4(1.0f);
		sphere.model = glm::mat4(1.0f);
		cube_test.model = glm::mat4(1.0f);
		// cone.model = glm::mat4(1.0f);
		
		// model = glm::scale(model, glm::vec3(0.5f));
        // helmet.model = glm::scale(helmet.model, glm::vec3(1.f));

//PHYSICS////////////
dynamicsWorld->stepSimulation(1.f / 60.f, 10);//,60,
		//print positions of all objects
		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
			// printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
			// helmet.model = glm::translate(helmet.model, glm::vec3(float(trans.getOrigin().getX()), 
			// 							  float(trans.getOrigin().getY()), 
			// 							  float(trans.getOrigin().getZ())));

			if(j == 0){
				cube.model = glm::translate(cube.model, glm::vec3(btv3_to_glmv3(trans.getOrigin())));
			}
			if(j == 1){
				trans.getOpenGLMatrix(glm::value_ptr(sphere.model));
			}
			if(j == 2){
				trans.getOpenGLMatrix(glm::value_ptr(cube_test.model));
			}
			if(j == 3){
				trans.getOpenGLMatrix(glm::value_ptr(helmet.model));
				trans.getOpenGLMatrix(glm::value_ptr(helmet_decomp.model));
			}
		}
///////////////////// 
        helmet.draw(lighting);
		
        sphere.model = glm::scale(sphere.model, glm::vec3(5.f));
        sphere.draw(lighting);

		//floor
		cube.model = glm::scale(cube.model, glm::vec3(20.f, 1.f, 20.f));//50, 1, 50
        //cube.model = glm::translate(cube.model, glm::vec3(btv3_to_glmv3(groundTransform.getOrigin())));
        cube.draw(lighting);

		cube_test.draw(lighting);

		     

        //spheres representing lights
        for (unsigned int i = 0; i < light_positions.size(); ++i)
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

		
		
		render_gui(window, gui_delta_clock, fps, frame_count, rotation, calc_perspective, light_colors);
		window.display();
	}
	
	  ImGui::SFML::Shutdown();
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


