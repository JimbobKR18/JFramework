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
// Vector2
//--------------------------------
Vector2::Vector2() : x(0), y(0)
{
}
Vector2::Vector2(float aX, float aY) : x(aX), y(aY)
{
}

float Vector2::length() const
{
  return sqrt((x * x) + (y * y));
}

float& Vector2::operator[](int const aValue)
{
  switch (aValue)
  {
    case 0:
      return x;
    case 1:
      return y;
    default:
      assert(!"Invalid value passed into Vector2 value retrieval");
  }
}

float Vector2::GetValue(int const aValue) const
{
  switch (aValue)
  {
    case 0:
      return x;
    case 1:
      return y;
    default:
      assert(!"Invalid value passed into Vector2 value retrieval");
  }
}

Vector2 Vector2::normalize() const
{
  Vector2 ret = *this;
  float const len = length();
  
  // Catch case if length is 0
  if (len == 0)
    return Vector2(1, 0);
  
  ret.x /= len;
  ret.y /= len;
  
  return ret;
}

Vector2 Vector2::abs() const
{
  Vector2 ret;
  ret.x = fabs(x);
  ret.y = fabs(y);
  return ret;
}

// Equivalence
void Vector2::operator=(Vector2 const &rhs)
{
  x = rhs.x;
  y = rhs.y;
}
bool Vector2::operator==(Vector2 const &rhs) const
{
  return ((int) x == (int) rhs.x && (int) y == (int) rhs.y);
}

// Scaling
Vector2 Vector2::operator*(float const aMultiplier) const
{
  Vector2 temp = *this;
  
  temp.x *= aMultiplier;
  temp.y *= aMultiplier;
  
  return temp;
}
Vector2 Vector2::operator/(float const aMultiplier) const
{
  Vector2 temp = *this;
  
  temp.x /= aMultiplier;
  temp.y /= aMultiplier;
  
  return temp;
}

// Operators with other vectors
float Vector2::operator*(Vector2 const &rhs) const
{
  return (x * rhs.x + y * rhs.y);
}
Vector2 Vector2::operator^(Vector2 const &rhs) const
{
  Vector2 temp = *this;
  
  temp.x = x * rhs.y - y * rhs.x;
  temp.y = y * rhs.x - x * rhs.y;
  
  return temp;
}
Vector2 Vector2::operator+(Vector2 const &rhs) const
{
  Vector2 temp = *this;
  
  temp.x += rhs.x;
  temp.y += rhs.y;
  
  return temp;
}
Vector2 Vector2::operator-(Vector2 const &rhs) const
{
  Vector2 temp = *this;
  
  temp.x -= rhs.x;
  temp.y -= rhs.y;
  
  return temp;
}
Vector2 Vector2::operator%(Vector2 const &rhs) const
{
  return rhs * ((*this * rhs) / (rhs * rhs));
}
Vector2 Vector2::operator-() const
{
  return (*this * -1.0f);
}

float Vector2::Dot(Vector2 const &rhs) const
{
  return *this * rhs;
}
Vector2 Vector2::Cross(Vector2 const &rhs) const
{
  return *this ^ rhs;
}
Vector2 Vector2::Add(Vector2 const &rhs) const
{
  return *this + rhs;
}
Vector2 Vector2::Subtract(Vector2 const &rhs) const
{
  return *this - rhs;
}
Vector2 Vector2::Multiply(Vector2 const &rhs) const
{
  Vector2 ret = *this;
  ret *= rhs;
  return ret;
}
Vector2 Vector2::Divide(Vector2 const &rhs) const
{
  Vector2 ret = *this;
  ret /= rhs;
  return ret;
}
Vector2 Vector2::Project(Vector2 const &rhs) const
{
  return *this % rhs;
}
Vector2 Vector2::Invert() const
{
  return -(*this);
}

void Vector2::operator+=(Vector2 const &rhs)
{
  x += rhs.x;
  y += rhs.y;
}
void Vector2::operator-=(Vector2 const &rhs)
{
  x -= rhs.x;
  y -= rhs.y;
}
void Vector2::operator*=(Vector2 const &rhs)
{
  x *= rhs.x;
  y *= rhs.y;
}
void Vector2::operator/=(Vector2 const &rhs)
{
  x /= rhs.x;
  y /= rhs.y;
}
void Vector2::operator*=(float const aMultiplier)
{
  x *= aMultiplier;
  y *= aMultiplier;
}
void Vector2::operator/=(float const aMultiplier)
{
  x /= aMultiplier;
  y /= aMultiplier;
}

float Vector2::AngleBetweenRadians(Vector2 const &rhs) const
{
  return atan2f(rhs.y,rhs.x) - atan2f(y,x);
}

float Vector2::AngleBetweenDegrees(Vector2 const &rhs) const
{
  return AngleBetweenRadians(rhs) * RADS_TO_DEGREE;
}

