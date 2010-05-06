/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hello;

/**
 *
 * @author agustin
 */
public class BtParser {
    //btspp://6A54017D1A00:11;authenticate=false;encrypt=false;master=false
    static String getProtocol(String addr){
        int pos = addr.indexOf(':');
        return addr.substring(0, pos);
    }

    static String getMac(String addr){
        int sPos = addr.indexOf('/') + 2;
        int ePos = addr.indexOf(':', sPos);

        return addr.substring(sPos, ePos);
    }

    static String getPort(String addr){
        int sPos = addr.lastIndexOf(':') + 1;
        int ePos = addr.indexOf(';', sPos);

        return addr.substring(sPos, ePos);
    }

    static String getParams(String addr){
        int sPos = addr.indexOf(';') + 1;
        int ePos = addr.length();

        return addr.substring(sPos, ePos);
    }

    /* funcion que verifica si tenemos alguna respuesta */
    static boolean haveResponse(String data) {
        int sepPos = data.indexOf(":");
        int sepPos2 = 0;
        String cmd = null;
        int sizeOfData = 0;

        
        if (sepPos < 4)
            return false;

        cmd = data.substring(sepPos - 4, sepPos);
        /* FIXME: deberiamos chequear el cmd sea uno de los que queremos */
        sepPos2 = data.indexOf(":", sepPos+1);
        sizeOfData = Integer.parseInt(data.substring(sepPos + 1, sepPos2));

        if (sepPos2 <= sepPos)
            return false;

        if (data.length() != sizeOfData + sepPos2 + 1)
            return false;

        cmd = null;

        return true;
    }

    /* Funcion que devuelve el mensaje de una respuesta
     * RETURNS:
     *      null    on error
     *      msg     if success
     */
    static String getMsg(String data) {
        int sepPos = data.indexOf(":");
        int sepPos2 = 0;
        String cmd = null;
        int sizeOfData = 0;


        if (sepPos < 4)
            return null;

        sepPos2 = data.indexOf(":", sepPos+1);
        sizeOfData = Integer.parseInt(data.substring(sepPos + 1, sepPos2));

        if (sepPos2 <= sepPos)
            return null;

        if (data.length() != sizeOfData + sepPos2 + 1)
            return null;

        /* no hay error => devolvemos los datos */
        return data.substring(sepPos2 + 1, sepPos2 + 1 + sizeOfData);

    }



    /* Funcion que parsea informacion enviada por el server y devuelve el codigo
     * de error.
     * RETURNS:
     *      < 0     if error
     *      { -1 = cmd:resp & data=ERROR, -2 = cmd:erro, -3 = unknown }
     *      == 0    otherwise
     */
    static int parseRecvData(String rData) {
        int result = -1;
        String cmd = null;
        int len = 0;
        String data = null;

        if(rData == null)
            return result;

        /* parseamos */
        cmd = rData.substring(0, 4);
        len = Integer.parseInt(rData.substring(6, rData.indexOf(":", 6)));
        data = rData.substring(rData.indexOf(":", 6) + 1, len);

        if (cmd.compareTo("errp") == 0)
            return -2;
        if (cmd.compareTo("resp") == 0){
            if (data.compareTo("ERROR") == 0)
                return -1;
            else if (data.compareTo("OK") == 0)
                return 0;
        } else
            /* tamos recibiendo cualquier otra cosa... */
            result = -3;


        return result;
    }

        

}
