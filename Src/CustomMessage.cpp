#include "CustomMessage.h"

CustomMessage::CustomMessage(MSG_TYPE msg_type)
: msg_type(msg_type)
{
//    this->msg_type = msg_type;
}

CustomMessage::~CustomMessage()
{
    //dtor
}

void CustomMessage::send(NetSocket &socket)
{
    socket.clear();
    socket.write(msg_type);
    socket.write(this->rem_len);


    CustomMessageItem *item = this->sequence.getFirst();

    while (item != nullptr)
    {
        item->send(socket);
        item = item->getNext();
    }
    socket.flush();

}

void CustomMessage::append(CustomMessageItem *msg_item)
{
    this->sequence.add(msg_item);
    this->rem_len += msg_item->length();
}

Sequence<CustomMessageItem> CustomMessage::getSequence()
{
    return this->sequence;
}

DWORD CustomMessage::getRemLen()
{
    return this->rem_len;
}
