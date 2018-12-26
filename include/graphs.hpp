#ifndef graphs_h
#define graphs_h

#include <type.hpp>

#define DRAW_TIMER 1
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

unsigned long fresh_screen(int signal);
void draw_fields(int signal);

class Color{
public:
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
    void setColor(RawColor color){
        red = color[0]/255;
        green = color[1]/255;
        blue = color[2]/255;
    }
    void setColor(Color color){
        red = color.red;
        blue = color.blue;
        green = color.green;
    }
    void brighter(void){
        red = sqrt(red);
        blue = sqrt(blue);
        green = sqrt(green);
    }
};

class Shape{
public:
    GLenum type;
    GLfloat point_size, line_size;
    vector<Point3> points;
    Color color;
    void flesh(void){
        if(point_size != 1.0f)
            glPointSize(point_size);
        else if(line_size != 1.0f)
            glLineWidth(line_size);
        glColor3f(color.red, color.blue, color.green);
    }
    void setColor(RawColor color){
        this->color.setColor(color);
    }
    void setColor(Color color){
        this->color.setColor(color);
    }
    Shape():type(GL_POINTS),point_size(2.0f),line_size(2.0f),color({255,255,255}){
    }
    
    void setRetangle(const pair<Point3,Point3> pos){
        double fx = pos.first[0],fy = pos.first[1];
        double sx = pos.second[0], sy = pos.second[1];
        setPolygon({{fx,fy,0},{sx,fy,0},{sx,sy,0},{fx,sy,0}});
    }
    
    void setPolygon(const vector<Point3> pos){
        type = GL_POLYGON;
        for(auto i = pos.begin(); i != pos.end(); i++){
            points.push_back(*i);
        }
    }
    
};


class View2D {
public:
    Point pos;
    vector<Shape> shapes;
    
};


class Bar:public View2D {
public:
    Bar(Point, GLint width){
        //Shape *p_shape = new Shape();
    }
};

class Window {
public:
    static list<Shape *> shapes;
    static map<string,View2D *> menus;
    Window(int argc, char **argv){
        glutInit(&argc, argv);
    }
    void playMode(int Mode){
        glutInitDisplayMode(Mode);
    }
    void size(Size sizexy){
        glutInitWindowSize(static_cast<int>(sizexy.first), static_cast<int>(sizexy.second));
    }
    void position(Point positionxy){
        glutPositionWindow(positionxy.first, positionxy.second);
    }
    void create(string name = "No Name",void (*func)() = Window::displayEngine){
        glutCreateWindow(name.data());
        glutDisplayFunc(func);
        glutMouseFunc(mouseListener);
        glutTimerFunc(DRAW_TIMER, draw_fields, 1);
        glutTimerFunc(FRESH_TIMER, rendEngine, 1);
        
    }
    void setColor(Color color){
        glClearColor(color.red, color.blue, color.green, 1.0f);
    }
    void show(void){
        glutMainLoop();
    }
    
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
    
    static void displayEngine(void){
        glClear(GL_COLOR_BUFFER_BIT);
        for(auto i = shapes.begin(); i != shapes.end(); i++)
            rendShape(*i);
        //for(auto i = menus.begin(); i != menus.end(); i++)
            //rendShape((*i).second);
        glutSwapBuffers();
    }
    static void mouseListener(int button,int state,int x,int y){
    }
    
    static void rendEngine(int value){
        unsigned long ms = fresh_screen(0);
        unsigned int now_fresh = (unsigned int)(FRESH_TIMER - ms);
        if(now_fresh < 0 || now_fresh > FRESH_TIMER) now_fresh = 0;
        glutTimerFunc(now_fresh, rendEngine, 1);
    }
    
    void draw_triangle(vector<Point3> pos, RawColor color={255, 255, 255}){
        Shape *p_shape = new Shape;
        p_shape->type = GL_TRIANGLES;
        p_shape->setColor(color);
        for(auto i = pos.begin(); i != pos.end(); i++){
            p_shape->points.push_back(*i);
        }
        draw_shape(p_shape);
    }
    
    void draw_point(Point3 pos, GLfloat size = 1.0f, RawColor color={255, 255, 255}){
        Shape *p_shape = new Shape;
        p_shape->type = GL_POINTS;
        p_shape->point_size = size;
        p_shape->setColor(color);
        p_shape->points.push_back(pos);
        draw_shape(p_shape);
    }
    
    void draw_polygon(vector<Point3> pos, RawColor color={255,255,255}){
        Shape *p_shape = new Shape;
        p_shape->type = GL_POLYGON;
        p_shape->setColor(color);
        p_shape->setPolygon(pos);
        draw_shape(p_shape);
    }
    
    void draw_line(pair<Point3,Point3> pos, GLfloat size = 1.0f, RawColor color={255,255,255}){
        Shape *p_shape = new Shape;
        p_shape->type = GL_LINES;
        p_shape->line_size = size;
        p_shape->setColor(color);
        p_shape->points.push_back(pos.first);
        p_shape->points.push_back(pos.second);
        draw_shape(p_shape);
    }
    
    void draw_retangle(pair<Point3,Point3> pos, RawColor color={255,255,255}){
        double fx = pos.first[0],fy = pos.first[1];
        double sx = pos.second[0], sy = pos.second[1];
        draw_polygon({{fx,fy},{sx,fy},{sx,sy},{fx,sy}},color);
    }
    
    void draw_shape(Shape *shape){
        shapes.insert(shapes.end(),shape);
    }
    
    void add_menu(string name, View2D *view){
        menus.insert({name,view});
    }
    
};



#endif /* graphs_h */
