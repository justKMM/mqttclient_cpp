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

    CustomByte keep_alive_msb(&msg, this->keep_alive_msb);
    CustomByte keep_alive_lsb(&msg, this->keep_alive_lsb);

    CustomString client_name(&msg, client_id);

    CustomString user(&msg, username);
    CustomString password(&msg, passwd);

    msg.send(socket);
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

    CustomByte packet_id_msb(&msg, msg.getPacketIdMSB());
    CustomByte packet_id_lsb(&msg, msg.getPacketIdLSB());

    CustomString topic_name(&msg, topic);

    CustomByte qos(&msg, 1);

    msg.send(socket);
}

void Application::unsubscribe(string topic)
{
    CustomMessage msg(CustomMessage::MSG_TYPE::UNSUBSCRIBE);

    CustomByte packet_id_msb(&msg, msg.getPacketIdMSB());
    CustomByte packet_id_lsb(&msg, msg.getPacketIdLSB());

    CustomString payload(&msg, topic);

    msg.send(socket);
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
}

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

    switch (received[0])
    {
        case CustomMessage::MSG_TYPE::CONNACK:
            this->isMQTTConnected = this->conack(received);
            break;
        case CustomMessage::MSG_TYPE::PUBACK:
            this->puback(received);
            break;
        case CustomMessage::MSG_TYPE::SUBACK:
            this->suback(received);
            break;
        case CustomMessage::MSG_TYPE::UNSUBACK:
            this->unsuback(received);
            break;
        case CustomMessage::MSG_TYPE::PUBLISH:
            this->publishReceived(received);
            break;
    }

}

bool Application::conack(unsigned char received[])
{
    printf("CONNACK package received.\n");
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


void Application::puback(unsigned char received[])
{
    printf("PUBACK package received.\n");
    if (received[1] < 2)
    {
        printf("ERROR: Package malformed. Rem length not correct.\n");
        return;
    }
    printf("Publish acknowledged by Broker.\n");
}

void Application::suback(unsigned char received[])
{
    printf("SUBACK package received.\n");
    if (received[1] < 2)
    {
        printf("ERROR: Package malformed. Rem length not correct.\n");
        return;
    }
    printf("Subscribe acknowledged by Broker.\n");
}

void Application::unsuback(unsigned char received[])
{
    printf("UNSUBACK package received.\n");
    if (received[1] < 2)
    {
        printf("ERROR: Package malformed. Rem length not correct.\n");
        return;
    }
    printf("Unsubscribe acknowledged by Broker.\n");
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
    // Length of the topic name is given in the next two bytes
    BYTE topic_length = payload[1];
    payload += 2; // Move past the topic length bytes
    string topic(payload, payload + topic_length);
    // Move past the topic name
    payload += topic_length;

    // Now payload points to the actual message payload
    BYTE msg_length = rem_len - (2 + topic_length);

    string message(payload, payload + msg_length);
    printf("From topic: %s, Message: %s\n", topic.c_str(), message.c_str());
}

