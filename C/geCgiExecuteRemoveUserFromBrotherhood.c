	/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:13:33 $
 * $Log: geCgiExecuteRemoveUserFromBrotherhood.c,v $
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
#include "sendmail.h"
#include "geCrypt.h"

int
main ( int argc, char *argv[] )
{
	environmentType environment;
	languageType language;
	char geLanguage [LANGUAGE_MAX_LENGTH + 1];
	char geNickname [NICKNAME_MAX_LENGTH + 1];
	geUserDataType userLoggedIn;
	char idString[21];
	char cookieValue[COOKIE_LENGTH + 1], cookieValueNew[COOKIE_LENGTH + 1];
	char geBrotherhoodName[BROTHERHOOD_MAX_LENGTH + 1];
	char geRemoveName [NICKNAME_MAX_LENGTH + 1];
	char linha [NICKNAME_MAX_LENGTH + 3];
	FILE *writing, *reading;
	unsigned selectedBrotherhood = 0;
	unsigned firstLine = 1;
	unsigned userIsInBrotherhood = 0;
	unsigned leaderIsTheOne = 0;

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
	
	if ( mlCgiGetFormStringNoNewLines ( "brotherhoodName", geBrotherhoodName, BROTHERHOOD_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o nome da confraria.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve the brotherhood's name.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "removeUserName", geRemoveName, NICKNAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o nome do usu&aacute;rio a ser deletado.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve the user's name that would be deleted.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "18" );
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
		
		printf("	Checando confraria %s...<br>\n", geBrotherhoodName );
		
		/* Checks if user is in selected brotherhood */
		
		reading = fopen ( "../Files/Brotherhoods", "r" );
		while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading ) != NULL )
		{
			if ( linha[strlen(linha) - 1] == '\n' )
				linha[strlen(linha) - 1] = '\0';
			if ( linha[0] == 'b' )
				if ( linha[1] == '|' )
				{
					if ( strcmp ( &linha[2], geBrotherhoodName ) == 0 )
						selectedBrotherhood = 1;
					else
						selectedBrotherhood = 0;
				}
			if ( linha[0] == 'u' )
				if ( linha[1] == '|' )
					if ( selectedBrotherhood == 1 )
						if ( strcmp ( &linha[2], geRemoveName ) == 0 )
							userIsInBrotherhood = 1;
			if ( linha[0] == 'c' )
				if ( linha[1] == '|' )
					if ( selectedBrotherhood == 1 )
						if ( strcmp ( userLoggedIn.email, &linha[2] ) == 0 )
							leaderIsTheOne = 1;
					
		}
		fclose ( reading );
		
		if ( ( leaderIsTheOne == 0 ) && ( userLoggedIn.group != 1 ) )
		{
			printf ( "<div align=\"center\">" );
			printf ( "Ops. Voc&ecirc; n&atilde;o &eacute; o dono da confraria.<br>\n" );
			createButton ( "Voltar.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
		
		if ( userIsInBrotherhood == 0 )
		{
			printf ( "<div align=\"center\">" );
			printf ( "Ops. O usu&aacute;rio n&atilde;o est&aacute; na confraria ou &eacute; o dono.<br>\n" );
			createButton ( "Voltar.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
		
		/*--------------------------------------*/
		
		/* Removes user from brotherhood */

		reading = fopen ( "../Files/Brotherhoods", "r" );
		writing = fopen ( "../Files/Brotherhoods.ruser", "w" );
		selectedBrotherhood = 0;
		firstLine = 1;

		while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading ) != NULL )
		{
			if ( linha[strlen(linha) - 1] == '\n' )
			linha[strlen(linha) - 1] = '\0';
			if ( linha[0] == 'b' )
				if ( linha[1] == '|' )
				{	
					selectedBrotherhood = 0;
					if ( strcmp ( &linha[2], geBrotherhoodName ) == 0 )
					{
						selectedBrotherhood = 1;
					}
					if ( firstLine == 1 )
						fprintf ( writing, "%s", linha );
					else
						fprintf ( writing, "\n%s", linha );
					firstLine = 0;
				}
			if ( ( linha[0] == 'u' ) || ( linha[0] == 'c' ) )
			{
				if ( linha[1] == '|' )
				{
					if ( selectedBrotherhood == 1 )
					{
						if ( strcmp ( geRemoveName, &linha[2] ) != 0 )
						{
							if ( firstLine == 1 )
								fprintf ( writing, "%s", linha );
							else
								fprintf ( writing, "\n%s", linha );
							firstLine = 0;
						}
					}
					else
					{
						if ( firstLine == 1 )
							fprintf ( writing, "%s", linha );
						else
							fprintf ( writing, "\n%s", linha );
						firstLine = 0;
					}
				}
			}
		}
		
		fclose ( reading );
		fclose ( writing );
		
		remove ( "../Files/Brotherhoods" );
		rename ( "../Files/Brotherhoods.ruser", "../Files/Brotherhoods" );
		
		/*--------------------------------------*/
		
		printf("    Executado com sucesso.<br> " );  
		createButton ( "Voltar para a p&aacute;gina anterior.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "18" );
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
		
		printf("	Checking Brotherhood: %s<br>\n", geBrotherhoodName );
		
		/* Checks if user is in selected brotherhood */
		
		reading = fopen ( "../Files/Brotherhoods", "r" );
		while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading ) != NULL )
		{
			if ( linha[strlen(linha) - 1] == '\n' )
				linha[strlen(linha) - 1] = '\0';
			if ( linha[0] == 'b' )
				if ( linha[1] == '|' )
				{
					if ( strcmp ( &linha[2], geBrotherhoodName ) == 0 )
						selectedBrotherhood = 1;
					else
						selectedBrotherhood = 0;
				}
			if ( linha[0] == 'u' )
				if ( linha[1] == '|' )
					if ( selectedBrotherhood == 1 )
						if ( strcmp ( &linha[2], geRemoveName ) == 0 )
							userIsInBrotherhood = 1;
			if ( linha[0] == 'c' )
				if ( linha[1] == '|' )
					if ( selectedBrotherhood == 1 )
						if ( strcmp ( userLoggedIn.email, &linha[2] ) == 0 )
							leaderIsTheOne = 1;
					
		}
		fclose ( reading );
		
		if ( ( leaderIsTheOne == 0 ) && ( userLoggedIn.group != 1 ) )
		{
			printf ( "<div align=\"center\">" );
			printf ( "Ops. You are not the Brotherhood's Creator.<br>\n" );
			createButton ( "Go back.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
		
		if ( userIsInBrotherhood == 0 )
		{
			printf ( "<div align=\"center\">" );
			printf ( "Ops. The user is not in the selected Brotherhood or he is the Creator of it.<br>\n" );
			createButton ( "Go back.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
		
		/*--------------------------------------*/
		
		/* Removes user from brotherhood */

		reading = fopen ( "../Files/Brotherhoods", "r" );
		writing = fopen ( "../Files/Brotherhoods.ruser", "w" );
		selectedBrotherhood = 0;
		firstLine = 1;

		while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading ) != NULL )
		{
			if ( linha[strlen(linha) - 1] == '\n' )
			linha[strlen(linha) - 1] = '\0';
			if ( linha[0] == 'b' )
				if ( linha[1] == '|' )
				{	
					selectedBrotherhood = 0;
					if ( strcmp ( &linha[2], geBrotherhoodName ) == 0 )
					{
						selectedBrotherhood = 1;
					}
					if ( firstLine == 1 )
						fprintf ( writing, "%s", linha );
					else
						fprintf ( writing, "\n%s", linha );
					firstLine = 0;
				}
			if ( ( linha[0] == 'u' ) || ( linha[0] == 'c' ) )
			{
				if ( linha[1] == '|' )
				{
					if ( selectedBrotherhood == 1 )
					{
						if ( strcmp ( geRemoveName, &linha[2] ) != 0 )
						{
							if ( firstLine == 1 )
								fprintf ( writing, "%s", linha );
							else
								fprintf ( writing, "\n%s", linha );
							firstLine = 0;
						}
					}
					else
					{
						if ( firstLine == 1 )
							fprintf ( writing, "%s", linha );
						else
							fprintf ( writing, "\n%s", linha );
						firstLine = 0;
					}
				}
			}
		}
		
		fclose ( reading );
		fclose ( writing );
		
		remove ( "../Files/Brotherhoods" );
		rename ( "../Files/Brotherhoods.ruser", "../Files/Brotherhoods" );
		
		/*--------------------------------------*/
		
		printf("    Success!<br> " );
		createButton ( "Go to the previous page.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "18" );
		createButton ( "Go to the main menu.", "geShowMenu.cgi", geLanguage, geNickname, "19" );
		
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
		
/* $RCSfile: geCgiExecuteRemoveUserFromBrotherhood.c,v $ */
