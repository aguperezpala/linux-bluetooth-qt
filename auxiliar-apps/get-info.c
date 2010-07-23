/* gcc -o detect-devices.c -lbluetooth*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int argc, char **argv)
{
	int adapter_id , sock;
	bdaddr_t mac;
	char *strMac = NULL;
	struct hci_dev_info devInfo;
	
	
	adapter_id = hci_get_route( NULL ) ;  
	/*! crea el socket para manejar el device local (dongle) */ 
	sock = hci_open_dev(adapter_id ) ;
	
	if (hci_devba(adapter_id, &mac) < 0)
		perror("Error al obtener la mac ");
	else {
		strMac = batostr(&mac);
		printf("La mac es:%s\n", strMac);
		free(strMac);
	}
		
	
	if ( adapter_id < 0 || sock < 0) {
		perror ( "opening socket " ) ;
		exit ( 1 ) ;
	}
	
	memset(&devInfo, '\0', sizeof(devInfo));
	
	/* obtenemos la info */
	if (hci_devinfo(adapter_id, &devInfo) < 0)
		perror("Error al obtener la info del dongle");
	else {
		strMac = batostr(&devInfo.bdaddr);
		printf("\n de devInfo la mac es:%s\n", strMac);
		free(strMac);
	}
	
	return 0 ;
}
