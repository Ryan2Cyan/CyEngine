#include <graphics/Gp.h>
#include <physics/Ph.h>
#include <iostream>
#include <random>

// Shaders filepaths:
const GLchar* light_v = "Additional_Files/shaders/light_vert.txt";
const GLchar* light_f = "Additional_Files/shaders/light_frag.txt";
const GLchar* postproc_v = "Additional_Files/shaders/postprocess_vert.txt";
const GLchar* threshold_f = "Additional_Files/shaders/threshold_frag.txt";
const GLchar* blur_f = "Additional_Files/shaders/blur_frag.txt";
const GLchar* gui_f = "Additional_Files/shaders/gui_frag.txt";
const GLchar* gui_v = "Additional_Files/shaders/gui_vert.txt";
const GLchar* null_f = "Additional_Files/shaders/null_frag.txt";
const GLchar* merge_f = "Additional_Files/shaders/merge_frag.txt";
const GLchar* cubemap_v = "Additional_Files/shaders/cubemap_vert.txt";
const GLchar* cubemap_f = "Additional_Files/shaders/cubemap_frag.txt";

// Texture filepaths:
const GLchar* image_filepath = "Additional_Files/images/skybox_1.png";
const GLchar* image_filepath2 = "Additional_Files/images/image_test_flip.png";

const GLchar* front = "Additional_Files/images/skybox/back.jpg";
const GLchar* bottom = "Additional_Files/images/skybox/bottom.jpg";
const GLchar* back = "Additional_Files/images/skybox/front.jpg";
const GLchar* left = "Additional_Files/images/skybox/left.jpg";
const GLchar* right = "Additional_Files/images/skybox/right.jpg";
const GLchar* top = "Additional_Files/images/skybox/top.jpg";

// Model
const GLchar* model_filepath = "Additional_Files/models/curuthers/curuthers.obj";
const GLchar* model_filepath2 = "Additional_Files/models/gun/mxng.obj";
const GLchar* model_filepath3 = "Additional_Files/models/graveyard/graveyard.obj";
const GLchar* model_filepath4 = "Additional_Files/models/cube/cube.obj";
const GLchar* model_tree = "Additional_Files/models/tree/tree.obj";
const GLchar* model_ground = "Additional_Files/models/tree/ground.obj";
const GLchar* model_skeleton = "Additional_Files/models/tree/skeleton.obj";
const GLchar* model_bird = "Additional_Files/models/tree/bird.obj";
const GLchar* sphere_mfp = "Additional_Files/models/Sphere/orange/sphere.obj";


