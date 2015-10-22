#include "Precomp.h"

#include "Window.h"
#include "utility/Timer.h"
#include "utility/Logger.h"
#include "utility/StringUtil.h"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "core/FileSystem.h"
#include "Application.h"
#include "SettingsManager.h"
#include "resources/ResourceManager.h"
#include "opengl/OpenGLExtensionLoader.h"

Application::Application(int32_t argc, const char ** argv)
{
	m_argc = argc;
	m_argv = argv;
}

Application::~Application()
{

}

bool Application::InitContextBasics()
{
	GetContext().p_timer = timer_ptr(new Timer());
	GetContext().p_fileSystem = new FileSystem(m_argv[0]);
	GetContext().p_settingsManager = new ApplicationSettingsManager();
	GetContext().p_logger = new Logger(0);
	GetContext().p_window = new ApplicationWindow();
	GetContext().p_resourceManager = new ResourceManager();

	return true;
}

bool Application::InitSimple(const std::string  &title)
{
	InitContextBasics();
	InitFileSystem();
	LoadConfig();

	if(!InitWindowAndOpenGL(title))
	{
		GetContext().GetLogger()->log(LOG_ERROR, "Failed to initialize ApplicationWindow.");
	}
	
	if (!GetContext().IsInitialized())
		throw "Failed to initialize app context.";

	return true;
}

bool Application::InitFileSystem()
{
	///set working directory to where the binary is.
	auto workingDirectory = GetContext().GetFileSystem()->GetWorkingDirectory();
	GetContext().GetFileSystem()->SetWriteDirectory(workingDirectory);

	///create directory for application
	auto applicationDirectory = Path(GetApplicationId());

	if(!GetContext().GetFileSystem()->CreateDirectory(applicationDirectory))
	{
		printf("%s\n", "Failed to create directory for current application.");
		exit(-1);
	}

	Path appendedPath = Path(workingDirectory).append(applicationDirectory.generic_string());
	GetContext().GetFileSystem()->SetWriteDirectory(appendedPath);
	GetContext().GetFileSystem()->AddSearchDirectory(applicationDirectory);


	auto & fileSystemVars = GetContext().GetApplicationSettingsManager()->GetGroup("filesystem");

	Path logPath(fileSystemVars.GetVar("log_path").ValueS());
	Path configPath(fileSystemVars.GetVar("config_path").ValueS());

	GetContext().GetFileSystem()->CreateDirectory(logPath);
	GetContext().GetFileSystem()->CreateDirectory(configPath);

	return true;
}

bool Application::LoadConfig()
{
	auto & fileSystemVars = GetContext().GetApplicationSettingsManager()->GetGroup("filesystem");
	
	Path logPath(fileSystemVars.GetVar("log_path").ValueS());
	Path configPath(fileSystemVars.GetVar("config_path").ValueS());

	///REFACTOR: Magic strings
	Path configFilePath = configPath;
	configFilePath.append("config.cfg");
	if(!GetContext().GetApplicationSettingsManager()->LoadSettings(configFilePath))
		GetContext().GetApplicationSettingsManager()->WriteSettings(configFilePath);

	return true;
}

bool Application::InitWindowAndOpenGL(const std::string & title)
{
	int32_t width = GetContext().GetApplicationSettingsManager()->GetGroup("video").GetVar("window_width").ValueI(),
		height = GetContext().GetApplicationSettingsManager()->GetGroup("video").GetVar("window_height").ValueI();
	
	if (!GetContext().GetWindow()->Init(title, width, height))
	{
		GetContext().GetLogger()->log(LOG_LOG, "Could not initialize ApplicationWindow with dimensions %ix%i", width, height);
		delete GetContext().p_window;
		GetContext().p_window = nullptr;
		return false;
	}

	GetContext().GetWindow()->SigWindowClosed().connect(sigc::mem_fun(this, &Application::OnWindowClose));

	///REFACTOR: Opengl initialization should have it's own place, worst case: extract method.
	GetContext().p_openGLExtensionLoader = new OpenGLExtensionLoader();

	if (!GetContext().GetOpenGLExtensionLoader()->load_extensions())
	{
		delete GetContext().p_window;
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

	GetContext().GetLogger()->log(LOG_CRITICAL, "Shading language: %s", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

bool Application::DestroyContext()
{
	if(GetContext().p_logger)
	{
		GetContext().p_logger->log(LOG_LOG, "Exitting.");
	}

	if(GetContext().p_guiEnv)
	{
		delete GetContext().p_guiEnv;
	}

	if(GetContext().p_openGLExtensionLoader)
	{
		delete GetContext().p_openGLExtensionLoader;
	}

	if(GetContext().p_window)
	{
		ApplicationWindow::DestroyWindow(GetContext().p_window);
	}

	if(GetContext().p_settingsManager)
	{
		delete GetContext().p_settingsManager;
	}

	if(GetContext().p_logger)
	{
		delete GetContext().p_logger;
	}

	if (GetContext().GetFileSystem())
	{
		delete GetContext().GetFileSystem();
	}

	GetContext().p_timer = nullptr;

	return true;
}

VarGroup & Application::GetEngineVars()
{
	return *GetContext().GetApplicationSettingsManager();
}
