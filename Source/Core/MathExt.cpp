#include "MathExt.h"
#include "LuaIncludes.h"
#include <assert.h>

// Helpful print functions
void print_matrix(Matrix33 const &_matrix)
{
  printf("MATRIX:\n");
  for (int i = 0; i < 3; ++i)
  {
    printf("ROW%d:", i);
    for (int j = 0; j < 3; ++j)
    {
      printf(" %f ", _matrix.values[i][j]);
    }
    printf("\n");
  }
  printf("DETERMINANT: %f\n", _matrix.Determinant());
}

void print_vector(Vector3 const &_point)
{
  printf("VECTOR:\n");
  printf("X: %f, Y: %f, Z: %f\n", _point.x, _point.y, _point.z);
}

void print_line(Line const &_line)
{
  printf("LINE:\n");
  printf("POSITION:\n");
  print_vector(_line.position);
  printf("LENGTH: %f\n", _line.length);
}

void print_circle(Circle const &_circle)
{
  printf("\n");
  printf("-------------------------\n");
  printf("RESULTS:\n");
  printf("-------------------------\n");
  printf("CIRCLE:\n");
  printf("POSITION:\n");
  print_vector(_circle.position);
  printf("UP:\n");
  print_vector(_circle.up);
  printf("RIGHT:\n");
  print_vector(_circle.right);
  printf("RADIUS: %f\n", _circle.radius);
}

//--------------------------------
// VECTOR3
//--------------------------------
Vector3::Vector3() : x(0), y(0), z(0)
{
}
Vector3::Vector3(float aX, float aY, float aZ) : x(aX), y(aY), z(aZ)
{
}

float Vector3::length() const
{
  return sqrt((x * x) + (y * y) + (z * z));
}

float& Vector3::operator[](int const aValue)
{
  switch (aValue)
  {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  default:
    assert(0);
  }
}

Vector3 Vector3::normalize() const
{
  Vector3 ret = *this;
  float const len = length();

  // Catch case if length is 0
  if (len == 0)
    return Vector3(1, 0, 0);

  ret.x /= len;
  ret.y /= len;
  ret.z /= len;

  return ret;
}

// Equivalence
void Vector3::operator=(Vector3 const &rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
}
bool Vector3::operator==(Vector3 const &rhs) const
{
  return ((int) x == (int) rhs.x && (int) y == (int) rhs.y
      && (int) z == (int) rhs.z);
}

// Scaling
Vector3 Vector3::operator*(float const aMultiplier) const
{
  Vector3 temp = *this;

  temp.x *= aMultiplier;
  temp.y *= aMultiplier;
  temp.z *= aMultiplier;

  return temp;
}
Vector3 Vector3::operator/(float const aMultiplier) const
{
  Vector3 temp = *this;

  temp.x /= aMultiplier;
  temp.y /= aMultiplier;
  temp.z /= aMultiplier;

  return temp;
}

// Operators with other vectors
float Vector3::operator*(Vector3 const &rhs) const
{
  return (x * rhs.x + y * rhs.y + z * rhs.z);
}
Vector3 Vector3::operator^(Vector3 const &rhs) const
{
  Vector3 temp = *this;

  temp.x = y * rhs.z - z * rhs.y;
  temp.y = z * rhs.x - x * rhs.z;
  temp.z = x * rhs.y - y * rhs.x;

  return temp;
}
Vector3 Vector3::operator+(Vector3 const &rhs) const
{
  Vector3 temp = *this;

  temp.x += rhs.x;
  temp.y += rhs.y;
  temp.z += rhs.z;

  return temp;
}
Vector3 Vector3::operator-(Vector3 const &rhs) const
{
  Vector3 temp = *this;

  temp.x -= rhs.x;
  temp.y -= rhs.y;
  temp.z -= rhs.z;

  return temp;
}
Vector3 Vector3::operator%(Vector3 const &rhs) const
{
  return rhs * ((*this * rhs) / (rhs * rhs));
}
Vector3 Vector3::operator-() const
{
  return (*this * -1.0f);
}

float Vector3::Dot(Vector3 const &rhs) const
{
  return *this * rhs;
}
Vector3 Vector3::Cross(Vector3 const &rhs) const
{
  return *this ^ rhs;
}
Vector3 Vector3::Add(Vector3 const &rhs) const
{
  return *this + rhs;
}
Vector3 Vector3::Subtract(Vector3 const &rhs) const
{
  return *this - rhs;
}
Vector3 Vector3::Project(Vector3 const &rhs) const
{
  return *this % rhs;
}
Vector3 Vector3::Invert() const
{
  return -(*this);
}

