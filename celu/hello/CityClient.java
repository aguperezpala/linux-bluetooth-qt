/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hello;

import java.util.Vector;
import javax.bluetooth.RemoteDevice;
import javax.microedition.lcdui.Form;


/**
 *
 * @author agustin
 */
public class CityClient {
    private Form status = null;
    private DeviceFinder df = null;


    public CityClient(Form st, DeviceFinder devF) {

        this.status = st;
        this.df = devF;
        
    }

    
    /* Funcion que va a intentar mandar un dato, en caso de error devuelve
     * < 0. Va a buscar algun dispositivo e intentar mandar datos y espera
     * una respuesta (del server).
     */
     public int sendData(String cmd, String data) {
         int result = 0;
         String dataToSend = cmd + ":" + data.length() + ":" + data;
         Vector workDevs = null;
         String url = null;
         boolean flag = false;
         int i = 0;
         

         if (this.status != null)
             /* limpiamos la barra de estado (el log) */
             this.status.deleteAll();
         
         /* ahora lo que vamos hacer es primero verificar si tenemos 
          * algun device confiable al cual enviar
          */
         workDevs = this.df.getWorkingDevices();
         if (workDevs == null || workDevs.size() == 0) {
             /* debemos empezar a buscar dispositivos */
             if (!this.df.isSearching())
                this.df.startFindDevices();

             // TODO: probablemente debamos esperar a que se terminen de buscar
             while (this.df.isSearching() && !flag)
             {
                 try {
                     Thread.sleep(500);
                 } catch (Exception e) {
                     flag = true;
                     this.status.append(e.toString() + "\n");
                 }
             }
             workDevs = this.df.getWorkingDevices();
         }
         /* si no tenemos devs aca devolvemos error */
         if (workDevs == null || workDevs.size() == 0) {
             this.status.append("No encontramos ningun dev\n");
             return -1;
         }
         /* si tenemos dipositivos para mandar => intentamos mandarle
          * a cualquiera de las conexiones
          */
         this.status.append("Buscando servidor...\n");
         flag = false;
         for (i = 0; i < workDevs.size() && !flag; i++) {
             RemoteDevice rD = (RemoteDevice) workDevs.elementAt(i);
             if (rD == null){
                 workDevs.removeElementAt(i);
                 continue;
             }             
             //btspp://6A54017D1A00:11;authenticate=false;encrypt=false;master=false
             url = CityBluetooth.PROTOCOL_ACCEPTED;
             url += "://";
             url += rD.getBluetoothAddress();
             url += ":" + Integer.toString(CityBluetooth.ACCEPTED_PORT) + ";";
             url += CityBluetooth.PARAMS;
             
             // creamos la conexion
             BtConnection btConn = new BtConnection(url, this.status,
                     CityBluetooth.CONN_MAX_TIME_OUT);
             if (btConn.getStatus() < 0) {
                 // no se pudo conectar => tiramos al chori la conexion
                 workDevs.removeElementAt(i);
                 i--;
                 continue;
             }
            
             /* ahora si nos conectamos entonces intentamos mandar datos */
             if (btConn.sendData(dataToSend) >= 0) {
                 int respCode = 0;
                 // pudimos mandar :D => esperamos respuesta del server..
                 String sResp = btConn.reciveData();
                 if (sResp == null || sResp.length() == 0) {
                     /* tenemos un error al recibir los datos... */
                     btConn.closeConnection();
                     btConn = null;
                     // probamos la siguiente conexion
                     continue;
                 }
                 // recibimos datos? debemos ver si son correctos
                 respCode = BtParser.parseRecvData(sResp);
                 /* de cualquier forma vamos a cerrar la conexion */
                 btConn.closeConnection();
                 btConn = null;
                 if (respCode == 0) {
                     // obtuvimos una buena respuesta
                     this.status.append("Se envio correctamente\n");
                     flag = true;
                 } else if (respCode == -1) {
                     this.status.append("No estas registrado para mandar datos\n");
                     flag = true;
                 } else {
                     this.status.append("Error al intentar conectarse al server\n");
                     continue;
                 }

             } else {
                 /* no pudimos mandar nada de datos... entonces anda mal el
                  * server, vamos a seguir buscando, pero avisamos de paso que
                  * anda mal el server...
                  */
                 this.status.append("Error al intentar conectarse al servidor,"+
                         " reintentando...\n");
                 btConn.closeConnection();
                 btConn = null;
                 continue;
             }
         }

         /* verificamos si salimos por la flag... o porque pudimos mandar datos
          * o por que causa fue */
         if (flag == false && (i < workDevs.size())) {
             /* pudimos mandar correctamente los datos */
             result = 1;
         } else
             /* no se pudieron mandar correctamente los datos => deberiamos
              * revisar que todos los devices que tenemos esten funcionando
              * bien */
             result = -1;
         

         return result;
     }




}
