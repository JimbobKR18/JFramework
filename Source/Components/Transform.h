#ifndef __JFramework_Transform_h_
#define __JFramework_Transform_h_

#include "Component.h"
#include "MathExt.h"

class Transform : public Component
{
  private:
    Vector3     mPosition;
    Vector3     mScale;
    Vector3     mSize;
    X_ALIGNMENT mXAlign;
    Y_ALIGNMENT mYAlign;
    Z_ALIGNMENT mZAlign;

  public:
    Transform();
    virtual ~Transform();

    Vector3&            GetPosition();
    void                SetPosition(Vector3 const &aPos);

    Vector3&            GetScale();
    void                SetScale(Vector3 const &aScale);

    Vector3&            GetSize();
    void                SetSize(Vector3 const &aSize);

    X_ALIGNMENT         GetXAlignment() const;
    void                SetXAlignment(X_ALIGNMENT const &aAlign);

    Y_ALIGNMENT         GetYAlignment() const;
    void                SetYAlignment(Y_ALIGNMENT const &aAlign);

    Z_ALIGNMENT         GetZAlignment() const;
    void                SetZAlignment(Z_ALIGNMENT const &aAlign);

    void                Update() {}
    void                SendMessage(Message const &aMessage) {}
    void                ReceiveMessage(Message const &aMessage) {}
    void                Serialize(Parser &aParser);
    void                Deserialize(Parser &aParser);
    static std::string  GetName() {return "Transform";}
    static void         SerializeLUA();
};

#endif
