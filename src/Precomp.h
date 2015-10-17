#ifndef _Precomp_H_
#define _Precomp_H_

#include "Config.h"

// C/C++ SUITE INCLUDES
//#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>

#include <string.h>
#include <string>
#include <stdarg.h>

// OPENGL RELATED LIBS
#include <GLXW/glxw.h>

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define OVR_OS_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#define OVR_OS_MAC
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#define OVR_OS_LINUX
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "glm.hpp"
#include "gtx/norm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/matrix_inverse.hpp"
#include "gtx/rotate_vector.hpp"

#include "gtc/type_ptr.hpp"

// OTHER LIBRARIES
#include "physfs.h"
#include "tinyxml2.h"
#include "sigc++/sigc++.h"

// PROJECT INCLUDES
#include "utility/Bit.h"
#include "utility/Helpers.h"

// BOOST INCLUDES
// jk, don't include boost here. pch size too big.

#endif
