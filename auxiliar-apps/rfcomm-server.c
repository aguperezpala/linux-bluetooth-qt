/* gcc -o detect-devices.c -lbluetooth*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
          
int main(int argc, char **argv)
{
    struct sockaddr_rc loc_addr = {0}, rem_addr = {0};
    char buff[1024] = {0};
    int s = 0, client = 0, bytes_read = 0;
    unsigned int opt = sizeof(rem_addr);
    
    /* creamos el socket */
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    
    /* cargamos loc_addr para luego hacer el bind */
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = 1;
    
    /* hacemos el bind NOTE: error check falta*/
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    /* listening con tamaño cola 1 */
    listen(s, 1);
    
    /* aceptamos conecciones */
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    ba2str(&rem_addr.rc_bdaddr,buff);
    printf("Aceptamos conexion de: %s\n", buff);
    memset(buff, 0, sizeof(buff));
    
    bytes_read = recv(client, buff, sizeof(buff), 0);
    if (bytes_read > 0)
	    printf("Recibimos %d\nDatos:%s\n", bytes_read, buff);
    
    close(client);
    close(s);
    
    return 0;
    
}