int main()
{
    srand(time(0));

    // Create window and assign OpenGL context:
    glm::ivec2 window_size{ 1000, 1000 };
    GLFWwindow* window = Gp::CreateWindow(window_size);
    glfwMakeContextCurrent(window);

    // Create context:
    std::shared_ptr<GpContext> context = Gp::CreateContext();
    std::shared_ptr<PhContext> phy_context = Ph::CreateContext();

    // Create Shader for on-screen rendering:
    std::shared_ptr<Shader> shader = context->CreateShader(light_v, light_f);
    std::shared_ptr<Shader> cubemap_shader = context->CreateShader(cubemap_v, cubemap_f);
	std::shared_ptr<Shader> theshold_shader = context->CreateShader(postproc_v, threshold_f);
	std::shared_ptr<Shader> blur_shader = context->CreateShader(postproc_v, blur_f);
	std::shared_ptr<Shader> merge_shader = context->CreateShader(postproc_v, merge_f);

    // Create camera:
    std::shared_ptr<GameObject> player = context->CreateGameObject();
    player->AddRigidbody(0.0f, false);
    player->AddSphereCollider(0.1, 5.0f);
    std::shared_ptr<Camera> main_cam = context->CreateCamera(
        false,
        glm::vec2((float)window_size.x, (float)window_size.y),
        glm::vec3(0.0f, 0.0f, 0.0f),  // position
        glm::vec3(0.0f, 0.0f, 1.0f),  // target
        70.0f                         //fov
    );
    context->SetMainCamera(main_cam); // "Main Camera" can be controlled by the user

    // Create curuthers gameobject:
    std::shared_ptr<Mesh> curuthers_m = context->CreateMesh(model_filepath);
    curuthers_m->SetDiffuse(glm::vec3(0.0, 0.0, 0.0));
    // Generate random numbers for curuther's position:
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(-250, 250); // define the range
    std::shared_ptr<GameObject> curuthers = context->CreateGameObject(curuthers_m);
    curuthers->SetPos(glm::vec3(
        distr(gen),
        distr(gen),
        distr(gen)));

    curuthers->Scale(glm::vec3(0.4f, 0.4f, 0.4f));
    curuthers->AddRigidbody(0.0f, false);
    curuthers->AddSphereCollider(0.1f, 5.0f);
    curuthers->name = "curuthers";
    shader->AddGameObject(curuthers);
   
    // Create graveyard tree gameobject:
    std::shared_ptr<Mesh> tree_m = context->CreateMesh(model_tree);
    tree_m->SetDiffuse(glm::vec3(0.5, 0.5, 1.0));
    std::shared_ptr<Mesh> ground_m = context->CreateMesh(model_ground);
    ground_m->SetDiffuse(glm::vec3(0.5, 0.5, 1.0));
    curuthers_m->SetSpecular(glm::vec3(0.0, 0.0, 0.0));
    std::shared_ptr<Mesh> skeleton_m = context->CreateMesh(model_skeleton);
    skeleton_m->SetDiffuse(glm::vec3(0.5, 0.5, 1.0));
    std::shared_ptr<Mesh> bird_m = context->CreateMesh(model_bird);
    bird_m->SetDiffuse(glm::vec3(0.5, 0.5, 1.0));
    std::shared_ptr<GameObject> graveyard_tree = context->CreateGameObject();
    graveyard_tree->AddMesh(tree_m);
    graveyard_tree->AddMesh(ground_m);
    graveyard_tree->AddMesh(skeleton_m);
    graveyard_tree->AddMesh(bird_m);
    graveyard_tree->SetPos(glm::vec3(5.0f, 0.0f, 0.0f));
    shader->AddGameObject(graveyard_tree);

    // Create render textures:
    std::shared_ptr<RenderTexture> render_texture = context->CreateRenderTexture(window_size);
    std::shared_ptr<RenderTexture> threshold_render_texture = context->CreateRenderTexture(window_size);
    std::shared_ptr<RenderTexture> blur_render_texture0 = context->CreateRenderTexture(window_size);
    std::shared_ptr<RenderTexture> blur_render_texture1 = context->CreateRenderTexture(window_size);
    std::shared_ptr<RenderTexture> merge_render_texture = context->CreateRenderTexture(window_size);
    std::vector<std::shared_ptr<RenderTexture>> render_textures{
        render_texture,
        threshold_render_texture,
        blur_render_texture0,
        blur_render_texture1,
        merge_render_texture
    };

    // Cubemap demo:
    std::vector<std::string> faces = {
        right,
        left,
        top,
        bottom,
        back,
        front
    };
    main_cam->SetCubeMap(context->CreateCubemap(faces));
    main_cam->SetCubeMapShader(cubemap_shader);
    main_cam->SetCubeMapObj(context->CreateUnitCube());

    // Create physics world to handle collisions:
    std::shared_ptr<PhysicsWorld> phy_world = phy_context->CreatePhysicsWorld();
    phy_world->AddGameObject(curuthers);
    phy_world->AddGameObject(player);

    // Game values:
    bool found = false;
    bool game_active = true;
    phy_world->start = true;

    // Render loop (called each frame):
    while (!glfwWindowShouldClose(window))
    {
        // Calc delta time:
        float delta_time = context->CalcDeltaTime();

        // Resize render textures to be size of window:
        if (render_texture->GetSize().x != (int)main_cam->GetSize().x ||
            render_texture->GetSize().y != (int)main_cam->GetSize().y){
            for (size_t i = 0; i < render_textures.size(); i++) {
                render_textures[i]->SetSize(main_cam->GetSize());
            }
        }
            
        // Input:
        context->ProcessInput(window, delta_time);

        // Game code:
        player->SetPos(main_cam->GetTransform().position);
        if (curuthers->GetRigidbody()->has_collided || player->GetRigidbody()->has_collided && !found) {
            if (game_active) {
                system("cls");
                std::cout << "Found Him!" << std::endl;
                curuthers_m->SetDiffuse(glm::vec3(1.0, 1.0, 1.0));
                found = true;
                game_active = false;
            }
        }

        if (!found) {
            // Print positions of player and curuthers:
            std::cout << 
                "Curuthers Pos:  [x] " << std::roundf(curuthers->GetTransform()->position.x) << 
                ",   [y]  " << std::roundf(curuthers->GetTransform()->position.y) <<
                ",   [z]  " << std::roundf(curuthers->GetTransform()->position.z) << std::endl;

            std::cout << "Camera Pos:     [x] " << std::roundf(main_cam->GetTransform().position.x) <<
                ",   [y]  " << std::roundf(main_cam->GetTransform().position.y) <<
                ",   [z]  " << std::roundf(main_cam->GetTransform().position.z) << std::endl;
        }

        // Render:
        shader->Render(main_cam, render_texture, true);

		theshold_shader->Swap(render_texture, threshold_render_texture, NULL);
		theshold_shader->Swap(threshold_render_texture, blur_render_texture0, NULL);
        blur_shader->Swap(blur_render_texture0, blur_render_texture1, NULL);
		for (size_t i = 0; i < 10; i++)
		{
            blur_shader->Swap(blur_render_texture1, blur_render_texture0, NULL);
            blur_shader->Swap(blur_render_texture0, blur_render_texture1, NULL);
		}
        merge_shader->Swap(blur_render_texture0, merge_render_texture, NULL);
        merge_shader->Swap(merge_render_texture, nullptr, render_texture->GetTexId());
        
        // Physics (collision):
        phy_world->Step(delta_time);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up:
    glfwTerminate();
    return 0;
}
