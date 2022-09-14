#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "GLFW/glfw3.h"

#include <ignition/common/Console.hh>
#include <ignition/rendering.hh>

using namespace ignition;
using namespace rendering;

unsigned int textureId = 0;
CameraPtr camera;
ImVec2 textureSize = {800, 600};
double cameraOffset = 0.0;
bool initialised = false;

void BuildScene(ScenePtr _scene)
{
  // initialize _scene
  _scene->SetAmbientLight(0.3, 0.3, 0.3);
  VisualPtr root = _scene->RootVisual();

  // create directional light
  DirectionalLightPtr light0 = _scene->CreateDirectionalLight();
  light0->SetDirection(-0.5, 0.5, -1);
  light0->SetDiffuseColor(0.5, 0.5, 0.5);
  light0->SetSpecularColor(0.5, 0.5, 0.5);
  root->AddChild(light0);

  // // create point light
  // PointLightPtr light1 = _scene->CreatePointLight();
  // light1->SetDiffuseColor(0.5, 0.5, 0.5);
  // light1->SetSpecularColor(0.5, 0.5, 0.5);
  // light1->SetLocalPosition(5, -5, 10);
  // root->AddChild(light1);

  // create point light
  PointLightPtr light2 = _scene->CreatePointLight();
  light2->SetDiffuseColor(0.5, 0.5, 0.5);
  light2->SetSpecularColor(0.5, 0.5, 0.5);
  light2->SetLocalPosition(3, 5, 5);
  root->AddChild(light2);

  // create green material
  MaterialPtr green = _scene->CreateMaterial();
  green->SetAmbient(0.0, 0.5, 0.0);
  green->SetDiffuse(0.0, 0.7, 0.0);
  green->SetSpecular(0.5, 0.5, 0.5);
  green->SetShininess(50);
  green->SetReflectivity(0);

  // create center visual
  VisualPtr center = _scene->CreateVisual();
  center->AddGeometry(_scene->CreateSphere());
  center->SetLocalPosition(3, 0, 0);
  center->SetLocalScale(0.1, 0.1, 0.1);
  center->SetMaterial(green);
  root->AddChild(center);

  //! [red material]
  // create red material
  MaterialPtr red = _scene->CreateMaterial();
  red->SetAmbient(0.5, 0.0, 0.0);
  red->SetDiffuse(1.0, 0.0, 0.0);
  red->SetSpecular(0.5, 0.5, 0.5);
  red->SetShininess(50);
  red->SetReflectivity(0);
  red->SetRenderOrder(3);
  //! [red material]

  // create sphere visual
  VisualPtr sphere = _scene->CreateVisual();
  sphere->AddGeometry(_scene->CreateSphere());
  sphere->SetOrigin(0.0, -0.5, 0.0);
  sphere->SetLocalPosition(3, 0, 0);
  sphere->SetLocalRotation(0, 0, 0);
  sphere->SetLocalScale(1, 1, 1);
  sphere->SetMaterial(red);
  root->AddChild(sphere);

  // create blue material
  MaterialPtr blue = _scene->CreateMaterial();
  blue->SetAmbient(0.0, 0.0, 0.3);
  blue->SetDiffuse(0.0, 0.0, 0.8);
  blue->SetSpecular(0.5, 0.5, 0.5);
  blue->SetShininess(50);
  blue->SetReflectivity(0);

  // create box visual
  VisualPtr box = _scene->CreateVisual();
  box->AddGeometry(_scene->CreateBox());
  box->SetOrigin(0.0, 0.5, 0.0);
  box->SetLocalPosition(3, 0, 0);
  box->SetLocalRotation(IGN_PI / 4, 0, IGN_PI / 3);
  box->SetLocalScale(1, 2.5, 1);
  box->SetMaterial(blue);
  root->AddChild(box);

  // create ellipsoid visual
  VisualPtr ellipsoidVisual = _scene->CreateVisual();
  auto ellipsoid = _scene->CreateSphere();
  ellipsoidVisual->SetLocalScale(1.2, 0.7, 0.5);
  ellipsoidVisual->AddGeometry(ellipsoid);
  ellipsoidVisual->SetLocalPosition(3, -1, 0);
  ellipsoidVisual->SetMaterial(green);
  root->AddChild(ellipsoidVisual);

  //! [white material]
  // create white material
  MaterialPtr white = _scene->CreateMaterial();
  white->SetAmbient(0.5, 0.5, 0.5);
  white->SetDiffuse(0.8, 0.8, 0.8);
  white->SetReceiveShadows(true);
  white->SetReflectivity(0);
  white->SetRenderOrder(0);
  //! [white material]

  VisualPtr capsuleVisual = _scene->CreateVisual();
  CapsulePtr capsule = _scene->CreateCapsule();
  capsule->SetLength(0.2);
  capsule->SetRadius(0.2);
  capsuleVisual->AddGeometry(capsule);
  capsuleVisual->SetOrigin(0.0, 0.0, 0.0);
  capsuleVisual->SetLocalPosition(4, 2, 0);
  capsuleVisual->SetLocalScale(1, 1, 1);
  capsuleVisual->SetMaterial(red);
  root->AddChild(capsuleVisual);

  // create plane visual
  VisualPtr plane = _scene->CreateVisual();
  plane->AddGeometry(_scene->CreatePlane());
  plane->SetLocalScale(5, 8, 1);
  plane->SetLocalPosition(3, 0, -0.5);
  plane->SetMaterial(white);
  root->AddChild(plane);

  // create plane visual
  VisualPtr plane2 = _scene->CreateVisual();
  plane2->AddGeometry(_scene->CreatePlane());
  plane2->SetLocalScale(5, 8, 1);
  plane2->SetLocalPosition(4, 0.5, -0.5);
  plane2->Scale(0.1, 0.1, 1);
  plane2->SetMaterial(red);
  root->AddChild(plane2);

  // create axis visual
  VisualPtr axis = _scene->CreateAxisVisual();
  axis->SetLocalPosition(4.0, 0.5, -0.4);
  root->AddChild(axis);

  // create camera
  camera = _scene->CreateCamera("camera");
  camera->SetLocalPosition(0.0, 0.0, 0.0);
  camera->SetLocalRotation(0.0, 0.0, 0.0);
  camera->SetImageWidth(textureSize.x);
  camera->SetImageHeight(textureSize.y);
  camera->SetAntiAliasing(8);
  camera->SetAspectRatio(1.333);
  camera->SetHFOV(IGN_PI / 2);
  root->AddChild(camera);

  // track target
  camera->SetTrackTarget(box);
}

