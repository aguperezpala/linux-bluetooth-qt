/* gcc -o detect-devices.c -lbluetooth*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

sdp_record_t record = { 0 };

sdp_session_t * register_service(void)
{
	uint32_t svc_uuid_int[] = { 0 , 0 , 0 , 0x0003 };
	uint8_t rfcomm_channel = 11;
	const char * service_name = "The Joystick Server" ;
	const char * svc_dsc = "Experimento para testear joysticks" ;
	const char * service_prov = "Joystick-Server" ;
	uuid_t root_uuid , l2cap_uuid , rfcomm_uuid , svc_uuid, svc_class_uuid ;
	sdp_list_t * l2cap_list = 0 ,
	* rfcomm_list = 0 ,
	* root_list = 0 ,
	* proto_list = 0 ,
	* access_proto_list = 0 ,
	* svc_class_list = 0 ,
	* profile_list = 0 ;
	
	sdp_data_t * channel = 0 ;
	sdp_profile_desc_t profile ;
	sdp_session_t * session = 0 ;
	bdaddr_t localDevice;
	
	// set the general service ID
	sdp_uuid128_create( &svc_uuid , &svc_uuid_int ) ;
	sdp_set_service_id ( &record , svc_uuid ) ;
	
	// set the service class
	sdp_uuid16_create(&svc_class_uuid , SERIAL_PORT_SVCLASS_ID ) ;
	svc_class_list = sdp_list_append( 0 , &svc_class_uuid ) ;
	sdp_set_service_classes(&record , svc_class_list ) ;
	
	// set the Bluetooth profile information
	sdp_uuid16_create(&profile.uuid , SERIAL_PORT_PROFILE_ID ) ;
	profile.version = 0x0100 ;
	profile_list = sdp_list_append( 0 , &profile ) ;
	sdp_set_profile_descs (&record , profile_list ) ;
	
	// make the service record publicly browsable
	sdp_uuid16_create(&root_uuid , PUBLIC_BROWSE_GROUP ) ;
	root_list = sdp_list_append ( 0 , &root_uuid ) ;
	sdp_set_browse_groups ( &record , root_list ) ;
	
	// set l2cap information
	sdp_uuid16_create(&l2cap_uuid , L2CAP_UUID ) ;
	l2cap_list = sdp_list_append ( 0 , &l2cap_uuid ) ;
	proto_list = sdp_list_append ( 0 , l2cap_list ) ;
	
	// register the RFCOMM channel for RFCOMM sockets
	sdp_uuid16_create(&rfcomm_uuid , RFCOMM_UUID ) ;
	channel = sdp_data_alloc ( SDP_UINT8 , &rfcomm_channel ) ;
	rfcomm_list = sdp_list_append ( 0 , &rfcomm_uuid ) ;
	sdp_list_append ( rfcomm_list , channel ) ;
	sdp_list_append ( proto_list , rfcomm_list ) ;
	access_proto_list = sdp_list_append ( 0 , proto_list ) ;
	sdp_set_access_protos ( &record , access_proto_list ) ;
	
	// set the name, provider, and description
	sdp_set_info_attr (&record , service_name , service_prov , svc_dsc ) ;
	
	// connect to the local SDP server, register the service record,
	
	// and disconnect
	session = sdp_connect(BDADDR_ANY , BDADDR_LOCAL , SDP_RETRY_IF_BUSY );
	//session = sdp_connect(&localDevice , BDADDR_LOCAL , SDP_RETRY_IF_BUSY );
	if (session == NULL){
		perror("Error al conectarse con el sdp\n");
		return NULL;
	}
	sdp_record_register ( session , &record , 0 ) ;
	// cleanup
	sdp_data_free (channel ) ;
	sdp_list_free (l2cap_list , 0 ) ;
	sdp_list_free (rfcomm_list , 0 ) ;
	sdp_list_free (root_list , 0 ) ;
	sdp_list_free (proto_list ,0);
	sdp_list_free (access_proto_list , 0 ) ;
	sdp_list_free (svc_class_list , 0 ) ;
	sdp_list_free (profile_list , 0 ) ;
	
	return session ;
}


/*
void generate_sdp_sessions(void)
{
	int i = 0;
	sdp_session_t * session = NULL;
	
	for (i = 0; i < 55; i++) {
		session = register_service();
		if (session = NULL)
			printf("error en la session i = %d\n", i);
	}

}
*/


int main(int argc, char **argv)
{
    struct sockaddr_rc loc_addr = {0}, rem_addr = {0};
    char buff[1024] = {0};
    int s = 0, client = 0, bytes_read = 1;
    unsigned int opt = sizeof(rem_addr);
    sdp_session_t * session = NULL;
    int leftToRead = 0;
    bdaddr_t localDevice;
    
    str2ba("00:1A:7D:01:54:6A", &localDevice);
    
    session = register_service();
    /* creamos el socket */
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    
    /* cargamos loc_addr para luego hacer el bind */
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = 11;
    
    /* hacemos el bind NOTE: error check falta*/
    if(bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0)
	    printf("Error al hacer el bind\n");
    else
	    printf("Hicimos el bind correctamente\n");
    /* listening con tamaño cola 1 */
    if (listen(s, 1) < 0)
	    printf("Error al hacer el listen\n");
    else
	    printf("Hicimos el listen correctamente\n");
    
    
    /* aceptamos conecciones */
    printf("Esperando clientes.... \n");
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    ba2str(&rem_addr.rc_bdaddr,buff);
    printf("Aceptamos conexion de: %s\n", buff);
    memset(buff, 0, sizeof(buff));
    bytes_read = recv(client, buff, sizeof(buff), 0);
    
    memcpy(&leftToRead, buff, bytes_read);
    printf("nos van a mandar %d bytes (strlen(buff):%d)\n",leftToRead,
	    (int)strlen(buff));
    while (leftToRead > 0) {
	memset(buff, 0, sizeof(buff));
	bytes_read = recv(client, buff, sizeof(buff), 0);
	leftToRead -= bytes_read;
	if (bytes_read > 0)
		printf("Recibimos %d\nDatos:%s\n", bytes_read, buff);
    }
    printf ("enviando...\n");
    send(client, "maraca", 6, 0);
	
    close(client);
    close(s);
    sdp_close(session);
    printf("\nTermino todo lindo\n");
    
    return 0;
    
}
