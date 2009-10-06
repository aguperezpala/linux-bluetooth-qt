#ifndef CONSTS_H
#define CONSTS_H

/* cantidad de caracteres de una mac CONTANDO los ':'. 11:22:33:44:55:66 */
#define MAC_SIZE	17	

#define qstrtochar(x)	x.toStdString().c_str()


/*! Algunas respuestas para los protocolos */
#define PRO_RESP_FALSE		"<false>"
#define PRO_RESP_TRUE		"<true>"

#endif
