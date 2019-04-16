/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:16:11 $
 * $Log: geCgiShowForgotPasswordForm.c,v $
 * Revision 1.1  2015/01/10 19:16:11  vitor.schoola
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
		printf("Incorrect environment. This program was developed for the web.\n" );
		exit( INCORRECT_ENVIRONMENT );
	}
 
	mlCgiBeginHttpHeader ( "text/html" );
		if ( mlCgiGetFormStringNoNewLines ( "geLanguage", geLanguage, LANGUAGE_MAX_LENGTH ) != ML_CGI_OK )
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve language", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>" );
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
		language = HandleLanguage ( geLanguage );	
	mlCgiEndHttpHeader ( );
	
 	printf("<html>\n");
	printf("  <head>\n");
 	printf("  <style> \n" );
	printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
	printf("  </style> \n" );
 
	if ( language == portuguese )
	{
		
		printf("    <title>	GelaDOS - Cervejarias</title>\n");
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n" );
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias</font></div> \n" );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\"><br>\n" );
		
		printf("    Esqueci minha senha! <br>\n" );
		printf ("<form action=\"geForgotPassword.cgi\" method=\"post\"> \n" );
		printf ("  <input type='hidden' name='geLanguage' value='%s'>\n", geLanguage );
		printf (" <b>Email</b>:<br> \n" );
		printf ("  <input type='text' name='geEmail' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
		printf ("  <input type=\"submit\" name=\"submit\" value=\"Enviar\"><br> \n" );
		printf (" </form>\n" );
		printf("     <a href=\"geShowMainForm.cgi?geLanguage=english\">Voltar para a p&aacute;gina principal</a><br>\n" );			
		printf("     <a href=\"geHelp.cgi?geLanguage=portuguese\" target=\"_blank\">(?)</a><br>\n" );

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
		
		printf("    I forgot my password! <br>\n" );
		printf ("<form action=\"geForgotPassword.cgi\" method=\"post\"> \n" );
		printf ("  <input type='hidden' name='geLanguage' value='%s'>\n", geLanguage );
		printf (" <b>Email</b>:<br> \n" );
		printf ("  <input type='text' name='geEmail' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
		printf ("  <input type=\"submit\" name=\"submit\" value=\"Send\"><br> \n" );
		printf (" </form>\n" );
		printf("     <a href=\"geShowMainForm.cgi?geLanguage=english\">Go back to the main page</a><br>\n" );			
		printf("     <a href=\"geHelp.cgi?geLanguage=english\" target=\"_blank\">(?)</a><br>\n" );	
	} 
 
 	printf ("    </td></tr></table> \n" );
	printf("  </body>\n");
	printf("</html>\n");
 
	mlCgiFreeResources();
	return ML_CGI_OK ;
}
 
/* $RCSfile: geCgiShowForgotPasswordForm.c,v $ */