std::vector<float> Vector2::ToVector() const
{
  std::vector<float> ret;
  ret.push_back(x);
  ret.push_back(y);
  return ret;
}

HashString Vector2::ToString() const
{
  return Common::FloatVectorToString(ToVector());
}

bool Vector2::validate() const
{
  if(std::isnan(x) || std::isnan(y))
    assert(!"Vector2 has failed validation");
  return true;
}

void Vector2::SerializeLUA()
{
  SLB::Class<Vector2>("Vector2").constructor<float, float>()
      .property("x", &Vector2::x)
      .property("y", &Vector2::y)
      .set("length", &Vector2::length)
      .set("SetEqual", &Vector2::operator=)
      .set("IsEqual", &Vector2::operator==)
      .set("Add", &Vector2::Add)
      .set("Subtract", &Vector2::Subtract)
      .set("Multiply", &Vector2::Multiply)
      .set("Divide", &Vector2::Divide)
      .set("Dot", &Vector2::Dot)
      .set("Cross", &Vector2::Cross)
      .set("Project", &Vector2::Project)
      .set("Invert", &Vector2::Invert);
}

HashString Vector2::GetName()
{
  return "Vector2";
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

Vector3 Vector3::abs() const
{
  Vector3 ret;
  ret.x = fabs(x);
  ret.y = fabs(y);
  ret.z = fabs(z);
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

float Vector3::AngleBetweenRadians(Vector3 const &rhs, Vector3 const &aNormal) const
{
  Vector3 cross = this->Cross(rhs);
  float angle = acosf(this->Dot(rhs) / (length() * rhs.length()));
  
  if(aNormal.Dot(cross) >= 0.0f)
    return angle;
  else
    return -angle;
}

float Vector3::AngleBetweenDegrees(Vector3 const &rhs, Vector3 const &aNormal) const
{
  return AngleBetweenRadians(rhs, aNormal) * RADS_TO_DEGREE;
}

std::vector<float> Vector3::ToVector() const
{
  std::vector<float> ret;
  ret.push_back(x);
  ret.push_back(y);
  ret.push_back(z);
  return ret;
}

HashString Vector3::ToString() const
{
  return Common::FloatVectorToString(ToVector());
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
      .property("x", &Vector3::x)
      .property("y", &Vector3::y)
      .property("z", &Vector3::z)
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

HashString Vector3::GetName()
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

Vector4 Vector4::abs() const
{
  Vector4 ret;
  ret.x = fabs(x);
  ret.y = fabs(y);
  ret.z = fabs(z);
  ret.w = fabs(w);
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

float Vector4::AngleBetweenRadians(Vector4 const &rhs, Vector4 const &aNormal) const
{
  Vector4 cross = this->Cross(rhs);
  float angle = acosf(this->Dot(rhs) / (length() * rhs.length()));
  
  if(aNormal.Dot(cross) >= 0.0f)
    return angle;
  else
    return -angle;
}

float Vector4::AngleBetweenDegrees(Vector4 const &rhs, Vector4 const &aNormal) const
{
  return AngleBetweenRadians(rhs, aNormal) * RADS_TO_DEGREE;
}

std::vector<float> Vector4::ToVector() const
{
  std::vector<float> ret;
  ret.push_back(x);
  ret.push_back(y);
  ret.push_back(z);
  ret.push_back(w);
  return ret;
}

HashString Vector4::ToString() const
{
  return Common::FloatVectorToString(ToVector());
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
      .property("x", &Vector4::x)
      .property("y", &Vector4::y)
      .property("z", &Vector4::z)
      .property("w", &Vector4::w)
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

HashString Vector4::GetName()
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

Matrix33::Matrix33(Vector3 const &aValues)
{
  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      if (i == j)
        values[i][j] = aValues.GetValue(i);
      else
        values[i][j] = 0;
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

void Matrix33::Scale(Vector3 const &aScale)
{
  float values[9] = {aScale.x, 0, 0,
                          0, aScale.y, 0,
                          0, 0, aScale.z};
  Matrix33 scaleMatrix = Matrix33(values);
  (*this) *= scaleMatrix;
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

HashString Matrix33::GetName()
{
  return "Matrix33";
}

//-----------------------------
// MATRIX44
//-----------------------------

Matrix44::Matrix44()
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      if (i == j)
        values[i][j] = 1;
      else
        values[i][j] = 0;
    }
  }
}

Matrix44::Matrix44(float aValues[4][4])
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      values[i][j] = aValues[i][j];
    }
  }
}

Matrix44::Matrix44(float aValues[16])
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      values[i][j] = aValues[(i * 4) + j];
    }
  }
}

Matrix44::Matrix44(Vector4 const &aValues)
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      if (i == j)
        values[i][j] = aValues.GetValue(i);
      else
        values[i][j] = 0;
    }
  }
}

Matrix44 Matrix44::Concatenate(Matrix44 const &rhs) const
{
  return *this * rhs;
}

