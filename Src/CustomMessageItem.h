//*******************************************************************
/*!
\file   CustomMessageItem.h
\author Khai Minh Mai
\date   01.07.2024
\brief  Header for CustomMessageItem - which can either be CustomString CustomByte or CustomWord
*/
//*******************************************************************
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
