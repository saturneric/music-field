#include <type.hpp>
#include <graphs.hpp>
#include <fields.hpp>
#include <wav.hpp>

int if_fresh = 1, if_draw = 0;
list<Shape *> Window::shapes = {};
map<string,View2D *> Window::menus = {};
vector<Field*> fields;
Window *p_nwd = NULL;
WAV twav("data/test.wav");
Shape *p_avg = new Shape();

int main(int argc, char * argv[]) {
    Window nwd(argc,argv);
    p_nwd = &nwd;
    
    nwd.playMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    nwd.size({700,700});
    nwd.position({200,100});
    
    twav.getInfo();
    twav.setTimer(20);
    
    int hw = sqrt(twav.m_pss.step);
    nwd.create("Fields");
    
    p_avg->setRetangle({{0.98,-0.98},{0.85,-0.85}});
    nwd.draw_shape(p_avg);
    for(int y = 0; y < hw; y++){
        for(int x = 0; x < hw; x++){
            double dx = x/16.8, dy = y/16.8;
            Field *p_field = new Field({{-0.9+dx,0.9-dy,0},{-(0.848)+dx,(0.848)-dy,0}});
            nwd.draw_shape(p_field);
            fields.push_back(p_field);
        }
    }
    
    fields[0]->color.setColor({0,0,0});
    nwd.show();
    return 0;
}
