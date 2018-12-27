#ifndef graphs_h
#define graphs_h

#include <type.hpp>

//计算函数执行时间间隔
#define DRAW_TIMER 1
//绘图函数执行时间间隔
#define FRESH_TIMER 20

typedef std::pair<double, double> Size;
typedef std::pair<double, double> Point;
typedef std::vector<double> Point3;
typedef std::vector<double> RawColor;

using std::string;
using std::vector;
using std::list;
using std::pair;
using std::map;

class Field;

//绘图函数声明
unsigned long fresh_screen(int signal);
//计算函数声明
void draw_fields(int signal);

int create_fields(int hw);

/************************************************
 颜色的抽象
 ***/
class Color{
public:
//    颜色的RGB值，范围从0到1
    double red, green, blue;
    
    Color():red(1.0),blue(1.0),green(1.0){
        
    }
    Color(RawColor color){
        red = color[0]/255;
        blue = color[1]/255;
        green = color[2]/255;
    }
    Color(const Color &color){
        red = color.red;
        blue = color.blue;
        green = color.green;
    }
    /**
     设置颜色对象的颜色
     
     @param color RawColor颜色数据
     */
    void setColor(RawColor color){
        red = color[0]/255;
        green = color[1]/255;
        blue = color[2]/255;
    }
    /**
     设置颜色对象的颜色
     
     @param color Color颜色数据
     */
    void setColor(Color color){
        red = color.red;
        blue = color.blue;
        green = color.green;
    }
//    将颜色按照相关比例变亮
    void brighter(void){
        red = sqrt(red);
        blue = sqrt(blue);
        green = sqrt(green);
    }
};

/************************************************
 图形的抽象
 ***/
class Shape{
public:
//    图形形状
    GLenum type;
//    点大小和画笔大小
    GLfloat point_size, line_size;
//    图形顶点的坐标
    vector<Point3> points;
//    图形颜色
    Color color;
    
    Shape():type(GL_POINTS),point_size(2.0f),line_size(2.0f),color({255,255,255}){
    }
    
    /**
     根据储存的信息调用glut函数库中的函数进行绘图前的初始化操作
     */
    void flesh(void){
        if(point_size != 1.0f)
            glPointSize(point_size);
        else if(line_size != 1.0f)
            glLineWidth(line_size);
        glColor3f(color.red, color.blue, color.green);
    }
    
    /**
     设置图形对象的颜色

     @param color RawColor颜色数据
     */
    void setColor(RawColor color){
        this->color.setColor(color);
    }
    
    /**
     设置图形对象的颜色

     @param color Color颜色数据
     */
    void setColor(Color color){
        this->color.setColor(color);
    }
    
    /**
     将图形对象的形状设置为矩形

     @param pos 对角线两端点坐标
     */
    void setRetangle(const pair<Point3,Point3> pos){
        double fx = pos.first[0],fy = pos.first[1];
        double sx = pos.second[0], sy = pos.second[1];
        setPolygon({{fx,fy,0},{sx,fy,0},{sx,sy,0},{fx,sy,0}});
    }
    
    /**
     将图形对象的形状设置为多边形

     @param pos 多边形每个顶点的坐标
     */
    void setPolygon(const vector<Point3> pos){
        type = GL_POLYGON;
        for(auto i = pos.begin(); i != pos.end(); i++){
            points.push_back(*i);
        }
    }
    
};

/************************************************
 控件的抽象
 ***/
class View2D {
public:
    Point pos;
    vector<Shape> shapes;
    
};

/************************************************
 进度条控件的抽象
 ***/
class Bar:public View2D {
public:
    Bar(Point, GLint width){
        //Shape *p_shape = new Shape();
    }
};

/************************************************
 窗体的抽象
 ***/
class Window {
public:
//    储存指向窗体中要包含的图形对象的指针
    static list<Shape *> shapes;
//    储存指向窗体中要包含的控件对象的指针
    static map<string,View2D *> menus;
    
    Window(int argc, char **argv){
        glutInit(&argc, argv);
    }
    
    /**
     设置显示模式

     @param Mode 参照GLUT函数库相关说明即可
     */
    void playMode(int Mode){
        glutInitDisplayMode(Mode);
    }
    
    /**
     设置窗体大小

     @param sizexy 输入Size数据
     */
    void size(Size sizexy){
        glutInitWindowSize(static_cast<int>(sizexy.first), static_cast<int>(sizexy.second));
    }
    
    /**
     设置窗体的初始位置

     @param positionxy 输入Point数据
     */
    void position(Point positionxy){
        glutPositionWindow(positionxy.first, positionxy.second);
    }
    
