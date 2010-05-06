/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hello;

import javax.bluetooth.*;
import java.util.Vector;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.lcdui.Form;


/**
 *
 * @author agustin
 */
public class DeviceFinder implements DiscoveryListener {

 

  private DiscoveryAgent discoveryAgent;
  LocalDevice localDevice;
  /* Vamos a definir 3 tipos de dispositivos, los que estan funcionando
   * correctamente (workingDevices), los que estan indefinidos, osea que no se
   * sabe si estan andando o no (undefinedDevices) y los que NO nos sirven
   * para nada (blockDev) (i.e. son dispositivos de otros usuarios confirmados).
   * SOLO vamos a guardar las MAC (strings)
   */
  private Vector workingDevices = null;
  private Vector undefinedDevices = null;
  private Vector blockDevices = null;
  private UUID[] uuidSet;
  private int acceptedChannel;
  private int[] attributes;
  private Form statusForm;
  private int searching;


  /* Constructor:
   * from = donde vamos a sacar el estado actual de las acciones.
   * uuid = uuid a buscar.
   * attr = attributos a buscar
   * channel = "puerto" a buscar (si encontramos servicio con puerto distinto
   *            automaticamente sera descartado).
   */
  public DeviceFinder(Form form, UUID uuid, int[] attr, int channel) {

      this.statusForm = form;
      this.acceptedChannel = channel;
      // creamos los vectores
      this.workingDevices = new Vector();
      this.undefinedDevices = new Vector();
      this.blockDevices = new Vector();

      this.uuidSet = new UUID[1];
      this.uuidSet[0] = uuid;
      this.attributes = attr;

      this.searching = -1;


      try {
          /* obtenemos el dispositivo local de bluetooth */
          localDevice = LocalDevice.getLocalDevice();
          /* obtenemos el discoveryAgent */
          discoveryAgent = localDevice.getDiscoveryAgent();
      } catch(Exception e) {
          statusForm.append("Error constructor: " + e.toString() + "\n");
      }

  }

  /* funcion que comienza la busqueda */
  public void startFindDevices(){
      try {
          this.discoveryAgent.startInquiry(DiscoveryAgent.GIAC, this);
          this.searching = 1;
      } catch (Exception e) {
          this.statusForm.append("Error al buscar dispositivos " + e.toString()
                   + "\n");
      }
  }

  public void deviceDiscovered(RemoteDevice btDevice, DeviceClass cod) {
    try {
        String mac = btDevice.getBluetoothAddress();
        
        // ahora verificamos si el btDevice ya esta en alguno de los vectores
        if (this.workingDevices.contains(btDevice)) {
            /* no hacemos nada.. */
            this.statusForm.append("Dev :" + mac + " working dev\n");
        } else if (this.blockDevices.contains(btDevice)) {
            /* tampoco hacemos nada */
            this.statusForm.append("Dev :" + mac + " blocked dev\n");
        } else {
            /* esta indefinido, asique lo amacenamos aca para luego determinar
             * a que grupo lo vamos a meter */
            if (!this.undefinedDevices.contains(btDevice))
                // lo agregamos si no estaba
                this.undefinedDevices.addElement(btDevice);
            this.statusForm.append("Dev :" + mac + " undefined dev\n");
        }
        //int searchID = discoveryAgent.searchServices(attributes,uuidSet,btDevice,this);

    } catch (Exception e) {
      this.statusForm.append("Device Discovered Error: " + e + "\n");
    }

  }

  public void inquiryCompleted(int discType) {
      int searchID = 0;
      String url = null;
      int i = 0;
      RemoteDevice rD = null;
      StreamConnection connection = null;


      this.statusForm.append("InquiryCompleted: " + discType + "\n");
      this.statusForm.append("Determinando undefined devices...\n");

      /* comenzamos a buscar ahora sobre los dispositivos indefinidos cuales
       * nos sirven y cuales no.. */
      /* Por el momento vamos a dejar a TODOS los undefined devices como
       * aceptables */
      this.searching = this.undefinedDevices.size();
     for (i = 0; i < this.undefinedDevices.size(); i++) {
         /*
          rD = null;
          rD = (RemoteDevice) this.undefinedDevices.elementAt(i);
          if (rD == null) {
              this.undefinedDevices.removeElementAt(i);
              i--;
              continue;
          }
          url = null;
          url = BtParser.generateUrl(rD.getBluetoothAddress());

          try {
              connection = null;
              connection = (StreamConnection) Connector.open(url,
                    Connector.READ_WRITE, true);
              connection.close();
          } catch (Exception e) {

          }
          */
          
          try {
              searchID = this.discoveryAgent.searchServices(this.attributes,
                      this.uuidSet,(RemoteDevice) this.undefinedDevices.elementAt(i),this);
          } catch (Exception e) {
              this.statusForm.append("Error searchServices: " + e.toString()
                       + "\n");
              /* decrementamos en uno la busqueda */
              this.searching--;
        }

      }

      
  }