void Vector3::operator+=(Vector3 const &rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
}
void Vector3::operator-=(Vector3 const &rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
}
void Vector3::operator*=(float const aMultiplier)
{
  x *= aMultiplier;
  y *= aMultiplier;
  z *= aMultiplier;
}
void Vector3::operator/=(float const aMultiplier)
{
  x /= aMultiplier;
  y /= aMultiplier;
  z /= aMultiplier;
}

void Vector3::SerializeLUA()
{
  SLB::Class<Vector3>("Vector3").constructor<float, float, float>()
      .set("x", &Vector3::x)
      .set("y", &Vector3::y)
      .set("z", &Vector3::z)
      .set("length", &Vector3::length)
      .set("SetEqual", &Vector3::operator=)
      .set("IsEqual", &Vector3::operator==)
      .set("Add", &Vector3::Add)
      .set("Subtract", &Vector3::Subtract)
      .set("Dot", &Vector3::Dot)
      .set("Cross", &Vector3::Cross)
      .set("Project", &Vector3::Project)
      .set("Invert", &Vector3::Invert);
}

std::string Vector3::GetName()
{
  return "Vector3";
}

//------------------------------
// VECTOR4
//------------------------------
Vector4::Vector4() :
    x(0), y(0), z(0), w(0)
{
}
Vector4::Vector4(float aX, float aY, float aZ, float aW) :
    x(aX), y(aY), z(aZ), w(aW)
{
}

float Vector4::length() const
{
  return sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

float Vector4::operator[](int aValue) const
{
  switch (aValue)
  {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  case 3:
    return w;
  default:
    assert(0);
  }
}

Vector4 Vector4::normalize() const
{
  Vector4 ret = *this;
  float const len = length();

  // Catch case if length is 0
  if (len == 0)
    return Vector4(1, 0, 0, 0);

  ret.x /= len;
  ret.y /= len;
  ret.z /= len;
  ret.w /= len;

  return ret;
}

// Equivalence
void Vector4::operator=(Vector4 const &rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
}
bool Vector4::operator==(Vector4 const &rhs) const
{
  return ((int) x == (int) rhs.x && (int) y == (int) rhs.y
      && (int) z == (int) rhs.z && (int) w == (int) rhs.w);
}

// Scaling
Vector4 Vector4::operator*(float const aMultiplier) const
{
  Vector4 temp = *this;

  temp.x *= aMultiplier;
  temp.y *= aMultiplier;
  temp.z *= aMultiplier;
  temp.w *= aMultiplier;

  return temp;
}
Vector4 Vector4::operator/(float const aMultiplier) const
{
  Vector4 temp = *this;

  temp.x /= aMultiplier;
  temp.y /= aMultiplier;
  temp.z /= aMultiplier;
  temp.w /= aMultiplier;

  return temp;
}

// Operators with other vectors
float Vector4::operator*(Vector4 const &rhs) const
{
  return (x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w);
}
Vector3 Vector4::operator^(Vector4 const &rhs) const
{
  Vector3 temp = Vector3(x, y, z);

  temp.x = y * rhs.z - z * rhs.y;
  temp.y = z * rhs.x - x * rhs.z;
  temp.z = x * rhs.y - y * rhs.x;

  return temp;
}
Vector4 Vector4::operator+(Vector4 const &rhs) const
{
  Vector4 temp = *this;

  temp.x += rhs.x;
  temp.y += rhs.y;
  temp.z += rhs.z;
  temp.w += rhs.w;

  return temp;
}
Vector4 Vector4::operator-(Vector4 const &rhs) const
{
  Vector4 temp = *this;

  temp.x -= rhs.x;
  temp.y -= rhs.y;
  temp.z -= rhs.z;
  temp.w -= rhs.w;

  return temp;
}
Vector4 Vector4::operator%(Vector4 const &rhs) const
{
  return rhs * ((*this * rhs) / (rhs * rhs));
}
Vector4 Vector4::operator-() const
{
  return (*this * -1.0f);
}

float Vector4::Dot(Vector4 const &rhs) const
{
  return *this * rhs;
}
Vector3 Vector4::Cross(Vector4 const &rhs) const
{
  return *this ^ rhs;
}
Vector4 Vector4::Add(Vector4 const &rhs) const
{
  return *this + rhs;
}
Vector4 Vector4::Subtract(Vector4 const &rhs) const
{
  return *this - rhs;
}
Vector4 Vector4::Project(Vector4 const &rhs) const
{
  return *this % rhs;
}
Vector4 Vector4::Invert() const
{
  return -(*this);
}

void Vector4::operator+=(Vector4 const &rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  w += rhs.w;
}
void Vector4::operator-=(Vector4 const &rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  w -= rhs.w;
}
void Vector4::operator*=(float const aMultiplier)
{
  x *= aMultiplier;
  y *= aMultiplier;
  z *= aMultiplier;
  w *= aMultiplier;
}
void Vector4::operator/=(float const aMultiplier)
{
  x /= aMultiplier;
  y /= aMultiplier;
  z /= aMultiplier;
  w /= aMultiplier;
}

void Vector4::SerializeLUA()
{
  SLB::Class<Vector4>("Vector4").constructor<float, float, float, float>()
      .set("x", &Vector4::x)
      .set("y", &Vector4::y)
      .set("z", &Vector4::z)
      .set("w", &Vector4::w)
      .set("length", &Vector4::length)
      .set("SetEqual", &Vector4::operator=)
      .set("IsEqual", &Vector4::operator==)
      .set("Add", &Vector4::Add)
      .set("Subtract", &Vector4::Subtract)
      .set("Dot", &Vector4::Dot)
      .set("Cross", &Vector4::Cross)
      .set("Project", &Vector4::Project)
      .set("Invert", &Vector4::Invert);
}

std::string Vector4::GetName()
{
  return "Vector4";
}

//-----------------------------
// MATRIX33
//-----------------------------

Matrix33::Matrix33()
{
  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      if (i == j)
        values[i][j] = 1;
      else
        values[i][j] = 0;
    }
  }
}

