// Dear ImGui: standalone example application for GLUT/FreeGLUT + OpenGL2, using legacy fixed pipeline
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

// !!! GLUT/FreeGLUT IS OBSOLETE PREHISTORIC SOFTWARE. Using GLUT is not recommended unless you really miss the 90's. !!!
// !!! If someone or something is teaching you GLUT today, you are being abused. Please show some resistance. !!!
// !!! Nowadays, prefer using GLFW or SDL instead!

// On Windows, you can install Freeglut using vcpkg:
//   git clone https://github.com/Microsoft/vcpkg
//   cd vcpkg
//   bootstrap - vcpkg.bat
//   vcpkg install freeglut --triplet=x86-windows   ; for win32
//   vcpkg install freeglut --triplet=x64-windows   ; for win64
//   vcpkg integrate install                        ; register include and libs in Visual Studio

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl2.h"
#include <windows.h>
#include <GL/GL.h>

#pragma comment(lib, "imgui.lib")
#pragma comment(lib, "opengl32.lib")

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif

#include "main_view.h"

static HDC		gHDC = NULL;
static HGLRC	gHRC = NULL;
static int		gWindowWidth = 300;
static int		gWindowHeight = 300;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL bSetupPixelFormat(HDC hdc);
void MainLoopStep();

int main(int argc, char** argv)
{


	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();
	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
	::RegisterClassExW(&wc);
	HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX12 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	// FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL2_Init();


	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	MainView::init();

	// Main loop
	bool done = false;
	while (!done)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		// See the WndProc() function below for our to dispatch events to the Win32 backend.
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done)
			break;

		// Main loop
		MainLoopStep();

		SwapBuffers(gHDC);
	}

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 0;
}

void MainLoopStep()
{
	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize = ImVec2((float)gWindowWidth, (float)gWindowHeight);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	MainView::render();

	// Rendering
	ImGui::Render();

	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClearColor(0.45f, 0.55f, 0.60f, 1.00f);

	glClear(GL_COLOR_BUFFER_BIT);

	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}


// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	PAINTSTRUCT    ps;

	switch (msg)
	{
	case WM_CREATE:
		gHDC = GetDC(hWnd);
		if (!bSetupPixelFormat(gHDC))
			PostQuitMessage(0);

		gHRC = wglCreateContext(gHDC);
		wglMakeCurrent(gHDC, gHRC);
		break;
	case WM_DESTROY:
		if (gHRC)
		{
			wglDeleteContext(gHRC);
			gHDC = NULL;
		}
		if (gHDC)
		{
			ReleaseDC(hWnd, gHDC);
			gHDC = NULL;
		}
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
	{
		// Notify OpenGL
		RECT rect;
		GetClientRect(hWnd, &rect);

		gWindowWidth = rect.right - rect.left;
		gWindowHeight = rect.bottom - rect.top;

		break;
	}
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;

	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

BOOL bSetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd, * ppfd;
	int pixelformat;

	ppfd = &pfd;

	ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	ppfd->nVersion = 1;
	ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	ppfd->dwLayerMask = PFD_MAIN_PLANE;
	ppfd->iPixelType = PFD_TYPE_COLORINDEX;
	ppfd->cColorBits = 8;
	ppfd->cDepthBits = 16;
	ppfd->cAccumBits = 0;
	ppfd->cStencilBits = 0;

	pixelformat = ChoosePixelFormat(hdc, ppfd);

	if ((pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0)
	{
		MessageBox(NULL, L"ChoosePixelFormat failed", L"Error", MB_OK);
		return FALSE;
	}

	if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
	{
		MessageBox(NULL, L"SetPixelFormat failed", L"Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}