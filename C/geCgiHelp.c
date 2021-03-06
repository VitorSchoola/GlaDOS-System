/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 *
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:14:53 $
 * $Log: geCgiHelp.c,v $
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
		printf("    <title>	GelaDOS - Cervejarias Ajuda</title>\n");
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n");
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias - Ajuda</font> \n" );
		printf("      <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table> \n" );
		printf("    Yaaaar, marujo, preste aten&ccedil;&atilde;o que s&oacute; falarei uma vez: <br>\n" );
		printf("    Voc&ecirc; est&aacute; em um sistema de avalia&ccedil;&atilde;o de cervejas.<br>\n" );
		printf("    Nele voc&ecirc; pode: <br><br>\n" );
		printf("    Se estiver no grupo [Recruta]: <br>\n" );
		printf("    - Avaliar cervejas<br>\n" );
		printf("    - Convidar novos recrutas<br><br>\n" );
		printf("    Se estiver no grupo [Pirata Experiente]: <br>\n");
		printf("    - Avaliar e Criar novas cervejas no sistema<br>\n");
		printf("    - Convidar novos recrutas e Piratas Experientes<br>\n");
		printf("    - Banir ou perdoar recrutas <br><br>\n" );
		printf("    Se estiver no grupo [Capit&atilde;o]: <br>\n");
		printf("    - Avaliar e Criar novas cervejas no sistema<br>\n" );
		printf("    - Convidar novos recrutas, piratas experientes e capit&atilde;es ( Vai d&aacute;-los barcos? ) <br>\n" );
		printf("    - Banir ou perdoar qualquer uma pessoa.<br>\n" );
		printf("    <br><br>Toda intera&ccedil;&atilde;o &eacute; feita depois do login e senha do usu&aacute;rio <br>\n" );
		printf("    <a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar ao menu principal</a>" );
		printf("    </div>\n" );
		printf("  </body> " );
		printf("</html>" );
	}
	else
	{
		printf("<html>\n");
		printf("  <head>\n");
		printf("    <title>	GelaDOS - Breweries Help</title>\n");
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
		printf("    <div align=\"center\"><font size=\"+3\"> Breweries</font> \n" );
		printf("    <br>\n" );
		printf("    Yaaaar, sailor, pay atention, 'cause I'll say it just once: <br>\n" );
		printf("    You're in a Beer Rating System.<br>\n" );
		printf("    You are able to: <br><br>\n" );
		printf("    If a [Recruit]: <br>\n" );
		printf("    - Rate beers<br>\n" );
		printf("    - Invite new recruits<br><br>\n" );
		printf("    If an [Adept Pirate]: <br>\n");
		printf("    - Rate and create new beers in our system<br>\n");
		printf("    - Invite new recruits and Adept Pirates<br>\n");
		printf("    - Ban or Unban recruits <br><br>\n" );
		printf("    If a [Captain]: <br>\n");
		printf("    - Rate and create new beers in our system<br>\n" );
		printf("    - Invite new recruits, adept pirates and captains ( you'll give 'em a boat? ) <br>\n" );
		printf("    - Ban or unban anyone<br>\n" );
		printf("    <br><br> Any and every interaction is made after logging in.<br>\n" );
		printf("    <a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>" );
		printf("    </div>\n" );
		printf("  </body>" );
		printf("</html>");
	}
	mlCgiFreeResources();
	return ML_CGI_OK;
}
 
/* $RCSfile: geCgiHelp.c,v $ */
