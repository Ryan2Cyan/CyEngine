#define SDL_MAIN_HANDLED 

#include "Gp.h"
//#include <wavefront/wavefront.h>
#include <SDL.h>
#include <glfw/glfw3.h>
#include <GL/glew.h>
#include <memory>
#include <ext.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Shaders filepaths:
const GLchar* vertex_shader_filepath = "Additional_Files/shaders/vertex_shader.txt";
const GLchar* basic_v_filepath = "Additional_Files/shaders/basic_vert.txt";
const GLchar* basic_f_filepath = "Additional_Files/shaders/basic_frag2.txt";
const GLchar* fragment_shader_filepath = "Additional_Files/shaders/fragment_shader.txt";
const GLchar* f_basic_lighting = "Additional_Files/shaders/basic_lighting_frag.txt";
const GLchar* v_basic_lighting = "Additional_Files/shaders/basic_lighting_vert.txt";
const GLchar* f_off_screen = "Additional_Files/shaders/off_screen_frag.txt";
const GLchar* v_off_screen = "Additional_Files/shaders/off_screen_vert.txt";

// Resource filepaths:
const GLchar* image_filepath = "Additional_Files/images/wall.jpg";
const GLchar* image_filepath2 = "Additional_Files/images/image_test_flip.png";
const GLchar* model_filepath = "Additional_Files/models/curuthers/curuthers.obj";
const GLchar* model_filepath2 = "Additional_Files/models/trump_obj/trump.obj";
const GLchar* model_filepath3 = "Additional_Files/models/graveyard/graveyard.obj";


int main()
{

    // Create window and assign OpenGL context:
    glm::ivec2 window_size{ 720, 720 };
    GLFWwindow* window = Gp::CreateWindow(window_size);
    glfwMakeContextCurrent(window);

    // Create context:
    std::shared_ptr<GpContext> context = Gp::CreateContext();


    // Create Shader for on-screen rendering:
    std::shared_ptr<Shader> shader = context->CreateShader(v_basic_lighting, f_basic_lighting);

    // Import model via assimp:
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(model_filepath2, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Error::ASSIMP::" << importer.GetErrorString() << std::endl;
        throw std::exception();
    }
    std::string path = model_filepath;
    std::string directory = path.substr(0, path.find_last_of('/'));
    for (unsigned int i = 0; i < scene->mRootNode->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[scene->mRootNode->mMeshes[i]];
    }
   

	//// Object 1:
 //   std::vector<glm::vec3> position = {
 //       { 0.5f, -0.5f, 0.0f }, 
 //       { 0.5f, 0.5f, 0.0f },
 //       { -0.5f, -0.5f, 0.0f },
 //       { 0.5f, 0.5f, 0.0f },
 //       { -0.5f, 0.5f, 0.0f },
 //       { -0.5f, -0.5f, 0.0f }
 //   };
 //   std::vector<glm::vec2> tex_coords = {
 //       { 1.0f, 0.0f },
 //       { 1.0f, 1.0f },
 //       { 0.0f, 0.0f },
 //       { 1.0f, 1.0f },
 //       { 0.0f, 1.0f },
 //       { 0.0f, 0.0f }
 //   };
 //   std::shared_ptr<VertexArray> quad = context->Create2D(position, tex_coords);
 //   std::shared_ptr<Texture> texture = context->CreateTexture(image_filepath2);
 //   glm::vec3 position0 = { 0.0f, 0.0f, 10.0f };
 //   std::shared_ptr<Mesh> quad_mesh = context->CreateMesh(quad, texture, position0);
 //   shader->AddMeshToRender(quad_mesh);


    //// Object 2:
    //glm::vec3 position2 = { 0.0f, 0.0f, 0.0f };
    //std::shared_ptr<Mesh> mc_grass = context->CreateMesh(model_filepath2, position2);
    //shader->AddMeshToRender(mc_grass);

 //   // Create render texture:
 //   std::shared_ptr<RenderTexture> render_texture = context->CreateRenderTexture(window_size);


 //   // Create Shader for off-screen rendering:
 //   std::shared_ptr<Shader> shader_off = context->CreateShader(v_off_screen, f_off_screen);


 //   // Vector of all meshes [heirarchy]:
 //   shader->AddMeshToRender(curuthers);
 //   shader->AddMeshToRender(curuthers2);

 //   // Create quad for render texture:
 //   std::shared_ptr<VertexArray> quad = context->Create2DImage();

    // Create camera:
    std::shared_ptr<Camera> main_cam = context->CreateCamera(
        false,
        glm::vec2((float)window_size.x, (float)window_size.y),
        glm::vec3(0.0f, 0.0f, -50.0f),  // position
        glm::vec3(0.0f, 0.0f, 0.0f),  // target
        70.0f                          //fov
    );

 //   // Main loop state:
 //   bool backface_cull = false;
 //   float camera_speed = 0.05f;
 //   float delta_time = 0;
 //   bool first_mouse = true;
 //   bool mouse_motion = false;
 //   float mouse_x = 0;
 //   float mouse_y = 0;
 //   float mouse_s = 0.001f;
 //   Uint64 last = 0;

  
    // Render loop (called each frame):
    while (!glfwWindowShouldClose(window))
    { 
        // Input loop:
        context->ProcessInput(window);

        // Uniforms:
        float timeValue = glfwGetTime() * 4;

      /*  float greenValue = 0.0f;
        static float blueValue;
        if (timeValue > 15) {
            greenValue = 1.0;
        }
        if (timeValue < 15) {
            blueValue = (cos(timeValue) / 2.0f) + 0.5f;
            main_cam->back_col = glm::vec4(blueValue, 0.0f, 0.0f, 1.0f);
        }
        if (timeValue > 17) {
            blueValue += 0.05;
            main_cam->back_col = glm::vec4(0.0f, blueValue, blueValue, 1.0f);
        }*/

       
       /* glm::vec4 newColor = { 0.0f , greenValue, greenValue, 1.0f };*/

        //glUseProgram(shader->GetId());
     /*   shader->SetUniform("newColor", newColor);*/

        // Render:
        shader->Render(main_cam, true);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // Clean up:
    glfwTerminate();
    return 0;
}