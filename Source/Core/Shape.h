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
    AABB = 0,
    SPHERE,
    CIRCLE,
    LINE,
    TRIANGLE,
    PLANE,
    OBB
  } shape;
  
  Shape(ShapeType aShape);
  Shape(ShapeType aShape, int aId, Vector3 const &aPosition, bool aPassable);
  virtual ~Shape();
  virtual float GetSize(int index) = 0;
  
  static void SerializeLUA();
};

struct AxisAlignedBoundingBox : public Shape
{
  Vector3 size;

  AxisAlignedBoundingBox();
  AxisAlignedBoundingBox(AxisAlignedBoundingBox const &aAxisAlignedBoundingBox);
  AxisAlignedBoundingBox(Vector3 const &aPosition, Vector3 const &aSize);
  virtual ~AxisAlignedBoundingBox();

  bool Get3DCollision(Vector3 const &aPosition);
  bool Get2DCollision(Vector3 const &aPosition);
  
  virtual float GetSize(int index);
};

struct OrientedBoundingBox : public Shape
{
  Vector3 up;
  Vector3 right;
  Vector3 forward;
  Vector3 extents;
  
  OrientedBoundingBox();
  OrientedBoundingBox(OrientedBoundingBox const &aOrientedBoundingBox);
  OrientedBoundingBox(Vector3 const &aPosition, Vector3 const &aUp, Vector3 const &aRight,
    Vector3 const &aForward, Vector3 const &aExtents);
  virtual ~OrientedBoundingBox();
  
  Vector3 GetAxis(int index);
  virtual float GetSize(int index);
};

struct Sphere : public Shape
{
  float radius;

  Sphere();
  Sphere(Sphere const &aSphere);
  Sphere(Vector3 const &aPosition, float const aRadius);
  virtual ~Sphere();
  
  virtual float GetSize(int index);
};

struct Circle : public Shape
{
  Vector3 up;
  Vector3 right;
  float radius;

  Circle();
  Circle(Circle const &aCircle);
  Circle(Vector3 const &aPosition, Vector3 const &aUp, Vector3 const &aRight, float const aRadius);
  virtual ~Circle();
  
  virtual float GetSize(int index);
};

struct Line : public Shape
{
  Vector3 direction;
  float length;

  Line();
  Line(Line const &aLine);
  Line(Vector3 const &aPosition, Vector3 const &aDirection, float aLength);
  Line(Vector3 const &aStart, Vector3 const &aEnd);
  virtual ~Line();

  bool GetCollisions(Line const &aCompare, Circle &aOutput);
  
  virtual float GetSize(int index);
};

struct Triangle : public Shape
{
  Vector3 points[3];
  
  Triangle();
  Triangle(Triangle const &aTriangle);
  Triangle(Vector3 const &aPoint1, Vector3 const &aPoint2, Vector3 const &aPoint3);
  Triangle(Vector3 const aPoints[3]);
  Triangle(std::vector<Vector3> const &aPoints);
  virtual ~Triangle();
  
  Vector3 GetPoint(int index);
  virtual float GetSize(int index);
};

struct Plane : public Shape
{
  Vector3 normal;
  float distance;
  
  Plane();
  Plane(Plane const &aPlane);
  Plane(Vector3 const &aNormal, float const aDistance);
  virtual ~Plane();
  
  Vector3 GetNormal();
  float GetDistance();
  virtual float GetSize(int index);
};

void print_line(Line const &_line);
void print_circle(Circle const &_circle);

#endif