Matrix44 Matrix44::Rotate(Vector4 const &aAxis, float const aAngle) const
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

  float xzvalues[4][4] =
  {
  { aAxis.x / templength, aAxis.y / templength, 0, 0 },
  { -aAxis.y / templength, aAxis.x / templength, 0, 0 },
  { 0, 0, 1, 0},
  { 0, 0, 0, 1 } };
  float zvalues[4][4] =
  {
  { aAxis.z / xyzlength, 0, -xylength / xyzlength, 0 },
  { 0, 1, 0, 0 },
  { xylength / xyzlength, 0, aAxis.z / xyzlength, 0 },
  { 0, 0, 0, 1} };
  float rotvalues[4][4] =
  {
  { cosine, -sine, 0, 0 },
  { sine, cosine, 0, 0 },
  { 0, 0, 1, 0 },
  { 0, 0, 0, 1 } };

  // Create identity matrix if on z plane already
  if (xylength <= 0.0f)
  {
    xzvalues[0][0] = 1.0f;
    xzvalues[1][1] = 1.0f;
  }

  Matrix44 ret;
  Matrix44 xztrans(xzvalues);
  Matrix44 ztrans(zvalues);
  Matrix44 xzinvert = xztrans.Invert();
  Matrix44 zinvert = ztrans.Invert();
  Matrix44 rotation(rotvalues);

  ret = xzinvert * zinvert * rotation * ztrans * xztrans;

  return ret;
}

Matrix44 Matrix44::Invert() const
{
  Matrix44 ret;
  float det = Determinant();

  ret.values[0][0] = 
    Determinant(values[1][1], values[1][1], values[2][3] * values[3][2], values[2][2] * values[3][3]) +
    Determinant(values[1][2], values[1][2], values[2][1] * values[3][3], values[2][3] * values[3][1]) + 
    Determinant(values[1][3], values[1][3], values[2][2] * values[3][1], values[2][1] * values[3][2]);
  ret.values[0][1] = 
    Determinant(values[0][1], values[0][1], values[2][2] * values[3][3], values[2][3] * values[3][2]) +
    Determinant(values[0][2], values[0][2], values[2][3] * values[3][1], values[2][1] * values[3][3]) + 
    Determinant(values[0][3], values[0][3], values[2][1] * values[3][2], values[2][2] * values[3][1]);
  ret.values[0][2] = 
    Determinant(values[0][1], values[0][1], values[1][3] * values[3][2], values[1][2] * values[3][3]) +
    Determinant(values[0][2], values[0][2], values[1][1] * values[3][3], values[1][3] * values[3][1]) + 
    Determinant(values[0][3], values[0][3], values[1][2] * values[3][1], values[1][1] * values[3][2]);
  ret.values[0][3] = 
    Determinant(values[0][1], values[0][1], values[1][2] * values[2][3], values[1][3] * values[2][2]) +
    Determinant(values[0][2], values[0][2], values[1][3] * values[2][1], values[1][1] * values[2][3]) + 
    Determinant(values[0][3], values[0][3], values[1][1] * values[2][2], values[1][2] * values[2][1]);

  ret.values[1][0] = 
    Determinant(values[1][0], values[1][0], values[2][2] * values[3][3], values[2][3] * values[3][2]) +
    Determinant(values[1][2], values[1][2], values[2][3] * values[3][0], values[2][0] * values[3][3]) + 
    Determinant(values[1][3], values[1][3], values[2][0] * values[3][2], values[2][2] * values[3][0]);
  ret.values[1][1] = 
    Determinant(values[0][0], values[0][0], values[2][3] * values[3][2], values[2][2] * values[3][3]) +
    Determinant(values[0][2], values[0][2], values[2][0] * values[3][3], values[2][3] * values[3][0]) + 
    Determinant(values[0][3], values[0][3], values[2][2] * values[3][0], values[2][0] * values[3][2]);
  ret.values[1][2] = 
    Determinant(values[0][0], values[0][0], values[1][2] * values[3][3], values[1][3] * values[3][2]) +
    Determinant(values[0][2], values[0][2], values[1][3] * values[3][0], values[1][0] * values[3][3]) + 
    Determinant(values[0][3], values[0][3], values[1][0] * values[3][2], values[1][2] * values[3][0]);
  ret.values[1][3] = 
    Determinant(values[0][0], values[0][0], values[1][3] * values[2][2], values[1][2] * values[2][3]) +
    Determinant(values[0][2], values[0][2], values[1][0] * values[2][3], values[1][3] * values[2][0]) + 
    Determinant(values[0][3], values[0][3], values[1][2] * values[2][0], values[1][0] * values[2][2]);

  ret.values[2][0] = 
    Determinant(values[1][0], values[1][0], values[2][3] * values[3][1], values[2][1] * values[3][3]) +
    Determinant(values[1][1], values[1][1], values[2][0] * values[3][3], values[2][3] * values[3][0]) + 
    Determinant(values[1][3], values[1][3], values[2][1] * values[3][0], values[2][0] * values[3][1]);
  ret.values[2][1] = 
    Determinant(values[0][0], values[0][0], values[2][1] * values[3][3], values[2][3] * values[3][1]) +
    Determinant(values[0][1], values[0][1], values[2][3] * values[3][0], values[2][0] * values[3][3]) + 
    Determinant(values[0][3], values[0][3], values[2][0] * values[3][1], values[2][1] * values[3][0]);
  ret.values[2][2] = 
    Determinant(values[0][0], values[0][0], values[1][3] * values[3][1], values[1][1] * values[3][3]) +
    Determinant(values[0][1], values[0][1], values[1][0] * values[3][3], values[1][3] * values[3][0]) + 
    Determinant(values[0][3], values[0][3], values[1][1] * values[3][0], values[1][0] * values[3][1]);
  ret.values[2][3] = 
    Determinant(values[0][0], values[0][0], values[1][1] * values[2][3], values[1][3] * values[2][1]) +
    Determinant(values[0][1], values[0][1], values[1][3] * values[2][0], values[1][0] * values[2][3]) + 
    Determinant(values[0][3], values[0][3], values[1][0] * values[2][1], values[1][1] * values[2][0]);

  ret.values[3][0] = 
    Determinant(values[1][0], values[1][0], values[2][1] * values[3][2], values[2][2] * values[3][1]) +
    Determinant(values[1][1], values[1][1], values[2][2] * values[3][0], values[2][0] * values[3][2]) + 
    Determinant(values[1][2], values[1][2], values[2][0] * values[3][1], values[2][1] * values[3][0]);
  ret.values[3][1] = 
    Determinant(values[0][0], values[0][0], values[2][2] * values[3][1], values[2][1] * values[3][2]) +
    Determinant(values[0][1], values[0][1], values[2][0] * values[3][2], values[2][2] * values[3][0]) + 
    Determinant(values[0][2], values[0][2], values[2][1] * values[3][0], values[2][0] * values[3][1]);
  ret.values[3][2] = 
    Determinant(values[0][0], values[0][0], values[1][1] * values[3][2], values[1][2] * values[3][1]) +
    Determinant(values[0][1], values[0][1], values[1][2] * values[3][0], values[1][0] * values[3][2]) + 
    Determinant(values[0][2], values[0][2], values[1][0] * values[3][1], values[1][1] * values[3][0]);
  ret.values[3][3] = 
    Determinant(values[0][0], values[0][0], values[1][2] * values[2][1], values[1][1] * values[2][2]) +
    Determinant(values[0][1], values[0][1], values[1][0] * values[2][2], values[1][2] * values[2][0]) + 
    Determinant(values[0][2], values[0][2], values[1][1] * values[2][0], values[1][0] * values[2][1]);

  return ret * (1.0f / det);
}

