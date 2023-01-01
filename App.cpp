#define _CRT_SECURE_NO_WARNINGS
#include "App.h"
#include <glm/gtx/string_cast.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"


App::App() {
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::exit(1);
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
    {
        std::exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(1);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
   show_demo_window = true;
    show_another_window = true;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

}

App::~App() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

}


void App::Run() {
    StartUp();

    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        Update();
     //   updateUI();
     //   renderUI();
       

        glfwSwapBuffers(window);

    }

}

void App::Update() {
   
   updateUI();
    updateWorld();
    displayWorld();
   renderUI();
}

void App::StartUp() {
    w.build();

    //fbo init
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    //texture for frame buffer
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w.viewPortWidth, w.viewPortHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    //render buffer
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w.viewPortWidth, w.viewPortHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //compute texture (path traced image)
    glCreateTextures(GL_TEXTURE_2D, 1, &w.screenTex);
    glTextureParameteri(w.screenTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(w.screenTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(w.screenTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(w.screenTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(w.screenTex, 1, GL_RGBA32F, w.viewPortWidth, w.viewPortHeight);
    glBindImageTexture(0, w.screenTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
   
   // LOAD HDRI environment image
    int x, y, n;
    data = stbi_loadf("images/interior.hdr", &x, &y, &n, 0);
  
    const int HDRIw = x;
    const int HDRIh = y;
    w.shader_data.HDRIwidth = x;
    w.shader_data.HDRIheight = y;

    HDRI = new float[HDRIw * HDRIh * 4];//rgba
    int jj = 0;
    for (int i = 0; i < HDRIw * HDRIh * 4; i += 4) {
        HDRI[i] = data[jj];
        HDRI[i + 1] = data[jj+1];
        HDRI[i + 2] = data[jj+2];
        HDRI[i + 3] = 1.0f;
        jj += 3;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &w.HDRI_image_ID);
    glBindTexture(GL_TEXTURE_2D, w.HDRI_image_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, HDRIw, HDRIh, 0, GL_RGBA, GL_FLOAT, HDRI);
    glBindImageTexture(6, w.HDRI_image_ID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void App::updateUI() {
    ImVec2 viewPortSize;
    
    {
        static float f = 0.0f;
        static float f2 = 0.0f;
        static float f3 = 0.0f;
        static float f4 = 0.0f;
        static float fov = 45.0f;
        static float fd = 5.0f;
        static float ap = 1.0f;
        static int counter = 0;
        static int rayDepth = 4;
        static int maxSamples = 64;
        static bool smoothNormals = true;

        ImGui::Begin("Main Window");                     
        if (ImGui::Button("RENDER")) {
            render = !render;
        }
        ImGui::SameLine();

        if (ImGui::Button("RESET")) {
            w.shader_data.samples = 0;
        
        }
      

        if (ImGui::Checkbox("Smooth normals", &smoothNormals)) {
            w.shader_data.smoothNormals = smoothNormals;
            w.shader_data.samples = 0;
        }

        if (ImGui::SliderInt("Ray Depth", &rayDepth, 0, 6)) {
            w.shader_data.rayDepth = rayDepth;
            w.shader_data.samples = 0;
        }

        if (ImGui::SliderInt("Samples", &maxSamples, 1, 16000)) {
            w.maxSamples = maxSamples;
            w.shader_data.samples = 0;
        }
         
        
        
     
        if (ImGui::SliderFloat("FOV", &fov, 15.0f, 90.0f)) {
            w.cam.FOV = fov;
            w.shader_data.samples = 0;
        }
        if (ImGui::SliderFloat("focus distance", &fd, 0.1f, 5.0f)) {
            w.cam.focusDistance = fd;
            w.shader_data.samples = 0;
        }
        if (ImGui::SliderFloat("aperture", &ap, 0.001f, 1.0f)) {
            w.cam.apertureRadius =ap;
            w.shader_data.samples = 0;
        }
        ImGui::Checkbox("Material Window", &show_another_window);

        ImGui::Text("Samples %.3f ", w.shader_data.samples/1.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
       

        if (ImGui::Button("SAVE png")) {
            float* pixels = new float[w.viewPortWidth * w.viewPortHeight * 4];
            glBindTexture(GL_TEXTURE_2D, w.screenTex);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels);
           

            unsigned char* rgbpixels = new unsigned char[w.viewPortWidth * w.viewPortHeight * 3];
            int j = 0;
            for (int i = 0; i < w.viewPortWidth * w.viewPortHeight * 4; i+=4) {
                rgbpixels[j] = unsigned char(255.0f*pixels[i]);
                rgbpixels[j+1] = unsigned char(255.0f * pixels[i+1]);
                rgbpixels[j+2] = unsigned char(255.0f * pixels[i+2]);
                j += 3;
              //  rgbpixels[i] = pixels[i];
            
            }
            j = 0;
            for (int i = 0; i < 100; i+=4) {
                std::cout << pixels[i] << " " << pixels[i + 1] << " " << pixels[i + 2] << " " << pixels[i + 3] << std::endl;
                std::cout << rgbpixels[j] << " " << rgbpixels[j + 1] << " " << rgbpixels[j + 2] << std::endl;
                j += 3;
            }


            stbi_write_jpg("sky_gray.jpg",w.viewPortWidth, w.viewPortHeight, 3, rgbpixels, 100);
            delete pixels;

        }
        
        ImGui::End();
    }


    //viewport
    {
        ImGui::Begin("viewPort");
        ImGui::SetCursorPos(ImVec2(0, 0));
     
        if (!render) {
            ImGui::Image((void*)texture, ImVec2(w.viewPortWidth, w.viewPortHeight), ImVec2(0, 0), ImVec2(1, -1));
        }
        else {
           ImGui::Image((void*)w.screenTex, ImVec2(w.viewPortWidth, w.viewPortHeight),ImVec2(0, 1), ImVec2(1, 0));             
        }

        ImVec2 screen_pos = ImGui::GetCursorScreenPos();
        ImVec2 prev_screen_pos = screen_pos;
        screen_pos = ImGui::GetCursorScreenPos();

        
        viewPortSize = ImGui::GetWindowSize();

        //check for resize
        if (viewPortSize.x != w.viewPortWidth || viewPortSize.y != w.viewPortHeight) {
            w.ResizeViewport(viewPortSize.x, viewPortSize.y,FBO,RBO,texture);
            w.shader_data.samples = 0;
        }

        ImGui::End();
            
        ImVec2 mousePos = ImGui::GetMousePos();
        if (firstMouse)
        {            
            lastX = mousePos.x;
            lastY = mousePos.y;
            firstMouse = false;
        }
        w.cam.zoomInOut = ImGui::GetIO().MouseWheel;
         
        //screen panning control
        w.cam.panX = 0;
        w.cam.panY = 0;
        if (ImGui::IsMouseDragging(2)) {
            
            //screen_X panning
            if (lastX - mousePos.x > 2)
                w.cam.panX = 1;
            if (lastX - mousePos.x < -2)
                w.cam.panX = -1;
            if ((lastX - mousePos.x > -2) && (lastX - mousePos.x < 2))
                w.cam.panX == 0;

            //screen_Y panning
            if (lastY - mousePos.y > 2)
                w.cam.panY = -1;
            if (lastY - mousePos.y < -2)
                w.cam.panY = 1;
            if ((lastY - mousePos.y > -2) && (lastY - mousePos.y < 2))
                w.cam.panY == 0;

        
        }
        else { w.cam.panX = 0.0f; w.cam.panY = 0.0f; }
         lastX = mousePos.x;
         lastY = mousePos.y;     
    }

    // 3. material window
    if (show_another_window) {       
        ImGui::Begin("Material Window", &show_another_window);    
        if (ImGui::Button("Close"))
            show_another_window = false;


        bool updateMat = false;
        static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 1.0f);
        static ImVec4 emission = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
        static int matNum = w.matID-1;
        ImGui::SliderInt("slider int 1", &matID, 0, matNum);

       
        static float roughness = 0.1f;
        static float specular = 0.0f;
       
        static bool alpha_preview = true;
        static bool alpha_half_preview = false;
        static bool drag_and_drop = true;
        static bool options_menu = true;
        static bool hdr = false;
        static float HDRI_rotate_X = 0.0f;
        static float HDRI_rotate_Y = 0.0f;
        static float HDRI_exposure = 1.0f;
      


        ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
        
        updateMat = ImGui::SliderFloat("roughness", &roughness, 0.0f, 1.0f);
        updateMat = updateMat || ImGui::SliderFloat("specular", &specular, 0.0f, 1.0f);
        updateMat = updateMat || ImGui::ColorPicker4("MyColor##4", (float*)&color, misc_flags, nullptr);
        updateMat = updateMat || ImGui::ColorPicker4("Emissive Color##4", (float*)&emission, misc_flags, nullptr);
        
        if (updateMat) {
            w.updateMaterial(matID, glm::vec4(color),roughness, glm::vec4(specular), glm::vec4(emission));     
        }

        if (ImGui::SliderFloat("HDRI_rotate_X", &HDRI_rotate_X, 0.0f, 6.26f)
            || ImGui::SliderFloat("HDRI_rotate_Y", &HDRI_rotate_Y, 0.0f, 6.26f)
            || ImGui::SliderFloat("HDRI_exposure", &HDRI_exposure, 0.0f, 5.0f)) {
           
            w.updateShaderData(HDRI_rotate_X, HDRI_rotate_Y, HDRI_exposure);
        }
        ImGui::End();
    }

}

void App::renderUI() {
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  
}

void App::updateWorld() {
   
    if (ImGui::GetIO().KeyAlt) {
        w.cam.updateRotation();
        w.cam.updateViewPlaneCorner();
        w.shader_data.samples = 0;
    }
    else if(ImGui::IsMouseDragging(2) || w.cam.zoomInOut!=0) {
        w.cam.updateTranslation();
        w.cam.updateViewPlaneCorner();
        w.shader_data.samples = 0;
    }
 
    w.cam.updateViewPlaneCorner();

    
}

void App::displayWorld() {

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glBindTexture(GL_TEXTURE_2D,texture);
    w.rasterize();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    if (render) {
        w.raytrace();
    }
}



