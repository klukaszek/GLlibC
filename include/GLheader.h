//Get Operating System
#if defined(_WIN32)
    #define PLATFORM_NAME windows // Windows
#elif defined (_WIN64)
    #define PLATFORM_NAME windows // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
    #define PLATFORM_NAME windows // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__linux__)
    #define PLATFORM_NAME linux // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
    #define PLATFORM_NAME osx // Apple OSX
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//gl function loader and interfacing library
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//C gl math library
#include <cglm/call.h>

//C logging library written by rxi
#include "GLlog.h"