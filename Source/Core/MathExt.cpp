#include "MathExt.h"
#include "LuaIncludes.h"
#include <assert.h>

// Helpful print functions
void print_matrix(Matrix33 const &_matrix)
{
  DebugLogPrint("MATRIX:\n");
  for (int i = 0; i < 3; ++i)
  {
    DebugLogPrint("ROW%d:", i);
    for (int j = 0; j < 3; ++j)
    {
      DebugLogPrint(" %f ", _matrix.values[i][j]);
    }
    DebugLogPrint("\n");
  }
  DebugLogPrint("DETERMINANT: %f\n", _matrix.Determinant());
}

void print_vector(Vector3 const &_point)
{
  DebugLogPrint("VECTOR:\n");
  DebugLogPrint("X: %f, Y: %f, Z: %f\n", _point.x, _point.y, _point.z);
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
    assert(!"Invalid value passed into Vector3 value retrieval");
  }
}

float Vector3::GetValue(int const aValue) const
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
    assert(!"Invalid value passed into Vector3 value retrieval");
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
Vector3 Vector3::Multiply(Vector3 const &rhs) const
{
  Vector3 ret = *this;
  ret *= rhs;
  return ret;
}
Vector3 Vector3::Divide(Vector3 const &rhs) const
{
  Vector3 ret = *this;
  ret /= rhs;
  return ret;
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
void Vector3::operator*=(Vector3 const &rhs)
{
  x *= rhs.x;
  y *= rhs.y;
  z *= rhs.z;
}
void Vector3::operator/=(Vector3 const &rhs)
{
  x /= rhs.x;
  y /= rhs.y;
  z /= rhs.z;
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

bool Vector3::validate() const
{
  if(std::isnan(x) || std::isnan(y) || std::isnan(z))
    assert(!"Vector3 has failed validation");
  return true;
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
      .set("Multiply", &Vector3::Multiply)
      .set("Divide", &Vector3::Divide)
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
Vector4::Vector4(Vector3 const &aVector3) :
    x(aVector3.x), y(aVector3.y), z(aVector3.z), w(1.0f)
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
    assert(!"Invalid value passed into Vector4 value retrieval");
  }
}

float Vector4::GetValue(int const aValue) const
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
    assert(!"Invalid value passed into Vector4 value retrieval");
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

bool Vector4::validate() const
{
  if(std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w))
    assert(!"Vector4 has failed validation");
  return true;
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

void Matrix33::operator=(Matrix33 const &aMatrix)
{
  for(int i = 0; i < 3; ++i)
  {
    for(int j = 0; j < 3; ++j)
    {
      values[i][j] = aMatrix.values[i][j];
    }
  }
}

bool Matrix33::operator==(Matrix33 const &aMatrix) const
{
  for(int i = 0; i < 3; ++i)
  {
    for(int j = 0; j < 3; ++j)
    {
      if(values[i][j] != aMatrix.values[i][j])
        return false;
    }
  }
  return true;
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

void Matrix33::RotateX(float const aAngle)
{
  (*this) = Rotate(Vector3(1,0,0), aAngle);
}

void Matrix33::RotateY(float const aAngle)
{
  (*this) = Rotate(Vector3(0,1,0), aAngle);
}

void Matrix33::RotateZ(float const aAngle)
{
  (*this) = Rotate(Vector3(0,0,1), aAngle);
}

void Matrix33::operator*=(Matrix33 const &rhs)
{
  *this = *this * rhs;
}

void Matrix33::operator*=(float const aValue)
{
  *this = *this * aValue;
}

bool Matrix33::validate() const
{
  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      if(std::isnan(values[i][j]))
        assert(!"Matrix33 has failed validation.");
    }
  }
  return true;
}

void Matrix33::SerializeLUA()
{
  SLB::Class<Matrix33>("Matrix33")
      .set("Concatenate", &Matrix33::Concatenate)
      .set("Rotate", &Matrix33::Rotate)
      .set("RotateX", &Matrix33::RotateX)
      .set("RotateY", &Matrix33::RotateY)
      .set("RotateZ", &Matrix33::RotateZ)
      .set("Invert", &Matrix33::Invert);
}

std::string Matrix33::GetName()
{
  return "Matrix33";
}
