/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/11 05:16:32 $
 * $Log: geCgiExecuteAddUser.c,v $
 * Revision 1.3  2015/01/11 05:16:32  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.2  2015/01/11 03:23:32  vitor.schoola
 * ,
 *
 * Revision 1.1  2015/01/10 19:11:55  vitor.schoola
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
	char geEmail [NICKNAME_MAX_LENGTH], geEmailConfirmation[NICKNAME_MAX_LENGTH];
	char gePass [PASSWORD_MAX_LENGTH], gePassConfirmation[PASSWORD_MAX_LENGTH];
	char geRealName [NAME_MAX_LENGTH];
	char geGroup [2];
	geUserDataType userLoggedIn;	
	char idString[21];
	char cookieValue[COOKIE_LENGTH + 1], cookieValueNew[COOKIE_LENGTH + 1];
	char coddedPassword[256];
	long long unsigned id;
	unsigned group;
	FILE *writing;
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
	
	if ( mlCgiGetFormStringNoNewLines ( "geEmail", geEmail, NICKNAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o login do novo usu&aacute;rio.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve the new user's Nickname", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "geEmailConfirmation", geEmailConfirmation, NICKNAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver a confirma&ccedil;&atilde;o do login do novo usu&aacute;rio.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve the new user's Confirmation of Nickname", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( strcmp ( geEmail, geEmailConfirmation ) != 0 )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "Email e confirma&ccedil;&atilde;o n&atilde;o batem.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Nickname and confirmation doesn't match.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "gePass", gePass, PASSWORD_MAX_LENGTH ) == ML_CGI_OK )
	{
		if ( mlCgiGetFormStringNoNewLines ( "gePassConfirmation", gePassConfirmation, PASSWORD_MAX_LENGTH ) == ML_CGI_OK )
		{
			if ( strcmp ( gePass, gePassConfirmation ) != 0 )
			{
				if ( language == portuguese ) 
				{
					mlCgiShowErrorPage ( "Error", "Password e confirma&ccedil;&atilde;o n&atilde;o batem.", ""  );
					printf ( "<div align=\"center\">" );
					createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
					printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
					printf ( "</div>" );
				}
				else
				{
					mlCgiShowErrorPage ( "Error", "Password and confirmation doesn't match.", ""  );
					printf ( "<div align=\"center\">" );
					createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
					printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
					printf ( "</div>" );
				}
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
		}
		else
		{
				if ( language == portuguese ) 
				{
					mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver a confirma&ccedil;&atilde;o da senha do novo usu&aacute;rio.", ""  );
					printf ( "<div align=\"center\">" );
					createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
					printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
					printf ( "</div>" );
				}
				else
				{
					mlCgiShowErrorPage ( "Error", "Could not retrieve the new user's Confirmation of Password", ""  );
					printf ( "<div align=\"center\">" );
					createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
					printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
					printf ( "</div>" );
				}
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
		}
	}
	
	if ( strcmp ( geEmail, "" ) == 0 )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "Email vazio.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Empty nickname.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( isUserRegistered ( geEmail ) != 0 )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "Email cadastrado/convidado.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Registered/Invited email.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}

	if ( mlCgiGetFormStringNoNewLines ( "geGroup", geGroup, 1 ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o grupo do novo usu&aacute;rio.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve the new user's Group", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "geRealName", geRealName, NAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o nome do novo usu&aacute;rio.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve the new user's name", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( strcmp ( geGroup, "1" ) == 0 )
		group = 1;
	else
	{
		if ( strcmp ( geGroup, "2" ) == 0 )
			group = 2;
		else
		{
			if ( strcmp ( geGroup, "4" ) == 0 )
				group = 4;
			else
			{
				if ( language == portuguese ) 
				{
					mlCgiShowErrorPage ( "Error", "Grupo inv&aacute;lido.", ""  );
					printf ( "<div align=\"center\">" );
					createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
					printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
					printf ( "</div>" );
				}
				else
				{
					mlCgiShowErrorPage ( "Error", "Invalid Group", ""  );
					printf ( "<div align=\"center\">" );
					createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
					printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
					printf ( "</div>" );
				}
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
		}
	}
	
	if ( userLoggedIn.userIdentifier == 4 )
	{
		if ( ( group == 2 ) || ( group == 1 ) )
		{
			if ( language == portuguese ) 
			{
				mlCgiShowErrorPage ( "Error", "Grupo inv&aacute;lido.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			else
			{
				mlCgiShowErrorPage ( "Error", "Invalid Group", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
	}

	if ( userLoggedIn.userIdentifier == 2 )
	{
		if ( group == 1 )
		{
			if ( language == portuguese ) 
			{
				mlCgiShowErrorPage ( "Error", "Grupo inv&aacute;lido.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			else
			{
				mlCgiShowErrorPage ( "Error", "Invalid Group", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
	}
	firstLine = 0;
	if ( strcmp ( gePass, "" ) == 0 )
	{
		InviteUser ( geEmail, language, geRealName, group );
		if ( fileExists ( "../Files/", "users.abeyances" ) == 0 )
			firstLine = 1;
		writing = fopen ( "../Files/users.abeyances", "a" );
		id = getNextId ();
		if ( firstLine == 1 )
			fprintf ( writing, "%020llu;%s;;%u;%s;%lld", id, geEmail, group, geRealName, (long long) time(NULL) );
		else
			fprintf ( writing, "\n%020llu;%s;;%u;%s;%lld", id, geEmail, group, geRealName, (long long) time(NULL) );
		fclose ( writing );
		firstLine = 0;
		if ( fileExists ( "../Files/", "users" ) == 0 )
			firstLine = 1;
		writing = fopen ( "../Files/users", "a" );
		if ( firstLine == 1 )
			fprintf ( writing, "%020llu;%s;;%u;%s;", id, geEmail, group, geRealName );
		else
			fprintf ( writing, "\n%020llu;%s;;%u;%s;", id, geEmail, group, geRealName );
		fclose ( writing );
		firstLine = 0;
	}
	else
	{
		if ( CodificarSenha ( gePass, coddedPassword ) != 0 )
		{
			if ( language == portuguese ) 
			{
				mlCgiShowErrorPage ( "Error", "Senha inv&aacute;lida.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			else
			{
				mlCgiShowErrorPage ( "Error", "Invalid password.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Go back.", "geAddUser.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
		
		/* Check if Users file already exists */
		
		if ( fileExists ( "../Files/", "users" ) == 0 )
		{
			writing = fopen ( "../Files/users", "w" );
			fclose( writing );
			firstLine = 1;
		}
		else
		{
			firstLine = 0;
		}
		
		/*-----------------------------------------*/
		
		writing = fopen ( "../Files/users", "a" );
		id = getNextId ();
		if ( firstLine == 1 )
			fprintf ( writing, "%020llu;%s;%s;%u;%s;", id, geEmail, coddedPassword, group, geRealName );
		else
			fprintf ( writing, "\n%020llu;%s;%s;%u;%s;", id, geEmail, coddedPassword, group, geRealName );
		fclose ( writing );
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
		
		printf("    Executado com sucesso.<br> " );  
		createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUser.cgi", geLanguage, geNickname, "18" );
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
		
		printf("    Success!<br> " );
		createButton ( "Go to the previous page.", "geAddUser.cgi", geLanguage, geNickname, "18" );
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
		
/* $RCSfile: geCgiExecuteAddUser.c,v $ */
