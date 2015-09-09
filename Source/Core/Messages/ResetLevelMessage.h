#ifndef RESETLEVELMESSAGE_H
#define RESETLEVELMESSAGE_H

#include "Message.h" // Base class: Message

class ResetLevelMessage : public Message
{
public:
    ResetLevelMessage() : Message()
    {
      SetDescription("ResetLevel");
      SetContent("");
    }
    ~ResetLevelMessage()
    {
    }
};

#endif // RESETLEVELMESSAGE_H
