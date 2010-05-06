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
    private ServerManager sm = null;


    public CityClient(Form st, ServerManager serverM) {

        this.status = st;
        this.sm = serverM;
        
    }

    
    /** Funcion que envia a cualquiera de los servidores disponibles
     * los datos pedidos.
     * @param  cmd  != null
     * @param  data != null
     * @return:
     *      -1  si no se encontro server
     *      -2  no se pudo conectar a ningun server
     *      -3  error interno
     *      >=0 if success
     */
     public int sendData(String cmd, String data) {         
         String dataToSend = cmd + ":" + data.length() + ":" + data;
         Vector servers = null;
         BtConnection btCon = null;
         String url = null;
         int result = -3;
         int i = 0;
         

         if (this.status != null)
             /* limpiamos la barra de estado (el log) */
             this.status.deleteAll();

         if (cmd == null || cmd.length() < 4 || data == null) {
             this.status.append("CC:sendData: cmd | data null\n");
             return -3;
         }

         /* obtenemos la lista de servers diponibles */
         servers = this.sm.getServerVector();
         if (servers == null || servers.size() == 0) {
             /* no tenemos naranja => devolvemos no se encontro server */
             return -1;
         }

         /* si tenemos server => entonces vamos a intentar conectarnos
          * a alguno y vamos a intentar mandar los datos */
         for (i = 0; i < servers.size(); i++) {
             btCon = null;
             url = null;
             url = BtParser.generateUrl((String) servers.elementAt(i));
             /* creamos la conexion */
             btCon = new BtConnection(url, this.status,
                     CityBluetooth.CONN_MAX_TIME_OUT);
             if (btCon == null || btCon.getStatus() < 0) {
                 /* no nos pudimos conectar al server => lo vamos a dejar por
                  * si estamos fuera del alcance. lo que deberiamos hacer
                  * es insertarlo al final... (reordenar el arreglo).
                  * */
                 /* por el momento simplemente vamos a pasar al siguiente
                  * server */
                 continue;
             }

             /* si estamos aca es porque si nos pudimos conectar, entonces
              * vamos a intentar mandar los datos */
             result = btCon.sendData(dataToSend);
             if (result < 0) {
                 this.status.append("CC:sendData: btCon.sendData() < 0\n");
                 /* hubo un error pero nos pudimos conectar... ???
                  * deberiamos nuevamente reordenar el vector e introducir este
                  * server al final */
                 /* ahora simplemente vamos a volver cerrar la conexion y buscar
                  * otro server */
                 btCon.closeConnection();
                 continue;
             }

             /* pudimos mandar todo asique esta genial */
             btCon.closeConnection();
             break;
         }

         /* ahora vamos a verificar porque salimos del ciclo */
         if (i < servers.size() && result >= 0) {
             /* pudimos mandar correctamente */
             return result;
         } else
             /* no pudimos mandar por ningun server */
             return -2;
     }




}