float Matrix44::Determinant() const
{
  return 
    Determinant(values[0][0] * values[1][1], values[0][0] * values[1][1], values[2][3] * values[3][2], values[2][2] * values[3][3]) +
    Determinant(values[0][0] * values[1][2], values[0][0] * values[1][2], values[2][1] * values[3][3], values[2][3] * values[3][1]) +
    Determinant(values[0][0] * values[1][3], values[0][0] * values[1][3], values[2][2] * values[3][1], values[2][1] * values[3][2]) +
    Determinant(values[0][1] * values[1][0], values[0][1] * values[1][0], values[2][2] * values[3][3], values[2][3] * values[3][2]) +
    Determinant(values[0][1] * values[1][2], values[0][1] * values[1][2], values[2][3] * values[3][0], values[2][0] * values[3][3]) +
    Determinant(values[0][1] * values[1][3], values[0][1] * values[1][3], values[2][0] * values[3][2], values[2][2] * values[3][0]) +
    Determinant(values[0][2] * values[1][0], values[0][2] * values[1][0], values[2][3] * values[3][1], values[2][1] * values[3][3]) +
    Determinant(values[0][2] * values[1][1], values[0][2] * values[1][1], values[2][0] * values[3][3], values[2][3] * values[3][0]) +
    Determinant(values[0][2] * values[1][3], values[0][2] * values[1][3], values[2][1] * values[3][0], values[2][0] * values[3][1]) +
    Determinant(values[0][3] * values[1][0], values[0][3] * values[1][0], values[2][1] * values[3][2], values[2][2] * values[3][1]) +
    Determinant(values[0][3] * values[1][1], values[0][3] * values[1][1], values[2][2] * values[3][0], values[2][0] * values[3][2]) +
    Determinant(values[0][3] * values[1][2], values[0][3] * values[1][2], values[2][0] * values[3][1], values[2][1] * values[3][0]);
}

float Matrix44::Determinant(float const aA, float const aB, float const aC,
    float const aD) const
{
  return (aA * aD) - (aB * aC);
}

