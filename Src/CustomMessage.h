#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include "CustomMessageItem.h"
using namespace std;
#include "EmbSysLib.h"
using namespace EmbSysLib::Std;

class CustomMessage
{
    public:
        typedef enum
        {
            CONNECT = 0x10,
            CONNACK = 0x20,
            PUBLISH = 0x30,
            PUBACK = 0x40,
            PUBREC = 0x50,
            PUBREL = 0x60,
            PUBCOMP = 0x70,
            SUBSCRIBE = 0x82,
            SUBACK = 0x90,
            UNSUBSCRIBE = 0xA2,
            UNSUBACK = 0xB0,
            DISCONNECT = 0xE0
        } MSG_TYPE;
        CustomMessage(MSG_TYPE msg_type);
        virtual ~CustomMessage();
        virtual void send(NetSocket &socket);
        virtual void append(CustomMessageItem* msg_item);
        virtual Sequence<CustomMessageItem> getSequence();
        virtual DWORD getRemLen();
        virtual int getPacketIdMSB() { return this->packet_id_msb; };
        virtual int getPacketIdLSB() { return this->packet_id_lsb++; };
    protected:

    private:
        Sequence<CustomMessageItem> sequence;
        DWORD rem_len = 0;
        MSG_TYPE msg_type;
        int packet_id_msb = 0;
        int packet_id_lsb = 10;
        char package[];
};

#endif // MESSAGE_H
