//*******************************************************************
/*!
\file   Application.h
\author Khai Minh Mai
\date   01.07.2024
\brief  Header for Application
*/
//*******************************************************************

#ifndef APPLICATION_H
#define APPLICATION_H

#include "CustomMessage.h"
#include "CustomString.h"
#include "CustomByte.h"
#include "CustomWord.h"
#include <iostream>
#include <string>
using namespace std;
//
#include "EmbSysLib.h"
using namespace EmbSysLib::Hw;

class Application: public NetSocket::Handler
{
public:
    Application(NetWinTCP &tcp, WORD port);
    NetWinTCP::Socket socket;
    // Check connection
    virtual bool isConnectedWithBroker() { return isMQTTConnected; };
    // Connect
    virtual void connect(string client_id, string username, string passwd);
    virtual bool conack(unsigned char received[]);
    virtual void disconnect();
    // Subscribe
    virtual void subscribe(string topic);
    virtual bool suback(unsigned char received[]);
    virtual void unsubscribe(string topic);
    virtual bool unsuback(unsigned char received[]);
    // Publish
    virtual void publish(string topic, string message);
    virtual bool puback(unsigned char received[]);
    virtual void publishReceived(unsigned char received[]);
    // Packet receiver
    virtual void onReceive(NetSocket &socketLocal);
    // PUBREC - for PUBLISH with QOS 2
    virtual void pubrec(unsigned char received[]);
    virtual void pubrel(unsigned char received[]);
    virtual void pubcomp(unsigned char received[]);
private:
    bool isMQTTConnected = false;
    int keep_alive_msb = 0;
    int keep_alive_lsb = 255;
    CustomMessage *last_msg;
};
#endif // APPLICATION_H
