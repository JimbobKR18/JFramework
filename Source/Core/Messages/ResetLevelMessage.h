#ifndef RESETLEVELMESSAGE_H
#define RESETLEVELMESSAGE_H

#include "Message.h" // Base class: Message

class ResetLevelMessage : public Message
{
public:
    ResetLevelMessage(HashString const &aLevelName) : Message()
    {
      SetDescription("ResetLevel");
      SetContent(aLevelName);
    }
    ~ResetLevelMessage()
    {
    }
};

#endif // RESETLEVELMESSAGE_H
