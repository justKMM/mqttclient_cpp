#include "CustomString.h"

CustomString::CustomString(CustomMessage* msg, std::string content)
: CustomMessageItem(msg), content(content)
{
    msg->append(this);
}

CustomString::~CustomString()
{
    // Destructor
}

void CustomString::send(NetSocket &socket)
{
    BYTE msb = 0;
    BYTE lsb = this->content.length();

    socket.write(msb);
    socket.write(lsb);
    for(char c : this->content)
    {
        socket.write(c);
    }

}
std::string CustomString::getContent()
{
    return this->content;
}

BYTE CustomString::length() const
{
    return (content.length() + 2);
}
