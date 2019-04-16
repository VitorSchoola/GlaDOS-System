/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:13:33 $
 * $Log: geCgiExecuteEditUser.c,v $
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
#include "geCrypt.h"

int
main ( int argc, char *argv[] )
{
	environmentType environment;
	languageType language;
	char willBeEdited [4];
	char geLanguage [LANGUAGE_MAX_LENGTH + 1];
	char geNickname [NICKNAME_MAX_LENGTH + 1];
	char geEditPreviousNickname [NICKNAME_MAX_LENGTH + 1];
	char geEditNickname [NICKNAME_MAX_LENGTH + 1];
	char geEditPassword [PASSWORD_MAX_LENGTH + 1];
	char geEditGroup [GROUP_LENGTH + 1];
	char geEditName [NAME_MAX_LENGTH + 1];
	char coddedPassword[256];
	geUserDataType userLoggedIn;
	geUserDataType userEditing;
	char idString[21];
	char cookieValue[COOKIE_LENGTH + 1], cookieValueNew[COOKIE_LENGTH + 1];
	FILE *reading, *writing;
	char linha[ ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 7 ];
	unsigned counter, counter2;
	unsigned willEditEmail, willEditPass, willEditGroup, willEditName, somethingEdited;
	unsigned firstLine = 1;
	willEditEmail = 2;
	willEditPass = 2;
	willEditGroup = 2;
	willEditName = 2;
	somethingEdited = 0;
	
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
	
	if ( mlCgiGetFormStringNoNewLines ( "previousEmail", geEditPreviousNickname, NICKNAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o email atual do usu&aacute;rio.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Couldn't retrieve user current email.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Sail back to main menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	willBeEdited[3] = '\0';
	CheckUserInfo ( geEditPreviousNickname, &userEditing );
	
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
	
	if ( mlCgiGetFormStringNoNewLines ( "geWillEditEmail", willBeEdited, 3 ) != ML_CGI_OK )
	{
		willEditEmail = 0;
	}
	
	if ( strcmp ( willBeEdited, "Yes" ) == 0 )
	{
		if ( mlCgiGetFormStringNoNewLines ( "geEditNickname", geEditNickname, NICKNAME_MAX_LENGTH ) != ML_CGI_OK )
		{
			if ( language == portuguese ) 
			{
				mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o email de edi&ccedil;&atilde;o.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			else
			{
				mlCgiShowErrorPage ( "Error", "Couldn't retrieve editing email.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Sail back to main menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
		}
		willEditEmail = 1;
		somethingEdited = 1;
		
		if ( isUserRegistered ( geEditNickname ) != 0 )
		{
			if ( language == portuguese ) 
			{
				mlCgiShowErrorPage ( "Error", "Email j&aacute; utilizado.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar para a p&aacute;gina anterior.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			else
			{
				mlCgiShowErrorPage ( "Error", "Email in use.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Go back.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
	}
		
	if ( mlCgiGetFormStringNoNewLines ( "geWillEditPassword", willBeEdited, 3 ) != ML_CGI_OK )
	{
		willEditPass = 0;
	}
	
	if ( strcmp ( willBeEdited, "Yes" ) == 0 )
	{
		if ( mlCgiGetFormStringNoNewLines ( "geEditPassword", geEditPassword, PASSWORD_MAX_LENGTH ) != ML_CGI_OK )
		{
			if ( language == portuguese ) 
			{
				mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver a senha de edi&ccedil;&atilde;o.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			else
			{
				mlCgiShowErrorPage ( "Error", "Couldn't retrieve editing password.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Sail back to main menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
		}
		willEditPass = 1;
		somethingEdited = 1;
		if ( CodificarSenha ( geEditPassword, coddedPassword ) != 0 )
		{
			if ( language == portuguese ) 
			{
				mlCgiShowErrorPage ( "Error", "Senha inv&aacute;lida.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			else
			{
				mlCgiShowErrorPage ( "Error", "Invalid Password.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Sail back to main menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
		}
	}	
	
	if ( mlCgiGetFormStringNoNewLines ( "geWillEditGroup", willBeEdited, 3 ) != ML_CGI_OK )
	{
		willEditGroup = 0;
	}
	
	if ( strcmp ( willBeEdited, "Yes" ) == 0 )
	{
		if ( mlCgiGetFormStringNoNewLines ( "geEditGroup", geEditGroup, GROUP_LENGTH ) != ML_CGI_OK )
		{
			if ( language == portuguese ) 
			{
				mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o grupo de edi&ccedil;&atilde;o.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			else
			{
				mlCgiShowErrorPage ( "Error", "Couldn't retrieve editing group.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Sail back to main menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
		}
		willEditGroup = 1;
		somethingEdited = 1;
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "geWillEditName", willBeEdited, 3 ) != ML_CGI_OK )
	{
		willEditName = 0;
	}
	
	if ( strcmp ( willBeEdited, "Yes" ) == 0 )
	{
		if ( mlCgiGetFormStringNoNewLines ( "geEditRealName", geEditName, NAME_MAX_LENGTH ) != ML_CGI_OK )
		{
			if ( language == portuguese ) 
			{
				mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o nome de edi&ccedil;&atilde;o.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
			else
			{
				mlCgiShowErrorPage ( "Error", "Couldn't retrieve editing Name.", ""  );
				printf ( "<div align=\"center\">" );
				createButton ( "Sail back to main menu.", "geShowMenu.cgi", geLanguage, geNickname, "18" );
				printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
				printf ( "</div>" );
			}
		}
		willEditName = 1;
		somethingEdited = 1;
	}
	
	/*---------------------------------*/
	
	if ( language == portuguese )
	{
		printf("    <title>	GelaDOS - Cervejarias (Logged)</title>\n");
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n" );
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias</font></div> \n" );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\"><br>\n" );
		
		printf("    Editar Usu&aacute;rios: <br>\n" );
		printf ("  <input type='hidden' name='geLanguage' value='%s'>\n", geLanguage );
		printf ("  <input type='hidden' name='nickname' value='%s'>\n", geNickname );
		
		printf ("  <p id=\"hiddenInputCookie\"></p> \n" );
		printf ("  <script> \n");
		printf ("  document.getElementById(\"hiddenInputCookie\").innerHTML = checkCookie(); \n" );
		printf ("  </script> \n");

		if ( userLoggedIn.group == 1 )
		{
			if ( somethingEdited == 0 )
			{
				printf ( "Nada solicitado para ser editado. Nada foi feito. <br>\n" );
				createButton ( "Voltar para a p&aacute;gina de sele&ccedil;&atilde;o.", "geChangeBrotherhoodName.cgi", geLanguage, geNickname, "18" );
				createButton ( "Voltar para a p&aacute;gina de menu.", "geShowMenu.cgi", geLanguage, geNickname, "19" );
				printf("      <a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a><br>\n", idString );	
				fclose ( reading );
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
			
			reading = fopen ( "../Files/users", "r" );
			writing = fopen ( "../Files/users.e", "w" );
			
			while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading ) != NULL )
			{
				if ( linha[strlen(linha) - 1] == '\n' )
					linha[strlen(linha) - 1] = '\0';
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
				if ( strcmp ( geEditPreviousNickname, &linha[counter + 1] ) == 0 )
				{
					if ( firstLine == 1 )
					{
						fprintf ( writing, "%s;", linha );
						firstLine = 0;
					}
					else
						fprintf ( writing, "\n%s;", linha );
					if ( willEditEmail == 1 )
						fprintf ( writing, "%s;", geEditNickname );						
					else
						fprintf ( writing, "%s;", userEditing.email );
					if ( willEditPass == 1 )
						fprintf ( writing, "%s;", coddedPassword );
					else
						fprintf ( writing, "%s;", userEditing.password );
					if ( willEditGroup == 1 )
						fprintf ( writing, "%s;", geEditGroup );
					else
						fprintf ( writing, "%c;", userEditing.group + '0' );
					if ( willEditName == 1 )
						fprintf ( writing, "%s;", geEditName );
					else
						fprintf ( writing, "%s;", userEditing.name );
				}
				else
				{
					linha[counter] = ';';
					linha[counter2] = ';';
					if ( firstLine == 0 )
						fprintf ( writing, "\n%s", linha );
					else
					{
						fprintf ( writing, "%s", linha );
						firstLine = 0;
					}
				}
			}
			
			fclose ( reading );
			fclose ( writing );
			remove ( "../Files/users" );
			rename ( "../Files/users.e", "../Files/users" );
			
			printf ( " Executado com sucesso! <br><b>Informa&ccedil;&otilde;es Antigas</b>: <br>\n " );
			printf ( " <b>Email</b>: %s <br> <b>Senha</b>: ------- <br> <b>Grupo</b>: %c <br> <b>Nome Real</b>: %s <br>\n", userEditing.email, userEditing.group + '0',userEditing.name );
			printf ( " <br><b>Novas Informa&ccedil;&otilde;es</b>: <br>\n" );
			if ( willEditEmail == 1 )
				printf ( "<b>Email</b>: %s<br>\n", geEditNickname );
			if ( willEditPass == 1 )
				printf ( "<b>Senha</b>: ------- <br>\n" );
			if ( willEditGroup == 1 )
				printf ( "<b>Group</b>: %s <br>\n", geEditGroup );
			if ( willEditName == 1 )
				printf ( "<b>Nome Real</b>: %s <br>\n", geEditName );
		}
		
		if ( userLoggedIn.group == 2 )
		{
		}
		
		if ( userLoggedIn.group == 4 )
		{
		}
		
		createButton ( "Voltar para a p&aacute;gina de edi&ccedil;&atilde;o.", "geEditUser.cgi", geLanguage, geNickname, "722" );
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
		
		printf("    Edit Users: <br>\n" );
		printf ("  <input type='hidden' name='geLanguage' value='%s'>\n", geLanguage );
		printf ("  <input type='hidden' name='nickname' value='%s'>\n", geNickname );
		
		printf ("  <p id=\"hiddenInputCookie\"></p> \n" );
		printf ("  <script> \n");
		printf ("  document.getElementById(\"hiddenInputCookie\").innerHTML = checkCookie(); \n" );
		printf ("  </script> \n");

		if ( userLoggedIn.group == 1 )
		{
			if ( somethingEdited == 0 )
			{
				printf ( "Nothing requested. The system didn't do a thing. <br>\n" );
				createButton ( "Go back to the Selection Page.", "geChangeBrotherhoodName.cgi", geLanguage, geNickname, "18" );
				createButton ( "Go back to the Main Menu.", "geShowMenu.cgi", geLanguage, geNickname, "19" );
				printf("      <a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a><br>\n", idString );	
				fclose ( reading );
				mlCgiFreeResources ( );
				exit ( ML_CGI_OK );
			}
			
			reading = fopen ( "../Files/users", "r" );
			writing = fopen ( "../Files/users.e", "w" );
			
			while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading ) != NULL )
			{
				if ( linha[strlen(linha) - 1] == '\n' )
					linha[strlen(linha) - 1] = '\0';
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
				if ( strcmp ( geEditPreviousNickname, &linha[counter + 1] ) == 0 )
				{
					if ( firstLine == 1 )
					{
						fprintf ( writing, "%s;", linha );
						firstLine = 0;
					}
					else
						fprintf ( writing, "\n%s;", linha );
					if ( willEditEmail == 1 )
						fprintf ( writing, "%s;", geEditNickname );						
					else
						fprintf ( writing, "%s;", userEditing.email );
					if ( willEditPass == 1 )
						fprintf ( writing, "%s;", coddedPassword );
					else
						fprintf ( writing, "%s;", userEditing.password );
					if ( willEditGroup == 1 )
						fprintf ( writing, "%s;", geEditGroup );
					else
						fprintf ( writing, "%c;", userEditing.group + '0' );
					if ( willEditName == 1 )
						fprintf ( writing, "%s;", geEditName );
					else
						fprintf ( writing, "%s;", userEditing.name );
				}
				else
				{
					linha[counter] = ';';
					linha[counter2] = ';';
					if ( firstLine == 0 )
						fprintf ( writing, "\n%s", linha );
					else
					{
						fprintf ( writing, "%s", linha );
						firstLine = 0;
					}
				}
			}
			
			fclose ( reading );
			fclose ( writing );
			remove ( "../Files/users" );
			rename ( "../Files/users.e", "../Files/users" );
			
			printf ( " Success! <br><b>Old Informations</b>: <br>\n " );
			printf ( " <b>Email</b>: %s <br> <b>Password</b>: ------- <br> <b>Group</b>: %c <br> <b>Real Name</b>: %s <br>\n", userEditing.email, userEditing.group + '0',userEditing.name );
			printf ( " <br><b>New Informations</b>: <br>\n" );
			if ( willEditEmail == 1 )
				printf ( "<b>Email</b>: %s<br>\n", geEditNickname );
			if ( willEditPass == 1 )
				printf ( "<b>Senha</b>: ------- <br>\n" );
			if ( willEditGroup == 1 )
				printf ( "<b>Group</b>: %s <br>\n", geEditGroup );
			if ( willEditName == 1 )
				printf ( "<b>Nome</b>: %s <br>\n", geEditName );
		}
		
		if ( userLoggedIn.group == 2 )
		{
		}
		
		if ( userLoggedIn.group == 4 )
		{
		}
		
		createButton ( "Go back to the Edit Page.", "geEditUser.cgi", geLanguage, geNickname, "722" );
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
		
/* $RCSfile: geCgiExecuteEditUser.c,v $ */
