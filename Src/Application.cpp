//*******************************************************************
/*!
\file   Application.app
\author Khai Minh Mai
\date   01.07.2024
\brief  Source file for Application
*/
//*******************************************************************
#include "Application.h"

using namespace EmbSysLib::Dev;

extern Terminal terminal;



Application::Application(NetWinTCP &tcp, WORD port)
: socket(tcp, this)
{

}
//************
// Ausgang
//************

void Application::connect(string client_id, string username, string passwd)
{
//    char package[16+2] = {16,16,
//        0,4,'M','Q','T','T',
//        4,0,0,0,
//        0,4,'t','e','s','t'};

    CustomMessage msg(CustomMessage::MSG_TYPE::CONNECT);

    CustomString protocol_name(&msg, "MQTT");

    CustomByte protocol_level(&msg, 4);
    CustomByte flags(&msg, 0xC0);

    CustomWord keep_alive_lsb(&msg, this->keep_alive_lsb);

    CustomString client_name(&msg, client_id);

    CustomString user(&msg, username);
    CustomString password(&msg, passwd);

    msg.send(socket);
    last_msg = &msg;
}


void Application::disconnect()
{
    this->isMQTTConnected = false;
    CustomMessage msg(CustomMessage::MSG_TYPE::DISCONNECT);
    msg.send(socket);
    printf("\nDisconnected from Broker.\n");
}

void Application::subscribe(string topic)
{
//    BYTE topic_len = topic.length();
//    unsigned char c = 130;
//    char package[17+2] = {static_cast<char>(c),17,
//    0,10,
//    0,5,'t','o','p','i','c',1,
//    0,4,'t','e','s','t',2};

    CustomMessage msg(CustomMessage::MSG_TYPE::SUBSCRIBE);

    CustomWord keep_alive_lsb(&msg, this->keep_alive_lsb);

    CustomString topic_name(&msg, topic);

    CustomByte qos(&msg, 1);

    msg.send(socket);
    last_msg = &msg;
}

void Application::unsubscribe(string topic)
{
    CustomMessage msg(CustomMessage::MSG_TYPE::UNSUBSCRIBE);

    CustomWord packet_id(&msg, msg.getPacketIdLSB());

    CustomString payload(&msg, topic);

    msg.send(socket);
    last_msg = &msg;
}

void Application::publish(string topic, string message)
{
//    char package[18+2] = {48,18,
//        0,5,'t','o','p','i','c',
//        'd','u',' ','d', 'u', 'm', 'm', 'k', 'o', 'p', 'f'};

    CustomMessage msg(CustomMessage::MSG_TYPE::PUBLISH);

    CustomString topic_name(&msg, topic);

    CustomString payload(&msg, message);

    msg.send(socket);
    last_msg = &msg;
}
//************
// Hilfsmethoden
//************


//************
// Eingang
//************

void Application::onReceive(NetSocket &socketLocal)
{
    BYTE c;
    char counter = 0;
    BYTE received[128];
    while(socketLocal.get(c))
    {
        received[counter++] = c;
    }
    bool acknowledged = false;
    switch (received[0])
    {
        case CustomMessage::MSG_TYPE::CONACK:
            this->isMQTTConnected = this->conack(received);
            break;
        case CustomMessage::MSG_TYPE::PUBACK:
            acknowledged = this->puback(received);
            break;
        case CustomMessage::MSG_TYPE::SUBACK:
            acknowledged = this->suback(received);
            break;
        case CustomMessage::MSG_TYPE::UNSUBACK:
            acknowledged = this->unsuback(received);
            break;
        case CustomMessage::MSG_TYPE::PUBLISH:
            this->publishReceived(received);
            break;
        case CustomMessage::MSG_TYPE::PUBLISH_QOS2:
            this->pubrec(received);
            break;
        case CustomMessage::MSG_TYPE::PUBREL:
            this->pubrel(received);
            break;
    }
    if (acknowledged)
    {
        printf("Package acknowledged.\n");
    }

}

bool Application::conack(unsigned char received[])
{
    printf("CONACK package received.\n");
    if (received[1] < 2)
    {
        printf("ERROR: Package malformed. Rem length not correct.\n");
        return false;
    }
    if (received[3] != 0)
    {
        printf("ERROR: Connection refused. ");
        if (received[3] == 1) printf("Unacceptable Protocol Version.\n");
        else if (received[3] == 2) printf("Identifier rejected.\n");
        else if (received[3] == 3) printf("Server unavailable.\n");
        else if (received[3] == 4) printf("Bad user name or password.\n");
        else if (received[3] == 5) printf("Not authorized.\n");
        return false;
    }
    printf("Connection acknowledged by Broker.\n");
    return true;
}


bool Application::puback(unsigned char received[])
{
    printf("PUBACK package received.\n");
    if (received[1] < 2)
    {
        printf("ERROR: Package malformed. Rem length not correct.\n");
        return false;
    }
    printf("Publish acknowledged by Broker.\n");
    return true;
}

bool Application::suback(unsigned char received[])
{
    printf("SUBACK package received.\n");
    if (received[1] < 2)
    {
        printf("ERROR: Package malformed. Rem length not correct.\n");
        return false;
    }
    printf("Subscribe acknowledged by Broker.\n");
    return true;
}

bool Application::unsuback(unsigned char received[])
{
    printf("UNSUBACK package received.\n");
    if (received[1] < 2)
    {
        printf("ERROR: Package malformed. Rem length not correct.\n");
        return false;
    }
    if ((last_msg->getPacketIdMSB() != received[3]) || (last_msg->getPacketIdLSB() != received[4]))
    {
        printf("ERROR: Packet ID not correct.\n");
        return false;
    }
    printf("Unsubscribe acknowledged by Broker.\n");
    return true;
}

void Application::publishReceived(unsigned char received[])
{
    printf("PUBLISH package received.\n");

    BYTE rem_len = received[1];
    if (rem_len < 2)
    {
        printf("ERROR: Package malformed. Rem length is not correct.\n");
    }

    unsigned char *payload = received + 2;
    BYTE topic_length = payload[1];
    payload += 2; // skip topic length bytes
    string topic(payload, payload + topic_length);
    // skip topic name
    payload += topic_length;

    BYTE msg_length = rem_len - (2 + topic_length);

    string message(payload, payload + msg_length);
    printf("From topic: %s, Message: %s\n", topic.c_str(), message.c_str());
}

void Application::pubrec(unsigned char received[])
{
    // skip header of package
    unsigned char *payload = received + 2;
    BYTE topic_length = payload[1];
    // skip len of topic
    payload += 2;
    // skip topic
    payload += topic_length;

    CustomMessage msg(CustomMessage::MSG_TYPE::PUBREL);

    CustomWord packet_id(&msg, payload[1]);

    msg.send(socket);
}

void Application::pubrel(unsigned char received[])
{
    printf("PUBREL package received.\n");
    if(received[1] < 2)
    {
        printf("ERROR: Package malformed. Rem length is not correct.\n");
        return;
    }
    printf("PUBREC acknowledged by Broker.\n");

    pubcomp(received);
}

void Application::pubcomp(unsigned char received[])
{
    CustomMessage msg(CustomMessage::MSG_TYPE::PUBCOMP);

    CustomWord packet_id(&msg, received[4]);

    msg.send(socket);
}

