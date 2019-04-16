/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:16:11 $
 * $Log: geCgiShowMainForm.c,v $
 * Revision 1.1  2015/01/10 19:16:11  vitor.schoola
 * Initial revision
 *
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
 
#include "mlcgi.h"
#include "geTypes.h"
#include "geConst.h"
#include "geError.h"
#include "geFunctions.h"
#include "time.h"
 
int
main ( int argc, char *argv[] )
{
	environmentType environment;
	
	FILE *infos;
	char geTemp [5];
	char geHumi [5];
	char geFunc [2];
	int read = 0;
	char linha [31];
   
	if ( mlCgiInitialize ( &environment ) )
		exit ( ML_CGI_OK );
 
	if ( environment == commandLine )
	{
		printf("Incorrect environment. This program was developed for the web.\n" );
		exit( INCORRECT_ENVIRONMENT );
	}
 
	mlCgiBeginHttpHeader ( "text/html" );
	
	if ( mlCgiGetFormStringNoNewLines ( "geFunc", geFunc, 1 ) != ML_CGI_OK )
	{
		read = 1;
	}	
	
	if ( read == 0 )
	{
		if ( mlCgiGetFormStringNoNewLines ( "geTemp", geTemp, 4 ) != ML_CGI_OK )
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve temperature", "" );
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}	
		
		if ( mlCgiGetFormStringNoNewLines ( "geHumi", geHumi, 4 ) != ML_CGI_OK )
		{
			mlCgiShowErrorPage ( "Error", "Could not retrieve humidity", "" );
			mlCgiFreeResources ( );
			exit ( ML_CGI_OK );
		}
		
		mlCgiEndHttpHeader ( );
		
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		
		infos = fopen ( "../../Files/Temperatures.txt", "a" );
		fprintf(infos, "%d/%d/%d %d:%d:%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fprintf (infos, "%sC\n%s\%\n", geTemp, geHumi );
		fclose (infos);
		
		printf("<html>\n" );
		printf("  <head>\n" );
		printf("    <title>GelaDOS - Cervejarias - Temperatura</title>\n" );
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n" );
		printf("  <body>\n");
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias</font></div> \n" );
		printf("      <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table><br> \n" );
		printf("      <table align=\"center\"> \n" );
		
		printf("      <tr>\n" );
		printf("        <td>Documento Atualizado. </td>\n" );
		printf("      </tr>\n" );
		
		printf("      <tr><td></td>\n" );
		printf("      </tr>\n" );
		printf("    </table>\n" );
		printf("    <br><br><br><br><br><br>\n" );
		printf("  </body>\n");
		printf("</html>\n");
		
		mlCgiFreeResources();
		exit (ML_CGI_OK) ;
	}
	
	mlCgiEndHttpHeader ( );
	
		printf("<html>\n" );
		printf("  <head>\n" );
		printf("    <title>GelaDOS - Cervejarias - Temperatura</title>\n" );
		printf("  <style> \n" );
		printf("  html,body { background: url(\"../../Files/background.jpg\") no-repeat center center fixed; -webkit-background-size: cover;-moz-background-size: cover; -o-background-size: cover;background-size: cover; } \n" );
		printf("  </style> \n" );
		printf("  </head>\n" );
		printf("  <body>\n");
		printf("    <br><div align=\"center\"><font size=\"+3\"> Cervejarias</font></div> \n" );
		printf("      <table align=\"center\"> \n" );
		printf("    <tr><td> \n" );
		printf("      <img src=\"../../Files/logo.png\" alt=\"GlaDOS\">\n" );
		printf("    </td></tr></table><br> \n" );
		printf("      <table align=\"center\"> \n" );
		
		if (!fileExists ( "../../Files/", "Temperatures.txt" )){
			printf("      <tr>\n" );
			printf("        <td>File does not exist.</td>\n" );
			printf("      </tr>\n" );
		}
		else{		
			infos = fopen ( "../../Files/Temperatures.txt", "r" );
			while (fgets (linha, 30, infos) != NULL){
				printf("      <tr>\n" );
				printf("        <td>Data: %s</td>\n", linha );
				printf("      </tr>\n" );
				fgets ( linha, 30, infos);
				printf("      <tr>\n" );
				printf("        <td>Temperatura: %s</td>\n", linha );
				printf("      </tr>\n" );
				fgets ( linha, 30, infos);
				printf("      <tr>\n" );
				printf("        <td>Humidade: %s</td>\n", linha );
				printf("      </tr>\n" );
			}
			fclose (infos);
		}
		
		printf("      <tr><td></td>\n" );
		printf("      </tr>\n" );
		printf("    </table>\n" );
		printf("    <br><br><br><br><br><br>\n" );
		printf("  </body>\n");
		printf("</html>\n");
 
	mlCgiFreeResources();
	return ML_CGI_OK ;
}
 
/* $RCSfile: geCgiShowMainForm.c,v $ */
