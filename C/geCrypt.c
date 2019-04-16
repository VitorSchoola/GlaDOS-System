/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 *
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:16:11 $
 * $Log: geCrypt.c,v $
 * Revision 1.1  2015/01/10 19:16:11  vitor.schoola
 * Initial revision
 *
 *
 */

#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "geFunctions.h"
#include "geCrypt.h"
#include "geError.h"
#include "geConst.h"


unsigned CodificarSenha (char *senhaPlana, char *senhaCodificada)
{
	char salt[256];
	char tipo[256];
	char *senhaCod;
	int counter;
	tipo[0] = '\0';
	if ( strlen ( senhaPlana ) > PASSWORD_MAX_LENGTH )
	{
		printf ( "Senha Invalida" );
		return ( 1 );
	}
	CriarStringAleatoria (ALFABETO, 8, salt );
	strcat ( tipo, "$1$" );
	strcat ( tipo, salt );
	salt[0] = *tipo;
	senhaCod = crypt ( senhaPlana , tipo );
	for (counter = 0; counter <= 255; counter++ )
		senhaCodificada[counter] = senhaCod[counter];
	senhaCodificada[counter] = '\0';
	return 0;
}

unsigned CodificarSenhaSalt (char *password, char *coddedPassword, char *salt)
{
	char *passwordString;
	unsigned counter;
    if ( strlen ( password ) > PASSWORD_MAX_LENGTH )
    {
        printf ( "Senha Invalida" );
        return ( 1 );
    }
    passwordString = crypt ( password , salt );
	for (counter = 0; counter <= strlen (passwordString); counter++ )
		coddedPassword[counter] = passwordString[counter];
	coddedPassword[counter] = '\0';
    return 0;
}

unsigned CheckPassword ( char *password, char *coddedPassword )
{
    char comparePassword[256];
    char salt[13];
    char memory;
    int counter;
    for ( counter = 0; counter <= 12; counter ++ )
        salt[counter] = '\0';
    if ( ( coddedPassword[0] == '$' ) & ( coddedPassword[1] == '1' ) )
    {
        memory = coddedPassword[12];
        coddedPassword[12] = '\0';
        strcat ( salt, coddedPassword );
        coddedPassword[12] = memory;
    }
	CodificarSenhaSalt ( password, &comparePassword[0], salt );
	/*printf ( "Codded: |%s| <br>\n Compa: |%s| <br>\n", coddedPassword, comparePassword );*/
	if ( strcmp ( coddedPassword, comparePassword ) == 0 )
		return (1);
	else
		return (0);
}

/* $RCSfile: geCrypt.c,v $ */