void Matrix44::operator=(Matrix44 const &aMatrix)
{
  for(int i = 0; i < 4; ++i)
  {
    for(int j = 0; j < 4; ++j)
    {
      values[i][j] = aMatrix.values[i][j];
    }
  }
}

bool Matrix44::operator==(Matrix44 const &aMatrix) const
{
  for(int i = 0; i < 4; ++i)
  {
    for(int j = 0; j < 4; ++j)
    {
      if(values[i][j] != aMatrix.values[i][j])
        return false;
    }
  }
  return true;
}

Matrix44 Matrix44::operator*(Matrix44 const &rhs) const
{
  Matrix44 ret;

  ret.values[0][0] = values[0][0] * rhs.values[0][0] + values[0][1] * rhs.values[1][0] 
      + values[0][2] * rhs.values[2][0] + values[0][3] * rhs.values[3][0];
  ret.values[0][1] = values[0][0] * rhs.values[0][1] + values[0][1] * rhs.values[1][1] 
      + values[0][2] * rhs.values[2][1] + values[0][3] * rhs.values[3][1];
  ret.values[0][2] = values[0][0] * rhs.values[0][2] + values[0][1] * rhs.values[1][2] 
      + values[0][2] * rhs.values[2][2] + values[0][3] * rhs.values[3][2];
  ret.values[0][3] = values[0][0] * rhs.values[0][3] + values[0][1] * rhs.values[1][3] 
      + values[0][2] * rhs.values[2][3] + values[0][3] * rhs.values[3][3];

  ret.values[1][0] = values[1][0] * rhs.values[0][0] + values[1][1] * rhs.values[1][0] 
      + values[1][2] * rhs.values[2][0] + values[1][3] * rhs.values[3][0];
  ret.values[1][1] = values[1][0] * rhs.values[0][1] + values[1][1] * rhs.values[1][1] 
      + values[1][2] * rhs.values[2][1] + values[1][3] * rhs.values[3][1];
  ret.values[1][2] = values[1][0] * rhs.values[0][2] + values[1][1] * rhs.values[1][2] 
      + values[0][2] * rhs.values[2][2] + values[1][3] * rhs.values[3][2];
  ret.values[1][3] = values[1][0] * rhs.values[0][3] + values[1][1] * rhs.values[1][3] 
      + values[1][2] * rhs.values[2][3] + values[1][3] * rhs.values[3][3];

  ret.values[2][0] = values[2][0] * rhs.values[0][0] + values[2][1] * rhs.values[1][0] 
      + values[2][2] * rhs.values[2][0] + values[0][3] * rhs.values[3][0];
  ret.values[2][1] = values[2][0] * rhs.values[0][1] + values[2][1] * rhs.values[1][1] 
      + values[2][2] * rhs.values[2][1] + values[2][3] * rhs.values[3][1];
  ret.values[2][2] = values[2][0] * rhs.values[0][2] + values[2][1] * rhs.values[1][2] 
      + values[2][2] * rhs.values[2][2] + values[2][3] * rhs.values[3][2];
  ret.values[2][3] = values[2][0] * rhs.values[0][3] + values[2][1] * rhs.values[1][3] 
      + values[2][2] * rhs.values[2][3] + values[2][3] * rhs.values[3][3];

  ret.values[3][0] = values[3][0] * rhs.values[0][0] + values[3][1] * rhs.values[1][0] 
      + values[3][2] * rhs.values[2][0] + values[3][3] * rhs.values[3][0];
  ret.values[3][1] = values[3][0] * rhs.values[0][1] + values[3][1] * rhs.values[1][1] 
      + values[3][2] * rhs.values[2][1] + values[3][3] * rhs.values[3][1];
  ret.values[3][2] = values[3][0] * rhs.values[0][2] + values[3][1] * rhs.values[1][2] 
      + values[3][2] * rhs.values[2][2] + values[3][3] * rhs.values[3][2];
  ret.values[3][3] = values[3][0] * rhs.values[0][3] + values[3][1] * rhs.values[1][3] 
      + values[3][2] * rhs.values[2][3] + values[3][3] * rhs.values[3][3];

  return ret;
}

Matrix44 Matrix44::operator*(float const aValue) const
{
  Matrix44 ret = *this;

  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      ret.values[i][j] *= aValue;
    }
  }

  return ret;
}

Vector4 Matrix44::operator*(Vector4 const &rhs) const
{
  Vector4 ret;

  ret.x = values[0][0] * rhs.x + values[0][1] * rhs.y + values[0][2] * rhs.z + values[0][3] * rhs.w;
  ret.y = values[1][0] * rhs.x + values[1][1] * rhs.y + values[1][2] * rhs.z + values[1][3] * rhs.w;
  ret.z = values[2][0] * rhs.x + values[2][1] * rhs.y + values[2][2] * rhs.z + values[2][3] * rhs.w;
  ret.w = values[3][0] * rhs.x + values[3][1] * rhs.y + values[3][2] * rhs.z + values[3][3] * rhs.w;

  return ret;
}

