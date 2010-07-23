/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hello;

import javax.bluetooth.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

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
    private ServerManager serverM = null;
    private CityClient cClient = null;
    private TextBox txtBox = null;
    private TextField txtFCode = null;
    private TextField txtFNick = null;
    private Form statusFrom = null;
    private Form generalForm = null;
    private String[] menuLabels = { "Registrarse", "Enviar mensaje", "Ayuda", "Log" };
    private final List menu = new List("Menu", List.IMPLICIT, menuLabels, null);
    private boolean itsRegistered = false;


  
    private Command exitCommand;    
    private Command okCommand;
    private Command backCommand;
  
    /**
     * The CityBluetooth constructor.
     */
    public CityBluetooth() {        
        this.statusFrom = new Form("Log");
        this.generalForm = new Form(" ");
        this.txtFCode = new TextField("Codigo:", "", 8, TextField.ANY);
        this.txtFNick = new TextField("Nick:", "", 15, TextField.ANY);
        
        /* generamos los botones */
        this.exitCommand = new Command("Salir", Command.EXIT, 1);
        this.backCommand = new Command("Volver", Command.BACK, 0);
        this.okCommand = new Command("Ok", Command.OK, 2);
        
        /* creamos las estructuras propias */
        this.serverM = new ServerManager(this.statusFrom);
        this.cClient = new CityClient(this.statusFrom, this.serverM);
    


        
        this.txtBox = new TextBox("CityEntert", "", 256,0);
        this.txtBox.addCommand(backCommand);
        this.txtBox.addCommand(okCommand);
        this.txtBox.setCommandListener(this);

        this.generalForm.append(txtFCode);
        this.generalForm.append(txtFNick);
        this.generalForm.addCommand(okCommand);
        this.generalForm.addCommand(backCommand);
        this.generalForm.setCommandListener(this);


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
                    if(this.itsRegistered)
                        return;
                    // Registrandose
                    this.txtFCode.setString("");
                    this.txtFNick.setString("");

                    Display.getDisplay(this).setCurrent(this.generalForm);
                    break;
                case 1:
                    // mandando mensaje
                    this.txtBox.setString("");
                    Display.getDisplay(this).setCurrent(this.txtBox);
               
                    break;
                case 2:
                    //Ayuda...
                    this.statusFrom.deleteAll();
                    this.statusFrom.append("Version de Prueba\n ante cualquier "+
                            "duda o problemas comunicarse con Matias Parmigiani " +
                            "mail: matiaspar@gmail.com\n o cel: 152135859\nMuchas gracias.");
                    Display.getDisplay(this).setCurrent(this.statusFrom);
                   // form.append("Ayuda");
               
                    break;
                case 3:
                    // Log
                    //form.append("Log");

                    break;
                default: // this shouldn't occur
                    break;
            }            
            
        } else if (displayable == this.generalForm) {
            /* nos estamos registrando! */
            if(command == this.okCommand) {
                int status = 0;
                String auxStr = "";
                boolean nickOk = false;

                /* ahora mostramos el log */
                Display.getDisplay(this).setCurrent(this.statusFrom);

                /* verificamos si el codigo es correcto! */
                if (CodeManager.checkCode(this.txtFCode.getString()) == false)
                {
                    /* el codigo es incorrecto => salimos */
                    this.statusFrom.append("Codigo incorrecto\n");
                    return;
                }
                /* ahora verificamos si el nick es correcto (teniendo en cuenta
                 * que los ultimos 3 digitos del nick tienen que ser numericos */
                auxStr = this.txtFNick.getString();
                auxStr = this.txtFNick.getString().substring(auxStr.length()-4,
                        auxStr.length()-1);
                try {
                    Integer.parseInt(auxStr);
                    nickOk = true;
                } catch (Exception e) {
                    nickOk = false;
                }

                if (!nickOk) {
                    this.statusFrom.append("Nick incorrecto, debe las 3 ultimas "+
                            "cifras del documento al final del nick\n");
                    return;
                }

                /* Si estamos aca es porque ta todo joya */
                /* intentamos mandar los datos */
                status = this.cClient.sendData("regi",
                        this.txtFCode.getString() + " " + this.txtFNick.getString());

                this.statusFrom.append("Espere por favor... \n");
                /* verificamos que nos devolvio */
                if (status >= 0) {
                    this.statusFrom.append("Codigo de registracion " +
                            "enviado correctamente\n");

                    /* Vamos a eliminar del menu la posibilidad de mandar codigo
                     * de registracion nuevamente */
                    this.menuLabels[0] = "-Registrado-";

                } else if (status == -1 || status == -2) {
                    this.statusFrom.append("\nNo hay servers disponibles. " +
                            "Porfavor vuelva a intentarlo mas tarde\n");
                    /* volvemos a activar el servicio de busqueda si y
                     * solo si NO encontramos servers */
                } else {
                    /* error interno => no pudimos mandar nada */
                    this.statusFrom.append("No se pudo enviar el codigo "+
                            "de registracion\n");
                }
            }

        } else if (displayable == this.txtBox) {
            /* vamos a verificar si apretaron ok */
            if(command == this.okCommand) {
                int status = 0;                
               
                /* ahora mostramos el log */
                Display.getDisplay(this).setCurrent(this.statusFrom);

                /* intentamos mandar el mensaje */
                status = this.cClient.sendData("text",
                        this.txtBox.getString());

                this.statusFrom.append("Espere por favor...\n");

                if (status >= 0) {
                    this.statusFrom.append("Mensaje enviado correctamente\n");
                } else if (status == -1 || status == -2) {
                    this.statusFrom.append("No hay servers disponibles. " +
                            "Porfavor vuelva a intentarlo mas tarde\n");
                  /* volvemos a activar el servicio de busqueda si y
                     * solo si NO encontramos servers */
                } else {
                    /* error interno => no pudimos mandar nada */
                    this.statusFrom.append("No se pudo enviar el mensaje\n");
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
