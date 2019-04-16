/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 *
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:16:11 $
 * $Log: geError.c,v $
 * Revision 1.1  2015/01/10 19:16:11  vitor.schoola
 * Initial revision
 *
 *
 */

#ifndef _GEERROR_
#define _GEERROR_              "@(#)geError.h $Revision: 1.1 $"

#include "geError.h"
#include "geConst.h"

char *errorMessages[NUMBER_OF_LANGUAGES][NUMBER_OF_ERRORS] = 
{
{	
	"OK",
	"Couldn't reatrieve language."
},
{
	"OK.",
	"Foi impossível obter a língua."
}	
};

#endif

/* $RCSfile: geError.c,v $ */
