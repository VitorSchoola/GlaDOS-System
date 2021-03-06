/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:16:11 $
 * $Log: geCgiShowMainForm.c,v $
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
 
	if ( language == portuguese )
	{
		printf("<html>\n" );
		printf("  <head>\n" );
		printf("    <title>GelaDOS - Cervejarias</title>\n" );
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n" );
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\" >\n");
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias</font></div> \n" );
		printf("      <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table><br> \n" );
		printf("      <table align=\"center\"> \n" );
		printf("      <tr>\n" );
		printf("        <form action=\"geLogin.cgi\" method=\"post\"> \n" );
		printf("        <td>Email: </td>\n" );
		printf("        <td><input type=\"text\" name=\"nickname\" maxlength=\"128\"></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr>\n" );
		printf("        <td>Senha: </td>\n" );
		printf("        <td><input type=\"password\" name=\"password\" maxlenght=\"128\"></td>\n" );
		printf("        <input type=\"hidden\" name=\"geLanguage\" value=\"portuguese\"></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr><td></td>\n" );
		printf("        <td align=\"left\"><input type=\"submit\" name=\"submit\" value=\"E uma garrafa de rum!\"></td>\n" );
		printf("      </form>\n" );
		printf("      </tr>\n" );
		printf("      <tr><td></td>\n" );
		printf("        <td align=\"left\"><a href=\"geShowForgotPasswordForm.cgi?geLanguage=portuguese\">Esqueci minha senha</a><br>\n" );
		printf("        <a href=\"geShowRequestRegistrationForm.cgi?geLanguage=portuguese\">Aplique-se para registrar</a><br>\n" );
		printf("        <a href=\"geLicense.cgi?geLanguage=portuguese\">Licen&ccedil;a</a><br>\n" );
		printf("        <a href=\"geHelp.cgi?geLanguage=portuguese\" target=\"_blank\">Ajuda (?)</a></td><br><br>\n" );
		printf("        <td align=\"right\"><a href=\"geShowMainForm.cgi?geLanguage=english\">See this page in english.</a><br>\n" );
		printf("        <a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Veja essa p&aacute;gina em portugu&ecirc;s.</a></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr><td></td>\n" );
		printf("        <td align=\"left\"><a href=\"mailto:vschoola@poli.ufrj.br\">Vitor Gouveia Schoola</a></td>\n" );
		printf("      </tr>\n" );
		printf("    </table>\n" );
		printf("    <br><br><br><br><br><br>\n" );
		printf("    <div align=\"center\"><audio id=\"player\" controls loop volume=\"50\">\n");
		printf("      <source src=\"../Files/music.mp3\" type=\"audio/mpeg\">\n" );
		printf("      Infelizmente seu navegador não suporta elementos de áudio.\n" );
		printf("      </audio>\n" );
		printf("    </div>\n");
		printf("  </body>\n");
		printf("</html>\n");
	}
	else
	{
		printf("<html>\n");
		printf("  <head>\n");
		printf("    <title>	GelaDOS - Breweries</title>\n");
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
		printf("    <div align=\"center\"><font size=\"+3\"> Breweries</font></div><br> \n" );
		printf("      <table align=\"center\"> \n" );
		printf("      <tr>\n" );
		printf("        <form action=\"geLogin.cgi\" method=\"post\"> \n" );
		printf("        <td>Email: </td>\n" );
		printf("        <td><input type=\"text\" name=\"nickname\" maxlength=\"128\"></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr>\n" );
		printf("        <td>Password: </td>\n" );
		printf("        <td><input type=\"password\" name=\"password\" maxlenght=\"128\"></td>\n" );
		printf("        <input type=\"hidden\" name=\"geLanguage\" value=\"english\"></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr><td></td>\n" );
		printf("        <td align=\"left\"><input type=\"submit\" name=\"submit\" value=\"And a bottle of rum'\"></td>\n" );
		printf("      </form>\n" );
		printf("      </tr>\n" );
		printf("      <tr><td></td>\n" );
		printf("        <td align=\"left\"><a href=\"geShowForgotPasswordForm.cgi?geLanguage=english\">I forgot my password</a><br>\n" );
		printf("        <a href=\"geShowRequestRegistrationForm.cgi?geLanguage=english\">Application to register</a><br>\n" );
		printf("        <a href=\"geLicense.cgi?geLanguage=english\">License</a><br>\n" );
		printf("        <a href=\"geHelp.cgi?geLanguage=english\" target=\"_blank\">Help (?)</a></td><br><br>\n" );
		printf("        <td align=\"right\"><a href=\"geShowMainForm.cgi?geLanguage=english\">See this page in english.</a><br>\n" );
		printf("        <a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Veja essa p&aacute;gina em portugu&ecirc;s.</a></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr><td></td>\n" );
		printf("        <td align=\"left\"><a href=\"mailto:vschoola@poli.ufrj.br\">Vitor Gouveia Schoola</a></td>\n" );
		printf("      </tr>\n" );
		printf("    </table>\n" );
		printf("    <br><br><br><br><br><br>\n" );
		printf("    <div align=\"center\"><audio id=\"player\" controls loop>\n");
		printf("      <source src=\"../Files/music.mp3\" type=\"audio/mpeg\">\n" );
		printf("      Unfortunatelly your browser does not support audio elements.\n" );
		printf("      </audio>\n" );
		printf("    </div>\n");
		printf("  </body>\n");
		printf("</html>\n");
	} 
 
	mlCgiFreeResources();
	return ML_CGI_OK ;
}
 
/* $RCSfile: geCgiShowMainForm.c,v $ */
