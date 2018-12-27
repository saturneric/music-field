#ifndef type_h
#define type_h


#include <iostream>
#include <stdint.h>
#include <cmath>
#include <time.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <sys/time.h>
#include <unistd.h>
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef WIN32
#include <Windows.h>
#include <glut.h>
#pragma warning(disable:4996)
#endif


#include <vector>
#include <list>
#include <map>

#endif
