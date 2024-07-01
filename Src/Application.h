//*******************************************************************
/*!
\file   Application.h
\author Khai Minh Mai
\date   01.07.2024
\brief  Header for
*/
//*******************************************************************

#ifndef APPLICATION_H
#define APPLICATION_H

#include "CustomMessage.h"
#include "CustomMessageItem.h"
#include "CustomString.h"
#include "CustomByte.h"
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
    DWORD x=0;
    // Check connection
    virtual bool isConnectedWithBroker() { return isMQTTConnected; };
    // Connect
    virtual void connect(string client_id, string username, string passwd);
    virtual bool conack(unsigned char received[]);
    virtual void disconnect();
    // Subscribe
    virtual void subscribe(string topic);
    virtual void suback(unsigned char received[]);
    virtual void unsubscribe(string topic);
    virtual void unsuback(unsigned char received[]);
    // Publish
    virtual void publish(string topic, string message);
    virtual void puback(unsigned char received[]);
    virtual void publishReceived(unsigned char received[]);
    // Packet receiver
    virtual void onReceive(NetSocket &socketLocal);
private:
    bool isMQTTConnected = false;
    int keep_alive_msb = 0;
    int keep_alive_lsb = 255;
};
#endif // APPLICATION_H
