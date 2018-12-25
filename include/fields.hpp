#ifndef fields_hpp
#define fields_hpp

#include <type.hpp>
#include <graphs.hpp>

class Field:public Shape{
    
public:
    double people;
    Field():people(0),Shape(){
        setRetangle({{0,0,0},{0,0,0}});
    }
    Field(pair<Point3,Point3> pos){
        setRetangle(pos);
    }
};

#endif /* fields_hpp */
