/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:13:33 $
 * $Log: geCgiExecuteForgotPassword.c,v $
 * Revision 1.1  2015/01/10 19:13:33  vitor.schoola
 * Initial revision
 *
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
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
	languageType language;
	char geLanguage [LANGUAGE_MAX_LENGTH + 1];
	char geName[NICKNAME_MAX_LENGTH + 1];
	char geNewPass[257];
	geUserDataType userEditing;
	FILE *reading, *writing;
	char linha[ ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 8 ];
	unsigned counter, counter2;
	unsigned firstLine = 1;
	
	if ( mlCgiInitialize ( &environment ) )
		exit ( ML_CGI_OK );
 
	if ( environment == commandLine )
	{
		printf("Incorrect environment. This program was developed for the web.\n");
		exit( INCORRECT_ENVIRONMENT );
	}
 
	mlCgiBeginHttpHeader ( "text/html" );
	mlCgiEndHttpHeader ( );
  
	if ( mlCgiGetFormStringNoNewLines ( "geLanguage", geLanguage, LANGUAGE_MAX_LENGTH ) != ML_CGI_OK )
	{
		mlCgiShowErrorPage ( "Error", "Could not retrieve language", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>" );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	language = HandleLanguage ( geLanguage );
	
	if ( mlCgiGetFormStringNoNewLines ( "geName", geName, NICKNAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o email.", "<a href='geShowMainForm.cgi?geLanguage=portuguese'>Voltar para a p&aacute;gina de menu.</a>"  );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Couldn't retrieve user's email.", "<a href='geShowMainForm.cgi?geLanguage=english'>Go back to main menu.</a>"  );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "geNewPass", geNewPass, 255 ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o email.", "<a href='geShowMainForm.cgi?geLanguage=portuguese'>Voltar para a p&aacute;gina de menu.</a>"  );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Couldn't retrieve user's email.", "<a href='geShowMainForm.cgi?geLanguage=english'>Go back to main menu.</a>"  );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	CheckUserInfo ( geName, &userEditing );
	
	printf("<html>\n");
	printf("  <head>\n");
	
	printf("  <style> \n" );
	printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
	printf("  </style> \n" );
	
	/* Change Password */
	
	reading = fopen ( "../Files/users", "r" );
	writing = fopen ( "../Files/users.cfor", "w" );
		
	while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 7, reading ) != NULL )
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
		if ( strcmp ( geName, &linha[counter + 1] ) == 0 )
		{
			if ( firstLine == 1 )
			{
				fprintf ( writing, "%020llu;%s;%s;%c;%s;*", userEditing.userIdentifier, userEditing.email, geNewPass, userEditing.group + '0', userEditing.name );
				firstLine = 0;
			}
			else
				fprintf ( writing, "\n%020llu;%s;%s;%c;%s;*", userEditing.userIdentifier, userEditing.email, geNewPass, userEditing.group + '0', userEditing.name );
		}
		else
		{
			linha[counter] = ';';
			linha[counter2] = ';';
			if ( firstLine == 1 )
			{
				fprintf ( writing, linha );
				firstLine = 0;
			}
			else
				fprintf ( writing, "\n%s", linha );
		}
	}
		
	fclose ( reading );
	fclose ( writing );
	remove ( "../Files/users" );
	rename ( "../Files/users.cfor", "../Files/users" );
	
	/*---------------------------------*/
	
	
	
	if ( language == portuguese )
	{
		printf("    <title>	GelaDOS - Cervejarias (Logged)</title>\n");
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n" );
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias</font></div> \n" );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\"><br>\n" );
		
		printf("    Editar Usu&aacute;rios: <br>\n" );
		printf("    Sua senha foi modificada com sucesso. N&atilde;o se esque&ccedil;a novamente.<br>\n" );
		printf("     <a href=\"geShowMainForm.cgi?geLanguage=english\">Voltar para a p&aacute;gina principal</a><br>\n" );			
		
	}
	else
	{

		printf("    <title>	GelaDOS - Breweries (Logged)</title>\n");
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n" );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\"><br>\n" );
		printf("      <br><div align=\"center\"><font size=\"+3\"> Breweries</font></div> \n" );
	
		printf("    Edit Users: <br>\n" );
		printf("    Your password was modified successfully. Do not forget it again.<br>\n" );	
		printf("     <a href=\"geShowMainForm.cgi?geLanguage=english\">Go back to the main page</a><br>\n" );	
		
	}
	printf("     <a href=\"geHelp.cgi?geLanguage=portuguese\" target=\"_blank\">(?)</a><br>\n" );
	printf ("    </td></tr></table> \n" );
	printf("  </body>\n");
	printf("</html>\n");
	
	mlCgiFreeResources();
	return ML_CGI_OK;
}

/*
		printf ("     <a href=\"#\" onClick=\"setCookie('%s','%s','%u')\">Set Cookie</a><br>\n", idString, cookieValue, MINUTES_LOGGED_IN );
		printf ("     <a href=\"#\" onClick=\"getCookie('%s')\">Get Cookie</a><br>\n", idString );
		printf ("     <a href=\"#\" onClick=\"checkCookie()\">Check Cookie</a><br>\n" );
		printf ("     Cookie:|%s| Ip:|%s|<br>Tempo Restante:|%lf|<br>", cookieValue, ipString, (double)(tempo - time ( NULL ))/3600 );
*/
		
/* $RCSfile: geCgiExecuteForgotPassword.c,v $ */