    /**
     创建窗体

     @param name 窗体的标题名字
     @param func 指向自定义绘制函数的函数指针
     */
    void create(string name = "No Name",void (*func)() = Window::displayEngine){
        glutCreateWindow(name.data());
        glutDisplayFunc(func);
        glutMouseFunc(mouseListener);
        glutTimerFunc(DRAW_TIMER, draw_fields, 1);
        glutTimerFunc(FRESH_TIMER, rendEngine, 1);
        
    }
    
    /**
     设置窗体的初始颜色

     @param color 输入Color数据
     */
    void setColor(Color color){
        glClearColor(color.red, color.blue, color.green, 1.0f);
    }
    
    /**
     显示窗体
     */
    void show(void){
        glutMainLoop();
    }
    
    /**
     进行图形对象的实际绘制操作

     @param shape 指向目标图形对象的指针
     */
    static void rendShape(Shape *shape){
        if(shape->point_size != 1.0f)
            glPointSize(shape->point_size);
        else if(shape->line_size != 1.0f)
            glLineWidth(shape->line_size);
        glColor3f(shape->color.red, shape->color.blue, shape->color.green);
        glBegin(shape->type);
        for(auto k = shape->points.begin(); k != shape->points.end();k++){
            glVertex3f((*k)[0],(*k)[1],(*k)[2]);
        }
        glEnd();
    }
    
    /**
     默认的绘制函数
     */
    static void displayEngine(void){
        glClear(GL_COLOR_BUFFER_BIT);
        for(auto i = shapes.begin(); i != shapes.end(); i++)
            rendShape(*i);
        //for(auto i = menus.begin(); i != menus.end(); i++)
            //rendShape((*i).second);
        glutSwapBuffers();
    }
    
    /**
     默认的鼠标相关事件的监听函数
     */
    static void mouseListener(int button,int state,int x,int y){
    }
    
    /**
     窗口刷新注册函数

     @param value 无用值
     */
    static void rendEngine(int value){
        unsigned long ms = fresh_screen(0);
        unsigned int now_fresh = (unsigned int)(FRESH_TIMER - ms);
        if(now_fresh < 0 || now_fresh > FRESH_TIMER) now_fresh = 0;
        glutTimerFunc(now_fresh, rendEngine, 1);
    }
    
    /**
     在窗体中直接绘制三角形并可以指定其填充颜色
     */
    void draw_triangle(vector<Point3> pos, RawColor color={255, 255, 255}){
        Shape *p_shape = new Shape;
        p_shape->type = GL_TRIANGLES;
        p_shape->setColor(color);
        for(auto i = pos.begin(); i != pos.end(); i++){
            p_shape->points.push_back(*i);
        }
        draw_shape(p_shape);
    }
    
    /**
     直接在窗体中绘制点并可以指定其填充颜色
     */
    void draw_point(Point3 pos, GLfloat size = 1.0f, RawColor color={255, 255, 255}){
        Shape *p_shape = new Shape;
        p_shape->type = GL_POINTS;
        p_shape->point_size = size;
        p_shape->setColor(color);
        p_shape->points.push_back(pos);
        draw_shape(p_shape);
    }
    
    
    /**
     直接在窗体中绘制多边形并可以指定其填充颜色
     */
    void draw_polygon(vector<Point3> pos, RawColor color={255,255,255}){
        Shape *p_shape = new Shape;
        p_shape->type = GL_POLYGON;
        p_shape->setColor(color);
        p_shape->setPolygon(pos);
        draw_shape(p_shape);
    }
    
    /**
     直接在窗体中绘制线并可以指定其填充颜色
     */
    void draw_line(pair<Point3,Point3> pos, GLfloat size = 1.0f, RawColor color={255,255,255}){
        Shape *p_shape = new Shape;
        p_shape->type = GL_LINES;
        p_shape->line_size = size;
        p_shape->setColor(color);
        p_shape->points.push_back(pos.first);
        p_shape->points.push_back(pos.second);
        draw_shape(p_shape);
    }
    
    /**
     直接在窗体中绘制矩形并可以指定其填充颜色
     */
    void draw_retangle(pair<Point3,Point3> pos, RawColor color={255,255,255}){
        double fx = pos.first[0],fy = pos.first[1];
        double sx = pos.second[0], sy = pos.second[1];
        draw_polygon({{fx,fy},{sx,fy},{sx,sy},{fx,sy}},color);
    }
    
    /**
     在窗体中添加图形对象

     @param shape 指向目标图形对象的指针
     */
    void draw_shape(Shape *shape){
        shapes.insert(shapes.end(),shape);
    }
    
    /**
     在窗体中添加控件对象

     @param name 控件名
     @param view 指向目标控件对象的指针
     */
    void add_menu(string name, View2D *view){
        menus.insert({name,view});
    }
    
};
#endif /* graphs_h */
