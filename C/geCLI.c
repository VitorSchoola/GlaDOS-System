/*Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma: 2014/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Vitor Gouveia Schoola
 * $Author: vitor.schoola $
 * $Date: 2015/01/11 04:18:58 $
 * $Log: geCLI.c,v $
 * Revision 1.12  2015/01/11 04:18:58  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.11  2015/01/11 03:55:44  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.5  2015/01/11 03:24:11  vitor.schoola
 * *** empty log message ***
 *
 * Revision 1.1  2015/01/11 00:38:21  vitor.schoola
 * Initial revision
 *
 * Revision 1.1  2015/01/10 19:07:51  vitor.schoola
 * Initial revision
 *
 *
 */
 
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
 
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
	int option, longIndex;
	unsigned index;
	geUserDataType userToAdd, userLoggedIn;
	char *subOpt, *value, geLanguage[4];
	char geNickname[ NICKNAME_MAX_LENGTH + 1 ], geEmail[ NICKNAME_MAX_LENGTH + 1 ], geEmailConfirmation[ NICKNAME_MAX_LENGTH + 1 ];
	char geRealName[ NAME_MAX_LENGTH + 1 ];
	char geGroup[ strlen ( "administrators" ) + 1 ];
	unsigned selectedLanguage = 0;
	unsigned infoUser = 0, infoEmail = 0, infoEmailConfirmation = 0, infoGroup = 0, infoUserName = 0;
	unsigned group = 3;
	char *passwordPointer, gePassword[PASSWORD_MAX_LENGTH];
	unsigned passCorrect;
	unsigned didItAdd;
	unsigned long long id;
	unsigned firstLine;
	FILE *reading, *writing;
	char linha[ ID_LENGTH + NICKNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + GROUP_LENGTH + NAME_MAX_LENGTH + 7 ];
	unsigned counter, counter2;
	
	const char *stringOpcoes = "ahizr";
	struct option estruturaOpcoes [] =
	{
	{"add", 0, NULL, 'a'},
	{"help", 0, NULL, 'h'},
	{"invite", 0, NULL, 'i'},
	{"initialize", 0, NULL, 'z'},
	{"remove", 0, NULL, 'r'},
	{NULL, 0, NULL, 0}
	};
	
	enum {USER = 0, EMAIL, EMAILCONFIRMATION, USERNAME, GROUP, LANGUAGE};

	char *token [] =
	{
	[USER] = "user",
	[EMAIL] = "email",
	[EMAILCONFIRMATION] = "confirmation",
	[USERNAME] = "username",
	[GROUP] = "group",
	[LANGUAGE] = "language",
	NULL
	};
	
	opterr = 0;
	
	if ( mlCgiInitialize ( &environment ) )
		exit ( OK );
 
	if ( environment != commandLine )
	{
		printf( "Incorrect environment. This program was developed for CLI.\n");
		exit( INCORRECT_ENVIRONMENT );
	}
	
	printf ( " _,.---.,_\n" );
	printf ( "<_  .'.\" _>\n" );
 	printf ( "|_\"\"---\"\"_|\n" );
 	printf ( "/ \"\"---\"\" \\\n" );
 	printf ( "\\         /\n" );
 	printf ( " | \"  \" \"| ~ GelaDOS - CLI Interface ~\n" );
 	printf ( " |       | - Commands: z: initialize\n" );
 	printf ( " | - - - | - a: add \t i:invite\n" );
 	printf ( " (       ) - r: remove \t h: help\n" );
 	printf ( "  \"-----\" \n" );
	
	while ((option = getopt_long (argc, argv, stringOpcoes, estruturaOpcoes, &longIndex)) != -1)
	{
		printf ("Selected: %c\n", option);
		switch (option)
		{
			case 'a': /* Add User */
				if ( fileExists ( "Files/", "users" ) == 0 )
				{
					printf ( "System was not initialized. Execute the geCLI -z function.\n" );
					return OK;
				}
				for (index = optind; index < argc; index++)
				{
					subOpt = argv [index];
					switch (getsubopt (&subOpt, token, &value))
					{
						case LANGUAGE:
							if ( selectedLanguage == 0 )
							{
								if ( strcmp ( value, "portuguese" ) == 0 )
								{
									geLanguage[0] = 'P';
									geLanguage[1] = 'T';
									geLanguage[2] ='\0';
									printf ( "Idioma: Portugues\n" );
									selectedLanguage = 1;
								}
								else
								{
									geLanguage[0] = 'E';
									geLanguage[1] = 'N';
									geLanguage[2] ='\0';
									printf ( "Language: Enlgish\n" );
									selectedLanguage = 1;
								}
							}
							break;
							
						case USER:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Nickname muito grande.\n" );
								else
									printf ( "The Nickname's length exceeds the limit.\n" );	
								exit ( ERRO_ARGUMENTO_NICKNAME );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geNickname[counter] = value[counter];
							infoUser = 1;
							break;
							
						case EMAIL:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Email muito grande.\n" );
								else
									printf ( "The Email's length exceeds the limit.\n" );									
								exit ( ERRO_ARGUMENTO_EMAIL );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geEmail[counter] = value[counter];
							infoEmail = 1;
							break;
							
						case EMAILCONFIRMATION:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Confirmacao de Email muito grande.\n" );
								else
									printf ( "The Email Confirmation's length exceeds the limit.\n" );			
								exit ( ERRO_ARGUMENTO_CONFIRMACAO_EMAIL );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geEmailConfirmation[counter] = value[counter];
							infoEmailConfirmation = 1;
							break;
							
						case USERNAME:
							if ( strlen ( value ) > NAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Nome Real muito grande.\n" );
								else
									printf ( "The Real Name's length exceeds the limit.\n" );			
								exit ( ERRO_ARGUMENTO_REALNAME );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geRealName[counter] = value[counter];
							infoUserName = 1;
							break;
							
						case GROUP:
							if ( strlen ( value ) > strlen ( "administrators" ) )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Grupo muito grande.\n" );
								else
									printf ( "The Group's length exceeds the limit.\n" );			
								exit ( ERRO_ARGUMENTO_GROUP );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geGroup[counter] = value[counter];
							infoGroup = 1;
							break;
							
						default:
							printf ( "Subopcao invalida\n" );
					}		
				}
				if ( selectedLanguage == 0 )
				{
					geLanguage[0] = 'E';
					geLanguage[1] = 'N';
					geLanguage[2] ='\0';
					printf ( "Language: Enlgish\n" );
					selectedLanguage = 1;
				}
				if ( strcmp ( geLanguage, "PT" ) == 0 )
					printf ( "GelaDOS: Adicionar Usuario.\n" );
				else
					printf ( "GelaDOS: Add User.\n" );
				/*-----------------------------------------*/
				/* Checks if everything needed was written */
				if ( strcmp ( geLanguage, "PT" ) == 0 )
				{
					if ( infoUser == 0 )
						printf ( "Voce nao disse seu login.\n" );
					if ( infoEmail == 0 )
						printf ( "Voce nao disse o email da nova conta.\n" );
					if ( infoEmailConfirmation == 0 )
						printf ( "Voce nao confirmou o email da nova conta.\n" );
					if ( infoGroup == 0 )
						printf ( "Voce nao disse o grupo da nova conta.\n" );
					if ( infoUserName == 0 )
						printf ( "Voce nao disse o nome real do dono da nova conta.\n" );
					if ( ( infoUser == 0 ) || ( infoEmail == 0 ) || ( infoEmailConfirmation == 0 ) || ( infoGroup == 0 ) || ( infoUserName == 0 ) )
					{
						printf ( "Voce deve usar o programa como descrito abaixo:\n" );
						printf ( "./geCLI -a \n\t user=<seu login>\n\t email=<email da nova conta>\n\t confirmation=<confirme o email da nova conta>\n\t username=<nome do dono da nova conta>\n\t group=administrators|masters|tasters\n\t [language=english|portuguese]\n" ); 
					}					
				}
				else
				{
					if ( infoUser == 0 )
						printf ( "You didn't say your login.\n" );
					if ( infoEmail == 0 )
						printf ( "You didn't say the new account's email.\n" );
					if ( infoEmailConfirmation == 0 )
						printf ( "You didn't confirm the new account's email.\n" );
					if ( infoGroup == 0 )
						printf ( "You didn't say the new account's group.\n" );
					if ( infoUserName == 0 )
						printf ( "You didn't say the new account owner's name.\n" );
					if ( ( infoUser == 0 ) || ( infoEmail == 0 ) || ( infoEmailConfirmation == 0 ) || ( infoGroup == 0 ) || ( infoUserName == 0 ) )
					{
						printf ( "You may use this program as described down bellow:\n" );
						printf ( "./geCLI -a \n\t user=<your login>\n\t email=<new account's email>\n\t confirmation=<confirm the new account's email>\n\t username=<new account owner's name>\n\t group=administrators|masters|tasters\n\t[language=english|portuguese]\n" ); 
						return DIDNT_EXECUTE;
					}
				}
				/*-----------------------------------------*/
				/* Checks if it is a valid group */
				if ( fileExists ( "Files/", "users" ) == 1 )
				{
					if ( ( strcmp ( geGroup, "administrators" ) == 0 ) || ( geGroup[0] == '1' ) )
					{
						group = 1;
					}
					if ( ( strcmp ( geGroup, "masters" ) == 0 ) || ( geGroup[0] == '2' ) )
					{
						group = 2;
					}
					if ( ( strcmp ( geGroup, "tasters" ) == 0 ) || ( geGroup[0] == '4' ) )
					{
						group = 4;
					}
					
					if ( group == 3 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
						{
							printf ( "Grupo invalido.\n" );
							printf ( "Use o nome do grupo [administrators|masters|tasters] ou o valor dele [1|2|4].\n" );
							return DIDNT_EXECUTE;
						}
						else
						{
							printf ( "Invalid Group.\n" );
							printf ( "Use the group's name [administrators|masters|tasters] or its value [1|2|4].\n" );
							return DIDNT_EXECUTE;
						}
					}
				}
				else
					group = 1;
				/*-------------------------------------------*/
				/* Checks if email and confirmation match */
				
				if ( strcmp ( geEmail, geEmailConfirmation ) != 0 )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
					{
						printf ( "ERRO: Email e confirmacao nao batem.\n" );
						printf ( "%s <> %s\n", geEmail, geEmailConfirmation );
					}
					else
					{
						printf ( "ERROR: Email and confirmation do not match.\n" );
						printf ( "%s <> %s\n", geEmail, geEmailConfirmation );
					}				
					return DIDNT_EXECUTE;
				}
				
				/*------------------------------------------*/
				/* Checks if new user is already registered */
				
				if ( isUserRegisteredToCLI ( geEmail ) == 1 )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
						printf ( "ERRO: Email ja esta no sistema.\n" );
					else
						printf ( "ERROR: Email already in use.\n" );						
					return DIDNT_EXECUTE;
				}
				
				/*------------------------------------------*/
				/* Checks if geNickname is registered */
				if ( fileExists ( "Files/", "users" ) == 1 )
				{
					if ( isUserRegisteredToCLI ( geNickname ) == 0 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
							printf ( "ERRO: %s nao esta cadastrado em nosso sistema.\n", geNickname );
						else
							printf ( "ERROR: %s is not registered in our system.\n", geNickname );
						return DIDNT_EXECUTE_ACCESS_DENIED;
					}
				}
				
				/*-------------------------------------------*/
				/* Asks for user's password */
				if ( fileExists ( "Files/", "users" ) == 1 )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
						printf ( "IDENTIFICACAO: Coloque sua senha, %s.\n", geNickname );		
					else
						printf ( "IDENTIFICATION: Write your password, %s.\n", geNickname );				
					if ( strcmp ( geLanguage, "PT" ) == 0 )
					{
						passwordPointer = getpass ( "Senha: " );
						for ( counter = 0; counter < PASSWORD_MAX_LENGTH; counter++ )
							gePassword[counter] = passwordPointer[counter];		
					}
					else
					{
						passwordPointer = getpass ( "Password: " );
						for ( counter = 0; counter < PASSWORD_MAX_LENGTH; counter++ )
							gePassword[counter] = passwordPointer[counter];	
					}
					for ( counter = 0; counter < PASSWORD_MAX_LENGTH; counter++ )
							passwordPointer[counter] = '\0';

					if ( ( passCorrect = CheckPasswordByNickToCLI ( geNickname, gePassword ) ) == 0 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
						{
							printf ( "ERRO: Senha Incorreta.\n" );
							return DIDNT_EXECUTE_ACCESS_DENIED;
						}
						else
						{
							printf ( "ERROR: Incorrect Password.\n" );
							return DIDNT_EXECUTE_ACCESS_DENIED;
						}
					}
					
					if ( passCorrect == 3 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
						{
							printf ( "ERRO: Aplicacao Pendente.\n" );
							return DIDNT_EXECUTE_ACCESS_DENIED;
						}
						else
						{
							printf ( "ERROR: Pending Application.\n" );
							return DIDNT_EXECUTE_ACCESS_DENIED;
						}
					}
					CheckUserInfo ( geNickname, &userLoggedIn );
				}
				
				/*-------------------------------------------*/
				for ( counter = 0; counter <= strlen ( geEmail ); counter++ )
					userToAdd.email[counter] = geEmail[counter];
				for ( counter = 0; counter <= strlen ( geRealName ); counter++ )
					userToAdd.name[counter] = geRealName[counter];
				if ( group == 1 )
					userToAdd.group = 1;
				if ( group == 2 )
					userToAdd.group = 2;
				if ( group == 4 )
					userToAdd.group = 4;
				didItAdd = geAddUser ( &userToAdd, geLanguage );
				
				if ( didItAdd == PASSWORD_DONT_MATCH )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
						printf ( "Senhas nao batem.\n" );
					else
						printf ( "Passwords didn't match.\n" );	
					return DIDNT_EXECUTE;
				}
				
				if ( didItAdd == INVALID_PASSWORD )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
						printf ( "Senha invalida.\n" );
					else
						printf ( "Invalid Password.\n" );	
					return DIDNT_EXECUTE;
				}
				if ( strcmp ( geLanguage, "PT" ) == 0 )
					printf ( "Feito.\n" );
				else
					printf ( "Done.\n" );
				return OK;
				break;
			/*-----------------------------------------------*/
			
			case 'i': /* Invite User */
				if ( fileExists ( "Files/", "users" ) == 0 )
				{
					printf ( "System was not initialized. Execute the geCLI -z function.\n" );
					return OK;
				}
				for (index = optind; index < argc; index++)
				{
					subOpt = argv [index];
					switch (getsubopt (&subOpt, token, &value))
					{
						case LANGUAGE:
							if ( selectedLanguage == 0 )
							{
								if ( strcmp ( value, "portuguese" ) == 0 )
								{
									geLanguage[0] = 'P';
									geLanguage[1] = 'T';
									geLanguage[2] ='\0';
									printf ( "Idioma: Portugues\n" );
									selectedLanguage = 1;
								}
								else
								{
									geLanguage[0] = 'E';
									geLanguage[1] = 'N';
									geLanguage[2] ='\0';
									printf ( "Language: Enlgish\n" );
									selectedLanguage = 1;
								}
							}
							break;
							
						case USER:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Nickname muito grande.\n" );
								else
									printf ( "The Nickname's length exceeds the limit.\n" );	
								exit ( ERRO_ARGUMENTO_NICKNAME );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geNickname[counter] = value[counter];
							infoUser = 1;
							break;
							
						case EMAIL:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Email muito grande.\n" );
								else
									printf ( "The Email's length exceeds the limit.\n" );									
								exit ( ERRO_ARGUMENTO_EMAIL );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geEmail[counter] = value[counter];
							infoEmail = 1;
							break;
							
						case EMAILCONFIRMATION:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Confirmacao de Email muito grande.\n" );
								else
									printf ( "The Email Confirmation's length exceeds the limit.\n" );			
								exit ( ERRO_ARGUMENTO_CONFIRMACAO_EMAIL );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geEmailConfirmation[counter] = value[counter];
							infoEmailConfirmation = 1;
							break;
							
						case USERNAME:
							if ( strlen ( value ) > NAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Nome Real muito grande.\n" );
								else
									printf ( "The Real Name's length exceeds the limit.\n" );			
								exit ( ERRO_ARGUMENTO_REALNAME );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geRealName[counter] = value[counter];
							infoUserName = 1;
							break;
							
						case GROUP:
							if ( strlen ( value ) > strlen ( "administrators" ) )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Grupo muito grande.\n" );
								else
									printf ( "The Group's length exceeds the limit.\n" );			
								exit ( ERRO_ARGUMENTO_GROUP );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geGroup[counter] = value[counter];
							infoGroup = 1;
							break;
							
						default:
							printf ( "Subopcao invalida\n" );
					}		
				}
				if ( selectedLanguage == 0 )
				{
					geLanguage[0] = 'E';
					geLanguage[1] = 'N';
					geLanguage[2] ='\0';
					printf ( "Language: Enlgish\n" );
					selectedLanguage = 1;
				}
				if ( strcmp ( geLanguage, "PT" ) == 0 )
					printf ( "GelaDOS: Convidar Usuario.\n" );
				else
					printf ( "GelaDOS: Invite User.\n" );
				/*-----------------------------------------*/
				/* Checks if everything needed was written */
				if ( strcmp ( geLanguage, "PT" ) == 0 )
				{
					if ( infoUser == 0 )
						printf ( "Voce nao disse seu login.\n" );
					if ( infoEmail == 0 )
						printf ( "Voce nao disse o email da nova conta.\n" );
					if ( infoEmailConfirmation == 0 )
						printf ( "Voce nao confirmou o email da nova conta.\n" );
					if ( infoGroup == 0 )
						printf ( "Voce nao disse o grupo da nova conta.\n" );
					if ( infoUserName == 0 )
						printf ( "Voce nao disse o nome real do dono da nova conta.\n" );
					if ( ( infoUser == 0 ) || ( infoEmail == 0 ) || ( infoEmailConfirmation == 0 ) || ( infoGroup == 0 ) || ( infoUserName == 0 ) )
					{
						printf ( "Voce deve usar o programa como descrito abaixo:\n" );
						printf ( "./geCLI -i \n\t user=<seu login>\n\t email=<email da nova conta>\n\t confirmation=<confirme o email da nova conta>\n\t username=<nome do dono da nova conta>\n\t group=administrators|masters|tasters\n\t [language=english|portuguese]\n" ); 
					}					
				}
				else
				{
					if ( infoUser == 0 )
						printf ( "You didn't say your login.\n" );
					if ( infoEmail == 0 )
						printf ( "You didn't say the new account's email.\n" );
					if ( infoEmailConfirmation == 0 )
						printf ( "You didn't confirm the new account's email.\n" );
					if ( infoGroup == 0 )
						printf ( "You didn't say the new account's group.\n" );
					if ( infoUserName == 0 )
						printf ( "You didn't say the new account owner's name.\n" );
					if ( ( infoUser == 0 ) || ( infoEmail == 0 ) || ( infoEmailConfirmation == 0 ) || ( infoGroup == 0 ) || ( infoUserName == 0 ) )
					{
						printf ( "You may use this program as described down bellow:\n" );
						printf ( "./geCLI -i \n\t user=<your login>\n\t email=<new account's email>\n\t confirmation=<confirm the new account's email>\n\t username=<new account owner's name>\n\t group=administrators|masters|tasters\n\t[language=english|portuguese]\n" ); 
						return DIDNT_EXECUTE;
					}
				}
				/*-----------------------------------------*/
				/* Checks if it is a valid group */
					if ( ( strcmp ( geGroup, "administrators" ) == 0 ) || ( geGroup[0] == '1' ) )
					{
						group = 1;
					}
					if ( ( strcmp ( geGroup, "masters" ) == 0 ) || ( geGroup[0] == '2' ) )
					{
						group = 2;
					}
					if ( ( strcmp ( geGroup, "tasters" ) == 0 ) || ( geGroup[0] == '4' ) )
					{
						group = 4;
					}
					
					if ( group == 3 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
						{
							printf ( "Grupo invalido.\n" );
							printf ( "Use o nome do grupo [administrators|masters|tasters] ou o valor dele [1|2|4].\n" );
							return DIDNT_EXECUTE;
						}
						else
						{
							printf ( "Invalid Group.\n" );
							printf ( "Use the group's name [administrators|masters|tasters] or its value [1|2|4].\n" );
							return DIDNT_EXECUTE;
						}
					}
				/*-------------------------------------------*/
				/* Checks if email and confirmation match */
				
				if ( strcmp ( geEmail, geEmailConfirmation ) != 0 )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
					{
						printf ( "ERRO: Email e confirmacao nao batem.\n" );
						printf ( "%s <> %s\n", geEmail, geEmailConfirmation );
					}
					else
					{
						printf ( "ERROR: Email and confirmation do not match.\n" );
						printf ( "%s <> %s\n", geEmail, geEmailConfirmation );
					}				
					return DIDNT_EXECUTE;
				}
				
				/*------------------------------------------*/
				/* Checks if new user is already registered */
				
				if ( isUserRegisteredToCLI ( geEmail ) == 1 )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
						printf ( "ERRO: Email ja esta no sistema.\n" );
					else
						printf ( "ERROR: Email already in use.\n" );						
					return DIDNT_EXECUTE;
				}
				
				/*------------------------------------------*/
				/* Checks if geNickname is registered */
					if ( isUserRegisteredToCLI ( geNickname ) == 0 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
							printf ( "ERRO: %s nao esta cadastrado em nosso sistema.\n", geNickname );
						else
							printf ( "ERROR: %s is not registered in our system.\n", geNickname );
						return DIDNT_EXECUTE_ACCESS_DENIED;
					}
			
				/*-------------------------------------------*/
				/* Asks for user's password */
					if ( strcmp ( geLanguage, "PT" ) == 0 )
						printf ( "IDENTIFICACAO: Coloque sua senha, %s.\n", geNickname );		
					else
						printf ( "IDENTIFICATION: Write your password, %s.\n", geNickname );				
					if ( strcmp ( geLanguage, "PT" ) == 0 )
					{
						passwordPointer = getpass ( "Senha: " );
						for ( counter = 0; counter < PASSWORD_MAX_LENGTH; counter++ )
							gePassword[counter] = passwordPointer[counter];		
					}
					else
					{
						passwordPointer = getpass ( "Password: " );
						for ( counter = 0; counter < PASSWORD_MAX_LENGTH; counter++ )
							gePassword[counter] = passwordPointer[counter];	
					}
					for ( counter = 0; counter < PASSWORD_MAX_LENGTH; counter++ )
							passwordPointer[counter] = '\0';

					if ( ( passCorrect = CheckPasswordByNickToCLI ( geNickname, gePassword ) ) == 0 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
						{
							printf ( "ERRO: Senha Incorreta.\n" );
							return DIDNT_EXECUTE_ACCESS_DENIED;
						}
						else
						{
							printf ( "ERROR: Incorrect Password.\n" );
							return DIDNT_EXECUTE_ACCESS_DENIED;
						}
					}
					
					if ( passCorrect == 3 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
						{
							if ( strcmp ( geLanguage, "PT" ) == 0 )
								printf ( "ERRO: Aplicacao Pendente.\n" );
							else
								printf ( "ERROR: Pending Application.\n" );
							return DIDNT_EXECUTE_ACCESS_DENIED;
						}
					}
					CheckUserInfoToCLI ( geNickname, &userLoggedIn );
					//printf ( "%u\n",userLoggedIn.group );
					if ( ( ( userLoggedIn.group == 2 ) && ( group == 1 ) ) || ( ( userLoggedIn.group == 4 ) && ( group == 2 ) ) || ( ( userLoggedIn.group == 4 ) && ( group == 1 ) ) )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
							printf ( "ERRO: Grupo invalido.\n" );
						else
							printf ( "ERROR: Invalid Group.\n" );
						return DIDNT_EXECUTE_ACCESS_DENIED;
					}
			/*-------------------------------------------*/	
				writing = fopen ( "Files/users.abeyances", "a" );
				id = getNextId ();
				fprintf ( writing, "\n%020llu;%s;;%u;%s;%lld", id, geEmail, group, geRealName, (long long) time(NULL) );
				fclose ( writing );
				writing = fopen ( "Files/users", "a" );
				fprintf ( writing, "\n%020llu;%s;;%u;%s;", id, geEmail, group, geRealName );
				fclose ( writing );
				if ( strcmp ( geLanguage, "PT" ) == 0 )
					InviteUser ( geEmail, portuguese, geRealName, group );
				else
					InviteUser ( geEmail, english, geRealName, group );
					
				if ( strcmp ( geLanguage, "PT" ) == 0 )
					printf ( "Usuario convidado com sucesso!\n" );
				else
					printf ( "User was successfully invited!\n" );
					
				return OK;
				break;
				
			/*-----------------------------------------------*/
			
			case 'r': /* Removes User */
				if ( fileExists ( "Files/", "users" ) == 0 )
				{
					printf ( "System was not initialized. Execute the geCLI -z function.\n" );
					return OK;
				}
				for (index = optind; index < argc; index++)
				{
					subOpt = argv [index];
					switch (getsubopt (&subOpt, token, &value))
					{
						case LANGUAGE:
							if ( selectedLanguage == 0 )
							{
								if ( strcmp ( value, "portuguese" ) == 0 )
								{
									geLanguage[0] = 'P';
									geLanguage[1] = 'T';
									geLanguage[2] ='\0';
									printf ( "Idioma: Portugues\n" );
									selectedLanguage = 1;
								}
								else
								{
									geLanguage[0] = 'E';
									geLanguage[1] = 'N';
									geLanguage[2] ='\0';
									printf ( "Language: Enlgish\n" );
									selectedLanguage = 1;
								}
							}
							break;
							
						case USER:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Nickname muito grande.\n" );
								else
									printf ( "The Nickname's length exceeds the limit.\n" );	
								exit ( ERRO_ARGUMENTO_NICKNAME );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geNickname[counter] = value[counter];
							infoUser = 1;
							break;
							
						case EMAIL:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Email muito grande.\n" );
								else
									printf ( "The Email's length exceeds the limit.\n" );									
								exit ( ERRO_ARGUMENTO_EMAIL );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geEmail[counter] = value[counter];
							infoEmail = 1;
							break;
							
						default:
							printf ( "Subopcao invalida\n" );
					}		
				}
				if ( selectedLanguage == 0 )
				{
					geLanguage[0] = 'E';
					geLanguage[1] = 'N';
					geLanguage[2] ='\0';
					printf ( "Language: Enlgish\n" );
					selectedLanguage = 1;
				}
				if ( strcmp ( geLanguage, "PT" ) == 0 )
					printf ( "GelaDOS: Remover Usuario.\n" );
				else
					printf ( "GelaDOS: Remove User.\n" );
				/*-----------------------------------------*/
				/* Checks if everything needed was written */
				if ( strcmp ( geLanguage, "PT" ) == 0 )
				{
					if ( infoUser == 0 )
						printf ( "Voce nao disse seu login.\n" );
					if ( infoEmail == 0 )
						printf ( "Voce nao disse o email da nova conta.\n" );
					if ( ( infoUser == 0 ) || ( infoEmail == 0 ) )
					{
						printf ( "Voce deve usar o programa como descrito abaixo:\n" );
						printf ( "./geCLI -r \n\t user=<seu login>\n\t email=<email da conta a deletar>\n\t [language=english|portuguese]\n" ); 
					}					
				}
				else
				{
					if ( infoUser == 0 )
						printf ( "You didn't say your login.\n" );
					if ( infoEmail == 0 )
						printf ( "You didn't say the new account's email.\n" );
					if ( ( infoUser == 0 ) || ( infoEmail == 0 ) )
					{
						printf ( "You may use this program as described down bellow:\n" );
						printf ( "./geCLI -i \n\t user=<your login>\n\t email=<deleting account's email> \n\t[language=english|portuguese]\n" ); 
						return DIDNT_EXECUTE;
					}
				}
				/*-------------------------------------------*/
				/* Checks if geNickname is registered */
					if ( isUserRegisteredToCLI ( geNickname ) == 0 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
							printf ( "ERRO: %s nao esta cadastrado em nosso sistema.\n", geNickname );
						else
							printf ( "ERROR: %s is not registered in our system.\n", geNickname );
						return DIDNT_EXECUTE_ACCESS_DENIED;
					}
			
				/*-------------------------------------------*/
				/* Checks if deleting user exists */
				
				if ( isUserRegisteredToCLI ( geEmail ) != 1 )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
						printf ( "ERRO: Email nao esta.\n" );
					else
						printf ( "ERROR: Email is not in use.\n" );						
					return DIDNT_EXECUTE;
				}
				
				/*------------------------------------------*/
				/* Asks for user's password */
					if ( strcmp ( geLanguage, "PT" ) == 0 )
						printf ( "IDENTIFICACAO: Coloque sua senha, %s.\n", geNickname );		
					else
						printf ( "IDENTIFICATION: Write your password, %s.\n", geNickname );				
					if ( strcmp ( geLanguage, "PT" ) == 0 )
					{
						passwordPointer = getpass ( "Senha: " );
						for ( counter = 0; counter < PASSWORD_MAX_LENGTH; counter++ )
							gePassword[counter] = passwordPointer[counter];		
					}
					else
					{
						passwordPointer = getpass ( "Password: " );
						for ( counter = 0; counter < PASSWORD_MAX_LENGTH; counter++ )
							gePassword[counter] = passwordPointer[counter];	
					}
					for ( counter = 0; counter < PASSWORD_MAX_LENGTH; counter++ )
							passwordPointer[counter] = '\0';

					if ( ( passCorrect = CheckPasswordByNickToCLI ( geNickname, gePassword ) ) == 0 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
						{
							printf ( "ERRO: Senha Incorreta.\n" );
							return DIDNT_EXECUTE_ACCESS_DENIED;
						}
						else
						{
							printf ( "ERROR: Incorrect Password.\n" );
							return DIDNT_EXECUTE_ACCESS_DENIED;
						}
					}
					
					if ( passCorrect == 3 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
						{
							if ( strcmp ( geLanguage, "PT" ) == 0 )
								printf ( "ERRO: Aplicacao Pendente.\n" );
							else
								printf ( "ERROR: Pending Application.\n" );
							return DIDNT_EXECUTE_ACCESS_DENIED;
						}
					}
					CheckUserInfoToCLI ( geNickname, &userLoggedIn );
					//printf ( "%u\n",userLoggedIn.group );
					if ( userLoggedIn.group != 1 )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
							printf ( "ERRO: Grupo invalido.\n" );
						else
							printf ( "ERROR: Invalid Group.\n" );
						return DIDNT_EXECUTE_ACCESS_DENIED;					
					}
					
					if ( ( ( userLoggedIn.group == 2 ) && ( group == 1 ) ) || ( ( userLoggedIn.group == 4 ) && ( group == 2 ) ) || ( ( userLoggedIn.group == 4 ) && ( group == 1 ) ) )
					{
						if ( strcmp ( geLanguage, "PT" ) == 0 )
							printf ( "ERRO: Grupo invalido.\n" );
						else
							printf ( "ERROR: Invalid Group.\n" );
						return DIDNT_EXECUTE_ACCESS_DENIED;
					}
				/*-------------------------------------------*/	
				reading = fopen ( "Files/users", "r" );
				writing = fopen ( "Files/users.d", "w" );
				
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
					if ( strcmp ( geEmail, &linha[counter + 1] ) != 0 )
					{
						linha[counter] = ';';
						linha[counter2] = ';';
						if ( firstLine == 0 )
							fprintf ( writing, "\n%s",linha );
						else
						{
							fprintf ( writing, "%s", linha );
							firstLine = 0;
						}
					}
				}
				
				fclose ( reading );
				fclose ( writing );
				remove ( "Files/users" );
				rename ( "Files/users.d", "Files/users" );
				
				firstLine = 1;
				
				/* Check if abeyances File exists */
				
				if ( fileExists ( "Files/", "users.abeyances" ) == 1 )
				{
					reading = fopen ( "Files/users.abeyances", "r" );
					writing = fopen ( "Files/users.abeyances.d", "w" );
					
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
						if ( strcmp ( geEmail, &linha[counter + 1] ) != 0 )
						{
							linha[counter] = ';';
							linha[counter2] = ';';
							if ( firstLine == 0 )
							{
								fprintf ( writing, "\n%s",linha );
							}
							else
							{
								fprintf ( writing, "%s", linha );
								firstLine = 0;
							}
						}
					}
					
					fclose ( reading );
					fclose ( writing );
					remove ( "Files/users.abeyances" );
					rename ( "Files/users.abeyances.d", "Files/users.abeyances" );
				}
			
			/*----------------------------------*/
				return OK;
				break;
				
			/*-----------------------------------------------*/
			case 'z': /* Initialize */
			if ( fileExists ( "Files/", "users" ) == 1 )
			{
				printf ( "This system has already been initialized.\n" );
				return OK;
			}
				for (index = optind; index < argc; index++)
				{
					subOpt = argv [index];
					switch (getsubopt (&subOpt, token, &value))
					{
						case LANGUAGE:
							if ( selectedLanguage == 0 )
							{
								if ( strcmp ( value, "portuguese" ) == 0 )
								{
									geLanguage[0] = 'P';
									geLanguage[1] = 'T';
									geLanguage[2] ='\0';
									printf ( "Idioma: Portugues\n" );
									selectedLanguage = 1;
								}
								else
								{
									geLanguage[0] = 'E';
									geLanguage[1] = 'N';
									geLanguage[2] ='\0';
									printf ( "Language: Enlgish\n" );
									selectedLanguage = 1;
								}
							}
							break;
							
						case USER:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Nickname muito grande.\n" );
								else
									printf ( "The Nickname's length exceeds the limit.\n" );	
								exit ( ERRO_ARGUMENTO_NICKNAME );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geNickname[counter] = value[counter];
							infoUser = 1;
							break;
							
						case EMAIL:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Email muito grande.\n" );
								else
									printf ( "The Email's length exceeds the limit.\n" );									
								exit ( ERRO_ARGUMENTO_EMAIL );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geEmail[counter] = value[counter];
							infoEmail = 1;
							break;
							
						case EMAILCONFIRMATION:
							if ( strlen ( value ) > NICKNAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Confirmacao de Email muito grande.\n" );
								else
									printf ( "The Email Confirmation's length exceeds the limit.\n" );			
								exit ( ERRO_ARGUMENTO_CONFIRMACAO_EMAIL );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geEmailConfirmation[counter] = value[counter];
							infoEmailConfirmation = 1;
							break;
							
						case USERNAME:
							if ( strlen ( value ) > NAME_MAX_LENGTH )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Nome Real muito grande.\n" );
								else
									printf ( "The Real Name's length exceeds the limit.\n" );			
								exit ( ERRO_ARGUMENTO_REALNAME );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geRealName[counter] = value[counter];
							infoUserName = 1;
							break;
							
						case GROUP:
							if ( strlen ( value ) > strlen ( "administrators" ) )
							{
								if ( strcmp ( geLanguage, "PT" ) == 0 )
									printf ( "Grupo muito grande.\n" );
								else
									printf ( "The Group's length exceeds the limit.\n" );			
								exit ( ERRO_ARGUMENTO_GROUP );
							}
							for ( counter = 0; counter <= strlen ( value ); counter++ )
								geGroup[counter] = value[counter];
							infoGroup = 1;
							break;
							
						default:
							printf ( "Subopcao invalida\n" );
					}		
				}
				if ( selectedLanguage == 0 )
				{
					geLanguage[0] = 'E';
					geLanguage[1] = 'N';
					geLanguage[2] ='\0';
					printf ( "Language: Enlgish\n" );
					selectedLanguage = 1;
				}
				if ( strcmp ( geLanguage, "PT" ) == 0 )
					printf ( "GelaDOS: Inicializar.\n" );
				else
					printf ( "GelaDOS: Initialize.\n" );
				/*-----------------------------------------*/
				/* Checks if everything needed was written */
				if ( strcmp ( geLanguage, "PT" ) == 0 )
				{
					if ( infoEmail == 0 )
						printf ( "Voce nao disse o email da nova conta.\n" );
					if ( infoEmailConfirmation == 0 )
						printf ( "Voce nao confirmou o email da nova conta.\n" );
					if ( infoUserName == 0 )
						printf ( "Voce nao disse o nome real do dono da nova conta.\n" );
					if ( ( infoEmail == 0 ) || ( infoEmailConfirmation == 0 ) || ( infoUserName == 0 ) )
					{
						printf ( "Voce deve usar o programa como descrito abaixo:\n" );
						printf ( "./geCLI -z \n\t email=<email da nova conta>\n\t confirmation=<confirme o email da nova conta>\n\t username=<nome do dono da nova conta>\n\t [language=english|portuguese]\n" ); 
					}					
				}
				else
				{
					if ( infoEmail == 0 )
						printf ( "You didn't say the new account's email.\n" );
					if ( infoEmailConfirmation == 0 )
						printf ( "You didn't confirm the new account's email.\n" );
					if ( infoUserName == 0 )
						printf ( "You didn't say the new account owner's name.\n" );
					if ( ( infoEmail == 0 ) || ( infoEmailConfirmation == 0 ) || ( infoUserName == 0 ) )
					{
						printf ( "You may use this program as described down bellow:\n" );
						printf ( "./geCLI -z \n\t email=<new account's email>\n\t confirmation=<confirm the new account's email>\n\t username=<new account owner's name>\n\t [language=english|portuguese]\n" ); 
						return DIDNT_EXECUTE;
					}
				}
				/*-----------------------------------------*/
				/* Sets group */
				group = 1;
				/*-------------------------------------------*/
				/* Checks if email and confirmation match */
				
				if ( strcmp ( geEmail, geEmailConfirmation ) != 0 )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
					{
						printf ( "ERRO: Email e confirmacao nao batem.\n" );
						printf ( "%s <> %s\n", geEmail, geEmailConfirmation );
					}
					else
					{
						printf ( "ERROR: Email and confirmation do not match.\n" );
						printf ( "%s <> %s\n", geEmail, geEmailConfirmation );
					}				
					return DIDNT_EXECUTE;
				}

				/*-------------------------------------------*/
				for ( counter = 0; counter <= strlen ( geEmail ); counter++ )
					userToAdd.email[counter] = geEmail[counter];
				for ( counter = 0; counter <= strlen ( geRealName ); counter++ )
					userToAdd.name[counter] = geRealName[counter];
				userToAdd.group = 1;
				didItAdd = geAddUser ( &userToAdd, geLanguage );
				
				if ( didItAdd == PASSWORD_DONT_MATCH )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
						printf ( "Senhas nao batem.\n" );
					else
						printf ( "Passwords didn't match.\n" );	
					return DIDNT_EXECUTE;
				}
				
				if ( didItAdd == INVALID_PASSWORD )
				{
					if ( strcmp ( geLanguage, "PT" ) == 0 )
						printf ( "Senha invalida.\n" );
					else
						printf ( "Invalid Password.\n" );	
					return DIDNT_EXECUTE;
				}
				
				if ( strcmp ( geLanguage, "PT" ) == 0 )
					printf ( "Feito.\n" );
				else
					printf ( "Done.\n" );
				return OK;
				break;
			/*-----------------------------------------------*/
			
			case 'h': /* Help */
				for (index = optind; index < argc; index++)
					{
						subOpt = argv [index];
						switch (getsubopt (&subOpt, token, &value))
						{
							case LANGUAGE:
								if ( strcmp ( value, "portuguese" ) == 0 )
								{
									geLanguage[0] = 'P';
									geLanguage[1] = 'T';
									geLanguage[2] ='\0';
									printf ( "Idioma: Portugues\n" );
									selectedLanguage = 1;
								}
								else
								{
									geLanguage[0] = 'E';
									geLanguage[1] = 'N';
									geLanguage[2] ='\0';
									printf ( "Language: Enlgish\n" );
									selectedLanguage = 1;
								}		
							break;
							default:
								printf ( "Invalid SubOption\n" );
						}
					}
				if ( selectedLanguage == 0 )
				{
					geLanguage[0] = 'E';
					geLanguage[1] = 'N';
					geLanguage[2] = '\0';
					printf ( "Language: English\n" );
				}
				if ( strcmp ( geLanguage, "PT" ) == 0 )
				{
					printf ( "GelaDOS - Ajuda.\n" );
					printf ( "Yaaaar, marujo, preste atencao que so falarei uma vez:\n" );
					printf ( "Voce esta em um sistema de avaliação de cervejas.\n" );
					printf ( "Nele voce pode: \n" );
					printf ( "\n" );
					printf ( "Se estiver no grupo [Recruta]: \n" );
					printf ( "- Avaliar cervejas\n" );
					printf ( "- Convidar novos recrutas\n" );
					printf ( "\n" );
					printf ( "Se estiver no grupo [Pirata Experiente]: \n" );
					printf ( "- Avaliar e Criar novas cervejas no sistema\n" );
					printf ( "- Convidar novos recrutas e Piratas Experientes\n" );
					printf ( "- Banir ou perdoar recrutas \n" );
					printf ( "\n" );
					printf ( "Se estiver no grupo [Capitao]: \n" );
					printf ( "- Avaliar e Criar novas cervejas no sistema\n" );
					printf ( "- Convidar novos recrutas, piratas experientes e capitaes ( Vai da-los barcos? ) \n\n" );
					printf ( "Toda interacao pode ser feita pelo ambiente WEB\n" );
					printf ( "O ambiente CLI foi criado para uma interacao mais do administrador\n" );
					printf ( "A maioria das acoes e feita apos o login do usuario.\n" );
				}
				else
				{
					printf ( "Yaaaar, sailor, pay atention, 'cause I'll say it just once: \n" );
					printf ( "You're in a Beer Rating System.\n" );
					printf ( "You are able to: \n" );
					printf ( "\n" );
					printf ( "If a [Recruit]: \n" );
					printf ( "- Rate beers\n" );
					printf ( "- Invite new recruits\n" );
					printf ( "\n" );
					printf ( "If an [Adept Pirate]: \n" );
					printf ( "- Rate and create new beers in our system\n" );
					printf ( "- Invite new recruits and Adept Pirates\n" );
					printf ( "- Ban or Unban recruits \n" );
					printf ( "\n" );
					printf ( "If a [Captain]: \n" );
					printf ( "- Rate and create new beers in our system\n" );
					printf ( "- Invite new recruits, adept pirates and captains ( you'll give 'em a boat? ) \n\n" );
					printf ( "Every interaction will be done in the WEB environment.\n\n" );
					printf ( "The CLI environment was created to the administrator, in general.\n" );
					printf ( "Most of the interactions are made after you login.\n" );
				}
				return OK;
				break;
			/*-----------------------------------------------*/
			
			default:
				printf ("Invalid Option.\n");
		}
	}

	mlCgiFreeResources();
	return OK;
}

/*
		printf ("     <a href=\"#\" onClick=\"setCookie('%s','%s','%u')\">Set Cookie</a><br>\n", idString, cookieValue, MINUTES_LOGGED_IN );
		printf ("     <a href=\"#\" onClick=\"getCookie('%s')\">Get Cookie</a><br>\n", idString );
		printf ("     <a href=\"#\" onClick=\"checkCookie()\">Check Cookie</a><br>\n" );
		printf ("     Cookie:|%s| Ip:|%s|<br>Tempo Restante:|%lf|<br>", cookieValue, ipString, (double)(tempo - time ( NULL ))/3600 );
*/
		
/* $RCSfile: geCLI.c,v $ */