CameraPtr CreateCamera(const std::string& _engineName)
{
  // create and populate scene
  std::map<std::string, std::string> params;

  // ensure that the QML application and Ignition / Ogre2 share an OpenGL
  // context
  params["useCurrentGLContext"] = "1";
  RenderEngine* engine = rendering::engine(_engineName, params);
  if (!engine)
  {
    std::cout << "Engine '" << _engineName << "' is not supported" << std::endl;
    return CameraPtr();
  }
  ScenePtr scene = engine->CreateScene("scene");
  BuildScene(scene);

  return camera;
}

void InitEngine()
{
  initialised = true;
  std::string engineName("ogre2");

  common::Console::SetVerbosity(4);

  try
  {
    camera = CreateCamera(engineName);
  }
  catch (...)
  {
    std::cerr << "Error starting up: " << engineName << std::endl;
  }

  if (!camera)
  {
    ignerr << "No cameras found. Scene will not be rendered" << std::endl;
    return;
  }

  // quick check on sizing...
  ignmsg << "imageW: " << camera->ImageWidth() << "\n";
  ignmsg << "imageH: " << camera->ImageHeight() << "\n";

  // pre-render will force texture creation and may update texture id
  camera->PreRender();
  textureId = camera->RenderTextureGLId();
}

void UpdateCamera()
{
  double angle = cameraOffset / 2 * M_PI;
  double x = sin(angle) * 3.0 + 3.0;
  double y = cos(angle) * 3.0;
  camera->SetLocalPosition(x, y, 0.0);

  cameraOffset += 0.0005;
}

void Render()
{
  if (camera->ImageWidth() != textureSize.x && textureSize.x > 0)
    camera->SetImageWidth(textureSize.x);

  if (camera->ImageHeight() != textureSize.y && textureSize.y > 0)
    camera->SetImageHeight(textureSize.y);

  // pre-render may regenerate textureId if the size changes
  camera->PreRender();

  textureId = camera->RenderTextureGLId();

  // render to texture
  camera->Update();

  // Move camera
  UpdateCamera();
}

namespace Venom
{
class VenomGui
{
 public:
  VenomGui() {}
  ~VenomGui()
  {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(_window);
    glfwTerminate();
  }

  static void GLFWErrorCallback(int error, const char* description)
  {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
  }

  void Initialize()
  {
    // Setup _window
    glfwSetErrorCallback(GLFWErrorCallback);
    if (!glfwInit()) return;

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    // Create _window with graphics context
    _window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example",
                               NULL, NULL);

