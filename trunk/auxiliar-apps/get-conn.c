#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int argc, char **argv)
{
	struct hci_conn_list_req *cl;
	struct hci_conn_info *ci;
	int i;
	int adapter_id , sock;
	
	adapter_id = hci_get_route( NULL ) ;  
	/*! crea el socket para manejar el device local (dongle) */ 
	sock = hci_open_dev(adapter_id ) ;
	
	
	if ( adapter_id < 0 || sock < 0) {
		perror ( "opening socket " ) ;
		exit ( 1 ) ;
	}
	
	
	if (!(cl = malloc(HCI_MAX_DEV * sizeof(*ci) + sizeof(*cl)))) {
		perror("Can't allocate memory");
		exit(1);
	}
	cl->dev_id = adapter_id;
	cl->conn_num = HCI_MAX_DEV;
	ci = cl->conn_info;

	if (ioctl(sock, HCIGETCONNLIST, (void*)cl)) {
		perror("Can't get connection list");
		exit(1);
	}
	
	printf("\nTenemos cl->conn_num = %d \n", (int) cl->conn_num);

	for (i=0; i < cl->conn_num; i++, ci++) {
		char addr[18];
		ba2str(&ci->bdaddr, addr);
		printf("\t%s %s %s handle %d state %d lm %s\n",
			ci->out ? "<" : ">",
			ci->type == ACL_LINK ? "ACL" : "SCO",
			addr, ci->handle, ci->state,
			hci_lmtostr(ci->link_mode));
	}
	return 0;
}