/*! Este modulo nos va a decir que tipos de archivos son siempre aceptados
 * y cuales no. Practicamente vamos a tener una sola funcion como interfaz.
 */
#ifndef OBEX_FILTER_H
#define OBEX_FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../debug.h"

/* vamos a especificar el tamaño maximo que queremos que acepte nuestro sistema
 * en bytes */
#define MAX_ACCEPTED_FILE_SIZE		512
/*! Tambien vamos a incluir el tipo sin extencion, eso en el codigo "hardcoded"
*/
#define ACCEPTED_FILE_FORMATS		".vnt .txt"

/* Funcion principal, que nos va a decir si un archivo va a ser aceptado SIEMPRE
 * o no, dependiendo del tamaño y del nombre del archivo (extension).
 * NOTE: Por el momento vamos a aceptar todo archivo que sea de texto/vnote o
 *	 tenga tamaño menor que MAX_ACCEPTED_FILE_SIZE
 * REQUIRES:
 *	fname != NULL
 *	fsize != NULL
 * RETURNS:
 *	true	if its always accepted
 *	false 	otherwise
 */
bool obf_always_accpted_file (const char * fname, const char * fsize);

#endif
