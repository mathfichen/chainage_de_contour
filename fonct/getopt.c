#include <stdio.h>
#include "image.h"


/* procedure de decodage de la chaine d'options 

   on passe en argument 

	l'adresse de la string representant l'option cherchee ( si '\0'
					toute string convient )
	le format de cette string ( si ce format est '\0' on ne decode pas
					l'option )
	la variable de reception de l'option ( utilisee si format != '\0' )
	le format de l'option liee 1 ( meme principe )
	la variable receptrice
	le format de l'option liee 2 ( meme principe )
	la variable receptrice
  */


getopt(opt,fopt,varopt,f1,v1,f2,v2)

char *opt;	/* option cherchee */
char *fopt;	/* format de l'option */
char *varopt;	/* pointeur sur la variable receptrice 
		   de type char pour satisfaire tous les cas */
char *f1;	/* format de l'option liee 1 */
char *v1;	/* pointeur sur la variable liee 1 */
char *f2;	/* format de l'option liee 2 */
char *v2;	/* pointeur sur la variable liee 2 */

{

	int i;


	for (i=1 ; i<xargc ; ++i) /* on scanne tous les arg */

		{ /* les arguments deja utilises == chaine vide */

		if (*xargv[i] != '\0')

			{ /* si option == '\0' toute chaine convient 
				sauf celles debutant par "-" ou un chiffre */

			if ( (*opt == '\0' &&
				(*xargv[i] != '-'|| strcmp("-",xargv[i])==0) &&
				(*xargv[i] < '0' || *xargv[i] > '9'))

				|| strcmp(opt,xargv[i]) == 0)
			
				{

				if (*fopt != '\0') sscanf(xargv[i],fopt,varopt);
				*xargv[i] = '\0' ; /* on invalide l'argument */

				if (*f1 != '\0' && ++i < xargc) 
					{
					sscanf(xargv[i],f1,v1);
					*xargv[i] = '\0'; /* on invalide
								l'argument */
					}
				if (*f2 != '\0' && ++i < xargc) 
					{
					sscanf(xargv[i],f2,v2);
					*xargv[i] = '\0'; /* on invalide
								l'argument */
					}
				
				return(1);	/* on a trouve l'option */

				}

			}

		} /* fin de boucle sur les arguments */

	return(0);	/* on a pas trouve l'option */

}


/*------------------------------------------------------------------------*/

/* procedure de test des options non 'consommees' */



testopt()


{

	int i,retour;

	retour = 1;

	for (i=1 ; i <xargc ; ++i )

		{

		if (*xargv[i] != '\0') /* c'est une option non consommee */
			{
			if ( strcmp(xargv[i],"-x") == 0 ) {
				i++;
				continue;
			}
			if ( strcmp(xargv[i],"-y") == 0 ) {
				i++;
				continue;
			}
			if ( strcmp(xargv[i],"-z") == 0 ) {
				i++;
				continue;
			}
			if ( strcmp(xargv[i],"-v") == 0 ) {
				i++;
				continue;
			}
			if ( strcmp(xargv[i],"-o") == 0 ) {
				i++;
				continue;
			}
			if ( strcmp(xargv[i],"-b") == 0 ) {
				i++;
				continue;
			}
			if ( strcmp(xargv[i],"-e") == 0 ) {
				i++;
				continue;
			}
			if ( strcmp(xargv[i],"-r") == 0 ) {
				continue;
			}
			if ( strcmp(xargv[i],"-f") == 0 ) {
				continue;
			}
			if ( strcmp(xargv[i],"-p") == 0 ) {
				continue;
			}
			if ( strcmp(xargv[i],"-s") == 0 ) {
				continue;
			}

			fprintf(stderr,"%s:%s option non reconnue\n",xargv[0],
								xargv[i]);
			retour = 0;
			}

		}

	return(retour);
}
