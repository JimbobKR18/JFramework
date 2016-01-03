#ifndef __JFramework_Shape_h_
#define __JFramework_Shape_h_

#include "MathExt.h"

struct Shape
{
  int id;
  Vector3 position;
  bool passable;
  
  enum ShapeType
  {
    CUBE = 0,
    SPHERE,
    CIRCLE,
    LINE,
    TRIANGLE
  } shape;
  
  Shape(ShapeType aShape);
  virtual ~Shape();
  virtual float GetSize(int index) = 0;
};

struct Cube : public Shape
{
  Vector3 size;

  Cube();
  Cube(Vector3 const &aPosition, Vector3 const &aSize);
  virtual ~Cube();

  bool Get3DCollision(Vector3 const &aPosition);
  bool Get2DCollision(Vector3 const &aPosition);
  
  virtual float GetSize(int index);
};

struct Sphere : public Shape
{
  float radius;

  Sphere();
  Sphere(Vector3 const &aPosition, float const aRadius);
  virtual ~Sphere();

  bool GetCollision(Vector3 const &aPosition);
  
  virtual float GetSize(int index);
};

struct Circle : public Shape
{
  Vector3 up;
  Vector3 right;
  float radius;

  Circle();
  Circle(Vector3 const &aPosition, Vector3 const &aUp, Vector3 const &aRight, float const aRadius);
  virtual ~Circle();
  
  virtual float GetSize(int index);
};

struct Line : public Shape
{
  Vector3 direction;
  float length;

  Line();
  Line(Vector3 const &aPosition, Vector3 const &aDirection, float aLength);
  Line(Vector3 const &aStart, Vector3 const &aEnd);
  virtual ~Line();

  Vector3 ClosestPointToPoint(Vector3 const &aPoint) const;
  bool GetCollisions(Line const &aCompare, Circle &aOutput);
  
  virtual float GetSize(int index);
};

struct Triangle : public Shape
{
  Vector3 points[3];
  
  Triangle();
  Triangle(Vector3 const &aPoint1, Vector3 const &aPoint2, Vector3 const &aPoint3);
  Triangle(Vector3 const aPoints[3]);
  Triangle(std::vector<Vector3> const &aPoints);
  virtual ~Triangle();
  
  Vector3 GetPoint(int index);
  virtual float GetSize(int index);
};

void print_line(Line const &_line);
void print_circle(Circle const &_circle);

#endif