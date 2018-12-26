//
//  graphs.c
//  Fields
//
//  Created by 胡一兵 on 2018/12/13.
//  Copyright © 2018年 Bakantu. All rights reserved.
//

#include <graphs.hpp>
#include <fields.hpp>
#include <wav.hpp>


extern int if_fresh, if_draw;
extern vector<Field*> fields;
extern Window *p_nwd;
extern WAV twav;
extern Shape *p_avg;

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