Matrix33::Matrix33(float aValues[3][3])
{
  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      values[i][j] = aValues[i][j];
    }
  }
}

Matrix33::Matrix33(float aValues[9])
{
  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      values[i][j] = aValues[(i * 3) + j];
    }
  }
}

Matrix33 Matrix33::Concatenate(Matrix33 const &rhs) const
{
  return *this * rhs;
}

Matrix33 Matrix33::Rotate(Vector3 const &aAxis, float const aAngle) const
{
  // THE IDEA:
  // Rotate to xz plane
  // Rotate to z axis
  // Rotate about z axis
  // Rotate by inverse z axis transform
  // Rotate by inverse xz plane transform
  float angle = aAngle * DEGREE_TO_RADS;
  float cosine = cos(angle);
  float sine = sin(angle);
  float xylength = sqrt(aAxis.x * aAxis.x + aAxis.y * aAxis.y);
  float xyzlength = aAxis.length();

  // What if we're already on the z plane?
  float templength = xylength > 0.0f ? xylength : 1.0f;

  float xzvalues[3][3] =
  {
  { aAxis.x / templength, aAxis.y / templength, 0 },
  { -aAxis.y / templength, aAxis.x / templength, 0 },
  { 0, 0, 1 } };
  float zvalues[3][3] =
  {
  { aAxis.z / xyzlength, 0, -xylength / xyzlength },
  { 0, 1, 0 },
  { xylength / xyzlength, 0, aAxis.z / xyzlength } };
  float rotvalues[3][3] =
  {
  { cosine, -sine, 0 },
  { sine, cosine, 0 },
  { 0, 0, 1 } };

  // Create identity matrix if on z plane already
  if (xylength <= 0.0f)
  {
    xzvalues[0][0] = 1.0f;
    xzvalues[1][1] = 1.0f;
  }

  Matrix33 ret;
  Matrix33 xztrans(xzvalues);
  Matrix33 ztrans(zvalues);
  Matrix33 xzinvert = xztrans.Invert();
  Matrix33 zinvert = ztrans.Invert();
  Matrix33 rotation(rotvalues);

  ret = xzinvert * zinvert * rotation * ztrans * xztrans;

  return ret;
}

Matrix33 Matrix33::Invert() const
{
  Matrix33 ret;
  float det = Determinant();

  ret.values[0][0] = Determinant(values[1][1], values[1][2], values[2][1],
      values[2][2]);
  ret.values[0][1] = Determinant(values[0][2], values[0][1], values[2][2],
      values[2][1]);
  ret.values[0][2] = Determinant(values[0][1], values[0][2], values[1][1],
      values[1][2]);

  ret.values[1][0] = Determinant(values[1][2], values[1][0], values[2][2],
      values[2][0]);
  ret.values[1][1] = Determinant(values[0][0], values[0][2], values[2][0],
      values[2][2]);
  ret.values[1][2] = Determinant(values[0][2], values[0][0], values[1][2],
      values[1][0]);

  ret.values[2][0] = Determinant(values[1][0], values[1][1], values[2][0],
      values[2][1]);
  ret.values[2][1] = Determinant(values[0][1], values[0][0], values[2][1],
      values[2][0]);
  ret.values[2][2] = Determinant(values[0][0], values[0][1], values[1][0],
      values[1][1]);

  return ret * (1.0f / det);
}

float Matrix33::Determinant() const
{
  return values[0][0]
      * ((values[1][1] * values[2][2]) - (values[1][2] * values[2][1]))
      - values[0][1]
          * ((values[1][0] * values[2][2]) - (values[1][2] * values[2][0]))
      + values[0][2]
          * ((values[1][0] * values[2][1]) - (values[1][1] * values[2][0]));
}

