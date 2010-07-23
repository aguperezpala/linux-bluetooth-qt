/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hello;

import java.util.Calendar;

/**
 *
 * @author agustin
 */
public class CodeManager {

    /*! vamos a definir los codigos de la siguiente forma:
 * XXXYYYYY
 * Donde XXX va a ser el numero del codigo (numCode) generado y YYYYY va a ser
 * el numero tal que es igual a: numDay * numMonth * numCode
 * Representado en hexadecimal, y va a tener que ser enviado como tal.
 * Los codigos van a ser validos si y solo si son validos del dia o del dia
 * anterior!
 */

    static boolean checkCode(String code)
    {
        int codeIndex = 0;
        int codePro = 0;
        int res1, res2;
        int nd, nm, yd, ym;
        Calendar cal = Calendar.getInstance();

        if (code.length() != 8)
            return false;

        /* extraemos el mes/dia de ahora y ayer */
        nd = cal.get(Calendar.DAY_OF_MONTH);
        nm = cal.get(Calendar.MONTH);
        cal.set(Calendar.DATE, cal.get(Calendar.DATE) - 1);
        yd = cal.get(Calendar.DAY_OF_MONTH);
        ym = cal.get(Calendar.MONTH);
        System.out.print("hoy- dia:" + nd + " mes:" + nm + "\nayer dia:" + yd + " mes:" + ym + "\n");

        /* primero obtenemos el numero de codigo */
        codeIndex = Integer.parseInt(code.substring(0, 2), 10);
        codePro = Integer.parseInt(code.substring(3, 7), 10);

        /* ahora vamos a verificar si es correcto el codigo */
        res1 = codeIndex * nd * nm;
        res2 = codeIndex * yd * ym;

        if (codePro == res1 || codePro == res2)
            return true;

        return false;
    }


}
