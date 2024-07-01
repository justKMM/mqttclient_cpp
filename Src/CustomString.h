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
