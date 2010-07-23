/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hello;


import javax.microedition.io.*;
import java.io.*;
import javax.microedition.lcdui.Form;


/**
 *
 * @author agustin
 */
public class BtConnection implements Runnable {

    private Form statusForm = null;
    private String strUrl = null;
    private StreamConnection connection = null;
    private OutputStream output = null;
    InputStream input = null;    
    /* el status es < 0 en caso de error */
    private int status = 0;
    private int waitMs = 0;
    private Thread thisTrhead = null;


    public BtConnection(String url, Form statusF, int timeOut){

        this.statusForm = statusF;
        this.strUrl = url;
        this.waitMs = timeOut;

        this.statusForm.append("Creando conn a: " + url + "\n");

        try {
            this.connection = (StreamConnection)Connector.open(this.strUrl,
                    Connector.READ_WRITE, true);

            
            /* comenzamos con el time out */
            this.thisTrhead = new Thread(this);
            this.thisTrhead.start();
            
            this.statusForm.append("Conexion establecida con: " + this.strUrl
                     + "\n");
        } catch (Exception e) {
            this.statusForm.append("Error al crear la conexion " + e.toString()
                     + "\n");
            this.status = -1;
        }
    }

    /** Funcion que manda datos en limpio
     * @param string to be sent
     *
     * @return bytes sent || < 0 on error
     */
    public int sendData(String data) {
        int result = data.length();

        try {
            output = this.connection.openOutputStream();
            //output.write(data.length());
            output.write(data.getBytes());
            output.close();
        } catch (Exception e) {
            result = -1;
            this.status = -1;
            this.statusForm.append("Error al intentar mandar datos " +
                    e.toString() + "\n");
        }
        output = null;

        return result;
    }

    /** Funcion que sirve para recibir datos en limpio
     * @return String con los datos recibidos (si es vacio || null => no se
     *          recibio nada ==> hubo error..)
     */
    public String reciveData(){
        String result = null;

    
        try {
            input = this.connection.openInputStream();
            int bytesRead = 0, bytesToRead = 0;
            int i = 0, retry = 3;
            boolean haveResp = false, timedOut = false;
            result = new String("");

            while (bytesRead < CityBluetooth.CONN_MAX_DATA_SIZE && !haveResp
                    && !timedOut) {
                 /* primero que todo vamos a esperar a tener algun dato
                  * viable para leer */
                 while(input.available() == 0 && retry > 0 &&
                         this.waitMs > 0){
                     /* vamos a esperar hasta que haya datos listos si y solo
                      * si no nos pasamos con el timeOut */
                     this.addNewTimeOut(500);                             
                     try {
                         Thread.sleep(500);
                     } catch (Exception e) {}

                     if (this.waitMs <= 0) {
                         timedOut = true;
                         break;
                     }
                     retry--;
                 }
                 /* verificamos si salimos por timed out */
                 if (this.waitMs <= 0 || input.available() == 0)
                     break;

                 /* si estamos aca es porque no salimos por timed out */
                 bytesToRead = input.available();                 
                 /* leemos bytesToRead bytes */
                 for (i = 0; i < bytesToRead; i++)
                     result += (char) input.read();

                 bytesRead += i;
                 haveResp = BtParser.haveResponse(result);

             }

            this.statusForm.append("haveResp: " + haveResp + "\tRecibimos: " +
                    result + "\n");
            

        } catch (IOException e) {
            this.statusForm.append("error al recibir datos: " + e.toString()+ "\n");
        } finally {
            // close input stream
            if (input != null) {
                try {
                    input.close();
                } catch (IOException e) {
                    this.statusForm.append("error al cerrar la conexion para recibir "
                            + e.toString() + "\n");
                    this.status = -1;
                }
            }
        }
        input = null;        
        return result;
  }
     public void run() {
         int wMs = 0;
        /* simplemente esperamos waitMs y luego cerramos la conexion */
         while (this.waitMs > 0) {
             try {
                 wMs = this.waitMs;
                 Thread.sleep(wMs);
             } catch (Exception e) {
                 /* .. */
             }
             this.waitMs = this.waitMs - wMs;
         }
         if (this.status >= 0) {
            this.statusForm.append("Conexion time out\n");
            this.closeConnection();
         }

    }

    public int getStatus() {
        return this.status;
    }

    public void addNewTimeOut(int ms) {
        this.waitMs += ms;
    }

    public void closeConnection(){
        this.status = -1;
        try {
            /* interrumpimos el thread */
            if (this.thisTrhead != null) {
               // this.thisTrhead.interrupt();
                Thread.yield();
                this.thisTrhead = null;
            }

            this.waitMs = 0;

            if (this.output != null) {
                this.output.close();
                this.output = null;
            }

            if (this.input != null){
                this.input.close();
                this.input = null;
            }

            if (this.connection != null){
                this.connection.close();
                this.connection = null;
            }
        } catch (IOException ioe) {
      	  this.statusForm.append("Error Closing connection " + ioe+ "\n");
        }
    }


    

}
