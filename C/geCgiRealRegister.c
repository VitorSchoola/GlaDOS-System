/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/11 05:16:55 $
 * $Log: geCgiRealRegister.c,v $
 * Revision 1.3  2015/01/11 05:16:55  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.2  2015/01/11 04:16:13  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.1  2015/01/10 19:14:53  vitor.schoola
 * Initial revision
 *
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "mlcgi.h"
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
	char geLanguage [LANGUAGE_MAX_LENGTH + 1];
	char geNickname [NICKNAME_MAX_LENGTH + 1];
	char gePassword [PASSWORD_MAX_LENGTH + 1];
	char geRealName [NAME_MAX_LENGTH + 1];
	char coddedPassword[256];
	unsigned counter, counter2, counter3, counter4, counter5;
	FILE *writing, *reading;
	char linha[ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 7];
	unsigned foundAbeyance, foundUser;
	unsigned firstLine = 1;
   
	foundAbeyance = 0;
	foundUser = 0;
   
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
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver seu login.", "<a href=\"geShowMainForm?geLanguage.cgi=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Error", "Could not retrieve Nickname", "<a href=\"geShowMainForm.cgi?geLanguage=easteregg\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}

	if ( mlCgiGetFormStringNoNewLines ( "password", gePassword, PASSWORD_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver sua senha", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Error", "Could not retrieve Password", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}

	if ( strcmp ( gePassword, "" ) == 0 )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Error", "Senha vazia.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Error", "Empty password.", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( CodificarSenha ( gePassword, coddedPassword ) != 0 )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Yaaaar!", "Senha inv&aacute;lida.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Yaaaar!", "Invalid password.", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( mlCgiGetFormStringNoNewLines ( "realName", geRealName, NAME_MAX_LENGTH ) != ML_CGI_OK )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Error", "N&atilde;o conseguimos reaver seu nome.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Error", "Could not retrieve name.", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( ( reading = fopen ( "../Files/users.abeyances", "r" ) ) == NULL )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Yaaaar!", "Erro no servidor ao abrir arquivo de usu&aacute;rios pendentes. Tente novamente mais tarde.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Yaaaar!", "Server error. Could not open abeyances file. Try again later.", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}

	if ( ( writing = fopen ( "../Files/users.abeyances.a", "w" ) ) == NULL )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Yaaaar!", "Erro no servidor ao abrir arquivo de usu&aacute;rios pendentes. Tente novamente mais tarde.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Yaaaar!", "Server error. Could not open abeyances file. Try again later.", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading) != NULL )
	{
		if ( linha[strlen(linha) - 1] == '\n' )
			linha[strlen(linha) - 1] = '\0';
		if ( linha[strlen(linha) - 2] == '\n' )
			linha[strlen(linha) - 2] = '\0';
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
		for ( counter3 = counter2 + 1; counter3 <= ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + 1; counter3++ )
			if ( linha[counter3] == ';' )
			{
				linha[counter3] = '\0';
				break;
			}
		if ( strcmp ( geNickname, &linha[counter + 1] ) != 0 )
		{
			linha[counter] = ';';
			linha[counter2] = ';';
			linha[counter3] = ';';
			if ( firstLine == 1 )
			{
				fprintf ( writing, "%s", linha );
				firstLine = 0;
			}
			else
				fprintf ( writing, "\n%s", linha );	
		}
		else
		{
			foundAbeyance = 1;
			if ( strcmp ( &linha[counter2 + 1], "" ) == 0 )
			{
				counter4 = counter3 + 2;
				linha[counter4] = '\0';
				for ( counter5 = counter4 + 1; counter5 <= ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 5; counter5++ )
					if ( linha[counter5] == ';' )
					{
							linha[counter5] = '\0';
							break;
					}
				//fprintf ( writing, "%s;%s;%s;%s;%s;", linha, &linha[counter + 1], coddedPassword, &linha[counter3 + 1], geRealName );
			}
			else
			{
				
			}
		}
	}
	fclose ( writing );
	fclose ( reading );
	remove ( "../Files/users.abeyances" );
	rename ( "../Files/users.abeyances.a", "../Files/users.abeyances" );
	
	if ( foundAbeyance == 0 )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Yaaaar!", "Seu nome n&atilde;o est&aacute; mais no arquivo de convites.<br> Seu convite pode ter expirado ou voc&acirc; j&aacute; se cadastrou.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Yaaaar!", "Your nickname is not in our abeyances file.<br> Your invite may have expired or you've already registered here.", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( ( reading = fopen ( "../Files/users", "r" ) ) == NULL )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Yaaaar!", "Erro no servidor ao abrir arquivo de usu&aacute;rios. Tente novamente mais tarde.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Yaaaar!", "Server error. Could not open users file. Try again later.", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}

	if ( ( writing = fopen ( "../Files/users.a", "w" ) ) == NULL )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Yaaaar!", "Erro no servidor ao abrir arquivo de usu&aacute;rios. Tente novamente mais tarde.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Yaaaar!", "Server error. Could not open users file. Try again later.", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	firstLine = 1;
	while ( fgets ( linha, ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 6, reading) != NULL )
	{
		if ( linha[strlen(linha) - 1] == '\n' )
			linha[strlen(linha) - 1] = '\0';
		if ( linha[strlen(linha) - 2] == '\n' )
			linha[strlen(linha) - 2] = '\0';
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
		for ( counter3 = counter2 + 1; counter3 <= ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + 1; counter3++ )
			if ( linha[counter3] == ';' )
			{
				linha[counter3] = '\0';
				break;
			}
		if ( strcmp ( geNickname, &linha[counter + 1] ) != 0 )
		{
			linha[counter] = ';';
			linha[counter2] = ';';
			linha[counter3] = ';';
			if ( firstLine == 1 )
			{
				fprintf ( writing, "%s", linha );
				firstLine = 0;
			}
			else
				fprintf ( writing, "\n%s", linha );	
			/*printf ( "Linha: |%s|<br>\n", linha );*/
		}
		else
		{
			foundUser = 1;
			if ( strcmp ( &linha[counter2 + 1], "" ) == 0 )
			{
				counter4 = counter3 + 2;
				linha[counter4] = '\0';
				for ( counter5 = counter4 + 1; counter5 <= ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 5; counter5++ )
					if ( linha[counter5] == ';' )
					{
							linha[counter5] = '\0';
							break;
					}
				if ( firstLine == 1 )
				{
					fprintf ( writing, "%s;%s;%s;%s;%s;", linha, &linha[counter + 1], coddedPassword, &linha[counter3 + 1], geRealName );
					firstLine = 0;
				}
				else
					fprintf ( writing, "\n%s;%s;%s;%s;%s;", linha, &linha[counter + 1], coddedPassword, &linha[counter3 + 1], geRealName );
				/*printf ( "Linha:|%s;%s;%s;%s;%s|<br> \n", linha, &linha[counter + 1], coddedPassword, &linha[counter3 + 1], geRealName );*/
			}
			else
			{
				
			}
		}
	}
	fclose ( writing );
	fclose ( reading );
	remove ( "../Files/users" );
	rename ( "../Files/users.a", "../Files/users" );
	
	
	if ( foundUser == 0 )
	{
		if ( language == portuguese ) 
			mlCgiShowErrorPage ( "Yaaaar!", "Seu nome n&atilde;o est&aacute; mais no arquivo de usu&aacute;rios.<br> Seu convite pode ter sido cancelado ou expirou. Contate o administrador em caso de d&uacute;vida.", "<a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar para a p&aacute;gina principal</a>"  );
		else
			mlCgiShowErrorPage ( "Yaaaar!", "Your nickname is not in our users file.<br> Your invite may have been canceled or have expired. If there is a problem, contact the system manager.", "<a href=\"geShowMainForm.cgi?geLanguage=english\">Sail back to the main page</a>"  );
		mlCgiFreeResources ( );
		exit ( ML_CGI_OK );
	}
	
	if ( language == portuguese )
	{
		printf("<html>\n");
		printf("  <head>\n");
		printf("    <title>	GelaDOS - Cervejarias</title>\n");
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n");
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias Aplica&ccedil;&atilde;o - %s</font>\n", geRealName );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("    <img src=\"../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table> " );
		printf("    Yaaaar, marujo, voc&ecirc; agora est&aacute; registrado! <br>\n" );
		printf("    Seu login ser&aacute seu email agora.<br>\n" );
		printf("    N&atilde;o esque&ccedil;a sua senha! Busc&aacute;-la quando cai no mar &eacute; imposs&iacute;vel<br>\n" );
		printf("    Caso ocorra ter&aacute; de desapegar e criar outra.<br>\n" );
		printf("    <br><br><a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Velejar ao menu principal</a>" );
		printf("    </div>\n" );
		printf("    <br><br><br><br><br><br>\n" );
		printf("    <div align=\"center\"><audio id=\"player\" control autoplay>\n");
		printf("      <source src=\"../Files/win.mp3\" type=\"audio/mpeg\">\n" );
		printf("      Infelizmente seu navegador não suporta elementos de áudio.\n" );
		printf("      </audio>\n" );
		printf("  </body> " );
		printf("</html>" );
	}
	else
	{
		printf("<html>\n");
		printf("  <head>\n");
		printf("    <title>GelaDOS - Breweries</title>\n");
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n");
		printf("  <body onload=\"document.getElementById('player').volume -= 0.5\">\n");
		printf("    <br><div align=\"center\"><font size=\"+3\"> Breweries Application - %s</font>\n", geRealName );
		printf("    <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("    <img src=\"../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table> " );
		printf("    Yaaaar, sailor, you are in our system now! <br>\n" );
		printf("    Your login will be your email now.<br>\n" );
		printf("    Do not forget your password! If you drop it in the sea, we'll never find it!<br>\n" );
		printf("    If it happens anyway, you'll have to create another.<br>\n" );
		printf("    <br><br><a href=\"geShowMainForm.cgi?geLanguage=portuguese\">Sail to the main menu</a>" );
		printf("    </div>\n" );
		printf("    <br><br><br><br><br><br>\n" );
		printf("    <div align=\"center\"><audio id=\"player\" control autoplay>\n");
		printf("      <source src=\"../Files/win.mp3\" type=\"audio/mpeg\">\n" );
		printf("      Unfortunatelly your browser does not support audio elements.\n" );
		printf("      </audio>\n" );
		printf("  </body> " );
		printf("</html>" );
	}
	mlCgiFreeResources();
	return ML_CGI_OK;
}
 
/* $RCSfile: geCgiRealRegister.c,v $ */
