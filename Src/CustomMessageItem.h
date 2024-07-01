#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <iostream>
#include <string>

//#include "CustomMessage.h"
class CustomMessage;
#include "EmbSysLib.h"
using namespace EmbSysLib::Std;
using namespace EmbSysLib::Hw;

class CustomMessageItem : public Sequence<CustomMessageItem>::Item
{
public:
    CustomMessageItem(CustomMessage* msg);
    virtual ~CustomMessageItem();
    virtual BYTE length() const = 0;
    virtual void send(NetSocket &socket) = 0;
protected:
    CustomMessage* msg;
};

#endif // MESSAGEITEM_H
