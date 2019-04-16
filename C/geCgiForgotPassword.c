/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 21:09:14 $
 * $Log: geCgiForgotPassword.c,v $
 * Revision 1.2  2015/01/10 21:09:14  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.1  2015/01/10 19:13:33  vitor.schoola
 * Initial revision
 *
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "mlcgi.h"
#include "sendmail.h"
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
	geUserDataType userEditing;
	unsigned counter;
	char geLanguage [LANGUAGE_MAX_LENGTH];
	char senhaPlana [PASSWORD_MAX_LENGTH + 1];
	char senhaPlanaBackup [PASSWORD_MAX_LENGTH + 1];
	char coddedPassword[256];
	char geEmail [NICKNAME_MAX_LENGTH];
	char textMessage [777];
	
	for ( counter = 0; counter < 777; counter++ )
		textMessage[counter] = '\0';
   
	if ( mlCgiInitialize ( &environment ) )
		exit ( ML_CGI_OK );
 
	if ( environment == commandLine )
	{
		printf("Incorrect environment. This program was developed for the web.\n" );
		exit( INCORRECT_ENVIRONMENT );
	}
 
	mlCgiBeginHttpHeader ( "text/html" );
		if ( mlCgiGetFormStringNoNewLines ( "geLanguage", geLanguage, LANGUAGE_MAX_LENGTH ) != ML_CGI_OK )
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve language", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>" );
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
		language = HandleLanguage ( geLanguage );	
	mlCgiEndHttpHeader ( );
	
 	printf("<html>\n");
	printf("  <head>\n");
 	printf("  <style> \n" );
	printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
	printf("  </style> \n" );
	
	if ( mlCgiGetFormStringNoNewLines ( "geEmail", geEmail, NICKNAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
		{
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver o email.", "<a href='geShowMainForm.cgi?geLanguage=portuguese'>Voltar para a p&aacute;gina de menu.</a>"  );
		}
		else
		{
			mlCgiShowErrorPage ( "Error", "Couldn't retrieve user's email.", "<a href='geShowMainForm.cgi?geLanguage=english'>Go back to main menu.</a>"  );
		}
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
 
 	/* Create new password */
	
	CriarStringAleatoria ( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 12, senhaPlana );
	for ( counter = 0; counter <=11; counter++ )
	senhaPlanaBackup[counter] = senhaPlana[counter];
	senhaPlanaBackup[counter] = '\0';
 	CodificarSenha ( senhaPlana, coddedPassword );
	
	/*--------------------------------------------*/
	
	/* Send Email */
	
	if ( language == portuguese )
	{
		strcat ( textMessage, "Sua nova senha, segundo pedido, no sistema GelaDOS e: \n" );
		strcat ( textMessage, senhaPlana );
		strcat ( textMessage, "\nClique nesse link para aplicar as mudancas:\n" );
		strcat ( textMessage, "www02.del.ufrj.br/~vitor.schoola/EEL270/GE/CGIs/geExecuteForgotPassword.cgi?geLanguage=portuguese&geNewPass=" );
		strcat ( textMessage, coddedPassword );
		strcat ( textMessage, "&geName=" );
		strcat ( textMessage, geEmail );
		strcat ( textMessage, "\n" );
		strcat ( textMessage, "\nGelaDOS System." );
		
		sendMail ( 	"del.ufrj.br",
				"smtp.del.ufrj.br",
				25,
				"GelaDOS@email.com",
				geEmail,
				NULL,
				NULL,
				"GelaDOS - Sua nova senha",
				textMessage, 
				NULL );
	}
	else
	{
		strcat ( textMessage, "Your new password, as asked, in the system GelaDOS will be the following: \n" );
		strcat ( textMessage, senhaPlana );
		strcat ( textMessage, "\nClick this link to apply change:\n" );
		strcat ( textMessage, "www02.del.ufrj.br/~vitor.schoola/EEL270/GE/CGIs/geExecuteForgotPassword.cgi?geLanguage=english&geNewPass=" );
		strcat ( textMessage, coddedPassword );
		strcat ( textMessage, "&geName=" );
		strcat ( textMessage, geEmail );
		strcat ( textMessage, "\n" );
		strcat ( textMessage, "\nGelaDOS System." );
		
		sendMail ( 	"del.ufrj.br",
				"smtp.del.ufrj.br",
				25,
				"GelaDOS@email.com",
				geEmail,
				NULL,
				NULL,
				"GelaDOS - Password retrieval",
				textMessage, 
				NULL );
	}
	
	/*------------------------------------------*/
	
	CheckUserInfo ( geEmail, &userEditing );
	 
	if ( language == portuguese )
	{
		printf("    <title>	GelaDOS - Cervejarias</title>\n");
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n" );
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias</font></div> \n" );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../Files/logo.png\" alt=\"GlaDOS\"><br>\n" );
		
		printf("    Esqueci minha senha! <br>\n" );
		printf (" <b>Instru&ccedil;&otilde;es</b>: A nova senha foi enviada para o email do<br> \n" );
		printf (" usu&aacute;rio. Preste aten&ccedil;&atilde;o para n&atilde;o esquec&ecirc;-la novamente.<br>\n" );
		
		printf("     <a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Voltar para a p&aacute;gina principal</a><br>\n" );			
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
		
		printf("    I forgot my password! <br>\n" );
		printf (" <b>Instructions</b>: The new password has been sent to the user's email.<br> \n" );
		printf ("  Pay attention so you do not forget it again.<br>\n" );
		
		printf("     <a href=\"geShowMainForm.cgi?geLanguage=english\">Go back to the main page</a><br>\n" );			
		printf("     <a href=\"geHelp.cgi?geLanguage=english\" target=\"_blank\">(?)</a><br>\n" );	
	} 
 
 	printf ("    </td></tr></table> \n" );
	printf("  </body>\n");
	printf("</html>\n");
 
	mlCgiFreeResources();
	return ML_CGI_OK ;
}
 
/* $RCSfile: geCgiForgotPassword.c,v $ */
