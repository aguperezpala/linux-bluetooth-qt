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

int create_connection(uint8_t port)
{
	struct sockaddr_rc addr = {0};
	int s = 0, status = 0;
	char dest[18] = "00:24:90:BF:72:F4";
	/* creamos el socket */
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	
	/* seteamos la addr para la conexion (connect) */
	addr.rc_family = AF_BLUETOOTH;    
	addr.rc_channel = port;
	str2ba(dest, &addr.rc_bdaddr);
	status = connect(s, (struct sockaddr*)&addr, sizeof(addr));
	if (status < 0){
		close(s);
		return -1;
	}
	return s;
}

int main(int argc, char **argv)
{
    int s = -11, status = 1;
    char buff[1024] = {0};
    int i = 0;
    
    for (i = 1; i < 31; i++) {
	s = create_connection(i);
	printf("Conectandose en puerto %d\n", i);
		
	if (s < 0){
		perror("error connectin");
		continue;
	}
	status = send(s,"hello!", 6, 0);
	if (status < 0){
		perror("error al enviar los datos");
		continue;
		close(s);
	}
	
	close(s);
    }
    
    
    
    return 0;
    
}