  public void servicesDiscovered(int transID, ServiceRecord[] servRecord) {
      String serviceUrl = null;
      boolean blockDevice = true;

      /* Ahora deberiamos buscar o determinar cual de los dispositivos son
       * posiblemente buenos... de esta forma vamos descartando dispositivos
       * que NO sirven
       */


      for (int i = 0; i < servRecord.length; i++) {
          serviceUrl = null;
          serviceUrl = servRecord[i].getConnectionURL(0,false);
          /* verificamos que sea del tipo de protocolo deseado */
          if (BtParser.getProtocol(serviceUrl).compareTo(
                  CityBluetooth.PROTOCOL_ACCEPTED) != 0){
              /* no son iguales => vamos al proximo servicio */
              continue;
          }
          /* si estamos aca es porque el protocolo es el correcto, veamos
           * si el puerto es el correcto */

          if(BtParser.getPort(serviceUrl).compareTo(
                  String.valueOf(this.acceptedChannel)) != 0){
              /* error, no estamso teniendo un puerto bien definido... */
              continue;
          }
          /* si aca llegamos es porque entonces si tenemos un buen
           * servicio. */
          blockDevice = false;
          break;
      }

      RemoteDevice rD = servRecord[0].getHostDevice();
      /* verificamos si es un blockdevice.. */
      if (blockDevice) {
          /* lo bloqueamos */
          if (this.undefinedDevices.contains(rD))
              this.undefinedDevices.removeElement(rD);
          else if (!this.blockDevices.contains(rD)){
              /* no lo contiene... wtf! estamos recibiendo servicios de un
               * dispositivo que no teniamos o de un dispositivo "working" pero
               * que dejo de serlo....
               */
              this.statusForm.append("servicesDiscovered: Error interno\n");
          } else {
              /* debemos agregar este dispositivo a blockDevice */
              this.statusForm.append("Agrgamos un blockDevice: " +
                      rD.getBluetoothAddress() + "\n");
              this.blockDevices.addElement(rD);
          }
      } else {
          /* no es un blockDevice => es un acceptDevice */
          if(this.undefinedDevices.contains(rD))
              this.undefinedDevices.removeElement(rD);
          if(this.blockDevices.contains(rD))
              this.blockDevices.removeElement(rD);
          if(this.workingDevices.contains(rD)) {
              /* NO DEBERIA CONTENERLO porque no estamos haciendo la busqueda
               * sobre los "workingDevices"
               */
              this.statusForm.append("servicesDiscovered: Error 2\n");
          } else {
              this.workingDevices.addElement(rD);
              this.statusForm.append("Agregamos un workingDev: " +
                      rD.getBluetoothAddress() + "\n");
          }
      }
  }

  public void serviceSearchCompleted(int transID, int responseCode) {

      this.searching--;
      switch(responseCode) {
          case SERVICE_SEARCH_ERROR:
              this.statusForm.append("ServiceSearchError: " + transID + "\n");
              break;
              
          case SERVICE_SEARCH_COMPLETED:
              this.statusForm.append("ServiceSearch completado\n");
              break;

          case SERVICE_SEARCH_TERMINATED:
              this.statusForm.append("SERVICE_SEARCH_TERMINATED: " + transID + "\n");
              break;
          case SERVICE_SEARCH_NO_RECORDS:
              this.statusForm.append("SERVICE_SEARCH_NO_RECORDS: " + transID + "\n");
              break;
          case SERVICE_SEARCH_DEVICE_NOT_REACHABLE:
              this.statusForm.append("SERVICE_SEARCH_DEVICE_NOT_REACHABLE: " +
                      transID + "\n");
              break;
      }
  }

  public Vector getWorkingDevices(){
      return this.workingDevices;
  }

  public Vector getUndefinedDevices() {
      return this.undefinedDevices;
  }

  public Vector getBlockedDevices() {
      return this.blockDevices;
  }

  public boolean isSearching() {
      return this.searching > 0;
  }
  
}
