#ifndef __JFramework__NetworkMessage__
#define __JFramework__NetworkMessage__

class NetworkMessage : public Message
{
private:
  NetworkMessage() : Message() {}
  NetworkMessage(NetworkMessage const &aMessage) : Message(aMessage) {}
  
public:
  NetworkMessage(HashString const &aContent) : Message()
  {
    SetDescription("Network");
    SetContent(aContent);
  }
  virtual ~NetworkMessage() {}
};

#endif