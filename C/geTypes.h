/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/10 19:16:11 $
 * $Log: geTypes.h,v $
 * Revision 1.1  2015/01/10 19:16:11  vitor.schoola
 * Initial revision
 *
 *
 */

#ifndef _GETYPES_
#define _GETYPES_              "@(#)geTypes.h $Revision: 1.1 $"

#include "geConst.h"

typedef enum { english, portuguese } languageType;

typedef unsigned long long geUserIdentifierType;

typedef unsigned char geGroupType;

typedef struct estructureGeUserDataType
{
	geUserIdentifierType 			userIdentifier;
	char 							email[NICKNAME_MAX_LENGTH];
	char 							password[PASSWORD_MAX_LENGTH];
	geGroupType 					group;
	char							name[NAME_MAX_LENGTH];
} geUserDataType;

#endif

/* $RCSfile: geTypes.h,v $ */
