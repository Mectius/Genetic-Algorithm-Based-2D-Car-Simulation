#ifndef CARAI_H
#define CARAI_H
#include <vector>
#include <raylib.h>
class Car{
  public:
    Rectangle core = {0,0,70,40};
    Vector2 vec = {core.width / 2,core.height / 2};
    int rotation=-90;
    int rotationSpeed=5;
    float movementSpeed=100;

    int fitness = 0;

    int active=1;
    int enabled=1;

    int Ax,Ay;
    std::vector<int> A_S;    //Distances
    int Bx,By;
    int B_reaction,B_side,B_rot_speed;
    std::vector<int> B_S;    //Distances
    int Cx,Cy;
    int C_reaction,C_side,C_rot_speed;
    std::vector<int> C_S;    //Distances
    int Dx,Dy;
    int D_reaction,D_side,D_rot_speed;
    std::vector<int> D_S;    //Distances
    int Ex,Ey;
    int E_reaction,E_side,E_rot_speed;
    std::vector<int> E_S;    //Distances

    //side variables -> 0(left),1(right)
};

typedef struct Circle {
    float x;
    float y;
    float radius;
} Circle;

typedef struct couple {
    Car a;
    Car b;
} couple;

#endif