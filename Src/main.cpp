//*******************************************************************
/*!
\file   main.cpp
\author Khai Minh Mai
\date   01.07.2024
\brief  Main file, containing the main function
*/
//*******************************************************************

#include <iostream>
//
#include "embsyslib.h"
#include "module/rtos/rtos.h"
#include "reporthandler.h"
#include "configHwNet.h"
#include "Application.h"

#include <string>
#include <vector>
using namespace std;

void getIPAddress();

bool isTCPConnected = false;

NetAddr<4> addr;

int main(void)
{
    while(net.getIP().addr[0] == 0) {
        net.update();
    }
    DWORD PORT = 1883;
    printf("TCP, %s, PORT: %d\r\n", (const char*)(net.getIP()), PORT);
    //disp.refresh();
    Application app(tcp, PORT);
    while (!isTCPConnected) {
        // IP Address
        getIPAddress();
        app.socket.open(addr, PORT);
        //
        switch( app.socket.state.getUnique() )
        {
          case NetSocket::UNDEFINED:                                    break;
          case NetSocket::CLOSED:      uart.set("\r\nCLOSED\r\n");      break;
          case NetSocket::LISTENING:   uart.set("\r\nLISTENING\r\n");   break;
          case NetSocket::CONNECTED:   uart.set("\r\nCONNECTED\r\n"); isTCPConnected=true;  break;
          case NetSocket::ERROR_STATE: uart.set("\r\nERROR_STATE\r\n"); break;
          default:                     uart.set("\r\nstate unkown\r\n");break;
        }

    }
    // Loop for the programm
    while(1)
    {
        net.update();
        // Skipping an iteration to wait for the incoming package
        char *str = terminal.getString();
        if(str == nullptr)
        {
            continue;
        }
        string command;
        cout << "\n> Command: ";

        getline(cin, command);
        if (command == "help" || command == "h")
        {
            cout << "\nAvailable Commands:\n"
            << "- help | h: Show available commands\n"
            << "- connect | con: Connect to broker\n"
            << "- subscribe | sub: Subscribe to topic\n"
            << "- unsubscribe | unsub: Unsubscribe from topic\n"
            << "- publish | pub: Publish to topic\n"
            << "- disconnect: Disconnect from broker\n"
            << "- end: Terminate programm\n";
        }
        else if (command == "end")
        {
            break;
        }
        // If the client is not yet connected with the Broker
        else if (!app.isConnectedWithBroker())
        {
            if ((command == "connect") || (command == "con"))
            {
                string client_id;
                string username;
                string passwd;

                cout << "\n> Client: ";
                getline(cin, client_id);
                cout << "\n> Username: ";
                getline(cin, username);
                cout << "\n> Password: ";
                getline(cin, passwd);

                app.socket.open(addr, PORT);
                app.connect(client_id, username, passwd);
                continue;
            }
            else if (command == "end")
            {
                break;
            }
            else
            {
                cout << "\nNot MQTT connected. Please use command 'connect'.\n";
            }
        }
        // If the client is connected with the Broker
        else if (app.isConnectedWithBroker())
        {
            if ((command == "connect") || (command == "con"))
            {
                cout << "\r\nAlready connected\r\n";
                continue;
            }
            else if (command == "disconnect")
            {
                app.disconnect();
                continue;
            }
            else if ((command == "subscribe") || (command == "sub"))
            {
                string topic;
                cout << "\n> Topic: ";
                getline(cin, topic);
                if (topic.empty())
                {
                    printf("Topic cannot be null\n");
                    continue;
                }
                app.subscribe(topic);
                cout << "Subscribed to topic " << topic << "\n";
                continue;
            }
            else if ((command == "unsubscribe") || (command == "unsub"))
            {
                string topic;
                cout << "\n> Topic: ";
                getline(cin, topic);
                if (topic.empty())
                {
                    printf("Topic cannot be null\n");
                    continue;
                }
                app.unsubscribe(topic);
                cout << "Unsubscribed to topic " << topic << "\n";
                continue;
            }
            else if ((command == "publish") || (command == "pub"))
            {
                string topic;
                string message;
                cout << "\n> Topic: ";
                getline(cin, topic);
                cout << "\n> Message: ";
                getline(cin, message);
                if (topic.empty() || message.empty())
                {
                    cout << "Topic/Message cannot be null\n";
                    continue;
                }
                app.publish(topic, message);
                cout << "\nPublished to topic " << topic << ", message: " << message << "\n";
                continue;
            }

            else
            {
                cout << "Command not valid.\n";
                continue;
            }
        }
    }

    cout << "Program terminated. Restart program to start over.\n";
    return 0;
}

void getIPAddress()
{
    //
    string ip_addr_;
    cout << "> IP Address of Broker: ";
    cin >> ip_addr_;
    // Default Broker IP: 18.199.122.97 (AWS Instance)
    if (ip_addr_ == "default" || ip_addr_ == "d")
    {
        addr = {18,199,122,97};
        cout << "Default IP Address set.";
        return;
    }

    vector<int> ip_parts;
    string delimiter = ".";
    size_t pos = 0;
    string token;
    while ((pos = ip_addr_.find(delimiter)) != string::npos) {
        token = ip_addr_.substr(0, pos);
        try {
            ip_parts.push_back(stoi(token));
        } catch (const invalid_argument& e) {
            cout << "Invalid IP address format." << endl;
            return;
        }
        ip_addr_.erase(0, pos + delimiter.length());
    }

    try {
        ip_parts.push_back(stoi(ip_addr_));
    } catch (const invalid_argument& e) {
        cout << "Invalid IP address format." << endl;
        return;
    }

    if (ip_parts.size() != 4) {
        cout << "Invalid IP address format. Make sure you entered four octets." << endl;
        return;
    }

    BYTE ip_1 = ip_parts[0];
    BYTE ip_2 = ip_parts[1];
    BYTE ip_3 = ip_parts[2];
    BYTE ip_4 = ip_parts[3];

    addr = {ip_1,ip_2,ip_3,ip_4};
}

////EOF
