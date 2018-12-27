#ifndef type_h
#define type_h

/*********************************************************************
 *
 *需要使用的库头文件的包含
 *
 *********************************************************************/
//C++标准库
#include <iostream>
#include <stdint.h>
#include <cmath>
#include <time.h>

//苹果操作系统
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <sys/time.h>
#include <unistd.h>
#define _CRT_SECURE_NO_WARNINGS
#endif

//Windows操作系统
#ifdef WIN32
#include <Windows.h>
#include <glut.h>
#pragma warning(disable:4996)
#endif

//容器类头文件包含
#include <vector>
#include <list>
#include <map>

/*********************************************************************
 *
 *编译相关常量的定义
 *
 *********************************************************************/
//计算函数执行时间间隔
#define DRAW_TIMER 1
//绘图函数执行时间间隔
#define FRESH_TIMER 20

/*********************************************************************
 *
 *需要使用的自定义变量
 *
 *********************************************************************/
typedef std::pair<double, double> Size;
typedef std::pair<double, double> Point;
typedef std::vector<double> Point3;
typedef std::vector<double> RawColor;

#endif /* type_h */
