/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 21:47:25 $
 * $Log: geCgiAddUserToBrotherhood.c,v $
 * Revision 1.2  2015/01/10 21:47:25  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.1  2015/01/10 19:07:51  vitor.schoola
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
	unsigned counter, counter2;
	char geLanguage [LANGUAGE_MAX_LENGTH];
	char geNickname [NICKNAME_MAX_LENGTH];
	char ipString [IP_MAX_LENGTH], ipStringNew[IP_MAX_LENGTH];
	char idString[21];
	char idStringD[22];
	FILE *reading;
	char cookieValue[COOKIE_LENGTH + 1], cookieValueNew[COOKIE_LENGTH + 1];
	char fullPath [strlen("../Files/Cookies/D") + strlen(idString) + 2];
	char brotherhoodName[BROTHERHOOD_MAX_LENGTH + 1];
	char linha[ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 7];
	geUserDataType userLoggedIn;
	time_t tempo;
	
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
	
	for ( counter = 0; counter < strlen ( fullPath ); counter++ )
		fullPath[counter] = '\0';
	strcat ( fullPath, "../Files/Cookies/D" );
	strcat ( fullPath, idString );
	for ( counter = 0; counter < strlen ( idStringD ); counter++ )
		idStringD[counter] = '\0';
	strcat ( idStringD, "D" );
	strcat ( idStringD, idString );
	printf ( "<p hidden> \n" );
	mlCgiCreateCookie ( "../Files/Cookies/", idStringD, 30 );
	printf ( "</p>\n" );
	readCookie ( "../Files/Cookies/D", idString, cookieValue, ipStringNew, NULL );
	remove ( fullPath );
	
	readCookie ( "../Files/Cookies/", idString, cookieValue, ipString, &tempo );
	
	if ( strcmp ( cookieValueNew, cookieValue ) != 0 )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "Cookie diferente do de login. Se logue novamente.", ""  );
			printf ( "<div align=\"center\">" );
			printf ( "<a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Cookie is not the same from the login. Login again.", "" );
			printf ( "<div align=\"center\">" );
			printf ( "<a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( strcmp ( ipString, ipStringNew ) != 0 )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "IP diferente do momento de login. Se logue novamente.", "" );
			printf ( "<div align=\"center\">" );
			printf ( "<a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "IP is not the same from the moment you logged in. Login again.", ""  );
			printf ( "<div align=\"center\">" );
			printf ( "<a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
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
		
		printf("    Adicionar/Convidar para Confraria: <br>\n" );
		printf ("<form action=\"geExecuteAddUserToBrotherhood.cgi\" method=\"post\"> \n" );
		printf ("  <input type='hidden' name='geLanguage' value='%s'>\n", geLanguage );
		printf ("  <input type='hidden' name='nickname' value='%s'>\n", geNickname );
		
		printf ("  <p id=\"hiddenInputCookie\"></p> \n" );
		printf ("  <script> \n");
		printf ("  document.getElementById(\"hiddenInputCookie\").innerHTML = checkCookie(); \n" );
		printf ("  </script> \n");

		if ( userLoggedIn.group == 1 )
		{
		
			printf (" Para qual confraria ir&aacute; convidar?<br> \n" );
			
			/* Check if Brotherhoods File exists */
			
			if ( fileExists ( "../Files/", "Brotherhoods" ) == 0 )
			{
				printf ( "</form>" );
				mlCgiShowErrorPage ( "Ops", "N&atilde;o h&aacute; nenhuma confraria no sistema.", "" );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar ao Menu Principal", "geShowMenu.cgi", geLanguage, geNickname, "0" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
			
			/*----------------------------------*/
			
			reading = fopen ( "../Files/Brotherhoods", "r" );
			while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading) != NULL )
			{
				if ( linha[strlen(linha) - 1] == '\n' )
					linha[strlen(linha) - 1] = '\0';
				if ( linha[0] == 'b' )
					if ( linha[1] == '|' )
					{
						for ( counter = 0; counter <= strlen ( linha ); counter++ )
							 brotherhoodName[counter] = linha[2 + counter];
						printf ( "<input type='radio' name='brotherhoodName' value='%s'><b>%s</b><br>\n", brotherhoodName, brotherhoodName );						
					}
				if ( linha[0] == 'u' )
					if ( linha[1] == '|' )
					{
						if ( strcmp ( &linha[2], userLoggedIn.email ) == 0 )
						{
						//	printf ( "<input type='radio' name='brotherhoodName' value='%s'><b>%s</b><br>\n", brotherhoodName, brotherhoodName );						
						}
					}
				if ( linha[0] == 'c' )
					if ( linha[1] == '|' )
					{
						if ( strcmp ( &linha[2], userLoggedIn.email ) == 0 )
						{
						//	printf ( "<input type='radio' name='brotherhoodName' value='%s'><B>%s</b> ( Voc&ecirc &eacute; o criador )<br>\n", brotherhoodName, brotherhoodName );						
						}
					}
			}
			
			printf (" Listagem de todos usu&aacute;rios:<br> \n" );
			
			/* Check if Users File exists */
			
			if ( fileExists ( "../Files/", "users" ) == 0 )
			{
				printf ( "</form>" );
				mlCgiShowErrorPage ( "Ops", "N&atilde;o h&aacute; nenhum usu&aacute;rio no sistema.", "" );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar ao Menu Principal", "geShowMenu.cgi", geLanguage, geNickname, "0" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
			
			/*----------------------------------*/
			
			reading = fopen ( "../Files/users", "r" );
			while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading) != NULL )
			{
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
				if ( strcmp ( &linha[counter + 1], userLoggedIn.email ) != 0 )
					if ( linha[counter2 + 1] != ';' )
						printf ( "<input type='radio' name='inviteUserName' value='%s'><b>%s</b><br>", &linha[counter + 1], &linha[counter + 1] );
			}
			fclose ( reading );
			
			printf (" Convidar <input type='radio' name='invoradd' value='i' checked> Adicionar <input type='radio' name='invoradd' value='a'><br>\n" );
			
			printf ("  <input type=\"submit\" name=\"submit\" value=\"Convidar/Adicionar\"><br> \n" );
			
		}
		
		if ( userLoggedIn.group == 2 )
		{
		
			printf (" Para qual confraria ir&aacute; convidar?<br> \n" );
			
			/* Check if Brotherhoods File exists */
			
			if ( fileExists ( "../Files/", "Brotherhoods" ) == 0 )
			{
				printf ( "</form>" );
				mlCgiShowErrorPage ( "Ops", "N&atilde;o h&aacute; nenhuma confraria no sistema.", "" );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar ao Menu Principal", "geShowMenu.cgi", geLanguage, geNickname, "0" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
			
			/*----------------------------------*/
			
			reading = fopen ( "../Files/Brotherhoods", "r" );
			while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading) != NULL )
			{
				if ( linha[strlen(linha) - 1] == '\n' )
					linha[strlen(linha) - 1] = '\0';
				if ( linha[0] == 'b' )
					if ( linha[1] == '|' )
					{
						for ( counter = 0; counter <= strlen ( linha ); counter++ )
							 brotherhoodName[counter] = linha[2 + counter];
					}
				if ( linha[0] == 'u' )
					if ( linha[1] == '|' )
					{
						if ( strcmp ( &linha[2], userLoggedIn.email ) == 0 )
						{
							printf ( "<input type='radio' name='brotherhoodName' value='%s'><b>%s</b><br>\n", brotherhoodName, brotherhoodName );						
						}
					}
				if ( linha[0] == 'c' )
					if ( linha[1] == '|' )
					{
						if ( strcmp ( &linha[2], userLoggedIn.email ) == 0 )
						{
							printf ( "<input type='radio' name='brotherhoodName' value='%s'><B>%s</b> ( Voc&ecirc &eacute; o criador )<br>\n", brotherhoodName, brotherhoodName );						
						}
					}
			}
			
			printf (" Listagem de todos usu&aacute;rios:<br> \n" );
			
			/* Check if Users File exists */
			
			if ( fileExists ( "../Files/", "users" ) == 0 )
			{
				printf ( "</form>" );
				mlCgiShowErrorPage ( "Ops", "N&atilde;o h&aacute; nenhum usu&aacute;rio no sistema.", "" );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar ao Menu Principal", "geShowMenu.cgi", geLanguage, geNickname, "0" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
			
			/*----------------------------------*/
			
			reading = fopen ( "../Files/users", "r" );
			while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading) != NULL )
			{
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
				if ( strcmp ( &linha[counter + 1], userLoggedIn.email ) != 0 )
					if ( linha[counter2 + 1] != ';' )
						printf ( "<input type='radio' name='inviteUserName' value='%s'><b>%s</b><br>", &linha[counter + 1], &linha[counter + 1] );
			}
			fclose ( reading );
			
			printf (" Convidar <input type='radio' name='eamfeecs' checked> <input type='hidden' name='invoradd' value='i'> <br>\n" );
			
			printf ("  <input type=\"submit\" name=\"submit\" value=\"Convidar\"><br> \n" );
		
		}
		
		if ( userLoggedIn.group == 4 )
		{
		}
		
		printf ( "     </form>\n" );
		createButton ( "Voltar ao Menu Principal", "geShowMenu.cgi", geLanguage, geNickname, "0" );
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

		printf("    Add/Invite to Brotherhood: <br>\n" );
		printf ("<form action=\"geExecuteAddUserToBrotherhood.cgi\" method=\"post\"> \n" );
		printf ("  <input type='hidden' name='geLanguage' value='%s'>\n", geLanguage );
		printf ("  <input type='hidden' name='nickname' value='%s'>\n", geNickname );
		
		printf ("  <p id=\"hiddenInputCookie\"></p> \n" );
		printf ("  <script> \n");
		printf ("  document.getElementById(\"hiddenInputCookie\").innerHTML = checkCookie(); \n" );
		printf ("  </script> \n");

		if ( userLoggedIn.group == 1 )
		{
		
			printf (" Which brotherhood will you invite the user to?<br> \n" );
			
			/* Check if Brotherhoods File exists */
			
			if ( fileExists ( "../Files/", "Brotherhoods" ) == 0 )
			{
				printf ( "</form>" );
				mlCgiShowErrorPage ( "Ops", "There is no brotherhood in the system.", "" );
				printf ( "<div align=\"center\">" );
				createButton ( "Go back to the Main Menu", "geShowMenu.cgi", geLanguage, geNickname, "0" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
			
			/*----------------------------------*/
			
			reading = fopen ( "../Files/Brotherhoods", "r" );
			while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading) != NULL )
			{
				if ( linha[strlen(linha) - 1] == '\n' )
					linha[strlen(linha) - 1] = '\0';
				if ( linha[0] == 'b' )
					if ( linha[1] == '|' )
					{
						for ( counter = 0; counter <= strlen ( linha ); counter++ )
							 brotherhoodName[counter] = linha[2 + counter];
						printf ( "<input type='radio' name='brotherhoodName' value='%s'><b>%s</b><br>\n", brotherhoodName, brotherhoodName );						
					}
				if ( linha[0] == 'u' )
					if ( linha[1] == '|' )
					{
						if ( strcmp ( &linha[2], userLoggedIn.email ) == 0 )
						{
						//	printf ( "<input type='radio' name='brotherhoodName' value='%s'><b>%s</b><br>\n", brotherhoodName, brotherhoodName );						
						}
					}
				if ( linha[0] == 'c' )
					if ( linha[1] == '|' )
					{
						if ( strcmp ( &linha[2], userLoggedIn.email ) == 0 )
						{
						//	printf ( "<input type='radio' name='brotherhoodName' value='%s'><B>%s</b> ( You're the creator )<br>\n", brotherhoodName, brotherhoodName );						
						}
					}
			}
			
			printf (" Listing of all Users:<br> \n" );
			
			/* Check if Users File exists */
			
			if ( fileExists ( "../Files/", "users" ) == 0 )
			{
				printf ( "</form>" );
				mlCgiShowErrorPage ( "Ops", "There is no user in our system.", "" );
				printf ( "<div align=\"center\">" );
				createButton ( "Go back to the Main Menu", "geShowMenu.cgi", geLanguage, geNickname, "0" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
			
			/*----------------------------------*/
			
			reading = fopen ( "../Files/users", "r" );
			while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading) != NULL )
			{
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
				if ( strcmp ( &linha[counter + 1], userLoggedIn.email ) != 0 )
					if ( linha[counter2 + 1] != ';' )
						printf ( "<input type='radio' name='inviteUserName' value='%s'><b>%s</b><br>", &linha[counter + 1], &linha[counter + 1] );
			}
			fclose ( reading );
			
			printf (" Invite <input type='radio' name='invoradd' value='i' checked> Add <input type='radio' name='invoradd' value='a'><br>\n" );
			
			printf ("  <input type=\"submit\" name=\"submit\" value=\"Invite/Add\"><br> \n" );
			
		}
		
		if ( userLoggedIn.group == 2 )
		{
		
			printf (" Which Brotherhood will you invite to?<br> \n" );
			
			/* Check if Brotherhoods File exists */
			
			if ( fileExists ( "../Files/", "Brotherhoods" ) == 0 )
			{
				printf ( "</form>" );
				mlCgiShowErrorPage ( "Ops", "There is no brotherhood in our system.", "" );
				printf ( "<div align=\"center\">" );
				createButton ( "Go back to the Main Menu", "geShowMenu.cgi", geLanguage, geNickname, "0" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
			
			/*----------------------------------*/
			
			reading = fopen ( "../Files/Brotherhoods", "r" );
			while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading) != NULL )
			{
				if ( linha[strlen(linha) - 1] == '\n' )
					linha[strlen(linha) - 1] = '\0';
				if ( linha[0] == 'b' )
					if ( linha[1] == '|' )
					{
						for ( counter = 0; counter <= strlen ( linha ); counter++ )
							 brotherhoodName[counter] = linha[2 + counter];
					}
				if ( linha[0] == 'u' )
					if ( linha[1] == '|' )
					{
						if ( strcmp ( &linha[2], userLoggedIn.email ) == 0 )
						{
							printf ( "<input type='radio' name='brotherhoodName' value='%s'><b>%s</b><br>\n", brotherhoodName, brotherhoodName );						
						}
					}
				if ( linha[0] == 'c' )
					if ( linha[1] == '|' )
					{
						if ( strcmp ( &linha[2], userLoggedIn.email ) == 0 )
						{
							printf ( "<input type='radio' name='brotherhoodName' value='%s'><B>%s</b> ( You're the creator )<br>\n", brotherhoodName, brotherhoodName );						
						}
					}
			}
			
			printf (" Listing of all Users:<br> \n" );
			
			/* Check if Users File exists */
			
			if ( fileExists ( "../Files/", "users" ) == 0 )
			{
				printf ( "</form>" );
				mlCgiShowErrorPage ( "Ops", "There is no user in our system.", "" );
				printf ( "<div align=\"center\">" );
				createButton ( "Go back to the Main Menu", "geShowMenu.cgi", geLanguage, geNickname, "0" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
			
			/*----------------------------------*/
			
			reading = fopen ( "../Files/users", "r" );
			while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading) != NULL )
			{
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
				if ( strcmp ( &linha[counter + 1], userLoggedIn.email ) != 0 )
					if ( linha[counter2 + 1] != ';' )
						printf ( "<input type='radio' name='inviteUserName' value='%s'><b>%s</b><br>", &linha[counter + 1], &linha[counter + 1] );
			}
			fclose ( reading );
			
			printf (" Invite <input type='radio' name='eamfeecs' checked> <input type='hidden' name='invoradd' value='i'> <br>\n" );
			
			printf ("  <input type=\"submit\" name=\"submit\" value=\"Invite\"><br> \n" );
		
		}
		
		if ( userLoggedIn.group == 4 )
		{
		}
		
		printf ( "     </form>\n" );
		createButton ( "Go back to the Main Menu", "geShowMenu.cgi", geLanguage, geNickname, "0" );
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
		
/* $RCSfile: geCgiAddUserToBrotherhood.c,v $ */
