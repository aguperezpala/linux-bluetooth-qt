/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hello;

import java.util.Vector;
import javax.microedition.lcdui.Form;


/**
 *
 * @author agustin
 */
public class CityClient implements Runnable {
    private Form status = null;
    private ServerManager sm = null;
    private String dataToSend = null;
    private Thread ccThread = null;
    private boolean isRunning = false;
    private int sendResult = 0;


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
         int waitMs = CityBluetooth.CONN_MAX_TIME_OUT * 2;

         /* vamos a revisar que no se este mandando nada */
         if (this.isRunning)
             return -3;

         dataToSend = null;
         if (this.status != null)
             /* limpiamos la barra de estado (el log) */
             this.status.deleteAll();

          if (cmd == null || cmd.length() < 4 || data == null) {
             this.status.append("CC:sendData: cmd | data null\n");
             return -3;
         }
         dataToSend = cmd + ":" + data.length() + ":" + data;

         /* vamos a ejecutar en un nuevo thread el envio */
         this.ccThread = new Thread(this);
         this.ccThread.start();
         this.isRunning = true;
         /* ahora vamos a esperar un determinado tiempo de envio
          * si no se envia vamos a interrumpir todo y a devolver error */
         while (waitMs > 0 && this.isRunning) {
             try {
                 Thread.sleep(300);
             } catch (Exception e) {}
             waitMs = waitMs - 300;
         }
         /* si llegamos aca y no se mando => isRunning = true ==> error interno
          */
         if (this.isRunning) {
             //this.ccThread.interrupt();
             Thread.yield();
             this.ccThread = null;
             this.isRunning = false;
             /* limpiamos con el gbc */
             System.gc();
             return -3;
         }
         
         return this.sendResult;
     }

    public void run() {
         Vector servers = null;
         BtConnection btCon = null;
         String url = null;
         int result = -3;
         int i = 0;


         /* obtenemos la lista de servers diponibles */
         servers = this.sm.getServerVector();
         if (servers == null || servers.size() == 0) {
             /* no tenemos naranja => devolvemos no se encontro server */
             this.sendResult = -1;
             this.isRunning = false;
             return;
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
             this.sendResult = result;
         } else {
             /* no pudimos mandar por ningun server */
             this.sendResult = -2;
         }
         this.isRunning = false;
    }




}
