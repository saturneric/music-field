#include <type.hpp>
#include <graphs.hpp>
#include <fields.hpp>
#include <wav.hpp>

int if_fresh = 1, if_draw = 0;
list<Shape *> Window::shapes = {};
map<string,View2D *> Window::menus = {};
vector<Field*> fields;
Window *p_nwd = NULL;
WAV twav("test.wav");
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

void draw_fields(int signal){
    if (if_fresh == 1 && if_draw == 0){
        timeval timer_a, timer_b;
        gettimeofday(&timer_a, NULL);
        Data tmp_data = twav.getData();
        tmp_data.Data2Color();
        if(tmp_data.active == true){
            if_fresh = 0;
            if_draw = 1;
            int idx = 1;
            for(auto i = tmp_data.color_data.begin(); i!= tmp_data.color_data.end(); i++){
                Color m_color = (*i);
                //printf("%f %f %f\n",m_color.red, m_color.green, m_color.blue);
                //m_color.brighter();
                if(idx>fields.size()-1) break;
                fields[idx]->color.setColor(m_color);
                idx++;
            }
            p_avg->setColor(tmp_data.avg_color);
        }
        else{
            for(auto i = fields.begin(); i!= fields.end(); i++){
                (*i)->color.setColor({0,0,0});
            }
            p_avg->setColor({0,0,0});
        }
        gettimeofday(&timer_b, NULL);
        //printf("Draw Time: %ums\n",(timer_b.tv_usec-timer_a.tv_usec)/1000);
    }
    glutTimerFunc(DRAW_TIMER, draw_fields, 1);
}

unsigned long fresh_screen(int signal){
    if(if_draw == 1 && if_fresh == 0){
        timeval timer_a, timer_b;
        if_fresh = 1;
        if_draw = 0;
        gettimeofday(&timer_a, NULL);
        p_nwd->displayEngine();
        gettimeofday(&timer_b, NULL);
        double ms = (timer_b.tv_usec-timer_a.tv_usec)/1000.0;
        if(ms - (unsigned long)ms > 0.65) ms = (unsigned long)ms + 1;
        //printf("MS: %lf\n",ms);
        return ms;
    }
    return -1;
}
