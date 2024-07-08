#include "CustomWord.h"

CustomWord::CustomWord(CustomMessage* msg, int value)
: CustomMessageItem(msg), lsb(value)
{
    msg->append(this);
}

CustomWord::~CustomWord()
{
    //dtor
}

DWORD CustomWord::getValue()
{
    return lsb;
}
BYTE CustomWord::length() const
{
    return 2;
}

void CustomWord::send(NetSocket &socket)
{
    socket.write(this->msb);
    socket.write(this->lsb);
}
