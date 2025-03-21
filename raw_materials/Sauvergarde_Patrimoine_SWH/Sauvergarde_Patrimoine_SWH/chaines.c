/*chaines
**************************************************************************
*                                                                        *
*   Giraudon - INRIA sophia     :      octobre   1984                    *
*                                                                        *
**************************************************************************
*                                                                        *
* Ce programme est le lanceur de l'executable "newkchain"  .             *
* Il recupere les  arguments :                                           *
* - inname   nom de l'image d'entree (eventuellement un pipe) (sur un    *
*            bit, octet ou entier 4 octets seulement)                    *
* - outname  nom du fichier des chaines                                  *
* - t (taille) dimension supplementaire des maillons (par defaut = 0)    *
* - ix = origine en colonne de l'image (par defaut = 1)                  *
* - iy = origine en lignes de l'image  (par defaut = 1)                  *
* - ex = extension en x de l'image (par defaut lfmt(0))                  *
* - ey = extension en y de l'image (par defaut lfmt(1))                  *
* - b  = taille des chaines a supprimer ( 2 par defaut).		 *
* - m  = extraction des metachaines.                                     *
* - ang = valeur du seuil en distance pour l'approximation polygonale    *
*               par defaut 2.2                                           *
* - d = debugging du programme                                           *
*       0 = pas de sortie  1 = sortie reduite  2 = sortie complete       *
*       par defaut 0                                                     *
* - seg = 1 si on desire l'approximation polygonale sans moindre carre   *
*         2 si "    "          "             "      avec  "        "     *
*         0 par defaut (pas d'approximation polygonale)                  *
*                                                                        *
*                                                                        *
**************************************************************************
*/

#include <inrimage/image.h>
#include <stdio.h>
#include <include/chaine/maillon.h>

extern debug_,koctet_;

int  xargc;
char **xargv;

main(argc,argv)
int argc;
char *argv[];
{
	char inname[80],outname[80];
	int taille = 0, ix = 1, iy = 1, ibruit = 2, meta = 0, idebug = 0;
	int ixfin, iyfin, lfmt[9], seg = 0, octet, somipoids, sizint, mcolo;
	int ipoids[3], *ichn;
	float VAL = 2.2;
	float *imag;

	struct maillon *kbuf;
	struct image *nf , *image_() ;

	xargc = argc;
	xargv = argv;

	koctet_ = 3000;		/* valeur par defaut */
	getopt("-k","",0    ,"%d",&koctet_  ,"",0);

	debug_ = getopt("-D","",0   ,"",0    ,"",0);

	if ( getopt("-","",0,"",0,"",0) || !getopt("","%s",inname,"",0,"",0) )
		{
		strcpy(inname,"<"); /* standart input */
		}
	if ( !getopt("","%s",outname,"",0,"",0) )
		{
		strcpy(outname,">"); /* standart output */
		}
	getopt("-t","",0,"%d",&taille,"",0);
	if ( taille >  4 ) 
		{
		fprintf(stderr," la taille ne peut depasser 4\n") ;
		exit(-1) ;
		}
	nf = image_(inname,"e","",lfmt) ;

	ixfin = lfmt[0] ;
	iyfin = lfmt[1] ;

	getopt("-ix"  ,"",0,"%d",&ix    ,"",0);
	getopt("-iy"  ,"",0,"%d",&iy    ,"",0);
	getopt("-ex" ,"",0,"%d",&ixfin ,"",0);
	getopt("-ey" ,"",0,"%d",&iyfin ,"",0);
	getopt("-seg","",0,"%d",&seg   ,"",0);
	getopt("-b"  ,"",0,"%d",&ibruit,"",0);
	getopt("-d"  ,"",0,"%d",&idebug,"",0);
	getopt("-m"  ,"",0,"%d",&meta,"",0);
	getopt("-ang"  ,"",0,"%f",&VAL,"",0);

	if ( !testopt() ) exit(-1);

	if (idebug >= 1) {
		printf(" INRIA Sophia \n") ;
		printf(" Linking Program Version of June 91\n");
		printf(" Any problem : contact G. Giraudon at mirsa.inria.fr \n") ;
	}


/*	if (ibruit < 2 ) ibruit = 2 ; Modification du 08 Avril 1989 
					sur les besoins de A. Meygret 
					mais ibruit = 2 par defaut */

/* initialisation du tableau ipoids */
	ipoids[0]=30;
	ipoids[1]=10;
	ipoids[2]=1;
	somipoids=ipoids[0]+ipoids[1]+ipoids[2];

/* calcul des tailles en int reservees */
	octet = koctet_ * 1024;
	sizint=sizeof(int);
	ipoids[0] = (int)(octet * ipoids[0]) / (sizint * somipoids);
	ipoids[1] = (int)(octet * ipoids[1]) / (sizeof(float) * somipoids);
	ipoids[2] = (int)(octet * ipoids[2]) / (sizint * somipoids);

/* allocation dynamique des tableaux */
	ichn = (int *) malloc(ipoids[0] * sizint);
	imag = (float *) malloc(ipoids[1] * sizeof(float));
	kbuf = (struct maillon *) malloc(ipoids[2] * sizint);

	mcolo = lfmt[0];

	chapri_(ichn,imag,kbuf,ipoids,nf,lfmt,&mcolo,&ix,&iy,&ixfin,&iyfin,
		&taille,&ibruit,meta,&idebug,&seg,&VAL,outname);
}
