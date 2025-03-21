/*apoly
+-----------------------------------------------------------------------+
!									!
!	Giraudon -INRIA Sophia     :      Avril 1988			!
!					(revisite de 16/11/93)          !
!									!
+-----------------------------------------------------------------------+
!									!
!	Ce programme permet de realiser l'approximation polygonale	!
!	d'un fichier de chaine.						!
!	Lecture des chaines avec le nouveau standart.			!
!	Differents filtres sur les chaines peuvent etre utilises 	!
!	La lecture des maillons se fait en short. 			!
!									!
!	En entree :							!
!	chaine		:	nom du fichier de chaine en entree	!
!	segment		:	nom du fichier de segment en sortie	!
!	nu		:	si nu = -1 alors appel interactif des   !
!					numeros des segments            !
!				si nu = 0 (par defaut) alors		!
!				on lit sequentiellement tous les seg.	!
!				si nu > 0 alors lecture que de ce segm. !
!				si nu=0 alors le programme peut recevoir!
!				un pipe en entree			!
!	lcmax		:	si nu=0 , alors lcmax est un seuil	!
!				(borne sup) sur la longueur de la chaine!
!				 a afficher				!
!	lcmin		:	si nu=0 , alors lcmin est un seuil	!
!				(borne inf) sur la longueur de la chaine!
!				 a afficher				!
!				Par defaut lcmax =10000 , lcmin = 0	!
!	ngmax		:	si nu=0 , alors ngmax est un seuil	!
!				(borne sup) sur la moyenne des niveaux  !
!				de gris de la chaine a afficher		!
!	ngmin		:	si nu=0 , alors lnmin est un seuil	!
!				(borne inf) sur la moyenne  de la chaine!
!				 a afficher				!
!				Par defaut lcmax =1000. , lcmin = 0.	!
!	vamax		:	si nu=0 , alors ngmax est un seuil	!
!				(borne sup) sur la variance des niveaux !
!				de gris de la chaine a afficher		!
!	vamin		:	si nu=0 , alors lnmin est un seuil	!
!				(borne inf) sur la variance de la chaine!
!				 a afficher				!
!				Par defaut lcmax =10000. , lcmin = 0.	!
!	FER		:	Si nu=0 et si FER present alors on 	!
!				n'affichera que les chaines fermees.	!
!	sent		:	si sent present on ecrira sur le stan.	!
!				de sortie les entetes de chaque chaine  !
!									!
!	app		:	= 1 pour approx sans moindres carres	!
!				= 2  "    "     avec  "         "	!
!				(par defaut = 2)			!
!	ang		:	valeur d angle de coupure ( en pixel)   !
!				par defaut 2.2				!
!									!
+-----------------------------------------------------------------------+

		 structure des chaines sur support externe 

   JO short :    nombre et liste des numeros des chaines connectees en tete
   JO short :    nombre et liste des numeros des chaines connectees en queue
   1 short  :	 si cette chaine est un contour ferme

   CHA float : 	moyenne des niveaux de gris
		variance



  structure des segments sur support externe 
   BA float : 	longueur du segment
		moyenne des niveaux de gris
		variance
		x milieu
		y milieu
		cosinus
		sinus
		orientation en degre 
		erreur relative a la droite
   ID short :	numero de la chaine mere
		nombre de maillons de cette chaine
		si cette chaine est un contour ferme
		nombre de segments autre qui composent cette chaine
		libre : doit etre utilise par segori
 */

#include <inrimage/image.h> 
#include <dzv.h>
#include <stdio.h>
#include <math.h>

#define MA 7   /* on ne peut creer des maillons a plus de 7 elements */
#define CHA 2   /* nombre de valeur flottante de l'entete chaine */
#define JO 5   /* nombre short par famille */

#define BA 9   /* nombre de valeur flottante de l'entete segment*/
#define ID 5   /* nombre de "short" de l'entete segment  */

extern debug_,koctet_;

double ANGLE_d ;
double varmax_d, varmin_d, nvgmax_d, nvgmin_d ;

int xargc;
char **xargv;

