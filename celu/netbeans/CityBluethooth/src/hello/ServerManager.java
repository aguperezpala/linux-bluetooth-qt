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
public class ServerManager  {
    
    private Vector serverList = new Vector();
    private Form statusForm = null;



  
    public ServerManager(Form stF) {

        this.statusForm = stF;
        /** Aca es donde se agregan la lista de servidores. */
        this.serverList.addElement(new String("111111111111"));
       
    }

   

    public Vector getServerVector() {
        return this.serverList;
    }


}
