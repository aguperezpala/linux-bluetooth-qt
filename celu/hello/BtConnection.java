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
            output.write(data.length());
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
        byte[] data = null;
        byte[] readData = null;
        String result = null;

    
        try {
            input = this.connection.openInputStream();            
            data = new byte[255];
            int byteRead = 0;
            int i = 0;
            boolean haveResp = false;

            
            /* FIXME: aca estamos suponiendo que siempre tenemos datos al
             * instante para leer, no siempre es asi, deberiamos cambiar esto
             * porque no nos esta andando el tema del timeout para la lectura.
             */
            i = 10;
             while (i > 0 && input.available() == 0) {
                 this.addNewTimeOut(500);
                 try {
                     Thread.sleep(500);
                 } catch (Exception e) {}
                 i--;
             }
            this.statusForm.append("Podemos leer sin que se clave: " +
                    input.available() + "\n y byte data[] = " + String.valueOf(data));

            i = 0;
            this.statusForm.append("\nEmpezamos con el ciclo\n");
            
            while (input.available() > 0)
                this.statusForm.append("char: "+ (char)input.read()+"\n");
            
            this.statusForm.append("terminamos con el ciclo\n");
            /*while (byteRead >= 0 && !haveResp && i < 255 && input.available() > 0) {
                this.statusForm.append("Entrando al ciclo de lectura i:" + i + "\n");
                byteRead = input.read(data, i, 255);
                this.statusForm.append("leimos: " + byteRead + "\n");
                i += byteRead;
                readData = null;
                readData = new byte[i];
                System.arraycopy(data, 0, readData, 0, i);
                haveResp = BtParser.haveResponse(String.valueOf(readData));
                this.statusForm.append("Recibiendo - " + byteRead + ": " + 
                        String.valueOf(readData) + "\n");
            }*/
            if (haveResp) {
                result = String.valueOf(readData);
                readData = null;
                this.statusForm.append("Terminamos de recibir:" + result + "\n");
            } else {
                data = null;
                readData = null;
            }

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
         this.statusForm.append("Conexion time out\n");
         this.closeConnection();

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
            this.thisTrhead.interrupt();
            this.waitMs = 0;

            if (this.output != null)
                this.output.close();
            if (this.input != null)
                this.input.close();
            this.connection.close();
        } catch (IOException ioe) {
      	  this.statusForm.append("Error Closing connection " + ioe+ "\n");
        }
    }


    

}
