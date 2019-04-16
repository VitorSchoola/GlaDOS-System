/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 *
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:14:53 $
 * $Log: geCgiLicense.c,v $
 * Revision 1.1  2015/01/10 19:14:53  vitor.schoola
 * Initial revision
 *
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
 
#include "mlcgi.h"
#include "geTypes.h"
#include "geConst.h"
#include "geError.h"
#include "geFunctions.h"

int
main ( int argc, char *argv[] )
{
	environmentType environment;
	languageType language;
	char geLanguage [LANGUAGE_MAX_LENGTH];
   
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
	
	if ( language == portuguese )
	{
		printf("<html>\n");
		printf("  <head>\n");
		printf("    <title>	GelaDOS - Cervejarias Licença</title>\n");
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n");
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias - Licença</font><br> \n" );
		printf("      <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table> \n" );
		printf("    Todo conte&uacute;do desse sistema &eacute; de direito de seu criador e, dessa forma, n&atilde;o pode ser pego ou utilizado sem a autoriza&ccedil;&atilde;o dele.<br>\n" );
		printf("    Para que o usu&aacute;rio possa utilizar todos servi&ccedil;os, deve ter uma conta.<br>\n" );
		printf("    Todo e qualquer termo aqui descrito pode ser alterado sem aviso pr&eacute;vio <br>\n" );
		printf("    <br><br>Toda intera&ccedil;&atilde;o &eacute; feita depois do login e senha do usu&aacute;rio <br>\n" );
		printf("    <a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar ao menu principal</a>" );
		printf("    </div>\n" );
		printf("  </body>" );
		printf("</html>");
	}
	else
	{
		printf("<html>\n");
		printf("  <head>\n");
		printf("    <title> GelaDOS - Breweries License</title>\n");
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n");
		printf("    <br> \n" );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table> \n" );
		printf("    <div align=\"center\"><font size=\"+3\"> Breweries</font><br> \n" );
		printf("    Any content that belongs to this system also belongs to its creator, so it may only be used after his permission.<br>\n" );
		printf("    In order to use our services, the user may create an account.<br>\n" );
		printf("    Our terms may change without any previous warning and will be valid as soon it's online.<br>\n" );
		printf("    <br><br> Any and every interaction is made after logging in.<br>\n" );
		printf("    <a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>" );
		printf("    </div>\n" );
		printf("  </body>" );
		printf("</html>");
	}
	mlCgiFreeResources();
	return ML_CGI_OK;
}
 
/* $RCSfile: geCgiLicense.c,v $ */
