/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:09:19 $
 * $Log: geCgiChosenEditUser.c,v $
 * Revision 1.1  2015/01/10 19:09:19  vitor.schoola
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
	char geLanguage [LANGUAGE_MAX_LENGTH + 1];
	char geNickname [NICKNAME_MAX_LENGTH + 1];
	geUserDataType userLoggedIn;
	char idString[21];
	char cookieValue[COOKIE_LENGTH + 1], cookieValueNew[COOKIE_LENGTH + 1];
	char editUserName [NICKNAME_MAX_LENGTH + 1];
	geUserDataType editedUser;
	
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
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver seu login.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
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
	
	printf("  <style> \n" );
	printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
	printf("  </style> \n" );
	
	if ( mlCgiGetFormStringNoNewLines ( "editUserName", editUserName, NICKNAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos o email para editar.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Voltar para a p&aacute;gina anterior.", "geEditUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=portuguese&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Couldn't retrieve email to edit.", ""  );
			printf ( "<div align=\"center\">" );
			createButton ( "Go back.", "geEditUser.cgi", geLanguage, geNickname, "18" );
			printf ( "<br><a href=\"geLogout.cgi?geLanguage=english&idString=%s\">Logout</a>", idString );
			printf ( "</div>" );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	CheckUserInfo ( editUserName, &editedUser );
	
	if ( language == portuguese )
	{
		printf("    <title>	GelaDOS - Cervejarias (Logged)</title>\n");
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n" );
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias</font></div> \n" );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\"><br>\n" );
		
		printf("    Editar Usu&aacute;rio %s: <br>\n", editUserName );
		printf ("<form action=\"geExecuteEditUser.cgi\" method=\"post\"> \n" );
		printf ("  <input type='hidden' name='geLanguage' value='%s'>\n", geLanguage );
		printf ("  <input type='hidden' name='nickname' value='%s'>\n", geNickname );
		
		printf ("  <p id=\"hiddenInputCookie\"></p> \n" );
		printf ("  <script> \n");
		printf ("  document.getElementById(\"hiddenInputCookie\").innerHTML = checkCookie(); \n" );
		printf ("  </script> \n");

		if ( userLoggedIn.group == 1 )
		{
			printf (" <input type='hidden' name='previousEmail' value='%s' >\n", editedUser.email );
			printf (" Email: \n" );
			printf (" Ser&aacute; editado?" );
			printf (" <input type='checkbox' name='geWillEditEmail' value='Yes'><br> " );
			printf (" <input type='text' name='geEditNickname' value='%s' maxlength='%u'><br>\n", editedUser.email, NICKNAME_MAX_LENGTH );
			
			printf (" Senha: \n" );
			printf (" Ser&aacute; editada?" );
			printf (" <input type='checkbox' name='geWillEditPassword' value='Yes'><br> " );
			printf (" <input type='password' name='geEditPassword' value='' maxlength='%u'><br>\n", PASSWORD_MAX_LENGTH );
			
			printf (" Grupo: \n" );
			printf (" Ser&aacute; editado?" );
			printf (" <input type='checkbox' name='geWillEditGroup' value='Yes'><br> " );
			printf (" <input type='text' name='geEditGroup' value='%c' maxlength='%u'><br>\n", editedUser.group + '0', GROUP_LENGTH );
			
			printf (" Nome: \n" );
			printf (" Ser&aacute; editado?" );
			printf (" <input type='checkbox' name='geWillEditName' value='Yes'><br> " );
			printf (" <input type='text' name='geEditRealName' value='%s' maxlength='%u'><br>\n", editedUser.name, NAME_MAX_LENGTH );

			printf ("  <input type=\"submit\" name=\"submit\" value=\"Editar\"><br> \n" );
		}
		
		if ( userLoggedIn.group == 2 )
		{
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
		
		printf("    Editing User: %s. <br>\n", editUserName );
		printf ("<form action=\"geExecuteEditUser.cgi\" method=\"post\"> \n" );
		printf ("  <input type='hidden' name='geLanguage' value='%s'>\n", geLanguage );
		printf ("  <input type='hidden' name='nickname' value='%s'>\n", geNickname );
		
		printf ("  <p id=\"hiddenInputCookie\"></p> \n" );
		printf ("  <script> \n");
		printf ("  document.getElementById(\"hiddenInputCookie\").innerHTML = checkCookie(); \n" );
		printf ("  </script> \n");

		if ( userLoggedIn.group == 1 )
		{
			printf (" <input type='hidden' name='previousEmail' value='%s' >\n", editedUser.email );
			printf (" Email: \n" );
			printf (" Will it be edited?" );
			printf (" <input type='checkbox' name='geWillEditEmail' value='Yes'><br> " );
			printf (" <input type='text' name='geEditNickname' value='%s' maxlength='%u'><br>\n", editedUser.email, NICKNAME_MAX_LENGTH );
			
			printf (" Password: \n" );
			printf (" Will it be edited?" );
			printf (" <input type='checkbox' name='geWillEditPassword' value='Yes'><br> " );
			printf (" <input type='password' name='geEditPassword' value='' maxlength='%u'><br>\n", PASSWORD_MAX_LENGTH );
			
			printf (" Group: \n" );
			printf (" Will it be edited?" );
			printf (" <input type='checkbox' name='geWillEditGroup' value='Yes'><br> " );
			printf (" <input type='text' name='geEditGroup' value='%c' maxlength='%u'><br>\n", editedUser.group + '0', GROUP_LENGTH );
			
			printf (" Real Name: \n" );
			printf (" Will it be edited?" );
			printf (" <input type='checkbox' name='geWillEditName' value='Yes'><br> " );
			printf (" <input type='text' name='geEditRealName' value='%s' maxlength='%u'><br>\n", editedUser.name, NAME_MAX_LENGTH );

			printf ("  <input type=\"submit\" name=\"submit\" value=\"Edit\"><br> \n" );
		}
		
		if ( userLoggedIn.group == 2 )
		{
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
		
/* $RCSfile: geCgiChosenEditUser.c,v $ */
