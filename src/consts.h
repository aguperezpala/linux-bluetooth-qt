#ifndef CONSTS_H
#define CONSTS_H

/* cantidad de caracteres de una mac CONTANDO los ':'. 11:22:33:44:55:66 */
#define MAC_SIZE	17	

#define qstrtochar(x)	x.toStdString().c_str()


/*! Algunas respuestas para los protocolos */
#define PRO_RESP_FALSE		"<false>"
#define PRO_RESP_TRUE		"<true>"



/*! 		OBEX RECEIVER			*/
#define OBREC_START_PORT		4561
#define OBREC_END_PORT			4566
#define OBREC_BUFF_SIZE			4096	/* 4 kb alcanzaran? */
#define OBREC_MAX_PKT_SIZE		1024	/* tamaño max de <file,MAC> */
#define OBREC_PKT_SEPARATOR_B		"{"
#define OBREC_PKT_SEPARATOR_E		"}"
#define OBREC_DATA_SEPARATOR_B		"<"	/* begin data separator */
#define OBREC_DATA_SEPARATOR_E		">"	/* end data separator */
#endif
