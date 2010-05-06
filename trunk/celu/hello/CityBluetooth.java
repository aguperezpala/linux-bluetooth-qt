/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hello;

import javax.bluetooth.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.io.*;
import java.io.*;

/**
 * @author agustin
 */
public class CityBluetooth extends MIDlet implements CommandListener {

    private boolean midletPaused = false;
    /* definimos cual va a ser el UUID que debemos buscar */
    public static final UUID RFCOMM_UUID = new UUID(0x0003);
    public static final int[] ATTRIBUTES = {0x100,0x101,0x102};
    public static final int ACCEPTED_PORT = 11;
    public static final String PARAMS = "authenticate=false;encrypt=false;master=false";
    public static final String PROTOCOL_ACCEPTED = "btspp";
    public static final int CONN_MAX_DATA_SIZE = 255; /* bytes */
    public static final int CONN_MAX_TIME_OUT = 10000; /* ms */



    
    /* Definimos la clase DeviceFinder */
    private DeviceFinder devFinder = null;
    private CityClient cClient = null;
    private TextBox txtBox = null;
    private Form statusFrom = null;
    private String[] menuLabels = { "Registrarse", "Enviar mensaje", "Ayuda", "Log" };
    private final List menu = new List("Menu", List.IMPLICIT, menuLabels, null);

  
    private Command exitCommand;    
    private Command okCommand;
    private Command backCommand;
  
    /**
     * The CityBluetooth constructor.
     */
    public CityBluetooth() {        
        this.statusFrom = new Form("Log");
        
        /* generamos los botones */
        this.exitCommand = new Command("Salir", Command.EXIT, 1);
        this.backCommand = new Command("Volver", Command.BACK, 0);
        this.okCommand = new Command("Elegir", Command.OK, 2);

        /* creamos las estructuras propias */
        this.devFinder = new DeviceFinder(this.statusFrom,RFCOMM_UUID, ATTRIBUTES,
                ACCEPTED_PORT);
        this.devFinder.startFindDevices();
        this.cClient = new CityClient(this.statusFrom,this.devFinder);


        this.txtBox = new TextBox("CityEntert", "", 256,0);
        this.txtBox.addCommand(backCommand);
        this.txtBox.addCommand(okCommand);
        this.txtBox.setCommandListener(this);


        this.menu.addCommand(exitCommand);
        this.menu.addCommand(okCommand);
        this.menu.setCommandListener(this);

        this.statusFrom.addCommand(backCommand);        
        this.statusFrom.setCommandListener(this);


    }

    /**
     * Initilizes the application.
     * It is called only once when the MIDlet is started. The method is called before the <code>startMIDlet</code> method.
     */
    private void initialize() {

    }
    /**
     * Performs an action assigned to the Mobile Device - MIDlet Started point.
     */
    public void startMIDlet() {

        switchDisplayable(null, this.menu);

    }

/**
     * Performs an action assigned to the Mobile Device - MIDlet Resumed point.
     */
    public void resumeMIDlet() {

    }

    
    /**
     * Switches a current displayable in a display. The <code>display</code> instance is taken from <code>getDisplay</code> method. This method is used by all actions in the design for switching displayable.
     * @param alert the Alert which is temporarily set to the display; if <code>null</code>, then <code>nextDisplayable</code> is set immediately
     * @param nextDisplayable the Displayable to be set
     */
    public void switchDisplayable(Alert alert, Displayable nextDisplayable) {
        // write pre-switch user code here
        Display display = Display.getDisplay(this);
        if (alert == null) {
            display.setCurrent(nextDisplayable);
        } else {
            display.setCurrent(alert, nextDisplayable);
        }
    }

    /**
     * Called by a system to indicated that a command has been invoked on a particular displayable.
     * @param command the Command that was invoked
     * @param displayable the Displayable where the command was invoked
     */
    public void commandAction(Command command, Displayable displayable) {

        if (command == backCommand) {
                /* volvemos a mostrar la ventana principal */
                switchDisplayable(null, this.menu);
                return;
        } else if (command == exitCommand)
            /* si es el exit ==> salimos asi nomas */
            exitMIDlet();


        if (displayable == this.menu) {
            switch (menu.getSelectedIndex()) {
                case 0:
                    // Registrandose
                    this.txtBox.setString("Codigo de registracion");
                    Display.getDisplay(this).setCurrent(this.txtBox);
                    break;
                case 1:
                    // mandando mensaje
                    this.txtBox.setString("Mensaje");
                    Display.getDisplay(this).setCurrent(this.txtBox);
               
                    break;
                case 2:
                    //Ayuda...
                   // form.append("Ayuda");
               
                    break;
                case 3:
                    // Log
                    //form.append("Log");

                    break;
                default: // this shouldn't occur
                    break;
            }            
            
        } else if (displayable == this.txtBox) {
            /* vamos a verificar si apretaron ok */
            if(command == this.okCommand) {
                
                switch(this.menu.getSelectedIndex()) {
                    case 0:
                        //Registrandose

                        /* ahora mostramos el log */
                        Display.getDisplay(this).setCurrent(this.statusFrom);
                         try {
                     Thread.sleep(500);
                 } catch (Exception e) {
                 }

                        /* intentamos mandar los datos */
                        if (this.cClient.sendData("regi",
                                this.txtBox.getString()) < 0)
                            this.statusFrom.append("Error al enviar el codigo de" +
                                    " registracion\n");
                        break;

                    case 1:
                        //Mandando Mensaje

                        /* ahora mostramos el log */
                        Display.getDisplay(this).setCurrent(this.statusFrom);
                        
                        /* intentamos mandar el mensaje */
                        if (this.cClient.sendData("text",
                                this.txtBox.getString()) < 0)
                            this.statusFrom.append("Error al enviar el mensaje");

                        break;
                    case 2:
                        //form.append("Ayuda");

                        break;
                    case 3:
                        //form.append("Log");

                        break;
                    default: // this shouldn't occur
                        break;
                }


            }

        }

    }

  
    /**
     * Exits MIDlet.
     */
    public void exitMIDlet() {
        switchDisplayable (null, null);
        destroyApp(true);
        notifyDestroyed();
    }

    /**
     * Called when MIDlet is started.
     * Checks whether the MIDlet have been already started and initialize/starts or resumes the MIDlet.
     */
    public void startApp() {
        if (midletPaused) {
            resumeMIDlet ();
        } else {
            initialize ();
            startMIDlet ();
        }
        midletPaused = false;
    }

    /**
     * Called when MIDlet is paused.
     */
    public void pauseApp() {
        midletPaused = true;
    }

    /**
     * Called to signal the MIDlet to terminate.
     * @param unconditional if true, then the MIDlet has to be unconditionally terminated and all resources has to be released.
     */
    public void destroyApp(boolean unconditional) {
    }

}
