/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/11 03:16:29 $
 * $Log: geCgiShowMenu.c,v $
 * Revision 1.3  2015/01/11 03:16:29  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.1  2015/01/10 19:16:11  vitor.schoola
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
	char idString[21];
	char cookieValue[COOKIE_LENGTH + 1], cookieValueNew[COOKIE_LENGTH + 1];
	
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
		printf("    Seja bem-vindo, %s! <br>\n", userLoggedIn.name );
		if ( userLoggedIn.group == 1 )
		{
			printf ( "Cervejas: <br>\n" );
			createButton ( "Adicionar Cerveja", "geAddBeer.cgi", geLanguage, geNickname, "0" );
			createButton ( "Avaliar Cerveja", "geEvaluateBeer.cgi", geLanguage, geNickname, "1" );
			createButton ( "Alterar nome de Cerveja", "geChangeBeerName.cgi", geLanguage, geNickname, "2" );
			createButton ( "Remover Cerveja", "geRemoveBeer.cgi", geLanguage, geNickname, "3" );
			createButton ( "Listar Cervejas", "geListBeer.cgi", geLanguage, geNickname, "4" );
			createButton ( "Procurar Cerveja", "geSearchBeer.cgi", geLanguage, geNickname, "5" );	
			
			printf ( "Tipos de Cervejas: <br>\n" );
			createButton ( "Adicionar Tipo de Cerveja", "geAddBeerType.cgi", geLanguage, geNickname, "6" );
			createButton ( "Alterar nome de Tipo de Cerveja", "geChangeBeerTypeName.cgi", geLanguage, geNickname, "7" );
			createButton ( "Remover Tipo de Cerveja", "geRemoveBeerType.cgi", geLanguage, geNickname, "8" );
			createButton ( "Listar Tipos de Cervejas", "geListBeerType.cgi", geLanguage, geNickname, "9" );
			createButton ( "Procurar Tipos de Cerveja", "geSearchBeerType.cgi", geLanguage, geNickname, "10" );
			createButton ( "Adicionar Cerveja ao Tipo", "geAddBeerToType.cgi", geLanguage, geNickname, "27" );
			
			printf ( "Confrarias: <br>\n" );
			createButton ( "Adicionar Confraria", "geAddBrotherhood.cgi", geLanguage, geNickname, "11" );
			createButton ( "Alterar nome de Confraria", "geChangeBrotherhoodName.cgi", geLanguage, geNickname, "12" );
			createButton ( "Remover Confraria", "geRemoveBrotherhood.cgi", geLanguage, geNickname, "13" );
			createButton ( "Adicionar/Convidar usu&aacute;rio a uma Confraria", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "14" );
			createButton ( "Pend&ecirc;ncias Confrarias", "geAcceptRejectBrotherhood.cgi", geLanguage, geNickname, "16" );
			createButton ( "Remover usu&aacute;rio de Confraria.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "17" );
			
			printf ( "Controle de Usu&aacute;rios:<br>\n" );
			createButton ( "Adicionar/Convidar Usu&aacute;rios", "geAddUser.cgi", geLanguage, geNickname, "18" );
			createButton ( "Remover Usu&aacute;rios", "geRemoveUser.cgi", geLanguage, geNickname, "21" );
			createButton ( "Editar Usu&aacute;rios", "geEditUser.cgi", geLanguage, geNickname, "22" );
			createButton ( "Criar senha tempor&aacute;ria para Usu&aacute;rios", "geCreateTempPassUser.cgi", geLanguage, geNickname, "23" );
			createButton ( "Listar Usu&aacute;rios", "geListUser.cgi", geLanguage, geNickname, "24" );	
			createButton ( "Procurar Usu&aacute;rio", "geSearchUser.cgi", geLanguage, geNickname, "25" );	
			createButton ( "Mudar senha", "geChangePassword.cgi", geLanguage, geNickname, "26" );	
		}
		if ( userLoggedIn.group == 2 )
		{
			printf ( "Cervejas: <br>\n" );
			createButton ( "Adicionar Cerveja", "geAddBeer.cgi", geLanguage, geNickname, "0" );
			createButton ( "Avaliar Cerveja", "geEvaluateBeer.cgi", geLanguage, geNickname, "1" );
			createButton ( "Listar Cervejas", "geListBeer.cgi", geLanguage, geNickname, "4" );
			createButton ( "Procurar Cerveja", "geSearchBeer.cgi", geLanguage, geNickname, "5" );	
			
			printf ( "Tipos Cervejas: <br>\n" );
			createButton ( "Adicionar Tipo de Cerveja", "geAddBeerType.cgi", geLanguage, geNickname, "6" );
			createButton ( "Listar Tipos de Cervejas", "geListBeerType.cgi", geLanguage, geNickname, "9" );
			createButton ( "Procurar Tipos de Cerveja", "geSearchBeerType.cgi", geLanguage, geNickname, "10" );
			createButton ( "Adicionar Cerveja ao Tipo", "geAddBeerToType.cgi", geLanguage, geNickname, "27" );
			
			printf ( "Confrarias: <br>\n" );
			createButton ( "Adicionar Confraria", "geAddBrotherhood.cgi", geLanguage, geNickname, "11" );
			createButton ( "Convidar usu&aacute;rio a uma Confraria", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "15" );
			createButton ( "Pend&ecirc;ncias Confrarias", "geAcceptRejectBrotherhood.cgi", geLanguage, geNickname, "16" );
			createButton ( "Remover usu&aacute;rio de Confraria.", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "17" );
			
			printf ( "Controle de Usu&aacute;rios:<br>\n" );
			createButton ( "Convidar Usu&aacute;rios", "geAddUser.cgi", geLanguage, geNickname, "19" );
			createButton ( "Mudar senha", "geChangePassword.cgi", geLanguage, geNickname, "26" );			
		}
		if ( userLoggedIn.group == 4 )
		{
			printf ( "Cervejas: <br>\n" );
			createButton ( "Avaliar Cerveja", "geEvaluateBeer.cgi", geLanguage, geNickname, "1" );
			createButton ( "Listar Cervejas", "geListBeer.cgi", geLanguage, geNickname, "4" );
			createButton ( "Procurar Cerveja", "geSearchBeer.cgi", geLanguage, geNickname, "5" );	
			
			printf ( "Tipos Cervejas: <br>\n" );
			createButton ( "Listar Tipos de Cervejas", "geListBeerType.cgi", geLanguage, geNickname, "9" );
			createButton ( "Procurar Tipos de Cerveja", "geSearchBeerType.cgi", geLanguage, geNickname, "10" );
			
			printf ( "Confrarias: <br>\n" );
			createButton ( "Pend&ecirc;ncias Confrarias", "geAcceptRejectBrotherhood.cgi", geLanguage, geNickname, "16" );
			
			printf ( "Controle de Usu&aacute;rios:<br>\n" );
			createButton ( "Convidar Usu&aacute;rios", "geAddUser.cgi", geLanguage, geNickname, "19" );
			createButton ( "Mudar senha", "geChangePassword.cgi", geLanguage, geNickname, "26" );
		}
		
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
		
		printf("    Welcome, %s! <br>\n", userLoggedIn.name );
		if ( userLoggedIn.group == 1 )
		{
			printf ( "Beers: <br>\n" );
			createButton ( "Add Beer", "geAddBeer.cgi", geLanguage, geNickname, "0" );
			createButton ( "Evaluate Beer", "geEvaluateBeer.cgi", geLanguage, geNickname, "1" );
			createButton ( "Change Beer's Name", "geChangeBeerName.cgi", geLanguage, geNickname, "2" );
			createButton ( "Remove Beer", "geRemoveBeer.cgi", geLanguage, geNickname, "3" );
			createButton ( "List Beers", "geListBeer.cgi", geLanguage, geNickname, "4" );
			createButton ( "Search Beer", "geSearchBeer.cgi", geLanguage, geNickname, "5" );	
			
			printf ( "Beer Types: <br>\n" );
			createButton ( "Add Beer Type", "geAddBeerType.cgi", geLanguage, geNickname, "6" );
			createButton ( "Change Beer Type's Name", "geChangeBeerTypeName.cgi", geLanguage, geNickname, "7" );
			createButton ( "Remove Beer Type", "geRemoveBeerType.cgi", geLanguage, geNickname, "8" );
			createButton ( "List Beer Types", "geListBeerType.cgi", geLanguage, geNickname, "9" );
			createButton ( "Search Beer Type", "geSearchBeerType.cgi", geLanguage, geNickname, "10" );
			createButton ( "Add Beer to a Type", "geAddBeerToType.cgi", geLanguage, geNickname, "27" );
			
			printf ( "Brotherhoods: <br>\n" );
			createButton ( "Add Brotherhood", "geAddBrotherhood.cgi", geLanguage, geNickname, "11" );
			createButton ( "Change Brotherhood's Name", "geChangeBrotherhoodName.cgi", geLanguage, geNickname, "12" );
			createButton ( "Remove Brotherhood", "geRemoveBrotherhood.cgi", geLanguage, geNickname, "13" );
			createButton ( "Add/Invite User to Brotherhood", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "14" );
			createButton ( "Brotherhood's Pendencies", "geAcceptRejectBrotherhood.cgi", geLanguage, geNickname, "16" );
			createButton ( "Remove User From Brotherhood", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "17" );
			
			printf ( "Users:<br>\n" );
			createButton ( "Add/Invite Users", "geAddUser.cgi", geLanguage, geNickname, "18" );
			createButton ( "Remove Users", "geRemoveUser.cgi", geLanguage, geNickname, "21" );
			createButton ( "Edit Users", "geEditUser.cgi", geLanguage, geNickname, "22" );
			createButton ( "Create Temporary Password for User", "geCreateTempPassUser.cgi", geLanguage, geNickname, "23" );
			createButton ( "List Users", "geListUser.cgi", geLanguage, geNickname, "24" );	
			createButton ( "Search User", "geSearchUser.cgi", geLanguage, geNickname, "25" );	
			createButton ( "Change your Password", "geChangePassword.cgi", geLanguage, geNickname, "26" );	
		}
		if ( userLoggedIn.group == 2 )
		{
			printf ( "Beers: <br>\n" );
			createButton ( "Add Beer", "geAddBeer.cgi", geLanguage, geNickname, "0" );
			createButton ( "Evaluate Beer", "geEvaluateBeer.cgi", geLanguage, geNickname, "1" );
			createButton ( "List Beers", "geListBeer.cgi", geLanguage, geNickname, "4" );
			createButton ( "Search Beer", "geSearchBeer.cgi", geLanguage, geNickname, "5" );	
			
			printf ( "Beer Types: <br>\n" );
			createButton ( "Add Beer Type", "geAddBeerType.cgi", geLanguage, geNickname, "6" );
			createButton ( "List Beer Types", "geListBeerType.cgi", geLanguage, geNickname, "9" );
			createButton ( "Search Beer Type", "geSearchBeerType.cgi", geLanguage, geNickname, "10" );
			createButton ( "Add Beer to a Type", "geAddBeerToType.cgi", geLanguage, geNickname, "27" );
			
			printf ( "Brotherhoods: <br>\n" );
			createButton ( "Add Brotherhood", "geAddBrotherhood.cgi", geLanguage, geNickname, "11" );
			createButton ( "Invite User to Brotherhood", "geAddUserToBrotherhood.cgi", geLanguage, geNickname, "14" );
			createButton ( "Brotherhood's Pendencies", "geAcceptRejectBrotherhood.cgi", geLanguage, geNickname, "16" );
			createButton ( "Remove User From Brotherhood", "geRemoveUserFromBrotherhood.cgi", geLanguage, geNickname, "17" );
			
			printf ( "Users:<br>\n" );
			createButton ( "Invite Users", "geAddUser.cgi", geLanguage, geNickname, "18" );
			createButton ( "Change your Password", "geChangePassword.cgi", geLanguage, geNickname, "26" );	
		}
		if ( userLoggedIn.group == 4 )
		{
			printf ( "Beers: <br>\n" );
			createButton ( "Evaluate Beer", "geEvaluateBeer.cgi", geLanguage, geNickname, "1" );
			createButton ( "List Beers", "geListBeer.cgi", geLanguage, geNickname, "4" );
			createButton ( "Search Beer", "geSearchBeer.cgi", geLanguage, geNickname, "5" );	
			
			printf ( "Beer Types: <br>\n" );
			createButton ( "List Beer Types", "geListBeerType.cgi", geLanguage, geNickname, "9" );
			createButton ( "Search Beer Type", "geSearchBeerType.cgi", geLanguage, geNickname, "10" );
			
			printf ( "Brotherhoods: <br>\n" );
			createButton ( "Brotherhood's Pendencies", "geAcceptRejectBrotherhood.cgi", geLanguage, geNickname, "16" );
			
			printf ( "Users:<br>\n" );
			createButton ( "Invite Users", "geAddUser.cgi", geLanguage, geNickname, "18" );
			createButton ( "Change your Password", "geChangePassword.cgi", geLanguage, geNickname, "26" );	
		}
		
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
		
/* $RCSfile: geCgiShowMenu.c,v $ */