void Matrix44::Scale(Vector4 const &aScale)
{
  float values[16] = {aScale.x, 0, 0, 0,
                          0, aScale.y, 0, 0,
                          0, 0, aScale.z, 0,
                          0, 0, 0, aScale.w};
  Matrix44 scaleMatrix = Matrix44(values);
  (*this) *= scaleMatrix;
}

void Matrix44::RotateX(float const aAngle)
{
  (*this) = Rotate(Vector4(1,0,0,0), aAngle);
}

void Matrix44::RotateY(float const aAngle)
{
  (*this) = Rotate(Vector4(0,1,0,0), aAngle);
}

void Matrix44::RotateZ(float const aAngle)
{
  (*this) = Rotate(Vector4(0,0,1,0), aAngle);
}

void Matrix44::RotateW(float const aAngle)
{
  (*this) = Rotate(Vector4(0,0,0,1), aAngle);
}

void Matrix44::operator*=(Matrix44 const &rhs)
{
  *this = *this * rhs;
}

void Matrix44::operator*=(float const aValue)
{
  *this = *this * aValue;
}

bool Matrix44::validate() const
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      if(std::isnan(values[i][j]))
        assert(!"Matrix44 has failed validation.");
    }
  }
  return true;
}

void Matrix44::SerializeLUA()
{
  SLB::Class<Matrix44>("Matrix44")
      .set("Concatenate", &Matrix44::Concatenate)
      .set("Rotate", &Matrix44::Rotate)
      .set("RotateX", &Matrix44::RotateX)
      .set("RotateY", &Matrix44::RotateY)
      .set("RotateZ", &Matrix44::RotateZ)
      .set("RotateW", &Matrix44::RotateW)
      .set("Invert", &Matrix44::Invert);
}

HashString Matrix44::GetName()
{
  return "Matrix44";
}

//-----------------------------
// QUATERNION
//-----------------------------

Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) 
{
}

Quaternion::Quaternion(float aX, float aY, float aZ, float aW) : x(aX), y(aY), z(aZ), w(aW) 
{
}

Quaternion::Quaternion(float aX, float aY, float aZ)
{
  Set(aX,aY,aZ);
}

Quaternion::Quaternion(Vector3 const& aVec)
{
  Set(aVec.x,aVec.y,aVec.z);
}

Quaternion Quaternion::normalize() const
{
  return *this * sqrtf(x*x + y*y + z*z + w*w);
}

bool Quaternion::operator==(Quaternion const& rhs) const
{
  return ((x == rhs.x) &&
    (y == rhs.y) &&
    (z == rhs.z) &&
    (w == rhs.w));
}

bool Quaternion::operator!=(Quaternion const& rhs) const
{
  return !(*this == rhs);
}

Quaternion& Quaternion::operator=(Quaternion const& rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
  return *this;
}

Quaternion Quaternion::operator*(Quaternion const& rhs) const
{
  Quaternion tmp;

  tmp.w = (rhs.w * w) - (rhs.x * x) - (rhs.y * y) - (rhs.z * z);
  tmp.x = (rhs.w * x) + (rhs.x * w) + (rhs.y * z) - (rhs.z * y);
  tmp.y = (rhs.w * y) + (rhs.y * w) + (rhs.z * x) - (rhs.x * z);
  tmp.z = (rhs.w * z) + (rhs.z * w) + (rhs.x * y) - (rhs.y * x);

  return tmp;
}

Quaternion Quaternion::operator*(float aS) const
{
  return Quaternion(aS*x, aS*y, aS*z, aS*w);
}

Quaternion& Quaternion::operator*=(float aS)
{
  x *= aS;
  y *= aS;
  z *= aS;
  w *= aS;
  return *this;
}

Quaternion& Quaternion::operator*=(Quaternion const& rhs)
{
  return (*this = rhs * (*this));
}

Quaternion Quaternion::operator+(Quaternion const& rhs) const
{
  return Quaternion(x+rhs.x, y+rhs.y, z+rhs.z, w+rhs.w);
}

Vector3 Quaternion::operator*(Vector3 const& aV) const
{
  Vector3 uv, uuv;
  Vector3 qvec(x, y, z);
  uv = qvec.Cross(aV);
  uuv = qvec.Cross(uv);
  uv *= (2.0f * w);
  uuv *= 2.0f;

  return aV + uv + uuv;
}

