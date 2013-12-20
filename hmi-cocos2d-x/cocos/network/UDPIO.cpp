//
//  UdpClient.cpp
//  cocos2d_libs
//
//  Created by Keith Ballantyne on 12/17/13.
//
//

#include "UDPIO.h"
#import <netinet/in.h>
#import <arpa/inet.h>
#import <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <thread>

namespace network {
    
    //method implementations
    UDPIO::UDPIO() {
        //        _clients = Dictionary::create();
        //        _clients->retain();
    }
    
    UDPIO::~UDPIO() {
        //        CC_SAFE_RELEASE(_clients);
    }
    
    UDPIO& UDPIO::instance() {
        static UDPIO it;
        
        return it;
    }
    
    //begin SIOClientImpl methods
    UDPIO::UDPClient::UDPClient(): socket_handle(-1), connected(-1), port(-1)
    {
    }
    
    UDPIO::UDPClient::~UDPClient()
    {
        UDPIO::instance().disconnect(*this);
    }
    
    bool UDPIO::createThread(UDPIO::UDPClient* client) {
        std::thread* threadInstance = new std::thread(&UDPIO::UDPClient::threadEntryFunction,client);
        client->setThread(threadInstance);
        return true;
    }
    
    
    bool UDPIO::connect(UDPIO::UDPClient& client, const std::string& uri)
    {
        std::string addr;
        std::string port;
        int iport;
        
        memset((char *) &client.si_me, 0, sizeof(client.si_me));
        sockaddr_in *setup=(sockaddr_in *)&client.si_me;
        setup->sin_family = AF_INET;
        
        int pos = uri.find(":");
        if(pos >= 0)
        {
            addr = uri.substr(0, pos);
            port = uri.substr(pos+1,uri.length()-pos);
            iport = atoi(port.c_str());
            setup->sin_port = htons(iport);
            setup->sin_addr.s_addr = htonl(INADDR_ANY);
        } else {
            fprintf(stderr,"malformed address\n");
        }
        
        int handle=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (handle == -1) {
            fprintf(stderr,"couldn't open socket\n");
            return false;
        }
        
        client.socket_handle=handle;
        if (bind(client.socket_handle, &client.si_me, sizeof(client.si_me))==-1) {
            fprintf(stderr,"couldn't bind address: %s\n",strerror(errno));
            return false;
        }
        
        addPort(iport,client);
        client.connected=true;
        createThread(&client);
        client.onConnect();
        return true;
    }
    
    void UDPIO::UDPClient::threadEntryFunction() {
        char buf[1000];
        socklen_t slen=sizeof(si_me);
        while (1) {
            int len = recvfrom(socket_handle, &buf, 1000, 0, &si_me, &slen);
            onMessageReceived(buf,len);
        }
    }
    
    void UDPIO::addPort(int port, UDPIO::UDPClient& client) {
        _clients[port]=&client;
    }
    
    void UDPIO::removePort(int port) {
        _clients.erase(port);
    }
    
    void UDPIO::disconnect(UDPIO::UDPClient& client) {
        if (!client.connected) return;
        
        client.onDisconnect();
//        delete client.thread;
//        client.thread = nullptr;
        close(client.socket_handle);
        client.socket_handle=-1;
        removePort(client.port);
        client.port=-1;
        client.connected=false;
        
        return;
    }
    
    const void UDPIO::UDPClient::send(std::string uri, const char *s, size_t& len)
    {
        std::string addr;
        std::string port;
        sockaddr si_other;
        
        memset((char *) &si_other, 0, sizeof(si_other));
        sockaddr_in *setup = (sockaddr_in *)&si_other;
        
        
        int pos = uri.find(":");
        if(pos >= 0)
        {
            addr = uri.substr(0, pos);
            port = uri.substr(pos+1,uri.length()-pos);
            short temp = atoi(port.c_str());
            setup->sin_port = htons(temp);
            if (inet_aton(addr.c_str(), &setup->sin_addr)==0) {
                fprintf(stderr,"Can't resolve address %s",addr.c_str());
            } else {
                if ((len=sendto(socket_handle, s, len, 0, &si_other, sizeof(si_other)))==-1) {
                    fprintf(stderr,"UDPClientImpl send failed");
                    return;
                }
                onSend();
            }
        }
    }
    
    void UDPIO::UDPClient::onMessageReceived(void *data, int len) {
        auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
        static auto udpEvent = new cocos2d::EventUDP(data,len);
        dispatcher->dispatchEvent(udpEvent);
    }
    
    
}
