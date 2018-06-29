#include "Shape.h"
#include "LUATypes.h"

//------------------------------
// SHAPE
//------------------------------
Shape::Shape(ShapeType aShape) : id(0), position(), passable(false), shape(aShape)
{
}

Shape::~Shape()
{
}

void Shape::SerializeLUA()
{
  SLB::Class<Shape, SLB::Instance::NoCopy>("Shape")
      .property("id", &Shape::id)
      .property("position", &Shape::position)
      .property("passable", &Shape::passable);
}

//------------------------------
// AXISALIGNEDBOUNDINGBOX
//------------------------------
AxisAlignedBoundingBox::AxisAlignedBoundingBox() : Shape(AABB), size()
{
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(Vector3 const &aPosition, Vector3 const &aSize) : Shape(AABB), size(aSize)
{
  position = aPosition;
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
{
}

/**
 * @brief See if point collides with box. 3D space.
 * @param aPosition Point to check
 * @return True if collided.
 */
bool AxisAlignedBoundingBox::Get3DCollision(Vector3 const &aPosition)
{
  // SEPARATING AXIS THEORUM
  Vector3 dist = aPosition - position;
  for (int i = 0; i <= 3; ++i)
  {
    if (fabs(dist[i]) > size[i])
    {
      return false;
    }
  }
  return true;
}

/**
 * @brief See if point collides with box. 2D space.
 * @param aPosition Point to check
 * @return True if collided.
 */
bool AxisAlignedBoundingBox::Get2DCollision(Vector3 const &aPosition)
{
  // SEPARATING AXIS THEORUM
  Vector3 dist = aPosition - position;
  for (int i = 0; i < 2; ++i)
  {
    if (fabs(dist[i]) > size[i])
    {
      return false;
    }
  }
  return true;
}

/**
 * @brief Get box size
 * @param index Axis to check (x = 0, y = 1, z = 2)
 * @return Size on axis
 */
float AxisAlignedBoundingBox::GetSize(int index)
{
  return size[index];
}

//------------------------------
// OrientedBoundingBox
//------------------------------
OrientedBoundingBox::OrientedBoundingBox() : Shape(OBB), up(), right(), forward(), extents()
{
}

OrientedBoundingBox::OrientedBoundingBox(Vector3 const &aPosition, Vector3 const &aUp, Vector3 const &aRight,
  Vector3 const &aForward, Vector3 const &aExtents) : Shape(OBB), up(aUp), right(aRight), forward(aForward), extents(aExtents)
{
  position = aPosition;
}
OrientedBoundingBox::~OrientedBoundingBox()
{
}
  
Vector3 OrientedBoundingBox::GetAxis(int index)
{
  switch(index)
  {
    case 0:
      return right;
      break;
    case 1:
      return up;
      break;
    case 2:
      return forward;
      break;
    default:
      assert(!"Wrong value handed into GetAxis for OBB");
      break;
  }
}

float OrientedBoundingBox::GetSize(int index)
{
  return extents[index];
}

//------------------------------
// SPHERE
//------------------------------
Sphere::Sphere() : Shape(SPHERE), radius(0)
{
}

Sphere::Sphere(Vector3 const &aPosition, float const aRadius) :
    Shape(SPHERE), radius(aRadius)
{
  position = aPosition;
}

Sphere::~Sphere()
{
}

/**
 * @brief Get size of sphere
 * @param index Does nothing
 * @return Size of sphere (radius)
 */
float Sphere::GetSize(int index)
{
  return radius;
}

//------------------------------
// CIRCLE
//------------------------------

Circle::Circle() :
    Shape(CIRCLE), up(), right(), radius(0)
{
}

Circle::Circle(Vector3 const &aPosition, Vector3 const &aUp,
    Vector3 const &aRight, float const aRadius) :
    Shape(CIRCLE), up(aUp), right(aRight), radius(fabs(aRadius))
{
  position = aPosition;
}

Circle::~Circle()
{
}

/**
 * @brief Get size of circle
 * @param index Does nothing
 * @return Size of circle (radius)
 */
float Circle::GetSize(int index)
{
  return radius;
}

//------------------------------
// LINE
//------------------------------

Line::Line() :
    Shape(LINE), direction(), length(0)
{
}

Line::Line(Vector3 const &aPosition, Vector3 const &aDirection, float aLength) :
    Shape(LINE), direction(aDirection), length(aLength)
{
  position = aPosition;
}

Line::Line(Vector3 const &aStart, Vector3 const &aEnd) :
    Shape(LINE)
{
  position = aStart;
  Vector3 diff = aEnd - aStart;
  direction = diff.normalize();
  length = diff.length();
}

Line::~Line()
{
}

/**
 * @brief Get all possible collisions with another line
 * @param aCompare Line to check
 * @param aOutput Output circle
 * @return If circle is found or not
 */
bool Line::GetCollisions(Line const &aCompare, Circle &aOutput)
{
  Vector3 d = aCompare.position - position;
  float dist = d.length();
  float longer = length >= aCompare.length ? length : aCompare.length;
  float shorter = length >= aCompare.length ? aCompare.length : length;

  print_line(*this);
  print_line(aCompare);

  // If they're in the exact same location that would be a problem
  if (position == aCompare.position)
  {
    // IS THIS THE SAME EXACT SPHERE?!
    if (length == aCompare.length)
    {
      aOutput.position = position;
      aOutput.radius = length;
      aOutput.up = Vector3(0, 1, 0);
      aOutput.right = Vector3(0, 0, 1);
      DebugLogPrint("Same exact sphere, gonna return false\n\n");
    }
    else
    {
      DebugLogPrint("One sphere is inside of another, WAT\n");
      DebugLogPrint("Therefore, cannot touch, gonna return false\n\n");
    }
    print_circle(aOutput);
    DebugLogPrint("\n\n");
    return false;
  }

  // > is an early out
  if (dist < length + aCompare.length && dist != longer - shorter)
  {
    // THE IDEA: Find one point, rotate about arbitrary axis between spheres
    // Reduce the problem to solving a circle
    Vector3 axis = d.normalize();
    float xylength = sqrt(axis.x * axis.x + axis.y * axis.y);
    float xyzlength = axis.length();
    // Don't want to divide by zero
    float templength = xylength > 0.0f ? xylength : 1.0f;
    // Our matrices
    // First, go to xz plane
    float xzvalues[3][3] =
    {
    { axis.x / templength, axis.y / templength, 0 },
    { -axis.y / templength, axis.x / templength, 0 },
    { 0, 0, 1 } };
    // Then, only to z axis
    float zvalues[3][3] =
    {
    { axis.z / xyzlength, 0, -xylength / xyzlength },
    { 0, 1, 0 },
    { xylength / xyzlength, 0, axis.z / xyzlength } };

    // Create identity matrix if we're already on the z plane
    if (xylength <= 0.0f)
    {
      xzvalues[0][0] = 1.0f;
      xzvalues[1][1] = 1.0f;
      xzvalues[1][0] = 0.0f;
      xzvalues[0][1] = 0.0f;
    }

    // Create our matrices and invert them
    Matrix33 xztrans(xzvalues);
    Matrix33 ztrans(zvalues);
    Matrix33 toZAxis = ztrans * xztrans;
    Matrix33 toZAxisInverse = toZAxis.Invert();

    Vector3 comparePos = toZAxis * d;
    dist = comparePos.length();

    // Circle collision function
    float z = ((dist * dist) - (aCompare.length * aCompare.length)
        + (length * length)) / (2.0f * dist);
    float y = sqrt((length * length) - (z * z));

    // Create our circle
    aOutput.position = Vector3(0, 0, z);
    aOutput.up = Vector3(0, 1, 0);
    aOutput.right = Vector3(0, 0, 1);
    aOutput.radius = y;

    aOutput.position = toZAxisInverse * aOutput.position;
    aOutput.position += position;
    aOutput.up = toZAxisInverse * aOutput.up;
    aOutput.right = toZAxisInverse * aOutput.right;
  }
  else if (dist == length + aCompare.length)
  {
    // They barely touch, from the outside
    Vector3 largerPos =
        length >= aCompare.length ? position : aCompare.position;
    Vector3 smallerPos =
        length >= aCompare.length ? aCompare.position : position;
    float smallerLen = length >= aCompare.length ? aCompare.length : length;
    d = smallerPos - largerPos;

    DebugLogPrint("The radii barely touch, will still return true, but radius is zero\n");
    aOutput.position = smallerPos - (d.normalize() * smallerLen);
    aOutput.radius = 0;
    aOutput.up = Vector3(0, 1, 0);
    aOutput.right = Vector3(0, 0, 1);
  }
  else if (dist == longer - shorter)
  {
    // They barely touch, from the inside
    Vector3 largerPos =
        length >= aCompare.length ? position : aCompare.position;
    Vector3 smallerPos =
        length >= aCompare.length ? aCompare.position : position;
    float smallerLen = length >= aCompare.length ? aCompare.length : length;
    d = smallerPos - largerPos;

    DebugLogPrint("The radii barely touch, will still return true, but radius is zero\n");
    aOutput.position = smallerPos + (d.normalize() * smallerLen);
    aOutput.radius = 0;
    aOutput.up = Vector3(0, 1, 0);
    aOutput.right = Vector3(0, 0, 1);
  }
  else
  {
    DebugLogPrint("These LineSegments can never touch. Returning false.\n\n");
    return false;
  }

  print_circle(aOutput);
  DebugLogPrint("\n\n");
  return true;
}

/**
 * @brief Get length of line
 * @param index Does nothing
 * @return Length
 */
float Line::GetSize(int index)
{
  return length;
}

//------------------------------
// TRIANGLE
//------------------------------
Triangle::Triangle() : Shape(TRIANGLE)
{
  points[0] = Vector3();
  points[1] = Vector3();
  points[2] = Vector3();
}

Triangle::Triangle(Vector3 const &aPoint1, Vector3 const &aPoint2, Vector3 const &aPoint3) : Shape(TRIANGLE)
{
  points[0] = aPoint1;
  points[1] = aPoint2;
  points[2] = aPoint3;
}

Triangle::Triangle(Vector3 const aPoints[3]) : Triangle(aPoints[0], aPoints[1], aPoints[2])
{
}

Triangle::Triangle(std::vector<Vector3> const &aPoints) : Triangle(aPoints[0], aPoints[1], aPoints[2])
{
}

Triangle::~Triangle()
{
}

/**
 * @brief Get point by index
 * @param index Point index
 * @return Point at index
 */
Vector3 Triangle::GetPoint(int index)
{
  return points[index];
}

/**
 * @brief Get max value along axis
 * @param index Axis (x = 0, y = 1, z = 2)
 * @return Size along axis
 */
float Triangle::GetSize(int index)
{
  float highest = -1000000.0f;
  float lowest = 1000000.0f;
  
  for(int i = 0; i < 3; ++i)
  {
    float value = points[i][index];
    if(value < lowest)
      lowest = value;
    if(value > highest)
      highest = value;
  }
  
  return fabs(highest - lowest);
}

//------------------------------
// PLANE
//------------------------------
Plane::Plane() : Shape(PLANE), normal(1,0,0), distance()
{
}

Plane::Plane(Vector3 const &aNormal, float const aDistance) : Shape(PLANE), normal(aNormal), distance(aDistance)
{
}

Plane::~Plane()
{
}
  
Vector3 Plane::GetNormal()
{
  return normal;
}

float Plane::GetDistance()
{
  return distance;
}

float Plane::GetSize(int index)
{
  return distance;
}

//------------------------------
// PRINTING
//------------------------------
/**
 * @brief Print line info
 * @param _line Line to print
 */
void print_line(Line const &_line)
{
  DebugLogPrint("LINE:\n");
  DebugLogPrint("POSITION:\n");
  print_vector(_line.position);
  DebugLogPrint("LENGTH: %f\n", _line.length);
}

/**
 * @brief Print circle info
 * @param _circle Circle to print
 */
void print_circle(Circle const &_circle)
{
  DebugLogPrint("\n");
  DebugLogPrint("-------------------------\n");
  DebugLogPrint("RESULTS:\n");
  DebugLogPrint("-------------------------\n");
  DebugLogPrint("CIRCLE:\n");
  DebugLogPrint("POSITION:\n");
  print_vector(_circle.position);
  DebugLogPrint("UP:\n");
  print_vector(_circle.up);
  DebugLogPrint("RIGHT:\n");
  print_vector(_circle.right);
  DebugLogPrint("RADIUS: %f\n", _circle.radius);
}