Matrix44 Quaternion::GetMatrixFast() const
{
  float values[16];
  
  values[0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
  values[1] = 2.0f*x*y + 2.0f*z*w;
  values[2] = 2.0f*x*z - 2.0f*y*w;
  values[3] = 0.0f;

  values[4] = 2.0f*x*y - 2.0f*z*w;
  values[5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
  values[6] = 2.0f*z*y + 2.0f*x*w;
  values[7] = 0.0f;

  values[8] = 2.0f*x*z + 2.0f*y*w;
  values[9] = 2.0f*z*y - 2.0f*x*w;
  values[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
  values[11] = 0.0f;

  values[12] = 0.0f;
  values[13] = 0.0f;
  values[14] = 0.0f;
  values[15] = 1.0f;

  return Matrix44(values);
}

Matrix44 Quaternion::GetMatrix(const Vector3 &aCenter) const
{
  Quaternion q = Quaternion(*this).normalize();
  float x = q.x;
  float y = q.y;
  float z = q.z;
  float w = q.w;

  float values[16];
  values[0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
  values[1] = 2.0f*x*y + 2.0f*z*w;
  values[2] = 2.0f*x*z - 2.0f*y*w;
  values[3] = 0.0f;

  values[4] = 2.0f*x*y - 2.0f*z*w;
  values[5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
  values[6] = 2.0f*z*y + 2.0f*x*w;
  values[7] = 0.0f;

  values[8] = 2.0f*x*z + 2.0f*y*w;
  values[9] = 2.0f*z*y - 2.0f*x*w;
  values[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
  values[11] = 0.0f;

  values[12] = aCenter.x;
  values[13] = aCenter.y;
  values[14] = aCenter.z;
  values[15] = 1.0f;

  return Matrix44(values);
}

Matrix44 Quaternion::GetMatrixCenter(const Vector3 &aCenter,
          const Vector3 &aTranslation) const
{
  Quaternion q = Quaternion(*this).normalize();
  float x = q.x;
  float y = q.y;
  float z = q.z;
  float w = q.w;

  float values[16];
  values[0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
  values[1] = 2.0f*x*y + 2.0f*z*w;
  values[2] = 2.0f*x*z - 2.0f*y*w;
  values[3] = 0.0f;

  values[4] = 2.0f*x*y - 2.0f*z*w;
  values[5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
  values[6] = 2.0f*z*y + 2.0f*x*w;
  values[7] = 0.0f;

  values[8] = 2.0f*x*z + 2.0f*y*w;
  values[9] = 2.0f*z*y - 2.0f*x*w;
  values[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
  values[11] = 0.0f;

  return Matrix44(values);
}

Matrix44 Quaternion::GetMatrixTransposed() const
{
  Matrix44 dest;

  Quaternion q = Quaternion(*this).normalize();
  float x = q.x;
  float y = q.y;
  float z = q.z;
  float w = q.w;

  float values[16];
  values[0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
  values[4] = 2.0f*x*y + 2.0f*z*w;
  values[8] = 2.0f*x*z - 2.0f*y*w;
  values[12] = 0.0f;

  values[1] = 2.0f*x*y - 2.0f*z*w;
  values[5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
  values[9] = 2.0f*z*y + 2.0f*x*w;
  values[13] = 0.0f;

  values[2] = 2.0f*x*z + 2.0f*y*w;
  values[6] = 2.0f*z*y - 2.0f*x*w;
  values[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
  values[14] = 0.0f;

  values[3] = 0.0f;
  values[7] = 0.0f;
  values[11] = 0.0f;
  values[15] = 1.0f;

  return Matrix44(values);
}

Quaternion& Quaternion::Invert()
{
  x = -x; y = -y; z = -z;
  return *this;
}

Quaternion& Quaternion::Set(float aX, float aY, float aZ, float aW)
{
  x = aX;
  y = aY;
  z = aZ;
  w = aW;
  return *this;
}

Quaternion& Quaternion::Set(float aX, float aY, float aZ)
{
  double angle;

  angle = aX * 0.5;
  const double sr = sin(angle);
  const double cr = cos(angle);

  angle = aY * 0.5;
  const double sp = sin(angle);
  const double cp = cos(angle);

  angle = aZ * 0.5;
  const double sy = sin(angle);
  const double cy = cos(angle);

  const double cpcy = cp * cy;
  const double spcy = sp * cy;
  const double cpsy = cp * sy;
  const double spsy = sp * sy;

  x = (float)(sr * cpcy - cr * spsy);
  y = (float)(cr * spcy + sr * cpsy);
  z = (float)(cr * cpsy - sr * spcy);
  w = (float)(cr * cpcy + sr * spsy);

  *this = normalize();
  return *this;
}

Quaternion& Quaternion::Set(Vector3 const& aVec)
{
  return Set(aVec.x, aVec.y, aVec.z);
}

Quaternion& Quaternion::Set(Quaternion const& aQuat)
{
  return (*this=aQuat);
}

bool Quaternion::Equals(Quaternion const& rhs, const float aTolerance) const
{
  return WithinRange<float>( x, rhs.x, aTolerance) &&
    WithinRange<float>( y, rhs.y, aTolerance) &&
    WithinRange<float>( z, rhs.z, aTolerance) &&
    WithinRange<float>( w, rhs.w, aTolerance);
}

Quaternion& Quaternion::Lerp(Quaternion aQ1, Quaternion aQ2, float aTime)
{
  const float scale = 1.0f - aTime;
  return (*this = (aQ1*scale) + (aQ2*aTime));
}

Quaternion& Quaternion::Slerp(Quaternion aQ1, Quaternion aQ2, float aTime, float aThreshold)
{
  float angle = aQ1.Dot(aQ2);

  // make sure we use the short rotation
  if (angle < 0.0f)
  {
    aQ1 *= -1.0f;
    angle *= -1.0f;
  }

  if (angle <= (1 - aThreshold)) // spherical interpolation
  {
    const float theta = acosf(angle);
    const float invsintheta = 1.0f / sinf(theta);
    const float scale = sinf(theta * (1.0f - aTime)) * invsintheta;
    const float invscale = sinf(theta * aTime) * invsintheta;
    return (*this = (aQ1*scale) + (aQ2*invscale));
  }
  else // linear interpolation
    return Lerp(aQ1,aQ2,aTime);
}

float Quaternion::Dot(Quaternion const& aQ2) const
{
  return (x * aQ2.x) + (y * aQ2.y) + (z * aQ2.z) + (w * aQ2.w);
}

Quaternion& Quaternion::FromAngleAxis(float aAngle, Vector3 const& aAxis)
{
  const float fHalfAngle = 0.5f*aAngle;
  const float fSin = sinf(fHalfAngle);
  w = cosf(fHalfAngle);
  x = fSin*aAxis.x;
  y = fSin*aAxis.y;
  z = fSin*aAxis.z;
  return *this;
}

void Quaternion::ToAngleAxis(float &aAngle, Vector3 &aAxis) const
{
  const float scale = sqrtf(x*x + y*y + z*z);

  if (WithinRange<float>(scale, 0, ROUNDING_ERROR) || w > 1.0f || w < -1.0f)
  {
    aAngle = 0.0f;
    aAxis.x = 0.0f;
    aAxis.y = 1.0f;
    aAxis.z = 0.0f;
  }
  else
  {
    const float invscale = 1.0f / scale;
    aAngle = 2.0f * acosf(w);
    aAxis.x = x * invscale;
    aAxis.y = y * invscale;
    aAxis.z = z * invscale;
  }
}

Vector3 Quaternion::ToEuler() const
{
  Vector3 euler;
  const double sqw = w*w;
  const double sqx = x*x;
  const double sqy = y*y;
  const double sqz = z*z;
  const double test = 2.0 * (y*w - x*z);

  if (WithinRange<float>(test, 1.0, 0.000001))
  {
    // heading = rotation about z-axis
    euler.z = (float) (-2.0*atan2(x, w));
    // bank = rotation about x-axis
    euler.x = 0;
    // attitude = rotation about y-axis
    euler.y = (float) (PI/2.0);
  }
  else if (WithinRange<float>(test, -1.0, 0.000001))
  {
    // heading = rotation about z-axis
    euler.z = (float) (2.0*atan2(x, w));
    // bank = rotation about x-axis
    euler.x = 0;
    // attitude = rotation about y-axis
    euler.y = (float) (PI/-2.0);
  }
  else
  {
    // heading = rotation about z-axis
    euler.z = (float) atan2(2.0 * (x*y +z*w),(sqx - sqy - sqz + sqw));
    // bank = rotation about x-axis
    euler.x = (float) atan2(2.0 * (y*z +x*w),(-sqx - sqy + sqz + sqw));
    // attitude = rotation about y-axis
    euler.y = (float) asin(Clamp<double>(test, -1.0, 1.0));
  }
  return euler;
}

Quaternion& Quaternion::Identity()
{
  w = 1.0f;
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
  return *this;
}

Quaternion& Quaternion::RotationFromTo(Vector3 const& aFrom, Vector3 const& aTo)
{
  // Based on Stan Melax's article in Game Programming Gems
  // Copy, since cannot modify local
  Vector3 v0 = aFrom.normalize();
  Vector3 v1 = aTo.normalize();

  const float d = v0.Dot(v1);
  if (d >= 1.0f) // If dot == 1, vectors are the same
  {
    return Identity();
  }
  else if (d <= -1.0f) // exactly opposite
  {
    Vector3 axis(1.0f, 0.0f, 0.0f);
    axis = axis.Cross(v0);
    if (axis.length() == 0)
    {
      axis = Vector3(0.0f,1.0f,0.0f);
      axis = axis.Cross(v0);
    }
    // same as fromAngleAxis(PI, axis).normalize();
    Set(axis.x, axis.y, axis.z, 0);
    *this = this->normalize();
    return *this;
  }

  const float s = sqrtf( (1+d)*2 ); // optimize inv_sqrt
  const float invs = 1.0f / s;
  const Vector3 c = v0.Cross(v1)*invs;
  Set(c.x, c.y, c.z, s * 0.5f);
  *this = this->normalize();
  return *this;
}
