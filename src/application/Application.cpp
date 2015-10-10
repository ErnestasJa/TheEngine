#include "Precomp.h"

#include "application/Window.h"
#include "utility/Timer.h"
#include "utility/Logger.h"
#include "utility/StringUtil.h"

#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

#include "Application.h"

#define DSP() "/"

Application::Application(int32_t argc, const char ** argv)
{
	m_argc = argc;
	m_argv = argv;
	m_appContext = new AppContext();
}

Application::~Application()
{
}

bool Application::Init(const std::string  &title)
{
	m_appContext->timer = timer_ptr(new Timer());
	m_appContext->fileSystem = new FileSystem(m_appContext, m_argv[0]);
	m_appContext->settingsManager = new ApplicationSettingsManager(m_appContext);
	m_appContext->logger = new Logger(m_appContext, 0);
	m_appContext->window = new Window();

	InitFileSystemAndLoadConfig();
	InitWindowAndOpenGL(title);
	
	if (!m_appContext->IsInitialized())
		throw "Failed to initialize app context.";

	return true;
}

void Application::InitFileSystemAndLoadConfig()
{
	auto applicationDirectory = m_appContext->fileSystem->GetWorkingDirectory();
	applicationDirectory.append(GetApplicationId());
	m_appContext->fileSystem->AddSearchDirectory(applicationDirectory);
	m_appContext->fileSystem->SetWriteDirectory(applicationDirectory);
	
	///REFACTOR: Magic strings
	if(m_appContext->settingsManager->LoadSettings("config.cfg"))
		m_appContext->settingsManager->WriteSettings("config.cfg");
}

bool Application::InitWindowAndOpenGL(const std::string & title)
{
	int32_t width = m_appContext->settingsManager->GetGroup("video").GetVar("window_width").ValueI(),
		height = m_appContext->settingsManager->GetGroup("video").GetVar("window_height").ValueI();
	
	if (!m_appContext->window->Init(title, width, height))
	{
		delete m_appContext->window;
		m_appContext->window = nullptr;
		return false;
	}

	m_appContext->window->SigWindowClosed().connect(sigc::mem_fun(this, &Application::OnWindowClose));

	///REFACTOR: Opengl initialization should have it's own place, worst case: extract method.
	this->m_appContext->glUtil = new OpenGLUtil(m_appContext->logger);

	if (!this->m_appContext->glUtil->load_extensions())
	{
		delete m_appContext->window;
		return false;
	}

#ifdef _DEBUG_OGL
	if (glDebugMessageCallback) {
		std::cout << "Register OpenGL debug callback " << std::endl;
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			&unusedIds,
			true);
	}
	else
		std::cout << "glDebugMessageCallback not available" << std::endl;
#endif

	m_appContext->logger->log(LOG_CRITICAL, "Shading language: %s", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void Application::Exit()
{
	///REFACTOR: or fix, definitelly not everything gets destroyed.
	m_appContext->logger->log(LOG_LOG, "Exitting.");

	Window::DestroyWindow(m_appContext->window);
	delete m_appContext->logger;

	if (m_appContext->fileSystem)
		delete m_appContext->fileSystem;

	m_appContext->timer = nullptr;
}

AppContext * Application::Ctx()
{
	return m_appContext;
}

VarGroup & Application::GetEngineVars()
{
	return *m_appContext->settingsManager;
}
