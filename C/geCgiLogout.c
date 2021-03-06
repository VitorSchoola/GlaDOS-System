/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 *
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:14:53 $
 * $Log: geCgiLogout.c,v $
 * Revision 1.1  2015/01/10 19:14:53  vitor.schoola
 * Initial revision
 *
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "mlcgi.h"
#include "geTypes.h"
#include "geConst.h"
#include "geError.h"
#include "geFunctions.h"
#include "geCrypt.h"

int
main ( int argc, char *argv[] )
{
	environmentType environment;
	languageType language;
	char geLanguage [LANGUAGE_MAX_LENGTH];
	char idString [21];
	unsigned counter;
	char fullPath[strlen("../Files/Cookies/") + 21 + 1];
   
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
	
	if ( mlCgiGetFormStringNoNewLines ( "idString", idString, 21 ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver seu id.", "<a href=\"geShowMainForm?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Error", "Could not retrieve Id", "<a href=\"geShowMainForm.cgi?geLanguage=easteregg\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	for ( counter = 0; counter < strlen("../Files/Cookies/") + strlen(idString) + 1; counter++ )
		fullPath[counter] = '\0';
	strcat ( fullPath, "../Files/Cookies/" );
	strcat ( fullPath, idString );
	remove ( fullPath );

	printf( "<html>\n" );
	printf( "	<head>\n" );
	
	printf( "<script>\n" );
	printf("  function setCookie(cname,cvalue,expirationMinutes) {\n" );
	printf("      var d = new Date();\n" );
	printf("      d.setTime(d.getTime() + (expirationMinutes*60*1000));\n" );
	printf("      var expires = \"expires=\" + d.toGMTString();\n" );
	printf("      document.cookie = cname+\"=\"+cvalue+\"; \"+expires;\n" );
	printf("  }\n" );
	
	printf(" function redirect (){\n" );
	if ( language == portuguese )
		printf(" window.location=\"./geShowMainForm.cgi?geLanguage=portuguese\"; " );
	else
		printf(" window.location=\"./geShowMainForm.cgi?geLanguage=english\"; " );		
	printf(" }\n" );
	
	printf( "</script>\n" );
	
	if ( language == portuguese )
	{
		printf( "<title>GelaDOS - Cervejarias (Logout)</title>\n" );
	}
	else
	{
		printf( "<title>GelaDOS - Breweries (Logout)</title>\n" );
	}
	
	printf( "	</head>\n" );
	printf( "   <body onload=\"setCookie('%s', 'a', '-30')&redirect()\"> \n", idString );

	printf( "   </body> \n" );
	printf( "</html>\n" );
	mlCgiFreeResources();
	return ML_CGI_OK;
}
 
/* $RCSfile: geCgiLogout.c,v $ */
