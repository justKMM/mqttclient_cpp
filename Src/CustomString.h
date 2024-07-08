//*******************************************************************
/*!
\file   CustomString.h
\author Khai Minh Mai
\date   01.07.2024
\brief  Header for CustomString - a concrete implementation of CustomMessageItem
*/
//*******************************************************************
#ifndef CUSTOMSTRING_H
#define CUSTOMSTRING_H

#include "CustomMessage.h"
#include "CustomMessageItem.h"
#include <string>
using namespace std;

class CustomString : public CustomMessageItem
{
    public:
        CustomString(CustomMessage* msg, std::string content);
        ~CustomString();
        std::string getContent();
        virtual BYTE length() const override;
        virtual void send(NetSocket &socket);
    private:
        std::string content;
};

#endif // CUSTOMSTRING_H
