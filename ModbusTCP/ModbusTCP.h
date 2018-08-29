/*
    ModbusTCP.h - Header for Modbus IP Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPServer.h"
#include "TCPSocket.h"
#include <Modbus.h>

#ifndef MODBUSTCP_H
#define MODBUSTCP_H

#define MODBUSTCP_PORT 	  502
#define MODBUSTCP_MAXFRAME 200

//#define TCP_KEEP_ALIVE

class ModbusTCP : public Modbus {
    private:
        TCPServer _server;
        TCPSocket client_sock;
        SocketAddress client_addr;
        byte _MBAP[7];

    public:
        ModbusTCP(EthernetInterface* _eth);
        void task();
};

#endif //MODBUSTCP_H

