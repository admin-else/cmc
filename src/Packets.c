// keep_alive,v,keep_alive_id

#include "MCbuffer.h"
#include "MConn.h"
#include <stdlib.h>

#define RETURN_IF_NULL(ptr) if(ptr == NULL) {*errmsg = "Memory allocation failed."; return;}
#define RETURN_AND_FREE_IF_ERRMSG if(*errmsg != NULL) {free(buff); return;}

typedef struct {
    int keep_alive_id;
} keep_alive_packet;

void send_packet_keep_alive(MConn *conn, int keep_alive_id, char **errmsg) {
    MCbuffer *buff = MCbuffer_init();
    RETURN_IF_NULL(buff)

    MCbuffer_pack_varint(buff, keep_alive_id, errmsg); //v
    RETURN_AND_FREE_IF_ERRMSG
    
    MConn_send_buffer(conn, buff, errmsg);
    MCbuffer_free(buff);
}

