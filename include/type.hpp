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
#endif
#include <vector>
#include <list>
#include <map>

//计算函数执行时间间隔
#define DRAW_TIMER 1
//绘图函数执行时间间隔
#define FRESH_TIMER 20

typedef std::pair<double, double> Size;
typedef std::pair<double, double> Point;
typedef std::vector<double> Point3;
typedef std::vector<double> RawColor;

#endif /* type_h */