float Matrix33::Determinant(float const aA, float const aB, float const aC,
    float const aD) const
{
  return (aA * aD) - (aB * aC);
}

Matrix33 Matrix33::operator*(Matrix33 const &rhs) const
{
  Matrix33 ret;

  ret.values[0][0] = values[0][0] * rhs.values[0][0]
      + values[0][1] * rhs.values[1][0] + values[0][2] * rhs.values[2][0];
  ret.values[0][1] = values[0][0] * rhs.values[0][1]
      + values[0][1] * rhs.values[1][1] + values[0][2] * rhs.values[2][1];
  ret.values[0][2] = values[0][0] * rhs.values[0][2]
      + values[0][1] * rhs.values[1][2] + values[0][2] * rhs.values[2][2];

  ret.values[1][0] = values[1][0] * rhs.values[0][0]
      + values[1][1] * rhs.values[1][0] + values[1][2] * rhs.values[2][0];
  ret.values[1][1] = values[1][0] * rhs.values[0][1]
      + values[1][1] * rhs.values[1][1] + values[1][2] * rhs.values[2][1];
  ret.values[1][2] = values[1][0] * rhs.values[0][2]
      + values[1][1] * rhs.values[1][2] + values[1][2] * rhs.values[2][2];

  ret.values[2][0] = values[2][0] * rhs.values[0][0]
      + values[2][1] * rhs.values[1][0] + values[2][2] * rhs.values[2][0];
  ret.values[2][1] = values[2][0] * rhs.values[0][1]
      + values[2][1] * rhs.values[1][1] + values[2][2] * rhs.values[2][1];
  ret.values[2][2] = values[2][0] * rhs.values[0][2]
      + values[2][1] * rhs.values[1][2] + values[2][2] * rhs.values[2][2];

  return ret;
}

Matrix33 Matrix33::operator*(float const aValue) const
{
  Matrix33 ret = *this;

  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      ret.values[i][j] *= aValue;
    }
  }

  return ret;
}

Vector3 Matrix33::operator*(Vector3 const &rhs) const
{
  Vector3 ret;

  ret.x = values[0][0] * rhs.x + values[0][1] * rhs.y + values[0][2] * rhs.z;
  ret.y = values[1][0] * rhs.x + values[1][1] * rhs.y + values[1][2] * rhs.z;
  ret.z = values[2][0] * rhs.x + values[2][1] * rhs.y + values[2][2] * rhs.z;

  return ret;
}

void Matrix33::operator*=(Matrix33 const &rhs)
{
  *this = *this * rhs;
}

void Matrix33::operator*=(float const aValue)
{
  *this = *this * aValue;
}

//------------------------------
// SHAPE
//------------------------------
Shape::Shape(ShapeType aShape) : position(), shape(aShape)
{
}

Shape::~Shape()
{
}

//------------------------------
// CUBE
//------------------------------
Cube::Cube() : Shape(CUBE)
{
}

Cube::Cube(Vector3 const &aPosition, Vector3 const &aSize) : 
    Shape(CUBE), size(aSize)
{
  position = aPosition;
}

Cube::~Cube()
{
}

bool Cube::Get3DCollision(Vector3 const &aPosition)
{
  // SEPARATING AXIS THEORUM
  Vector3 dist = aPosition - position;
  for (int i = 0; i <= 2; ++i)
  {
    if (fabs(dist[i]) > size[i])
    {
      return false;
    }
  }
  return true;
}

bool Cube::Get2DCollision(Vector3 const &aPosition)
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

float Cube::GetSize(int index)
{
  return size[index];
}

//------------------------------
// SPHERE
//------------------------------
Sphere::Sphere() : Shape(SPHERE)
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

bool Sphere::GetCollision(Vector3 const &aPosition)
{
  float dist = (aPosition - position).length();
  return dist <= radius;
}

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

// Returns true is circle is found, otherwise false
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
      printf("Same exact sphere, gonna return false\n\n");
    }
    else
    {
      printf("One sphere is inside of another, WAT\n");
      printf("Therefore, cannot touch, gonna return false\n\n");
    }
    print_circle(aOutput);
    printf("\n\n");
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

    printf(
        "The radii barely touch, will still return true, but radius is zero\n");
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

    printf(
        "The radii barely touch, will still return true, but radius is zero\n");
    aOutput.position = smallerPos + (d.normalize() * smallerLen);
    aOutput.radius = 0;
    aOutput.up = Vector3(0, 1, 0);
    aOutput.right = Vector3(0, 0, 1);
  }
  else
  {
    printf("These LineSegments can never touch. Returning false.\n\n");
    return false;
  }

  print_circle(aOutput);
  printf("\n\n");
  return true;
}

float Line::GetSize(int index)
{
  return length;
}
