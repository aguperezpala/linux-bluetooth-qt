#ifndef CONSTS_H
#define CONSTS_H

/* cantidad de caracteres de una mac CONTANDO los ':'. 11:22:33:44:55:66 */
#define MAC_SIZE	17	

#define qstrtochar(x)	x.toStdString().c_str()


/*! Algunas respuestas para los protocolos */
#define PRO_RESP_FALSE		"<false>"
#define PRO_RESP_TRUE		"<true>"
#define PRO_RESP_OK		"ok"



/*! 		OBEX RECEIVER			*/
#define OBREC_START_PORT		4561
#define OBREC_END_PORT			4566
#define OBREC_BUFF_SIZE			4096	/* 4 kb alcanzaran? */
#define OBREC_MAX_PKT_SIZE		1024	/* tamaño max de <file,MAC> */
#define OBREC_PKT_SEPARATOR_B		"{"
#define OBREC_PKT_SEPARATOR_E		"}"
#define OBREC_DATA_SEPARATOR_B		"<"	/* begin data separator */
#define OBREC_DATA_SEPARATOR_E		">"	/* end data separator */


/*! Tamaño para los distintos tipos de archivos... Esto es muy relativo, 
  * podriamos tambien verificar la extencion de los archivos, y ese tipo de 
  * cosas
  */
#define FILE_TEXT_MAX_SIZE		512	/* bytes */
/* Vamos a considerar que si un archivo es mayor que 512 bytes => es una imagen
#define FILE_PICTURE_MAX_SIZE		4194304	// 4 Mbytes 
*/



/*! 		tamaño de la tabla (DispObjTable)  minimo		 */
#define MW_TABLE_MIN_SIZE_X		600
#define MW_TABLE_MIN_SIZE_Y		400


/*!		Tamaño maximo del DispObjFilter				*/
#define DOF_MAX_SIZE_X			600
#define DOF_MAX_SIZE_Y			600


/*! DEFINIMOS TIEMPO MAXIMO DE LAS CONEXIONES! */
#define MAX_CONN_TIME_USCES		15000000	/* 15 segundos.. */

#endif