main(argc,argv)
int argc;
char *argv[];
{
	xargc = argc ;
	xargv = argv ;
	
	{
	DZV *dzv_fic , *dzv_elt , *dzv_mai ;
	DZV *dzv_fic_s , *dzv_elt_s , *dzv_mai_s ;

        int     dzv_dimx, dzv_dimy;

	char *typage , *chaine  ,  *segment , *calloc() ;

	int *taillz , nb_zone , ta_mai, ta_jo , ta_mo ;
	int head ,lgmax , lgmin , numero , fd_c , fd_s, nomb , vfer , Icar=2 , nb_seg;
	short  *jonct , *joncq , *identif;
	char *TYP, COMM[80] ;
	float *moye , nvgmax , nvgmin, varmax , varmin , ANGLE = 2.2 , *bavre;


/* Initialisation des variables par defaut */
	head =  lgmin = vfer =  0 ;
	nb_seg = numero = 0 ;
	lgmax = 10000 ; nvgmax = 1000. ; varmax = 10000. ; nvgmin = 1. - 1000. ;
	varmin = 1. - 1000.;

                              /********************************/
                               /* Initialisation des arguments */
 
 
        arg_init(xargc,xargv,"  Approximation polygonale de chaines DZV (sans calcul de l'orientation)" ,
                                "15/11/93 (inrimage version 4.3)",
                                " G. Giraudon ") ;
 
      arg_get("%S","Nom du fichier des chaines (entree - DZV)",&chaine) ;
      arg_get("%S","Nom du fichier des segments (sortie - DZV)",&segment) ;

      arg_get(""," ");
      arg_get("","/* *********************** Parametres ********************** */");

arg_get("-app %d 2","Options : \n 1 approx. poly. sans moindres carres \n 2 approx. poly. avec moindres carres",&Icar) ;
        arg_get(""," ");
        arg_get("","/* *********************** Options ********************** */");
arg_get("-nu %d 0","Options : \n 0 Traitement de toutes les chaines \n x traitement de la chaine numero x \n -1 traitement interactif",&numero) ;
arg_get("-ang %lf 2.2","Valeur du cone pour l'approx. polyg.",&ANGLE_d) ;

arg_get("-lcmax %d 10000","Longueur maximale des chaines a traiter",&lgmax) ;
arg_get("-lcmin %d 0","Longueur minimale des chaines a traiter",&lgmin) ;
arg_get("-ngmax %lf 10000.","Si nu=0, Borne superieure sur la moyenne des niveaux de gris des chaines a traiter",&nvgmax_d) ;
arg_get("-ngmin %lf 0.","Si nu=0, Borne inferieure sur la moyenne des niveaux de gris des chaines a traiter",&nvgmin_d) ;
arg_get("-vamax %lf 10000.","Si nu=0, Borne superieure sur la variance des niveaux de gris des chaines a traiter",&varmax_d) ;
arg_get("-vamin %lf 0.","Si nu=0, Borne inferieure sur la variance des niveaux de gris des chaines a traiter",&varmin_d) ;
arg_get("-FER %d 0","Options : \n 0 Si nu=0, Traitement de toutes les chaines \n 1 Si nu=0 traitement des chaines fermees uniquement",&vfer) ;
arg_get("-sent %d 0","Flag : si 1 alors on affiche sur stdout l'entete des chaines",&head) ;
         arg_end() ;

	ANGLE = (float)ANGLE_d ; nvgmax = (float)nvgmax_d ; nvgmin = (float)nvgmin_d;
	varmax = (float)varmax_d ; varmin = (float)varmin_d ;


	pr_fic("stderr",chaine) ;	

/* OUVERTURE DU FICHIER DE CHAINE */
	fd_c = op_fic_base(chaine,&TYP,&nomb,&dzv_fic,&dzv_elt,&dzv_mai);
							de_bug("ouverture mal passee") ;
	if ( strcmp("chaines",TYP) )
		{
		  fprintf(stderr," Le fichier n'est pas du type chaines mais du type %s \n",TYP);
		exit(-1) ;
		}
	re_fic(fd_c,COMM, &dzv_dimx, &dzv_dimy ); 	de_bug("re_fic:  mal passee") ;

	
/* Definition du fichier sur disque auquel on s'attend */
	re_dzv(dzv_elt,&nb_zone,&taillz,&typage) ;
	ta_jo = taillz[1] ; 	ta_mo = taillz[4] ;
	if ( nb_zone != 4 && ta_jo != 5 && ta_mo != 2 )
		{
		 fprintf(stderr," La definition de dzv_elt n'est pas celle du fichier lu \n") ;
		 exit(-1) ;
		}
	jonct = (short*) calloc( ta_jo , sizeof(short) ) ;
	joncq = (short*) calloc( ta_jo , sizeof(short) ) ;
	identif = (short*) calloc( ID , sizeof(short) ) ;
	moye = (float*) calloc( ta_mo , sizeof(float) ) ;
	bavre = (float*) calloc( BA , sizeof(float) ) ;

	re_dzv(dzv_mai,&nb_zone,&taillz,&typage) ;
	ta_mai = taillz[1] ;
	if (nb_zone != 1 && ta_mai != 3  )
		{
		 fprintf(stderr," La definition de dzv_mai n'est pas celle du fichier lu \n") ;
		 exit(-1) ;
		}


/* DEFINITION DU FICHIER SEGMENT SUR DISQUE */

	dzv_fic_s = cr_dzv( 3, 20,"char","segments",1,"int","dimx",1,"int","dimy");
						de_bug("dzv_fic mal passe") ;
	dzv_elt_s = cr_dzv(2,BA,"float","longueur moy varia xmi ymi cos sin orie err",
			     ID,"short","nucha nbmai fermeture nbseg vide") ;
						de_bug("dzv_elt mal passe") ;
	dzv_mai_s = cr_dzv(1,ta_mai,"float","x y ng") ;
						de_bug("dzv_mai mal passe") ;

	fd_s = cr_fic_base(segment,"segments",nomb,dzv_fic_s,dzv_elt_s,dzv_mai_s);
						de_bug("creation mal passee") ;

	if (Icar == 1)
	{
		wr_fic(fd_s,"ss moind. carre",&dzv_dimx, &dzv_dimy );
	}
	else
	{
		wr_fic(fd_s,"avec moind. carre",&dzv_dimx, &dzv_dimy );
	}

	de_bug("wr_fic :  mal passee") ;

/* FIN DE CREATION DU FICHIER SEGMENT */

	if (numero == 0 )
		{
labes :
	lfcars(fd_c,fd_s,ta_mai,jonct,joncq,moye,head,vfer,lgmax,lgmin,nvgmax,nvgmin,varmax,varmin,Icar,ANGLE,bavre,identif,&nb_seg) ;
		goto fin ;
		}
	

	if (numero == -1)
		{
laber : 
 	fprintf(stderr," \n Donner le numero de la chaine (-1 pour fin) : ") ;
	scanf("%d",&numero) ;
	if (numero <= -1) goto fin ;
	if (numero == 0) goto labes ;

	lfcarr(fd_c,fd_s,numero,ta_mai,jonct,joncq,moye,head,Icar,ANGLE,bavre,identif,&nb_seg) ;  
	goto laber ;
		}

/* ici on lit un seul segment donne par numero et on sort */
	lfcarr(fd_c,fd_s,numero,ta_mai,jonct,joncq,moye,head,Icar,ANGLE,bavre,identif,&nb_seg) ;  

fin :  
	fprintf(stderr," nombre de segment %d :\n ",nb_seg) ;
	cl_fic( fd_s) ;
	cl_fic(fd_c) ;
	exit(1) ;


	}
 }




	/************************************************
	*  lecture sequentielle d'un element du fichier *
	*************************************************/

