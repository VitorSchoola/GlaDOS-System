	/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:11:15 $
 * $Log: geCgiExecuteAddBeerType.c,v $
 * Revision 1.1  2015/01/10 19:11:15  vitor.schoola
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
	char geLanguage [LANGUAGE_MAX_LENGTH];
	char geNickname [NICKNAME_MAX_LENGTH];
	geUserDataType userLoggedIn;	
	char idString [21];
	char cookieValue[COOKIE_LENGTH + 1], cookieValueNew[COOKIE_LENGTH + 1];
	char geBeerType [BEER_NAME_MAX_LENGTH + 1];
	char geComment [COMMENT_MAX_LENGTH + 1];
	char linha [BEER_NAME_MAX_LENGTH + COMMENT_MAX_LENGTH + 8 + 2];
	FILE *writing, *reading;
	unsigned firstLine;
	
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
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver seu login.", "<a href=\"geShowMainForm?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Error", "Could not retrieve Nickname", "<a href=\"geShowMainForm.cgi?geLanguage=easteregg\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	CheckUserInfo ( geNickname, &userLoggedIn );
	sprintf ( &idString[0], "%020llu", userLoggedIn.userIdentifier );
	
	if ( mlCgiGetFormStringNoNewLines ( "cookieFromWeb", cookieValueNew, COOKIE_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver seu cookie.", "" );
			printf ( "<div align=\"center\">" );
			printf ( "<a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve cookie", ""  );
			printf ( "<div align=\"center\">" );
			printf ( "<a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	checkCookieAndIp ( idString, cookieValueNew, language );
	
	printf("<html>\n");
	printf("  <head>\n");
	
	printf("  <script> \n" );
	
	printf("  function setCookie(cname,cvalue,expirationMinutes) {\n" );
	printf("      var d = new Date();\n" );
	printf("      d.setTime(d.getTime() + (expirationMinutes*60*1000));\n" );
	printf("      var expires = \"expires=\" + d.toGMTString();\n" );
	printf("      document.cookie = cname+\"=\"+cvalue+\"; \"+expires;\n" );
	printf("  }\n" );
		
	printf("  function getCookie(cname) {\n" );
	printf("      var name = cname + \"=\";\n" );
	printf("      var ca = document.cookie.split(';');\n" );
	printf("      for(var i=0; i<ca.length; i++) {\n" );
	printf("          var c = ca[i];\n" );
	printf("          while (c.charAt(0)==' ') c = c.substring(1);\n" );
	printf("          if (c.indexOf(name) == 0) {\n" );
//	printf("              alert(c.substring(name.length, c.length));\n" );	
	printf("              return c.substring(name.length, c.length);\n" );
	printf("          }\n" );
	printf("      }\n" );
//	printf("      alert (\"Vazia\"); \n" );
	printf("      return \"\";\n" );
	printf("  }\n" );
	
	printf("  function checkCookie() {\n" );
//	printf("      alert(\"%s\");\n", idString );
	printf("      var logged = getCookie('%s');\n", idString );
	printf("      if ( logged != \"\") {\n" );
//	printf("        alert ( \"Cookie Found\" ); \n" );
	printf("      } else { \n" );
	printf("        setCookie('%s','%s','%u');\n", idString, cookieValue, MINUTES_LOGGED_IN );
	printf("      }\n" );
	printf("  return \"<input type='hidden' name='cookieFromWeb' value='\" + logged + \"'>\"\n" );
	printf("  }\n" );
	
	printf("  </script> \n" );
	
	if ( userLoggedIn.userIdentifier == 4 )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "Grupo inv&aacute;lido.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddBeerType.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Invalid Group", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddBeerType.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "geBeerType", geBeerType, BEER_NAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o nome do novo tipo de cerveja.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddBeerType.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve the new beer type's name.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddBeerType.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "geComment", geComment, COMMENT_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o coment&aacute;rio da nova cerveja.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddBeerType.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve the new beer's comment.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddBeerType.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	printf("  <style> \n" );
	printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
	printf("  </style> \n" );
	
	if ( language == portuguese )
	{
		printf("    <title>	GelaDOS - Cervejarias (Logged)</title>\n");
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n" );
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias</font></div> \n" );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\"><br>\n" );
		
		printf("	Criando Tipo de Cerveja %s...<br>\n", geBeerType );
		
		/* Check if Beer Types file already exists */
		
		if ( fileExists ( "../Files/", "BeerTypes" ) == 0 )
		{
			writing = fopen ( "../Files/BeerTypes", "w" );
			fclose( writing );
			firstLine = 1;
		}
		else
		{
			firstLine = 0;
		}
		
		/*-----------------------------------------*/
		
		/* Checks if beer type already exists */
		
		reading = fopen ( "../Files/BeerTypes", "r" );
		while ( fgets ( linha, BEER_NAME_MAX_LENGTH + COMMENT_MAX_LENGTH + 8, reading ) != NULL )
		{
			if ( linha[strlen(linha) - 1] == '\n' ) 
				linha[strlen(linha) - 1] = '\0';
			if ( linha[strlen(linha) - 2] == '\n' ) 
				linha[strlen(linha) - 2] = '\0';
			if ( linha[0] == 't' )
				if ( linha[1] == '|' )
				{
					if ( strcmp ( &linha[2], geBeerType ) == 0 )
					{
						printf ( "Nome j&aacute; utilizado.<br>\n" );
						createButton ( "Voltar para a p&aacute;gina anterior.", "geAddBeerType.cgi", geLanguage, geNickname, "18" );
						createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "19" );
						printf("      <a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a><br>\n", idString );
						
						fclose ( reading );
						mlCgiFreeResources ( );
						exit ( ML_CGI_OK );
					}
				}
		}
		fclose ( reading );
		/*--------------------------------------*/
		
		/* Apends new beer type */

		writing = fopen ( "../Files/BeerTypes", "a" );
		
		if ( firstLine == 1 )
			fprintf ( writing, "t|%s", geBeerType );
		else
			fprintf ( writing, "\nt|%s", geBeerType );
		fprintf ( writing, "\nd|%s", userLoggedIn.email ); 
		fprintf ( writing, "\nc|%s", geComment );
		
		fclose ( writing );
		/*--------------------------------------*/
		
		printf("    Executado com sucesso.<br> " );  
		createButton ( "Voltar para a p&aacute;gina anterior.", "geAddBeerType.cgi", geLanguage, geNickname, "18" );
		createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "19" );
		
		printf ("      <a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a><br>\n", idString );
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
		
		printf("	Creating new Beer Type: %s...<br>\n", geBeerType );
		
		/* Check if Beer Types file already exists */
		
		if ( fileExists ( "../Files/", "BeerTypes" ) == 0 )
		{
			writing = fopen ( "../Files/BeerTypes", "w" );
			fclose( writing );
			firstLine = 1;
		}
		else
		{
			firstLine = 0;
		}
		
		/*-----------------------------------------*/
		
		/* Checks if beer type already exists */
		
		reading = fopen ( "../Files/BeerTypes", "r" );
		while ( fgets ( linha, BEER_NAME_MAX_LENGTH + COMMENT_MAX_LENGTH + 8, reading ) != NULL )
		{
			if ( linha[strlen(linha) - 1] == '\n' ) 
				linha[strlen(linha) - 1] = '\0';
			if ( linha[strlen(linha) - 2] == '\n' ) 
				linha[strlen(linha) - 2] = '\0';
			if ( linha[0] == 't' )
				if ( linha[1] == '|' )
				{
					if ( strcmp ( &linha[2], geBeerType ) == 0 )
					{
						printf ( "This name is already in use.<br>\n" );
						createButton ( "Go back to the Previous Page.", "geAddBeerType.cgi", geLanguage, geNickname, "18" );
						createButton ( "Go back to the Main Menu.", "geShowMenu.cgi", geLanguage, geNickname, "19" );
						printf("      <a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a><br>\n", idString );
						
						fclose ( reading );
						mlCgiFreeResources ( );
						exit ( ML_CGI_OK );
					}
				}
		}
		fclose ( reading );
		/*--------------------------------------*/
		
		/* Apends new beer type */

		writing = fopen ( "../Files/BeerTypes", "a" );
		
		if ( firstLine == 1 )
			fprintf ( writing, "t|%s", geBeerType );
		else
			fprintf ( writing, "\nt|%s", geBeerType );
		fprintf ( writing, "\nd|%s", userLoggedIn.email ); 
		fprintf ( writing, "\nc|%s", geComment );
		
		fclose ( writing );
		/*--------------------------------------*/
		
		printf("    Successfully executed.<br> " );  
		createButton ( "Go back to the Previous Page.", "geAddBeerType.cgi", geLanguage, geNickname, "18" );
		createButton ( "Go back to the Main Menu.", "geShowMenu.cgi", geLanguage, geNickname, "19" );
		
		printf ("      <a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a><br>\n", idString );
		printf("     <a href=\"geHelp.cgi?geLanguage=english\" target=\"_blank\">(?)</a><br>\n" );
	}
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
		
/* $RCSfile: geCgiExecuteAddBeerType.c,v $ */
