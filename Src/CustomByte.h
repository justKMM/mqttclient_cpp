//*******************************************************************
/*!
\file   CustomByte.h
\author Khai Minh Mai
\date   01.07.2024
\brief  Header for CustomByte - a concrete implementation of CustomMessageItem
*/
//*******************************************************************
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
