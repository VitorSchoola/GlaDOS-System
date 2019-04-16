/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 *
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:16:11 $
 * $Log: geCrypt.h,v $
 * Revision 1.1  2015/01/10 19:16:11  vitor.schoola
 * Initial revision
 *
 *
 */

#ifndef _GECRYPT_
#define _GECRYPT_              "@(#)geCrypt.h $Revision: 1.1 $"

#define ALFABETO "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789./"
/*
CodificarSenha
char* (I) - Password to be codded
char* (O) - Codded password

Returns 1 if Invalid password
Returns 0 if OK.

Code a password using MD5 creating a random salt.
*/
unsigned CodificarSenha (char *senhaPlana, char *senhaCodificada);

/*
CodificarSenhaSalt
char* (I) - Password to be codded
char* (O) - Codded password
char* (I) - Desired Salt

Returns 1 if Invalid password
Returns 0 if OK.

Code a password using MD5 with custom salt
*/
unsigned CodificarSenhaSalt (char *password, char *coddedPassword, char *salt);

/*
CheckPassword
char* (I) - Password not codded
char* (I) - Codded password

Returns 0 if Codded password is NOT equal the plain password codded with same salt.
Returns 1 if Codded password is equal the plain password codded with same salt.
*/
unsigned CheckPassword ( char *password, char *coddedPassword );

#endif

/* $RCSfile: geCrypt.h,v $ */
