/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/11 05:16:55 $
 * $Log: geCgiRegister.c,v $
 * Revision 1.3  2015/01/11 05:16:55  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.2  2015/01/11 04:17:46  vitor.schoola
 * *** empty log message ***
 *
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
#include "geCrypt.h"

int
main ( int argc, char *argv[] )
{
	environmentType environment;
	languageType language;
	long long unsigned id;
	char geLanguage [LANGUAGE_MAX_LENGTH + 1];
	char geNickname [NICKNAME_MAX_LENGTH + 1];
	char gePassword [PASSWORD_MAX_LENGTH + 1];
	char geName [NAME_MAX_LENGTH + 1];
	char coddedPassword[256];
	FILE *writing;
   
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
	
	if ( mlCgiGetFormStringNoNewLines ( "nickname", geNickname, NICKNAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver seu login.", "<a href=\"geShowMainForm.cgi?geLanguage.cgi=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Error", "Could not retrieve Nickname", "<a href=\"geShowMainForm.cgi?geLanguage=easteregg\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "password", gePassword, PASSWORD_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver sua senha", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Error", "Could not retrieve Password", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( isUserRegistered ( geNickname ) != 0 )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Yaaaar!", "Email j&aacute; cadastrado.", "<a href=\"geShowRegisterForm.cgi?geLanguage=portuguese\">Registrar</a><br><a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Yaaaar!", "This email is already registered.", "<a href=\"geShowRegisterForm.cgi?geLanguage=english\">Register</a><br><a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( CodificarSenha ( gePassword, coddedPassword ) != 0 )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Yaaaar!", "Senha inv&aacute;lida.", "<a href=\"geShowRegisterForm.cgi?geLanguage=portuguese\">Registrar</a><br><a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Yaaaar!", "Invalid password.", "<a href=\"geShowRegisterForm.cgi?geLanguage=english\">Register</a><br><a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "name", geName, NAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver seu nome.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Error", "Could not retrieve name.", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( ( writing = fopen ( "../Files/users", "a" ) ) == NULL )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Yaaaar!", "Erro no servidor ao abrir arquivo de usu&aacute;rios. Tente novamente mais tarde.", "<a href=\"geShowRegisterForm.cgi?geLanguage=portuguese\">Registrar</a><br><a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Yaaaar!", "Server error. Could not open users file. Try again later.", "<a href=\"geShowRegisterForm.cgi?geLanguage=english\">Register</a><br><a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	id = getNextId ();
	fprintf ( writing, "\n%020llu;%s;;4;%s;", id, geNickname, geName );
	fclose ( writing );
	
	if ( fileExists ( "../Files/", "users.requests" ) == 0 )
	{
		writing = fopen ( "../Files/users.requests", "w" );	
		fprintf ( writing, "%020llu;%s;%s;4;%s;%lld", id, geNickname, coddedPassword, geName, (long long) time (NULL) );
	}
	else
	{
		writing = fopen ( "../Files/users.requests", "a" );
		fprintf ( writing, "\n%020llu;%s;%s;4;%s;%lld", id, geNickname, coddedPassword, geName, (long long) time (NULL) );
	}

	
	if ( language == portuguese )
	{
		printf("<html>\n");
		printf("  <head>\n");
		printf("    <title>	GelaDOS - Cervejarias ( Fun&ccedil;&atilde;o inativa )</title>\n");
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n");
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias Aplica&ccedil;&atilde;o - %s</font>\n", geName );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("    <img src=\"../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table> " );
		printf("    Yaaaar, marujo, voc&ecirc; agora est&aacute; na lista de espera! <br>\n" );
		printf("    Seu login ser&aacute seu email, caso aceito.<br>\n" );
		printf("    N&atilde;o esque&ccedil;a sua senha! Busc&aacute;-la quando cai no mar &eacute; imposs&iacute;vel<br>\n" );
		printf("    Caso ocorra ter&aacute; de desapegar e criar outra.<br>\n" );
		printf("    <br><br><a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar ao menu principal</a>" );
		printf("    </div>\n" );
		printf("    <br><br><br><br><br><br>\n" );
		printf("    <div align=\"center\"><audio id=\"player\" control autoplay>\n");
		printf("      <source src=\"../Files/win.mp3\" type=\"audio/mpeg\">\n" );
		printf("      Infelizmente seu navegador não suporta elementos de áudio.\n" );
		printf("      </audio>\n" );
		printf("  </body> " );
		printf("</html>" );
	}
	else
	{
		printf("<html>\n");
		printf("  <head>\n");
		printf("    <title>GelaDOS - Breweries ( Inactive funcion )</title>\n");
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n");
		printf("    <br><div align=\"center\"><font size=\"+3\"> Breweries Application - %s</font>\n", geName );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("    <img src=\"../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table> " );
		printf("    Yaaaar, sailor, you are in the waiting list now! <br>\n" );
		printf("    Your login will be your email if you get accepted.<br>\n" );
		printf("    Do not forget your password! If you drop it in the sea, we'll never find it!<br>\n" );
		printf("    If it happens anyway, you'll have to create another.<br>\n" );
		printf("    <br><br><a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Sail to the main menu</a>" );
		printf("    </div>\n" );
		printf("    <br><br><br><br><br><br>\n" );
		printf("    <div align=\"center\"><audio id=\"player\" control autoplay>\n");
		printf("      <source src=\"../Files/win.mp3\" type=\"audio/mpeg\">\n" );
		printf("      Unfortunatelly your browser does not support audio elements.\n" );
		printf("      </audio>\n" );
		printf("  </body> " );
		printf("</html>" );
	}
	mlCgiFreeResources();
	return ML_CGI_OK;
}
 
/* $RCSfile: geCgiRegister.c,v $ */