lfcars(fd_c,fd_s,ta_mai,jonct,joncq,moye,head,vfer,lgmax,lgmin,nvgmax,nvgmin,varmax,varmin,Icar,ANGLE,bavre,identif,nb_seg) 

int fd_c, fd_s, ta_mai , vfer , Icar , head ,  lgmax , lgmin  , *nb_seg;
float  bavre[] , moye[]  , nvgmax, nvgmin , varmax , varmin ,ANGLE ;
short jonct[] ,joncq[] , identif[] ;
{

	int numero, nb_mai , k , first , VALI, FAR , index;
	short ferme ;
	char *calloc() ;
	static	ancien_nb_maillons=0;
	static int *MAILLONS=NULL ;


	while ( re_elt(fd_c,&numero,&nb_mai,jonct,joncq,&ferme,moye) )
	{
		de_bug("erreur sur la lecture ") ;

		index = nb_mai * ta_mai ;
		if (nb_mai > ancien_nb_maillons )
		{
			if (MAILLONS != NULL) free( MAILLONS );
			MAILLONS = (int*) calloc(index , sizeof( int) );
			ancien_nb_maillons = nb_mai;
		}
		re_maillons( fd_c, MAILLONS, nb_mai , ta_mai , index);


/* Test sur les parametres de validilite de la chaine */
		 VALI = 0 ;
		 if ( (nb_mai > lgmin) && (nb_mai < lgmax) && (moye[0] > nvgmin) && (moye[0] < nvgmax) && (moye[1] > varmin) && (moye[1] < varmax) ) VALI = 1 ;
		 FAR = 0 ;
		 if ((ferme != 0) && vfer) FAR = 1 ;


  		if ((vfer == 0) && VALI )
		{
			approx(fd_s,numero,nb_mai,ta_mai,MAILLONS,ferme,ANGLE,bavre,identif,Icar,nb_seg) ;
			if (head) imprhead(numero,nb_mai,jonct,joncq,ferme,moye) ; 

		}
  		if (VALI && FAR)
		{
			approx(fd_s,numero,nb_mai,ta_mai,MAILLONS,ferme,ANGLE,bavre,identif,Icar,nb_seg) ;
			if (head) imprhead(numero,nb_mai,jonct,joncq,ferme,moye) ; 
		}


	}

	  return(0) ;
  }



	/*******************************************
	*  lecture directe d'un element du fichier *
	********************************************/

