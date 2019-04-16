/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:07:51 $
 * $Log: geCgiAddUser.c,v $
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
	unsigned counter;
	char geLanguage [LANGUAGE_MAX_LENGTH];
	char geNickname [NICKNAME_MAX_LENGTH];
	char ipString [IP_MAX_LENGTH], ipStringNew[IP_MAX_LENGTH];
	char idString[21];
	char idStringD[22];
	char cookieValue[COOKIE_LENGTH + 1], cookieValueNew[COOKIE_LENGTH + 1];
	char fullPath [strlen("../Files/Cookies/D") + strlen(idString) + 2];
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
		
		printf("    Adicionar/Convidar Usu&aacute;rio: <br>\n" );
		printf ("<form action=\"geExecuteAddUser.cgi\" method=\"post\"> \n" );
		printf ("  <input type='hidden' name='geLanguage' value='%s'>\n", geLanguage );
		printf ("  <input type='hidden' name='nickname' value='%s'>\n", geNickname );
		
		printf ("  <p id=\"hiddenInputCookie\"></p> \n" );
		printf ("  <script> \n");
		printf ("  document.getElementById(\"hiddenInputCookie\").innerHTML = checkCookie(); \n" );
		printf ("  </script> \n");

		if ( userLoggedIn.group == 1 )
		{
			printf (" Email:<br> \n" );
			printf ("  <input type='text' name='geEmail' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Confirma&ccedil;&atilde;o de Email:<br> \n" );
			printf (" <input type='text' name='geEmailConfirmation' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Senha: ( Deixe em branco para ser um convite )<br> \n" );
			printf (" <input type='password' name='gePass' maxlength='%u'><br>\n", PASSWORD_MAX_LENGTH );
			printf (" Confirma&ccedil;&atilde;o da Senha:<br> \n" );
			printf (" <input type='password' name='gePassConfirmation' maxlength='%u'><br>\n", PASSWORD_MAX_LENGTH );
			printf (" Nome real:<br> \n" );
			printf (" <input type='text' name='geRealName' maxlength='%u'><br>\n", NAME_MAX_LENGTH );
			printf (" Grupo: [ Capit&atilde;o(1) ] [ Pirata Experiente(2) ] [ Recruta(4) ]<br> \n" );
			printf (" <input type='text' name='geGroup' maxlength='1'><br>\n" );
			
			printf ("  <input type=\"submit\" name=\"submit\" value=\"Adicionar/Convidar\"><br> \n" );
		}
		if ( userLoggedIn.group == 2 )
		{
			printf (" Email:<br> \n" );
			printf ("  <input type='text' name='geEmail' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Confirma&ccedil;&atilde;o de Email:<br> \n" );
			printf (" <input type='text' name='geEmailConfirmation' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Nome real do novo usu&aacute;rio:<br> \n" );
			printf (" <input type='text' name='geRealName' maxlength='%u'><br>\n", NAME_MAX_LENGTH );
			printf (" Grupo: [ Pirata Experiente(2) ] [ Recruta(4) ]<br> \n" );
			printf (" <input type='number' name='geGroup' maxlength='1' min='1' max='4'><br>\n" );
			
			printf ("  <input type=\"submit\" name=\"submit\" value=\"Convidar\"><br> \n" );
		}
		if ( userLoggedIn.group == 4 )
		{
			printf (" Email:<br> \n" );
			printf ("  <input type='text' name='geEmail' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Confirma&ccedil;&atilde;o de Email:<br> \n" );
			printf (" <input type='text' name='geEmailConfirmation' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Nome real do novo usu&aacute;rio:<br> \n" );
			printf (" <input type='text' name='geRealName' maxlength='%u'><br>\n", NAME_MAX_LENGTH );
			printf (" Grupo do novo usu&aacute;rio: [ Recruta(4) ]<br> \n" );
			printf (" <input type='hidden' name='geGroup' maxlength='1' min='1' max='4' value='4'>\n" );
			
			printf ("  <input type=\"submit\" name=\"submit\" value=\"Convidar\"><br> \n" );
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

		printf("    Add/Invite User: <br>\n" );
		printf ("<form action=\"geExecuteAddUser.cgi\" method=\"post\"> \n" );
		printf ("  <input type='hidden' name='geLanguage' value='%s'>\n", geLanguage );
		printf ("  <input type='hidden' name='nickname' value='%s'>\n", geNickname );
		
		printf ("  <p id=\"hiddenInputCookie\"></p> \n" );
		printf ("  <script> \n");
		printf ("  document.getElementById(\"hiddenInputCookie\").innerHTML = checkCookie(); \n" );
		printf ("  </script> \n");

		if ( userLoggedIn.group == 1 )
		{
			printf (" Email:<br> \n" );
			printf ("  <input type='text' name='geEmail' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Email Confirmation:<br> \n" );
			printf (" <input type='text' name='geEmailConfirmation' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Password: ( Leave it blank to make an invite )<br> \n" );
			printf (" <input type='password' name='gePass' maxlength='%u'><br>\n", PASSWORD_MAX_LENGTH );
			printf (" Password Confirmation:<br> \n" );
			printf (" <input type='password' name='gePassConfirmation' maxlength='%u'><br>\n", PASSWORD_MAX_LENGTH );
			printf (" Real Name:<br> \n" );
			printf (" <input type='text' name='geRealName' maxlength='%u'><br>\n", NAME_MAX_LENGTH );
			printf (" Group: [ Captain(1) ] [ Experient Pirate(2) ] [ Recruit(4) ]<br> \n" );
			printf (" <input type='text' name='geGroup' maxlength='1'><br>\n" );
			
			printf ("  <input type=\"submit\" name=\"submit\" value=\"Add/Invite\"><br> \n" );
		}
		if ( userLoggedIn.group == 2 )
		{
			printf (" Email:<br> \n" );
			printf ("  <input type='text' name='geEmail' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Email Confirmation:<br> \n" );
			printf (" <input type='text' name='geEmailConfirmation' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Real Name:<br> \n" );
			printf (" <input type='text' name='geRealName' maxlength='%u'><br>\n", NAME_MAX_LENGTH );
			printf (" Group: [ Experient Pirate(2) ] [ Recruit(4) ]<br> \n" );
			printf (" <input type='text' name='geGroup' maxlength='1'><br>\n" );
			
			printf ("  <input type=\"submit\" name=\"submit\" value=\"Invite\"><br> \n" );
		}
		if ( userLoggedIn.group == 4 )
		{
			printf (" Email:<br> \n" );
			printf ("  <input type='text' name='geEmail' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Email Confirmation:<br> \n" );
			printf (" <input type='text' name='geEmailConfirmation' maxlength='%u'><br>\n", NICKNAME_MAX_LENGTH );
			printf (" Real Name:<br> \n" );
			printf (" <input type='text' name='geRealName' maxlength='%u'><br>\n", NAME_MAX_LENGTH );
			printf (" Group: [ Recruit(4) ]<br> \n" );
			printf (" <input type='hidden' name='geGroup' maxlength='1' value='4'>\n" );
			
			printf ("  <input type=\"submit\" name=\"submit\" value=\"Invite\"><br> \n" );
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
		
/* $RCSfile: geCgiAddUser.c,v $ */
