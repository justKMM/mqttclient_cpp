#ifndef CUSTOMBYTE_H
#define CUSTOMBYTE_H

#include "CustomMessage.h"
#include "CustomMessageItem.h"

class CustomByte : public CustomMessageItem
{
public:
    CustomByte(CustomMessage* msg, int value);
    ~CustomByte();
    BYTE getValue() const;
    virtual BYTE length() const override { return 1; }
    virtual void send(NetSocket &socket);
private:
    BYTE value;
};

#endif // CUSTOMBYTE_H
