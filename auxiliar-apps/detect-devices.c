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
    inquiry_info * devices = NULL;
    int max_rsp , num_rsp ;
    int adapter_id , sock , len , flags ;
    int i ;
    char addr[19] = { 0 } ;
    char name[248] = { 0 } ;
    
    /*! aca se elige el dispositivo, podemos usar NULL para obtener
     * el primer dispositivo (dongle) libre en el sistema o podemos tambien
     * usar int hci_devid(const char * dev_addr); para especificar cual disp
     * queremos usar, tener en cuenta que seguro que hay alguna funcion que
     * lista todos los dispositivos y devuelve su addr...
     */
  adapter_id = hci_get_route( NULL ) ;  
  /*! crea el socket para manejar el device local (dongle) */ 
  sock = hci_open_dev(adapter_id ) ;
  
  if ( adapter_id < 0 || sock < 0) {
          perror ( "opening socket " ) ;
          exit ( 1 ) ;
  }
  
  len = 8 ; /*! tiempo a esperar (1.28 secs * len) */
  max_rsp = 255; /*! max cant de dispositivos */
  /*! limpiar el cache para devolver los detectados en la ultima pasada de
   * reconocimiento */
  flags = IREQ_CACHE_FLUSH ; 
  devices = ( inquiry_info * ) malloc ( max_rsp * sizeof ( inquiry_info ) ) ;
  num_rsp = hci_inquiry( adapter_id , len , max_rsp , NULL , &devices ,
                 flags ) ;
  if ( num_rsp < 0 ) perror ( " hci inquiry " ) ;
  
  for ( i = 0 ; i < num_rsp ; i++) {
          ba2str (&(devices+i)->bdaddr, addr);
          memset ( name , 0 , sizeof ( name ) );
          if (0 != hci_read_remote_name ( sock , &(devices+i)->bdaddr ,
                            sizeof ( name ) , name , 0 ) ) {
                 strcpy ( name , "[unknown] " ) ;
          }
          printf ( "%s %s\n" , addr , name ) ;
  }
  free ( devices ) ;
  close ( sock ) ;
  return 0 ;
}
