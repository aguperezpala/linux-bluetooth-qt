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
public class ServerManager implements Runnable {
    
    private boolean isRunning = false;
    private Vector serverList = new Vector();
    private Thread smThread = null;
    private DeviceFinder devFinder = null;
    private Form statusForm = null;



    /** <<                   Funciones privadas                  >> */

    /** Funcion que intenta conectarse a un RemoteDevice, e intenta obtener
     * la lista de macs de los servidores.
     * @return  True if success | False if not
     * @param   rD != null
     */

    private boolean getServersList(RemoteDevice rD) {
        boolean result = false;
        BtConnection btCon = null;
        String url = null;
        String serverResp = null;
        int status = 0;
        Vector auxVec = null;

        if (rD == null)
            return result;

        /* generamos la url */        
        url = BtParser.generateUrl(rD.getBluetoothAddress());
       

        /* intentamos conectarnos */
        btCon = new BtConnection(url, this.statusForm,
                CityBluetooth.CONN_MAX_TIME_OUT);
        if (btCon == null || btCon.getStatus() < 0) {
            this.statusForm.append("SM: getServersList: btCon creation err\n");
            return result;
        }

        /* estamos conectados... vamos a tratar de mandarle la peticion de
         * servidores */
        status = btCon.sendData("requ:15:get_server_list");
        if (status < 0) {
            this.statusForm.append("SM: getServersList: btCon sending err\n");
            btCon.closeConnection();
            return result;
        }

        /* si llegamos aca => pudimos mandar correctamente, ahora intentamos
         * ver y desifrar que nos mando el server
         */
        serverResp = btCon.reciveData();        
        if (serverResp == null) {
            this.statusForm.append("SM: getServersList: btCon receiving err\n");
            btCon.closeConnection();
            return result;
        }
        btCon.closeConnection();

        this.statusForm.append("obtuvimos respuesta de un server: " +
                serverResp + "\n y vamos a parsear\n");
        /* obtenemos el mensaje y lo parseamos entonces */
        auxVec = BtParser.parseServerList(BtParser.getMsg(serverResp));
        if (auxVec == null) {
            this.statusForm.append("SM: getServersList: parsing ServerList err\n");
            return result;
        }
        this.statusForm.append("Ya parseamos: " + auxVec.toString() + "\n");
        
        /* si llegamos aca esta todo joya => agregamos todos los servers que
         * no tengamos ya */
        result = true;
        for (int i = 0; i < auxVec.size(); i++)
            if (!this.serverList.contains(auxVec.elementAt(i)))
                    /* lo agregamos */
                    this.serverList.addElement(auxVec.elementAt(i));
        

        return result;
    }




 /** <<                   Funciones publicas                  >> */




    public ServerManager(Form stF) {
        this.statusForm = stF;
        this.devFinder = new DeviceFinder(this.statusForm,
                CityBluetooth.RFCOMM_UUID, CityBluetooth.ATTRIBUTES,
                CityBluetooth.ACCEPTED_PORT);

        /* comenzamos a buscar de antemano */
        if (this.devFinder != null)
            this.devFinder.startFindDevices();
    }

    public void startSearchingServers() {
        /* verificamos que no este buscando */
        if(this.isRunning)
            return;
        /* generamos todo */
        this.smThread = new Thread(this);
        this.smThread.start();
    }

    public void stopSearching() {
        if ((this.smThread.isAlive() || this.isRunning)) {
            this.smThread.interrupt();
            this.smThread = null;
        }
        this.isRunning = false;
    }

    public Vector getServerVector() {
        return this.serverList;
    }


    public void run() {
        boolean haveFound = false;
        Vector workDevs = null;
        RemoteDevice rD = null;
        int retry = 4;  /* cantidad de veces que vamos a buscar esto */


        this.isRunning = true;
        
        /* vamos a esperar primero que todo a que el devFinder termine de
         * buscar dispositivos... una vez que termine vamos a intentar
         * conectarnos a cada uno de ellos para obtener la lista de servers
         */
        while(!haveFound && retry > 0) {
            retry--;
            /* buscamos y esperamos esperamos */
            if (!this.devFinder.isSearching())
                this.devFinder.startFindDevices();
            while (this.devFinder.isSearching())
                try {
                    Thread.sleep(500);
                } catch (Exception e){}

            /* ya dejo de buscar, verificamos si tenemos dispositivos */
            workDevs = this.devFinder.getWorkingDevices();
            if (workDevs == null || workDevs.size() == 0) {
                /* no encontramos nada => hacemos una pausa y volvemos a buscar */
                try {
                    /* deberiamos setear una constante cada cuanto volver a
                     * buscar */
                    Thread.sleep(10000);
                } catch (Exception e){}
                this.statusForm.append("ServerManager: no encontramos devs\n");
                continue;
            }
            /* si estamos aca es porque SI encontramos dispositivos =>
             * vamos a intentar conectarnos e intentar obtener la lista
             * de verdaderos servidores */
            for (int i = 0; i < workDevs.size(); i++) {
                rD = null;
                rD = (RemoteDevice) workDevs.elementAt(i);

                /* ahora vamos a verificar si podemos obtener la lista o no
                 * de este server... */
                if (this.getServersList(rD) == false) {
                    /* no pudimos => lo eliminamos del vector y seguimos */
                    workDevs.removeElementAt(i);
                    i--;
                    continue;
                }
                /* si estamos aca es porque ya obtuvimos lo que queriamos
                 * y tenemos en this.serverList la lista con todos los
                 * servidores => salimos diretcamente */
                haveFound = true;
                break;
            }
        }
        this.isRunning = false;
    }


}
