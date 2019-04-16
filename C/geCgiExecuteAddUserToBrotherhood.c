	/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 21:47:25 $
 * $Log: geCgiExecuteAddUserToBrotherhood.c,v $
 * Revision 1.3  2015/01/10 21:47:25  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.2  2015/01/10 21:23:15  vitor.schoola
 * *** empty log message ***
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
#include "sendmail.h"
#include "geCrypt.h"

int
main ( int argc, char *argv[] )
{
	environmentType environment;
	languageType language;
	char geLanguage [LANGUAGE_MAX_LENGTH + 1];
	char geNickname [NICKNAME_MAX_LENGTH + 1];
	unsigned counter;
	geUserDataType userLoggedIn;
	geUserDataType userInvited;
	char idString[21];
	char cookieValue[COOKIE_LENGTH + 1], cookieValueNew[COOKIE_LENGTH + 1];
	char geBrotherhoodName[BROTHERHOOD_MAX_LENGTH + 1];
	char selectedOpt[3];
	char geNewUserName[NICKNAME_MAX_LENGTH + 1];
	char linha [NICKNAME_MAX_LENGTH + 3];
	FILE *writing, *reading;
	unsigned selectedBrotherhood = 0;
	unsigned firstLine = 1;
	unsigned writtenNewName = 0;
	char textMessage[NAME_MAX_LENGTH + 777]; 
	unsigned erro = 0;
	
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
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve the brotherhood's name.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "inviteUserName", geNewUserName, NICKNAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o nome da confraria.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve the brotherhood's name.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "invoradd", selectedOpt, 4 ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver a op&ccedil;&atilde;o (convidar ou adicionar).", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve selected option (invite or add).", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
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
		
		/* Checks if user is already in selected brotherhood */
		
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
					{
						if ( strcmp ( &linha[2], geNewUserName ) == 0 )
						{
							printf ( "<div align=\"center\">" );
							printf ( "Ops. O usu&aacute;rio j&aacute; est&aacute; na confraria.<br>\n" );
							createButton ( "Voltar.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
							printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
							printf ( "</div>" );
							mlCgiFreeResources ( );
							exit ( ML_CGI_OK );
						}
					}
		}
		fclose ( reading );
		/*--------------------------------------*/
		
		/* Apends new user to brotherhood or invites him */

		if ( selectedOpt[0] == 'a' )
		{
			writing = fopen ( "../Files/Brotherhoods.auser", "w" );
			reading = fopen ( "../Files/Brotherhoods", "r" );
			while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading ) != NULL )
			{
				if ( linha[strlen(linha) - 1] == '\n' )
					linha[strlen(linha) - 1] = '\0';
				if ( linha[0] == 'b' )
				{
					if ( linha[1] == '|' )
					{
						if ( firstLine == 0 )
							fprintf ( writing, "\n%s",linha );
						else
						{
							fprintf ( writing, "%s", linha );
							firstLine = 0;
						}
						if ( strcmp ( geBrotherhoodName, &linha[2] ) == 0 )
						{
							fprintf ( writing, "\nu|%s", geNewUserName );
							writtenNewName = 1;
						}
					}
				}
				else
				{
					if ( linha[1] == '|' )
					{
						if ( firstLine == 0 )
							fprintf ( writing, "\n%s",linha );
						else
						{
							fprintf ( writing, "%s", linha );
							firstLine = 0;
						}
					}
				}
			}
			
			fclose ( reading );
			fclose ( writing );
			
			remove ( "../Files/Brotherhoods" );
			rename ( "../Files/Brotherhoods.auser", "../Files/Brotherhoods" );
			
		}
		else
		{
			CheckUserInfo ( geNewUserName, &userInvited );
			
			if ( fileExists ( "../Files/", "Brotherhood.invited" ) == 0 )
			{
				writing = fopen ( "../Files/Brotherhood.invited", "w" );
				fprintf ( writing, "b|%s\ni|%s", geBrotherhoodName, geNewUserName );
				fclose ( writing );
			}
			else
			{
				reading = fopen ( "../Files/Brotherhood.invited", "r" );
				writing = fopen ( "../Files/Brotherhood.invited.a", "w" );
				selectedBrotherhood = 0;
				writtenNewName = 0;
				firstLine = 1;

				while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading ) != NULL )
				{
					if ( linha[strlen(linha) - 1] == '\n' )
						linha[strlen(linha) - 1] = '\0';
					if ( linha[strlen(linha) - 2] == '\n' )
						linha[strlen(linha) - 2] = '\0';
					if ( linha[0] == 'b' )
					{
						if ( linha[1] == '|' )
						{
							if ( selectedBrotherhood == 1 )
							{
								fprintf ( writing ,"\ni|%s", geNewUserName );
								selectedBrotherhood = 0;
								writtenNewName = 1;
							}
							if ( strcmp ( &linha[2], geBrotherhoodName ) == 0 )
							{
								selectedBrotherhood = 1;
							}
						}
					}
					if ( linha[0] == 'i' )
						if ( linha[1] == '|' )
							if ( selectedBrotherhood == 1 )
								if ( strcmp ( geNewUserName, &linha[2] ) == 0 )
								{
									printf ( "Usu&aacute;rio j&aacute; convidado.<br>\n" );
									createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
									createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "19" );
									printf("      <a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a><br>\n", idString );
									erro = 1;
								}
					if ( firstLine == 1 )
						fprintf ( writing, "%s", linha );
					else
						fprintf ( writing, "\n%s", linha );
					firstLine = 0;
				}
				if ( ( writtenNewName == 0 ) && ( selectedBrotherhood == 0 ) )
				{
					if ( firstLine == 1 )
						fprintf ( writing, "b|%s\ni|%s", geBrotherhoodName, geNewUserName );
					else
						fprintf ( writing, "\nb|%s\ni|%s", geBrotherhoodName, geNewUserName );
				}
				if ( ( writtenNewName == 0 ) && ( selectedBrotherhood == 1 ) )
				{
					if ( selectedBrotherhood == 1 )
					{
						fprintf ( writing ,"\ni|%s", geNewUserName );
						selectedBrotherhood = 0;
						writtenNewName = 1;
					}
				}
				fclose ( reading );
				fclose ( writing );
				
				if ( erro == 1 )
				{
					remove ( "../Files/Brotherhood.invited.a" );
					mlCgiFreeResources ( );
					exit ( ML_CGI_OK );
				}
				
				remove ( "../Files/Brotherhood.invited" );
				rename ( "../Files/Brotherhood.invited.a", "../Files/Brotherhood.invited" );	
			}
			
			for ( counter = 0; counter < NAME_MAX_LENGTH + 600; counter++ )
				textMessage[counter] = '\0';
			strcat ( textMessage, "Ola, " );
			strcat ( textMessage, userInvited.name );
			strcat ( textMessage, "!\nVoce foi convidado para a confraria " );
			strcat ( textMessage, geBrotherhoodName );
			strcat ( textMessage, ".\nPara se juntar, se logue e va nas opcoes de confrarias.\n" );
			sendMail ( 	"del.ufrj.br",
			"smtp.del.ufrj.br",
			25,
			"GelaDOS@email.com",
			userInvited.email,
			NULL,
			NULL,
			"Foi convidado para uma confraria!",
			textMessage,
			NULL );
			
		}
		
		/*--------------------------------------*/
		
		printf("    Executado com sucesso.<br> " );  
		createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
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
		
		/* Checks if user is already in selected brotherhood */
		
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
					{
						if ( strcmp ( &linha[2], geNewUserName ) == 0 )
						{
							printf ( "<div align=\"center\">" );
							printf ( "Ops. This user is already in the selected brotherhood.<br>\n" );
							createButton ( "Go back.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
							printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
							printf ( "</div>" );
							mlCgiFreeResources ( );
							exit ( ML_CGI_OK );
						}
					}
		}
		fclose ( reading );
		/*--------------------------------------*/
		
		/* Apends new user to brotherhood or invites him */

		if ( selectedOpt[0] == 'a' )
		{
			writing = fopen ( "../Files/Brotherhoods.auser", "w" );
			reading = fopen ( "../Files/Brotherhoods", "r" );
			while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading ) != NULL )
			{
				if ( linha[strlen(linha) - 1] == '\n' )
					linha[strlen(linha) - 1] = '\0';
				if ( linha[0] == 'b' )
				{
					if ( linha[1] == '|' )
					{
						if ( firstLine == 0 )
							fprintf ( writing, "\n%s",linha );
						else
						{
							fprintf ( writing, "%s", linha );
							firstLine = 0;
						}
						if ( strcmp ( geBrotherhoodName, &linha[2] ) == 0 )
						{
							fprintf ( writing, "\nu|%s", geNewUserName );
							writtenNewName = 1;
						}
					}
				}
				else
				{
					if ( linha[1] == '|' )
					{
						if ( firstLine == 0 )
							fprintf ( writing, "\n%s",linha );
						else
						{
							fprintf ( writing, "%s", linha );
							firstLine = 0;
						}
					}
				}
			}
			
			fclose ( reading );
			fclose ( writing );
			
			remove ( "../Files/Brotherhoods" );
			rename ( "../Files/Brotherhoods.auser", "../Files/Brotherhoods" );
			
		}
		else
		{
			CheckUserInfo ( geNewUserName, &userInvited );
			
			if ( fileExists ( "../Files/", "Brotherhood.invited" ) == 0 )
			{
				writing = fopen ( "../Files/Brotherhood.invited", "w" );
				fprintf ( writing, "b|%s\ni|%s", geBrotherhoodName, geNewUserName );
				fclose ( writing );
			}
			else
			{
				reading = fopen ( "../Files/Brotherhood.invited", "r" );
				writing = fopen ( "../Files/Brotherhood.invited.a", "w" );
				selectedBrotherhood = 0;
				writtenNewName = 0;
				firstLine = 1;

				while ( fgets ( linha, NICKNAME_MAX_LENGTH + 2, reading ) != NULL )
				{
					if ( linha[strlen(linha) - 1] == '\n' )
						linha[strlen(linha) - 1] = '\0';
					if ( linha[strlen(linha) - 2] == '\n' )
						linha[strlen(linha) - 2] = '\0';
					if ( linha[0] == 'b' )
					{
						if ( linha[1] == '|' )
						{
							if ( selectedBrotherhood == 1 )
							{
								fprintf ( writing ,"\ni|%s", geNewUserName );
								selectedBrotherhood = 0;
								writtenNewName = 1;
							}
							if ( strcmp ( &linha[2], geBrotherhoodName ) == 0 )
							{
								selectedBrotherhood = 1;
							}
						}
					}
					if ( linha[0] == 'i' )
						if ( linha[1] == '|' )
							if ( selectedBrotherhood == 1 )
								if ( strcmp ( geNewUserName, &linha[2] ) == 0 )
								{
									printf ( "Usu&aacute;rio j&aacute; convidado.<br>\n" );
									createButton ( "Voltar para a p&aacute;gina anterior.", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "18" );
									createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "19" );
									printf("      <a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a><br>\n", idString );
									erro = 1;
								}
					if ( firstLine == 1 )
						fprintf ( writing, "%s", linha );
					else
						fprintf ( writing, "\n%s", linha );
					firstLine = 0;
				}
				if ( ( writtenNewName == 0 ) && ( selectedBrotherhood == 0 ) )
				{
					if ( firstLine == 1 )
						fprintf ( writing, "b|%s\ni|%s", geBrotherhoodName, geNewUserName );
					else
						fprintf ( writing, "\nb|%s\ni|%s", geBrotherhoodName, geNewUserName );
				}
				if ( ( writtenNewName == 0 ) && ( selectedBrotherhood == 1 ) )
				{
					if ( selectedBrotherhood == 1 )
					{
						fprintf ( writing ,"\ni|%s", geNewUserName );
						selectedBrotherhood = 0;
						writtenNewName = 1;
					}
				}
				fclose ( reading );
				fclose ( writing );
				
				if ( erro == 1 )
				{
					remove ( "../Files/Brotherhood.invited.a" );
					mlCgiFreeResources ( );
					exit ( ML_CGI_OK );
				}
				
				remove ( "../Files/Brotherhood.invited" );
				rename ( "../Files/Brotherhood.invited.a", "../Files/Brotherhood.invited" );	
			}
			
			for ( counter = 0; counter < NAME_MAX_LENGTH + 600; counter++ )
				textMessage[counter] = '\0';
			strcat ( textMessage, "Hello, " );
			strcat ( textMessage, userInvited.name );
			strcat ( textMessage, "!\nYou were invited to the following brotherhood: " );
			strcat ( textMessage, geBrotherhoodName );
			strcat ( textMessage, ".\nTo join, login and select the 'Check Pending Invites' option.\n" );
			sendMail ( 	"del.ufrj.br",
			"smtp.del.ufrj.br",
			25,
			"GelaDOS@email.com",
			userInvited.email,
			NULL,
			NULL,
			"You were invited to a Brotherhood!",
			textMessage,
			NULL );
		}
		
		/*--------------------------------------*/
		
		printf("    Success!<br> " );
		createButton ( "Go to the previous page.", "geAddBrotherhood.cgi", geLanguage, geNickname, "18" );
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
		
/* $RCSfile: geCgiExecuteAddUserToBrotherhood.c,v $ */
