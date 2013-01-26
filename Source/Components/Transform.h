#ifndef __JFramework_Transform_h_
#define __JFramework_Transform_h_

#include "Component.h"
#include "MathExt.h"

class Transform : public Component
{
  private:
    Vector3 mPosition;
    Vector3 mScale;
    Vector3 mSize;

  public:
    Transform();
    ~Transform();

    Vector3             GetPosition() const;
    void                SetPosition(Vector3 const &aPos);

    Vector3             GetScale() const;
    void                SetScale(Vector3 const &aScale);

    Vector3             GetSize() const;
    void                SetSize(Vector3 const &aSize);

    void                Update() {}
    void                SendMessage(Message const &aMessage) {}
    void                ReceiveMessage(Message const &aMessage) {}
    static std::string  GetName() {return "Transform";}
};

#endif
