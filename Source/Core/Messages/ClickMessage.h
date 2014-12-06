#ifndef CLICKMESSAGE_H
#define CLICKMESSAGE_H

class ClickMessage: public Message
{
private:
  Vector3 mLocation;
  bool mContact;
  
  ClickMessage() : Message() {}
  ClickMessage(ClickMessage const &aMessage) : Message() {}
  
public:
  ClickMessage(Vector3 const &aLocation) : mContact(false)
  {
    mLocation = aLocation;
    SetDescription("Click");
    SetContent("");
  }
  virtual ~ClickMessage() {}
  
  Vector3 GetLocation()
  {
    return mLocation;
  }
  bool GetContacted()
  {
    return mContact;
  }
  
  void SetContacted(bool aContact)
  {
    mContact = aContact;
  }
};

#endif // CLICKMESSAGE_H
