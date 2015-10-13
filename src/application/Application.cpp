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

	if(InitWindowAndOpenGL(title))
	{
		m_appContext->logger->log(LOG_ERROR, "Failed to initialize window.");
	}
	
	if (!m_appContext->IsInitialized())
		throw "Failed to initialize app context.";

	return true;
}


static void printSearchPath(void *data, const char *pathItem)
{
	printf("[%s] is in the search path.\n", pathItem);
}

void Application::InitFileSystemAndLoadConfig()
{
	///set working directory to where the binary is.
	auto workingDirectory = m_appContext->fileSystem->GetWorkingDirectory();
	m_appContext->fileSystem->SetWriteDirectory(workingDirectory);

	///create directory for application
	auto applicationDirectory = Path(GetApplicationId());
	
	if(!m_appContext->fileSystem->DirectoryExists(applicationDirectory))
	{
		if(!m_appContext->fileSystem->CreateDirectory(applicationDirectory))
		{
			printf("%s\n", "Failed to create directory for current application.");
			exit(-1);
		}
	}

 	PHYSFS_getSearchPathCallback(printSearchPath, NULL);

	m_appContext->fileSystem->SetWriteDirectory(applicationDirectory);
	m_appContext->fileSystem->AddSearchDirectory(applicationDirectory);

 	// ...
 	PHYSFS_getSearchPathCallback(printSearchPath, NULL);
	
	auto & fileSystemVars = m_appContext->settingsManager->GetGroup("filesystem");
	
	Path logPath(fileSystemVars.GetVar("log_path").ValueS());

	//REFACTOR: Method that creates if not dir doesn't exist
	if(!m_appContext->fileSystem->DirectoryExists(logPath))
	{
		m_appContext->fileSystem->CreateDirectory(logPath);
	}

	Path configPath(fileSystemVars.GetVar("config_path").ValueS());

	if(!m_appContext->fileSystem->DirectoryExists(configPath))
	{
		m_appContext->fileSystem->CreateDirectory(configPath);
	}

	///REFACTOR: Magic strings
	if(!m_appContext->settingsManager->LoadSettings("config.cfg"))
		m_appContext->settingsManager->WriteSettings("config.cfg");
}

bool Application::InitWindowAndOpenGL(const std::string & title)
{
	int32_t width = m_appContext->settingsManager->GetGroup("video").GetVar("window_width").ValueI(),
		height = m_appContext->settingsManager->GetGroup("video").GetVar("window_height").ValueI();
	
	m_appContext->logger->log(LOG_LOG, "Trying to initialize window, dimensions %ix%i", width, height);
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

#if 0

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

AppContext * Application::GetContext()
{
	return m_appContext;
}

AppContext * Application::Ctx()
{
	return m_appContext;
}

VarGroup & Application::GetEngineVars()
{
	return *m_appContext->settingsManager;
}
