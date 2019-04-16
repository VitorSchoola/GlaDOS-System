/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/15 18:13:30 $
 * $Log: geClean.c,v $
 * Revision 1.2  2015/01/15 18:13:30  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.1  2015/01/11 05:16:55  vitor.schoola
 * Initial revision
 *
 * Revision 1.11  2015/01/11 03:55:44  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.5  2015/01/11 03:24:11  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.1  2015/01/11 00:38:21  vitor.schoola
 * Initial revision
 *
 * Revision 1.1  2015/01/10 19:07:51  vitor.schoola
 * Initial revision
 *
 *
 */
 

#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
 
#include "mlcgi.h"
#include "geFunctions.h"
#include "geTypes.h"
#include "geConst.h"
#include "geError.h"
#include "geCrypt.h"

int
main ( int argc, char *argv[] )
{
	environmentType environment;
	FILE *reading, *writing;
	FILE *reading2, *writing2;
	unsigned counter, counter2, counter3, counter4, counter5, counter6, counter7;
	char linha[ ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 7 ];
	char linha2[ ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 7 ];
	unsigned firstLine = 1, firstLine2 = 1;
	char *verificacao;
	unsigned long long tempo;
	
	if ( mlCgiInitialize ( &environment ) )
		exit ( OK );
 
	if ( environment != commandLine )
	{
		printf( "Incorrect environment. This program was developed for CLI.\n");
		exit( INCORRECT_ENVIRONMENT );
	}
	
	/* Check if users.abeyances File exists, if it does, execute clean */
		
	if ( fileExists ( "Files/", "users.abeyances" ) != 0 )
	{
		reading = fopen ( "Files/users.abeyances", "r" );
		writing = fopen ( "Files/users.abeyances.clean", "w" );
		
		while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading ) != NULL )
		{
			if ( linha[strlen(linha) - 1] == '\n' )
				linha[strlen(linha) - 1] = '\0';
			if ( linha[strlen(linha) - 2] == '\n' )
				linha[strlen(linha) - 2] = '\0';
			for ( counter = 0; counter <= ID_LENGTH + 1; counter++ )
				if ( linha[counter] == ';' )
				{
					linha[counter] = '\0';
					break;
				}
			for ( counter2 = counter + 1; counter2 <= ID_LENGTH + NICKNAME_MAX_LENGTH + 2; counter2++ )
				if ( linha[counter2] == ';' )
				{
					linha[counter2] = '\0';
					break;
				}
			for ( counter3 = counter2 + 1; counter3 <= ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + 3; counter3++ )
				if ( linha[counter3] == ';' )
				{
					linha[counter3] = '\0';
					break;
				}
			for ( counter4 = counter3 + 1; counter4 <= ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + 4; counter4++ )
				if ( linha[counter4] == ';' )
				{
					linha[counter4] = '\0';
					break;
				}
			for ( counter5 = counter4 + 1; counter5 <= ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 5; counter5++ )
				if ( linha[counter5] == ';' )
				{
					linha[counter5] = '\0';
					break;
				}
			linha[counter5 + 11] = '\0';
			tempo = strtoull ( &linha[counter5 + 1], &verificacao, 10 );
			if ( time (NULL) >= tempo + 259200 )
			{
				//printf ( "Hora de excluir %s.\n", &linha[counter + 1] );
				firstLine2 = 1;
				reading2 = fopen ( "Files/users", "r" );
				writing2 = fopen ( "Files/users.clean", "w" );
				
				while ( fgets ( linha2, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading2 ) != NULL )
				{
					if ( linha2[strlen(linha2) - 1] == '\n' )
						linha2[strlen(linha2) - 1] = '\0';
					if ( linha2[strlen(linha2) - 2] == '\n' )
						linha2[strlen(linha2) - 2] = '\0';
					for ( counter6 = 0; counter6 <= ID_LENGTH + 1; counter6++ )
						if ( linha2[counter6] == ';' )
						{
							linha2[counter6] = '\0';
							break;
						}
					for ( counter7 = counter6 + 1; counter7 <= ID_LENGTH + NICKNAME_MAX_LENGTH + 2; counter7++ )
						if ( linha2[counter7] == ';' )
						{
							linha2[counter7] = '\0';
							break;
						}
					if ( strcmp ( &linha[counter + 1], &linha2[counter6 + 1] ) != 0 )
					{
						linha2[counter6] = ';';
						linha2[counter7] = ';';
						if ( firstLine2 == 0 )
							fprintf ( writing2, "\n%s",linha2 );
						else
						{
							fprintf ( writing2, "%s", linha2 );
							firstLine2 = 0;
						}
					}
				}
				
				fclose ( reading2 );
				fclose ( writing2 );
				remove ( "Files/users" );
				rename ( "Files/users.clean", "Files/users" );
		
			}
			else
			{
				linha[counter] = ';';
				linha[counter2] = ';';
				linha[counter3] = ';';
				linha[counter4] = ';';
				linha[counter5] = ';';
				linha[counter5] = ';';
				linha[counter5 + 11] = '\0';
				if ( firstLine == 0 )
					fprintf ( writing, "\n%s",linha );
				else
				{
					fprintf ( writing, "%s", linha );
					firstLine = 0;
				}
			}
		}
		
		fclose ( reading );
		fclose ( writing );
		
		remove ( "Files/users.abeyances" );
	rename ( "Files/users.abeyances.clean", "Files/users.abeyances" );
	}
		
	/*----------------------------------*/
	/* Check if users.requests File exists, if it does, execute clean */	
	if ( fileExists ( "Files/", "users.requests" ) != 0 )
	{
		reading = fopen ( "Files/users.requests", "r" );
		writing = fopen ( "Files/users.requests.clean", "w" );
		
		while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading ) != NULL )
		{
			if ( linha[strlen(linha) - 1] == '\n' )
				linha[strlen(linha) - 1] = '\0';
			if ( linha[strlen(linha) - 2] == '\n' )
				linha[strlen(linha) - 2] = '\0';
			for ( counter = 0; counter <= ID_LENGTH + 1; counter++ )
				if ( linha[counter] == ';' )
				{
					linha[counter] = '\0';
					break;
				}
			for ( counter2 = counter + 1; counter2 <= ID_LENGTH + NICKNAME_MAX_LENGTH + 2; counter2++ )
				if ( linha[counter2] == ';' )
				{
					linha[counter2] = '\0';
					break;
				}
			for ( counter3 = counter2 + 1; counter3 <= ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + 3; counter3++ )
				if ( linha[counter3] == ';' )
				{
					linha[counter3] = '\0';
					break;
				}
			for ( counter4 = counter3 + 1; counter4 <= ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + 4; counter4++ )
				if ( linha[counter4] == ';' )
				{
					linha[counter4] = '\0';
					break;
				}
			for ( counter5 = counter4 + 1; counter5 <= ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 5; counter5++ )
				if ( linha[counter5] == ';' )
				{
					linha[counter5] = '\0';
					break;
				}
			linha[counter5 + 11] = '\0';
			tempo = strtoull ( &linha[counter5 + 1], &verificacao, 10 );
			if ( time (NULL) >= tempo + 259200 )
			{
				//printf ( "Hora de excluir %s.\n", &linha[counter + 1] );
				firstLine2 = 1;
				reading2 = fopen ( "Files/users", "r" );
				writing2 = fopen ( "Files/users.clean", "w" );
				
				while ( fgets ( linha2, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading2 ) != NULL )
				{
					if ( linha2[strlen(linha2) - 1] == '\n' )
						linha2[strlen(linha2) - 1] = '\0';
					if ( linha2[strlen(linha2) - 2] == '\n' )
						linha2[strlen(linha2) - 2] = '\0';
					for ( counter6 = 0; counter6 <= ID_LENGTH + 1; counter6++ )
						if ( linha2[counter6] == ';' )
						{
							linha2[counter6] = '\0';
							break;
						}
					for ( counter7 = counter6 + 1; counter7 <= ID_LENGTH + NICKNAME_MAX_LENGTH + 2; counter7++ )
						if ( linha2[counter7] == ';' )
						{
							linha2[counter7] = '\0';
							break;
						}
					if ( strcmp ( &linha[counter + 1], &linha2[counter6 + 1] ) != 0 )
					{
						linha2[counter6] = ';';
						linha2[counter7] = ';';
						if ( firstLine2 == 0 )
							fprintf ( writing2, "\n%s",linha2 );
						else
						{
							fprintf ( writing2, "%s", linha2 );
							firstLine2 = 0;
						}
					}
				}
				
				fclose ( reading2 );
				fclose ( writing2 );
				remove ( "Files/users" );
				rename ( "Files/users.clean", "Files/users" );
		
			}
			else
			{
				linha[counter] = ';';
				linha[counter2] = ';';
				linha[counter3] = ';';
				linha[counter4] = ';';
				linha[counter5] = ';';
				linha[counter5] = ';';
				linha[counter5 + 11] = '\0';
				if ( firstLine == 0 )
					fprintf ( writing, "\n%s",linha );
				else
				{
					fprintf ( writing, "%s", linha );
					firstLine = 0;
				}
			}
		}
		
		fclose ( reading );
		fclose ( writing );
		
		remove ( "Files/users.requests" );
		rename ( "Files/users.requests.clean", "Files/users.requests" );
	}
	/*------------------------------------------*/

	mlCgiFreeResources();
	return OK;
}

/*
		printf ("     <a href=\"#\" onClick=\"setCookie('%s','%s','%u')\">Set Cookie</a><br>\n", idString, cookieValue, MINUTES_LOGGED_IN );
		printf ("     <a href=\"#\" onClick=\"getCookie('%s')\">Get Cookie</a><br>\n", idString );
		printf ("     <a href=\"#\" onClick=\"checkCookie()\">Check Cookie</a><br>\n" );
		printf ("     Cookie:|%s| Ip:|%s|<br>Tempo Restante:|%lf|<br>", cookieValue, ipString, (double)(tempo - time ( NULL ))/3600 );
*/
		
/* $RCSfile: geClean.c,v $ */
