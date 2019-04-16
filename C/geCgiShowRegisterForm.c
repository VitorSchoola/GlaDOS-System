/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 *
 * $Author$
 * $Date$
 * $Log$
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
 
	if (language==portuguese)
	{
		printf("<html>\n");
		printf("  <head>\n");
		printf("    <title>GelaDOS - Cervejarias Registro</title>\n");
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n");
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias - Registro</font> \n" );
		printf("      <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table> \n" );
		printf("    Note que: Esse &eacute; um PEDIDO de registro, N&Atilde;O o registro em si.<br>\n" );
		printf("    </div>\n");
		printf("      <table align=\"center\"> \n" );
		printf("      <tr>\n" );
		printf("        <form action=\"geRegister.cgi\" method=\"post\"> \n" );
		printf("        <td>Email: </td>\n" );
		printf("        <td><input type=\"text\" name=\"nickname\" maxlength=\"128\"></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr>\n" );
		printf("        <td>Nome: </td>\n" );
		printf("        <td><input type=\"text\" name=\"realName\" maxlenght=\"128\"></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr>\n" );
		printf("        <td>Senha: </td>\n" );
		printf("        <td><input type=\"password\" name=\"password\" maxlenght=\"64\"></td>\n" );
		printf("        <input type=\"hidden\" name=\"geLanguage\" value=\"portuguese\"></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr><td></td>\n" );
		printf("        <td align=\"center\"><input type=\"submit\" name=\"submit\" value=\"Aplicar\"></td>\n" );
		printf("      </form>\n" );
		printf("      </tr>\n" );
		printf("    </table>\n" );
		printf("    <div align=\"center\"><a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Voltar ao menu principal.</a><br>\n" );
		printf("    <br><br><br><br><br><br>\n" );
		printf("    <audio id=\"player\" controls autoplay loop>\n");
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
		printf("    <title>	GelaDOS - Breweries Register</title>\n");
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
		printf("    <div align=\"center\"><font size=\"+3\"> Breweries - Register</font><br> \n" );
		printf("    Note: This is an APPLICATION, so does NOT guarantees your registration.<br>\n" );
		printf("    </div>\n");
		printf("      <table align=\"center\"> \n" );
		printf("      <tr>\n" );
		printf("        <form action=\"geRegister.cgi\" method=\"post\"> \n" );
		printf("        <td>Email: </td>\n" );
		printf("        <td><input type=\"text\" name=\"nickname\" maxlength=\"128\"></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr>\n" );
		printf("        <td>Password: </td>\n" );
		printf("        <td><input type=\"password\" name=\"password\" maxlenght=\"64\"></td>\n" );
		printf("        <input type=\"hidden\" name=\"geLanguage\" value=\"english\"></td>\n" );
		printf("      </tr>\n" );
		printf("      <tr><td></td>\n" );
		printf("        <td align=\"center\"><input type=\"submit\" name=\"submit\" value=\"Submit\"></td>\n" );
		printf("      </form>\n" );
		printf("      </tr>\n" );
		printf("    </table>\n" );
		printf("    <div align=\"center\"><a href=\"geShowMainForm.cgi?geLanguage=english\">Go back to the main page.</a><br>\n" );
		printf("    <br><br><br><br><br><br>\n" );
		printf("    <audio id=\"player\" controls autoplay loop>\n");
		printf("      <source src=\"../Files/music.mp3\" type=\"audio/mpeg\">\n" );
		printf("      Unfortunatelly your browser does not support audio elements.\n" );
		printf("      </audio>\n" );
		printf("    </div>\n");
		printf("  </body>\n");
		printf("</html>\n");
	} 
 
	mlCgiFreeResources();
	return ML_CGI_OK;
}
 
/* $RCSfile$ */
