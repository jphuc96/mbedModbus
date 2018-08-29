/*
    ModbusTCP.cpp - Source for Modbus IP Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include "ModbusTCP.h"

ModbusTCP::ModbusTCP(EthernetInterface* _eth) {
    _server.open(&_eth);
    _server.bind(_eth.get_ip_address(),MODBUSTCP_PORT);
    _server.listen(5);
}

void ModbusTCP::task() {
    // EthernetClient client = _server.available();
    nsapi_error_t _serverStatus _server.accept(&client_sock, &client_addr);

    if (_serverStatus == NSAPI_ERROR_OK ) 
    {
        int i = 0;
        while (client_sock.){
            _MBAP[i] = client.read();
            i++;
            if (i==7) break;  //MBAP length has 7 bytes size
        }
        _len = _MBAP[4] << 8 | _MBAP[5];
        _len--;  // Do not count with last byte from MBAP

        if (_MBAP[2] !=0 || _MBAP[3] !=0) return;   //Not a MODBUSTCP packet
        if (_len > MODBUSTCP_MAXFRAME) return;      //Length is over MODBUSTCP_MAXFRAME

        _frame = (byte*) malloc(_len);
        i = 0;
        while (client.available()){
            _frame[i] = client.read();
            i++;
            if (i==_len) break;
        }

        this->receivePDU(_frame);
        if (_reply != MB_REPLY_OFF) {
            //MBAP
            _MBAP[4] = (_len+1) >> 8;     //_len+1 for last byte from MBAP
            _MBAP[5] = (_len+1) & 0x00FF;

            byte sendbuffer[7 + _len];

            for (i = 0 ; i < 7 ; i++) {
                sendbuffer[i] = _MBAP[i];
            }
            //PDU Frame
            for (i = 0 ; i < _len ; i++) {
                sendbuffer[i+7] = _frame[i];
            }
            client.write(sendbuffer, _len + 7);
        }

        #ifndef TCP_KEEP_ALIVE
            client.stop();
        #endif
            free(_frame);
            _len = 0;
    }
}
