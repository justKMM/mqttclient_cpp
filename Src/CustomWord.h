//*******************************************************************
/*!
\file   CustomWord.h
\author Khai Minh Mai
\date   01.07.2024
\brief  Header for CustomWord - a concrete implementation of CustomMessageItem
*/
//*******************************************************************

#ifndef CUSTOMWORD_H
#define CUSTOMWORD_H

#include <CustomMessageItem.h>
#include "CustomMessage.h"


class CustomWord : public CustomMessageItem
{
    public:
        CustomWord(CustomMessage* msg, int value=10);
        virtual ~CustomWord();
        DWORD getValue();
        virtual BYTE length() const override;
        virtual void send(NetSocket &socket);
    protected:

    private:
        BYTE msb = 0;
        BYTE lsb = 10;
};

#endif // CUSTOMWORD_H
