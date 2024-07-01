#include "CustomByte.h"

CustomByte::CustomByte(CustomMessage* msg, int value)
: CustomMessageItem(msg), value(value)
{
    msg->append(this);
}

CustomByte::~CustomByte()
{
    // Destructor
}

void CustomByte::send(NetSocket &socket)
{
    socket.write(value);
}

BYTE CustomByte::getValue() const
{
    return this->value;
}

//void CustomByte::addToArray()
//{
//
//}
