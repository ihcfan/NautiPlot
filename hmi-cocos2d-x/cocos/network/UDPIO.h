//
//  UdpClient.h
//  cocos2d_libs
//
//  Created by Keith Ballantyne on 12/17/13.
//
//

#ifndef __cocos2d_libs__UDPIO__
#define __cocos2d_libs__UDPIO__

#include "cocos2d.h"
#include <string>
#include <map>
#include <sys/socket.h>
#include <thread>

namespace network {
    
    //forward declarations
    class UDPClientImpl;
    class UDPClient;
    
    /**
     *  @brief Singleton and wrapper class to provide static creation method as well as registry of all sockets
     */
    class UDPIO
    {
    public:
        UDPIO();
        virtual ~UDPIO(void);
        
        static UDPIO& instance();
        
        /**
         *  @brief The delegate class to process socket.io events
         */
        class UDPClient: public cocos2d::Object
        {
            friend UDPIO;
            
        public:
            UDPClient();
            virtual ~UDPClient();
            virtual void onConnect() {}
            virtual void onMessageReceived(void *data, int len);
            virtual void onDisconnect() {}
            virtual void onError(const char *data) {}
            virtual void onSend() {}
            /**
             *  @brief Send a message out UDP
             */
            const void send(std::string uri, const char *s, size_t& len);
            const int getPort() { return port; }
            const bool isConnected() { return connected; }
        private:
            int socket_handle;
            bool connected;
            int port;
            sockaddr si_me;
            std::thread *thread;
        protected:
            void dispatchEvents(float delta);
            void threadEntryFunction();
            void setThread(std::thread* threadInstance) { thread=threadInstance; }
            bool lazyInitThreadSemaphore();
            std::mutex rxQueueMutex;
            cocos2d::Array* rxQueue = NULL;
        };
        
        /**
         *  @brief  Static client creation method, similar to socketio.connect(uri) in JS
         *  @param  delegate The delegate which want to receive events from the socket.io client
         *  @param  uri      The URI of the socket.io server
         *  @return An initialized SIOClient if connected successfully, otherwise NULL
         */
        bool connect(UDPIO::UDPClient& client, const std::string& uri);
        void disconnect(UDPIO::UDPClient& client);
        
    private:
        
        static UDPIO *_inst;
        std::map<int, UDPClient*> _clients;
        
        void addPort(const int port, UDPIO::UDPClient& client);
        bool createThread(UDPIO::UDPClient* client);
        void removePort(int port);
        
    };
    
    /**
     *  @brief A single connection to a socket.io endpoint
     */
    
}

#endif /* defined(__cocos2d_libs__UDPIO__) */