    if (_window == NULL) return;

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);  // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable
    // Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
    io.ConfigFlags |=
        ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport
                                           // / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can
    // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
    // them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
    // need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please
    // - The fonts will be rasterized at a given size (w/ oversampling) and
    // stored into a texture when calling
    // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame
    // below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string
    // literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    // ImFont* font =
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
    // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

    // IGNITION WITHIN SAME OPENGL CONTEXT
    // Expose engine name to command line because we can't instantiate both
    // ogre and ogre2 at the same time
  }

  void Run()
  {
    ImGuiIO& io = ImGui::GetIO();

    // Our state
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(_window))
    {
      // IGNITION AND STUFF
      if (!initialised)
      {
        InitEngine();
      }

      // check if engine has been successfully initialized
      if (!initialised)
      {
        ignerr << "Unable to initialize renderer" << std::endl;
        return;
      }

      Render();

      //
      // Poll and handle events (inputs, _window resize, etc.)
      // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
      // tell if dear imgui wants to use your inputs.
      // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
      // your main application, or clear/overwrite your copy of the mouse data.
      // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
      // data to your main application, or clear/overwrite your copy of the
      // keyboard data. Generally you may always pass all inputs to dear imgui,
      // and hide them from your application based on those two flags.
      glfwPollEvents();

      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      ///////////////////////////////////////////////////////////////////////////
      static bool opt_fullscreen = true;
      static bool opt_padding = false;
      static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

      // We are using the ImGuiWindowFlags_NoDocking flag to make the parent
      // window not dockable into, because it would be confusing to have two
      // docking targets within each others.
      ImGuiWindowFlags window_flags =
          ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
      if (opt_fullscreen)
      {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar |
                        ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                        ImGuiWindowFlags_NoNavFocus;
      }
      else
      {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
      }

      // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will
      // render our background and handle the pass-thru hole, so we ask Begin()
      // to not render a background.
      if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

      // Important: note that we proceed even if Begin() returns false (aka
      // window is collapsed). This is because we want to keep our DockSpace()
      // active. If a DockSpace() is inactive, all active windows docked into it
      // will lose their parent and become undocked. We cannot preserve the
      // docking relationship between an active window and an inactive docking,
      // otherwise any change of dockspace/settings would lead to windows being
      // stuck in limbo and never being visible.
      if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGui::Begin("DockSpace Demo", nullptr, window_flags);
      if (!opt_padding) ImGui::PopStyleVar();

      if (opt_fullscreen) ImGui::PopStyleVar(2);

      // Submit the DockSpace
      ImGuiIO& io = ImGui::GetIO();
      if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
      {
        ImGuiID dockspace_id = ImGui::GetID("VenomApplicationDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
      }
      else
      {
        // ShowDockingDisabledMessage();
      }

      if (ImGui::BeginMenuBar())
      {
        if (ImGui::BeginMenu("Options"))
        {
          // Disabling fullscreen would allow the window to be moved to the
          // front of other windows, which we can't undo at the moment without
          // finer window depth/z control.
          ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
          ImGui::MenuItem("Padding", NULL, &opt_padding);
          ImGui::Separator();

          if (ImGui::MenuItem(
                  "Flag: NoSplit", "",
                  (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
          {
            dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
          }
          if (ImGui::MenuItem(
                  "Flag: NoResize", "",
                  (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
          {
            dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
          }
          if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "",
                              (dockspace_flags &
                               ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
          {
            dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
          }
          if (ImGui::MenuItem(
                  "Flag: AutoHideTabBar", "",
                  (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
          {
            dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
          }
          if (ImGui::MenuItem("Flag: PassthruCentralNode", "",
                              (dockspace_flags &
                               ImGuiDockNodeFlags_PassthruCentralNode) != 0,
                              opt_fullscreen))
          {
            dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
          }
          ImGui::Separator();

          ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
      }

      // Draw all the contents inside the dockspace...
      // Draw content...
      ImGui::ShowDemoWindow(&show_demo_window);

      ImGui::Begin("Viewport");
      textureSize = ImGui::GetContentRegionAvail();
      ImGui::Image((void*)textureId, textureSize);
      ImGui::End();

      // Dockspace ends...
      ImGui::End();
      ///////////////////////////////////////////////////////////////////////////

      // Rendering
      ImGui::Render();
      int display_w, display_h;
      glfwGetFramebufferSize(_window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                   clear_color.z * clear_color.w, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      // Update and Render additional Platform Windows
      // (Platform functions may change the current OpenGL context, so we
      // save/restore it to make it easier to paste this code elsewhere.
      //  For this specific demo app we could also call
      //  glfwMakeContextCurrent(_window) directly)
      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
      {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
      }

      glfwSwapBuffers(_window);
    }
  }

 private:
  GLFWwindow* _window;
};
}  // namespace Venom