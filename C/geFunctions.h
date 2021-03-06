/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/11 02:59:49 $
 * $Log: geFunctions.h,v $
 * Revision 1.4  2015/01/11 02:59:49  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.1  2015/01/10 19:16:11  vitor.schoola
 * Initial revision
 *
 *
 */

#ifndef _GEFUNCTIONS_
#define _GEFUNCTIONS_              "@(#)geFunctions.h $Revision: 1.4 $"

#include "geTypes.h"
#include <time.h>

/*
HandleLanguage
char* (I) - Name of the language in a string
Returns 
languageType - Selected language. If not existent returns default (english).

This function receives the name of the language, searches in the database, and return the code for the language,
if supported or default ( english )
*/
languageType HandleLanguage ( char *language );

/*
IsUserRegistered
char* (I) - Name of the user

Returns 
Returns 0 if the user is not registered;
Returns 1 if the user is registered;
Returns 2 if it was not possible to open the text file.
*/
unsigned isUserRegistered ( char *user );

/*
IsUserRegisteredToCLI
char* (I) - Name of the user

Returns 
Returns 0 if the user is not registered;
Returns 1 if the user is registered;
Returns 2 if it was not possible to open the text file.
*/
unsigned isUserRegisteredToCLI ( char *user );

/*
CriarStringAleatoria
char* (I) - Valid characters to the output
unsigned (I) - length of the output
char* (O) - Random created string with desired length and using valid characters.

Returns 0 everytime. Considered ideal input.

Creates a random string with desired characters and length
*/
unsigned CriarStringAleatoria ( char *caracteresValidos, unsigned comprimento, char *stringCriada );

/*
CheckPasswordByNick
char* (I) - Nickname of desired user
char* (I) - Password that will be tested

Returns 0 if password is not correct.
Returns 1 if password is correct.
Returns 2 if could not open users file.

Check if username put the correct password checking users file.
*/
unsigned CheckPasswordByNick ( char *geNickname, char *gePassword );

/*
CheckPasswordByNickToCLI
char* (I) - Nickname of desired user
char* (I) - Password that will be tested

Returns 0 if password is not correct.
Returns 1 if password is correct.
Returns 2 if could not open users file.

Check if username put the correct password checking users file.
*/
unsigned CheckPasswordByNickToCLI ( char *geNickname, char *gePassword );
/*
CheckUserInfo
Char* (I) - Nickname of desired user
geUserDataType* (O) - Struct where every information will be stored

Returns 1 if could not open users file
Returns 0 if OK

Put every information about the user ( geNickname ) in ( user ) struct.
*/
unsigned long long CheckUserInfo ( char *geNickname, geUserDataType *user );

/*
CheckUserInfoToCLI
Char* (I) - Nickname of desired user
geUserDataType* (O) - Struct where every information will be stored

Returns 1 if could not open users file
Returns 0 if OK

Put every information about the user ( geNickname ) in ( user ) struct.
*/
unsigned long long CheckUserInfoToCLI ( char *geNickname, geUserDataType *user );

/*
getNextId
Returns 0 if could not open users file.
Returns Next ID in the list if OK.

Check which ID should be the next when creating an account.
*/
long long unsigned getNextId ( );

/*
readCookie
char* path (I) - Path of the Cookie
char* name (I) - Name of the Cookie
char* valueCookie (O) - Value of the read cookie
char* ipUser (O) - Ip of the read cookie
char* time_t (O) - Expiration time of the read cookie

Returns 1 if could not open cookie
		0 if Ok

*/
unsigned readCookie ( char *path, char *name, char *valueCookie, char *ipUser, time_t *tempo );
/*
writeCookie
char* path (I) - Path of the desired Cookie
char* name (I) - Name of the desired Cookie
char* cookieValue (I) - Value of the desired cookie
char* ipString (I) - Ip of the desired cookie
char* unsigned (I) - Expiration time of the desired cookie in minutes

Returns 1 if could not create cookie
		0 if Ok
*/
unsigned writeCookie ( char *path, char *name, char *cookieValue, char *ipString, unsigned minutes );

/*
fileExists
char* path (I) - Path of the desired File
char* name (I) - Name of the desired File
*/
unsigned fileExists ( char *path, char *name );

/*
createButton
char* nameButton (I) - Name of the button
char* executedCgi (I) - Name of the CGI
char* geLanguage (I) - Language of the page
char* geNickname (I) - Nickname of logged user
char* counter (I) - Number to label input

returns 0

Creates a button to execute CGI sending geLanguage and geNickname by post method.
*/
unsigned createButton ( char *nameButton, char *executedCgi, char *geLanguage, char *geNickname, char *counter );

/*
checkCookieAndIp
char *idString (I) - id of the logged user
char *cookieValueNew (I) - cookie received from the previous cgi
languageType language (I) - Language

Returns 0 if Cookie and Ip are the same
Exit ML_CGI_OK if not, showing the error page
*/
unsigned checkCookieAndIp ( char idString[21], char cookieValueNew[COOKIE_LENGTH + 1], languageType language );

/*
char *email (I) - Email that the invite will be sent to
languageType *language (I) - Language of the email
char *realName (I) - Name of the user
unsigned group (I) - Group in which the user will be added to

Returns 0
*/
unsigned InviteUser ( char *email, languageType language, char *realName, unsigned group );

/*
geUserDataType *userToAdd (I) - Informations about user to add
char geLanguage[4] (I) - Language to show messages

Returns 0 if OK
		INVALID_PASSWORD if the password is invalid
		PASSWORD_DONT_MATCH if the passwords do not match
		
This function adds an user to the system.
*/
unsigned geAddUser ( geUserDataType *userToAdd, char geLanguage[4] );
#endif

/* $RCSfile: geFunctions.h,v $ */
