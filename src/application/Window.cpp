#include "Precomp.h"

#include "Window.h"

std::map<GLFWwindow*, ApplicationWindow*>   ApplicationWindow::_windows;

void MouseMoveEvent(GLFWwindow * wnd, double x, double y)
{
	ApplicationWindow::_windows[wnd]->SigMouseMoved().emit(x, y);
}

void MouseKeyEvent(GLFWwindow * wnd, int32_t button, int32_t action, int32_t mod)
{
	ApplicationWindow::_windows[wnd]->SigMouseKey().emit(button, action, mod);
}

void MouseScrollEvent(GLFWwindow * wnd, double sx, double sy)
{
	ApplicationWindow::_windows[wnd]->SigMouseScroll().emit(sx, sy);
}

void WindowResize(GLFWwindow * wnd, int32_t w, int32_t h)
{
	ApplicationWindow::_windows[wnd]->SigWindowResized().emit(w, h);
}

void WindowClose(GLFWwindow * wnd)
{
	ApplicationWindow::_windows[wnd]->SigWindowClosed().emit();
	ApplicationWindow::_windows[wnd]->_shouldClose = true;
}

void KeyEvent(GLFWwindow * wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
	ApplicationWindow::_windows[wnd]->SigKeyEvent().emit(key, scan_code, action, modifiers);
}

void TextEvent(GLFWwindow * wnd, uint32_t scan_code)
{
	ApplicationWindow::_windows[wnd]->SigTextEvent().emit(scan_code);
}

sigc::signal<void, double, double> & ApplicationWindow::SigMouseMoved()
{
	return _sigMouseMoved;
}

sigc::signal<void, int32_t, int32_t, int32_t> & ApplicationWindow::SigMouseKey()
{
	return _sigMouseKey;
}

sigc::signal<void, double, double> & ApplicationWindow::SigMouseScroll()
{
	return _sigMouseScroll;
}

sigc::signal<void, int32_t, int32_t> & ApplicationWindow::SigWindowResized()
{
	return _sigWindowResized;
}

sigc::signal<void> & ApplicationWindow::SigWindowClosed()
{
	return _sigWindowClosed;
}

sigc::signal<void, int32_t, int32_t, int32_t, int32_t> & ApplicationWindow::SigKeyEvent()
{
	return _sigKeyEvent;
}

sigc::signal<void, uint32_t> & ApplicationWindow::SigTextEvent()
{
	return _sigTextEvent;
}

void ApplicationWindow::DestroyWindow(ApplicationWindow * wnd)
{
	delete wnd;
	ApplicationWindow::_windows.erase(wnd->GetWindow());
}

ApplicationWindow::ApplicationWindow()
{
	_window = nullptr;
	_shouldClose = false;
}

ApplicationWindow::~ApplicationWindow()
{
	_sigWindowResized.clear();
	_sigWindowClosed.clear();
	_sigMouseMoved.clear();
	_sigMouseKey.clear();
	_sigMouseScroll.clear();
	_sigKeyEvent.clear();
	_sigTextEvent.clear();

	glfwDestroyWindow(_window);
	glfwTerminate();
}

bool ApplicationWindow::Init(const std::string  &title, uint32_t width, uint32_t height, bool fullscreen, bool windowed, uint32_t r, uint32_t g, uint32_t b, uint32_t alpha, uint32_t depth, uint32_t stencil)
{
	if (!glfwInit())
	{
		printf("glfw init failed!\n"); //log
		return false;
	}

	//glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); //We don't want the old OpenGL
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //We don't want the old OpenGL
#ifdef _DEBUG_OGL
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	glfwWindowHint(GLFW_RESIZABLE, 0);

	/* Create a ApplicationWindowed mode ApplicationWindow and its OpenGL context */
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = nullptr;

	if (fullscreen&&windowed)
	{
		mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	}
	else
	if (fullscreen)
	{
		_window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
	}
	else
	if (windowed)
	{
		_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	}

	if (!_window)
	{
		printf("glfw open ApplicationWindow failed!\n"); //log
		return false;
	}

	_windows[_window] = this;
	glfwMakeContextCurrent(_window);

	glfwSetWindowSizeCallback(_window, &WindowResize);
	glfwSetWindowCloseCallback(_window, &WindowClose);
	glfwSetCursorPosCallback(_window, &MouseMoveEvent);
	glfwSetMouseButtonCallback(_window, &MouseKeyEvent);
	glfwSetScrollCallback(_window, &MouseScrollEvent);
	glfwSetKeyCallback(_window, &KeyEvent);
	glfwSetCharCallback(_window, &TextEvent);

	glfwSetWindowShouldClose(_window, GL_FALSE);

	glfwSwapInterval(0);

	glfwSwapBuffers(_window);
	glfwPollEvents();
	return true;
}

bool ApplicationWindow::GetKey(uint32_t key)
{
	return glfwGetKey(_window, key);
}

glm::ivec2 ApplicationWindow::GetMousePos()
{
	double x, y;
	glfwGetCursorPos(_window, &x, &y);
	return glm::ivec2(x, y);
}

glm::ivec2 ApplicationWindow::GetWindowSize()
{
	int32_t x, y;
	glfwGetWindowSize(_window, &x, &y);
	return glm::ivec2(x, y);
}

void ApplicationWindow::SetMousePos(const glm::ivec2 & pos)
{
	glfwSetCursorPos(_window, pos.x, pos.y);
}

void ApplicationWindow::SetCursorDisabled(bool disabled)
{
	glfwSetInputMode(_window, GLFW_CURSOR, disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

GLFWwindow * ApplicationWindow::GetWindow()
{
	return _window;
}

bool ApplicationWindow::Update()
{
	glfwPollEvents();
	return _window != nullptr;
}

void ApplicationWindow::SwapBuffers()
{
	glfwSwapBuffers(_window);
}

bool ApplicationWindow::GetShouldClose()
{
	return _shouldClose;
}