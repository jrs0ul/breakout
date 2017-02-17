#ifndef USEFULL_H
#define USEFULL_H
#include "Vectors.h"


//apvalina floata
int roundDoube2Int(double x);
//ar kolidina du apskritimai?
bool CirclesColide(float x1,float y1,float radius1, float x2, float y2, float radius2);

Vector3D MakeVector(float speedx, float speedy, float _angle );

Vector3D Vec2Reflection(Vector3D& vector, Vector3D& plane);



#endif// USEFULL_H
