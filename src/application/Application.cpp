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
#include "opengl/OpenGLUtil.h"

#define DSP() "/"

Application::Application(int32_t argc, const char ** argv)
{
	m_argc = argc;
	m_argv = argv;
}

Application::~Application()
{
}

bool Application::Init(const std::string  &title)
{
	GetContext().p_timer = timer_ptr(new Timer());
	GetContext().p_fileSystem = new FileSystem(m_argv[0]);
	GetContext().p_settingsManager = new ApplicationSettingsManager();
	GetContext().p_logger = new Logger(0);
	GetContext().p_window = new ApplicationWindow();

	InitFileSystemAndLoadConfig();

	GetContext().GetLogger()->SetTimestampedLogFile();

	if(!InitWindowAndOpenGL(title))
	{
		GetContext().GetLogger()->log(LOG_ERROR, "Failed to initialize ApplicationWindow.");
	}
	
	if (!GetContext().IsInitialized())
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
	auto workingDirectory = GetContext().GetFileSystem()->GetWorkingDirectory();
	GetContext().GetFileSystem()->SetWriteDirectory(workingDirectory);

	///create directory for application
	auto applicationDirectory = Path(GetApplicationId());

	if(!GetContext().GetFileSystem()->CreateDirectory(applicationDirectory))
	{
		printf("%s\n", "Failed to create directory for current application.");
		exit(-1);
	}

 	PHYSFS_getSearchPathCallback(printSearchPath, NULL);

	Path appendedPath = Path(workingDirectory).append(applicationDirectory.generic_string());
	GetContext().GetFileSystem()->SetWriteDirectory(appendedPath);
	GetContext().GetFileSystem()->AddSearchDirectory(applicationDirectory);

 	// ...
 	PHYSFS_getSearchPathCallback(printSearchPath, NULL);
	
	auto & fileSystemVars = GetContext().GetApplicationSettingsManager()->GetGroup("filesystem");
	
	Path logPath(fileSystemVars.GetVar("log_path").ValueS());
	Path configPath(fileSystemVars.GetVar("config_path").ValueS());

	GetContext().GetFileSystem()->CreateDirectory(logPath);
	GetContext().GetFileSystem()->CreateDirectory(configPath);

	///REFACTOR: Magic strings
	Path configFilePath = configPath;
	configFilePath.append("config.cfg");
	if(!GetContext().GetApplicationSettingsManager()->LoadSettings(configFilePath))
		GetContext().GetApplicationSettingsManager()->WriteSettings(configFilePath);
}

bool Application::InitWindowAndOpenGL(const std::string & title)
{
	int32_t width = GetContext().GetApplicationSettingsManager()->GetGroup("video").GetVar("window_width").ValueI(),
		height = GetContext().GetApplicationSettingsManager()->GetGroup("video").GetVar("window_height").ValueI();
	
	GetContext().GetLogger()->log(LOG_LOG, "Trying to initialize ApplicationWindow, dimensions %ix%i", width, height);
	if (!GetContext().GetWindow()->Init(title, width, height))
	{
		delete GetContext().p_window;
		GetContext().p_window = nullptr;
		return false;
	}

	GetContext().GetWindow()->SigWindowClosed().connect(sigc::mem_fun(this, &Application::OnWindowClose));

	///REFACTOR: Opengl initialization should have it's own place, worst case: extract method.
	GetContext().p_openGLUtil = new OpenGLUtil();

	if (!GetContext().GetOpenGLUtil()->load_extensions())
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

void Application::Exit()
{
	///REFACTOR: or fix, definitelly not everything gets destroyed.
	GetContext().GetLogger()->log(LOG_LOG, "Exitting.");

	ApplicationWindow::DestroyWindow(GetContext().p_window);
	delete GetContext().GetLogger();

	if (GetContext().GetFileSystem())
		delete GetContext().GetFileSystem();

	GetContext().p_timer = nullptr;
}

VarGroup & Application::GetEngineVars()
{
	return *GetContext().GetApplicationSettingsManager();
}