lfcarr(fd_c,fd_s,num_elt,ta_mai,jonct,joncq,moye,head,Icar,ANGLE,bavre,identif,nb_seg) 
int fd_c, fd_s, num_elt , ta_mai , head ,Icar  , *nb_seg ;
float  moye[] , ANGLE , bavre[];
short  jonct[] , joncq[] , identif[];
{

	int numero, nb_mai , index;
	short ferme ;
	char *calloc() ;

	static	ancien_nb_maillons=0;
	static int *MAILLONS=NULL ;

	if ( ra_fic(fd_c,num_elt,0) )
	{
				de_bug(" ra_fic mal passe ") ;
		re_elt(fd_c,&numero,&nb_mai,jonct,joncq,&ferme,moye) ;
				de_bug(" Erreur sur la lecture d'un element ") ;

		index = nb_mai * ta_mai ;
		if (nb_mai > ancien_nb_maillons )
		{
			if (MAILLONS != NULL) free( MAILLONS );
			MAILLONS = (int*) calloc(index , sizeof( int) );
			ancien_nb_maillons = nb_mai;
		}
		re_maillons( fd_c, MAILLONS, nb_mai , ta_mai , index);

		approx(fd_s,numero,nb_mai,ta_mai,MAILLONS,ferme,ANGLE,bavre,identif,Icar,nb_seg) ;
		if (head) imprhead(numero,nb_mai,jonct,joncq,ferme,moye) ; 
	}


 return(1) ;
}



/***************************************************\
* 						    *
* Lecture des maillons et stockage dans un buffer.  *
* 						    *
* Ce buffer est alloue dynamiquement si necessaire  *
* 						    *
\***************************************************/
		
int re_maillons( fd_c, MAILLONS,  nb_mai , ta_mai ,index)
	int *MAILLONS ;
	int  nb_mai, fd_c,ta_mai,index;
	{
	short	*intermediaire , *tab_maillons;

	if (nb_mai == 0) return( 1 );
	if (MAILLONS == NULL ) return (-1) ;

	tab_maillons = (short *) MAILLONS ;
	intermediaire = tab_maillons;

	while ( re_mai( fd_c, intermediaire) ) intermediaire += ta_mai ;

	while ( index >= 0) {
				index-- ;
				*(MAILLONS + index) = (int) *(tab_maillons + index) ;

			    }
	}



	/********************************************************
	 *	impression sur stdout de l'entete du segment	*
	 ********************************************************/

int imprhead(numero,nb_mail,jonct,joncq,ferme,moye) 
float moye[] ;
int   numero ,  nb_mail ;
short jonct[] , joncq[] , ferme ;
	{
	int nblien , i ;

	fprintf(stdout,"\n\n ****** Caracteristiques de la chaine numero : %d \n",numero) ;

	fprintf(stdout,"        Nombre de maillons : %d\n",nb_mail) ;
	
	fprintf(stdout,"        Moyenne (niv. gris) : %f    Variance  : %f \n",moye[0],moye[1]) ;

	nblien = (int)jonct[0] ;
	fprintf(stdout,"        Nbre de lien en tete : %d \n",nblien) ;
	if ( nblien != 0)
	{
	for(i=0; i < nblien ; i++)
	fprintf(stdout,"        - Numero de la chaine en lien %d \n",(int)jonct[i+1]) ;
	}

	nblien = (int)joncq[0] ;
	fprintf(stdout,"        Nbre de lien en queue : %d \n",nblien) ;
	if ( nblien != 0)
	{
	for(i=0; i < nblien ; i++)
	fprintf(stdout,"        - Numero de la chaine en lien %d \n",(int)joncq[i+1]) ;
	}

	if (ferme)
		fprintf(stdout," Cette chaine est une boucle fermee \n") ;
	

	return(1) ;
	}



