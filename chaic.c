#include <stdio.h>
#include <include/chaine/maillon.h>
#include <include/chaine/common_ext.h>

#define OPEN	0
#define CLOSE	1
#define CONTOUR 2
#define NIL	0
#define MAGIC	0xce

extern char **xargv;
extern xargc;

struct ext_chaine	/* structure de la chaine sur support externe */
	{
	int no;			/* numero de la chaine */
	int nbm;		/* nombre de maillons */
	int taille;		/* taille d'un maillon */
	int entree[4];		/* 4 chaines en entree */
	int sortie[4];		/* 4 chaines en sortie */
	int contour;		/* contour : 0 si non contour */
	} ext_chaine;

struct maillon *core; 	/* variable statique sur l'espace de travail */
int numero = 0;		/* numero des chaines */
float pad[8];		/* padding pour le fichier resultat */


/****************************************************/
/*						    */
/* initialisation de la memoire en une chaine libre */
/*						    */
/****************************************************/
initch_(a,nbwords)
struct maillon *a;
int nbwords;
{
	register int i, nbmaillons;
	register struct maillon *maillon;
	
	core = a;

	nbmaillons = ( nbwords * 4 ) / sizeof(struct maillon);

	maillon = core;

	for ( i = 0 ; i < nbmaillons ; i++ )
		{
		maillon->succ = maillon + 1;
		maillon++;
		}

	(maillon - 1)->succ = NIL;

}

/**************************/
/* ouverture d'une chaine */
/**************************/
struct maillon *opench_()

{
	struct maillon *tete, *ext1, *ext2;
	register int i;

	if ( (tete = core->succ) == NIL ) return(NIL);	/* c'est plein */

	if ( (ext1 = tete->succ) == NIL ) return(NIL);	/* c'est plein */

	if ( (ext2 = ext1->succ) == NIL ) return(NIL);	/* c'est plein */

	tete->u.t.mode = OPEN;
	tete->u.t.dernier = ext2;
	tete->u.t.magic = MAGIC;
	tete->u.t.numero = ++numero;

	for ( i=0 ; i<4 ; ++i)
		{
		ext1->u.e.famille[i] = 0;	/* pas d'aieul */
		ext2->u.e.famille[i] = 0;	/* pas de fils */
		}

	core->succ = ext2->succ;
	ext2->succ = NIL;

	return(tete);
}
/****************************/
/* mise en place d'un aieul */
/****************************/
int aieul_(tete,aieul)

struct maillon *tete;
int    aieul;
{
	struct maillon *ext1;
	register int i;

	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s aieul:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}
	if( aieul == 0 )
		{
		fprintf(stdout,"%s aieul:%d n'est pas un aieul\n",
							xargv[0],aieul);
		exit(-1);
		}

	ext1 = tete->succ;
	if ( abs(aieul) == tete->u.t.numero) return(0);
	for ( i=0 ; i<4 ; ++i)
		{
		if ( ext1->u.e.famille[i] == 0 ) 
			{
			ext1->u.e.famille[i] = aieul ;
			return(1);
			}
		if ( abs(ext1->u.e.famille[i]) == abs(aieul) ) return(0);
		}
	return(0);
}
/******************************************/
/* mise en place d'un aieul dans refiliat */
/******************************************/
int aieul_b(tete,aieul)

struct maillon *tete;
int    aieul;
{
	struct maillon *ext1;
	register int i;

	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s aieul:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}
	if( aieul == 0 )
		{
		fprintf(stdout,"%s aieul:%d n'est pas un aieul\n",
							xargv[0],aieul);
		exit(-1);
		}

	ext1 = tete->succ;
	for ( i=0 ; i<4 ; ++i)
		{
		if ( ext1->u.e.famille[i] == 0 ) 
			{
			ext1->u.e.famille[i] = aieul ;
			return(1);
			}
		if ( ext1->u.e.famille[i] == aieul ) return(0);
		}
	return(0);
}
/************************************************************************/
/*									*/
/* SRAIEUL : fonction qui recherche dans les aieuls de <tete> la chaine */
/*	     de numero <n_ancien> et le remplace par <n_new>	   	*/
/*	     Retourne  vrai si tout c'est bien passe			*/
/*		       faux sinon.				        */
/************************************************************************/
int sraieul_(tete,n_ancien,n_new)

struct maillon *tete;
int    n_ancien,n_new;
{
	struct maillon *ext1;
	register int i;

	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s aieul:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}

	ext1 = tete->succ;
	for ( i=0 ; i<4 ; ++i)
		{
		if ( ext1->u.e.famille[i] == 0 ) break;
		if ( abs(ext1->u.e.famille[i]) == n_ancien )
			{ ext1->u.e.famille[i] = n_new; 
			  return(1);
			}
		}
	return(0);
}

/* ************************************************************************* */
/*                                                                           */
/*   LAIEUL   :   determine le nombre d' aieuls 'nb' de la chaine 'tete'     */
/*                la liste des aieuls est recopiee dans 'aieul'             */
/*                                                                           */
/* ************************************************************************* */
int *laieul_(tete,nb)

struct maillon *tete;
int *nb;
{
	struct maillon *ext1;
	register int   i;
	int            *aieul;

	*nb = 0;
	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s laieul:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}
	aieul = (tete->succ)->u.e.famille;
	
	for ( i=0 ; i<4 ; ++i)
		{
		if ( aieul[i]  == 0 ) break;
		else (*nb)++;
		}
	return(aieul);
}

/* ----------------------------------------------------------------------- */
/*                                                                         */
/*    SSAIEUL   :   Cette procedure supprime la chaine d'adresse "nume"    */
/*                  de la liste des aieuls de la chaine "tete" et          */
/*                  recompacte le tableau "famille" .                      */
/*                  Elle retourne "vrai" si elle a pu supprime             */
/*                                                                         */
/* ----------------------------------------------------------------------- */

int ssaieul_ (tete,nume)

struct maillon *tete ;
int nume ;

{
	struct maillon *ext1;
	register int i;
	int found = 0 ;
 
	if ( tete->u.t.magic != MAGIC )
		{
       		    fprintf(stdout,"%s ssaieul :%d n'est pas une chaine\n",
							xargv[0],tete);
		    exit(-1);
		}
	nume = abs(nume);
	if ( nume == 0 )
		{
		    fprintf(stdout," ssaieul : 0 n'est pas un aieul \n") ;
		    exit(-1);
		}

	ext1 = tete->succ ;
	for ( i=0 ; i<4 ; ++i)  /*  On cherche "nume"  */
		{
		    if (ext1->u.e.famille[i] == 0) break ;	
		    if (abs(ext1->u.e.famille[i]) == nume) found = i+1 ;
		}
	if (found)    /* On compacte le tableau "famille"    */
		{
		    ext1->u.e.famille[found-1] = ext1->u.e.famille[i-1] ;
		    ext1->u.e.famille[i-1] = 0 ;
		}
	return (found) ;
}


/***************************/
/* mise en place d'un fils */
/***************************/
int fils_(tete,fils)

struct maillon *tete;
int    fils;	
{
	struct maillon *ext2;
	register int i;

	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s fils:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}
	ext2 = (tete->succ)->succ;
	if ( abs(fils) == tete->u.t.numero) return(0);	
	for ( i=0 ; i<4 ; ++i)
		{
		
		if ( ext2->u.e.famille[i] == 0 )
			{
			ext2->u.e.famille[i] = fils ;
			return(1);
			}
		if ( abs(ext2->u.e.famille[i]) == abs(fils) ) return(0) ;
		}
	return(0);
}

/*****************************************/
/* mise en place d'un fils dans refiliat */
/*****************************************/
int fils_b(tete,fils)

struct maillon *tete;
int    fils;	
{
	struct maillon *ext2;
	register int i;

	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s fils:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}
	ext2 = (tete->succ)->succ;
	for ( i=0 ; i<4 ; ++i)
		{
		
		if ( ext2->u.e.famille[i] == 0 )
			{
			ext2->u.e.famille[i] = fils ;
			return(1);
			}
		if ( ext2->u.e.famille[i] == fils ) return(0) ;
		}
	return(0);
}
/************************************************************************/
/*									*/
/* SRFILS  : fonction qui recherche dans les fils de <tete> la chaine	*/
/*	     de numero <n_ancien> et le remplace par <n_new>	   	*/
/*	     Retourne  vrai si tout c'est bien passe			*/
/*		       faux sinon.				        */
/************************************************************************/
int srfils_(tete,n_ancien,n_new)

struct maillon *tete;
int    n_ancien,n_new;
{
	struct maillon *ext2;
	register int i;

	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s aieul:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}

	ext2 = (tete->succ)->succ;
	for ( i=0 ; i<4 ; ++i)
		{
		if ( ext2->u.e.famille[i] == 0 ) break;
		if ( abs(ext2->u.e.famille[i]) == n_ancien )
			{ ext2->u.e.famille[i] = n_new; 
			  return(1);
			}
		}
	return(0);
}

/* ************************************************************************ */
/*                                                                          */
/*    LFILS   :   determine le nombre de fils 'nb' de la chaine 'tete'      */
/*                la liste des fils est recopiee dans 'fils'                */
/*                                                                          */
/* ************************************************************************ */

int *lfils_(tete,nb)

struct maillon *tete;
int *nb;
{
	struct maillon *ext2;
	register int i;
	int      *fils;
	*nb = 0;
	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s lfils:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}

	fils = ((tete->succ)->succ)->u.e.famille;
	
	for ( i=0 ; i<4 ; ++i)
		{
		if ( fils[i] == 0 ) break;
		else (*nb)++;
		}
	return(fils);
}

/* ----------------------------------------------------------------------- */
/*                                                                         */
/*    SSFILS    :   Cette procedure supprime la chaine d'adresse "nume"    */
/*                  de la liste des fils de la chaine "tete" et            */
/*                  recompacte le tableau "famille" des fils .             */  
/*                  Elle retourne "vrai" si elle a pu supprime             */
/*                                                                         */
/* ----------------------------------------------------------------------- */

int ssfils_ (tete,nume)

struct maillon *tete ;
int nume ;  

{
	struct maillon *ext2;
	register int i;
	int found = 0 ;
 
	if ( tete->u.t.magic != MAGIC )
		{
       		    fprintf(stdout,"%s ssfils :%d n'est pas une chaine\n",
							xargv[0],tete);
		    exit(-1);
		}

	if ( nume == 0 )
		{
		    fprintf(stdout," ssfils : 0 n'est pas un aieul \n") ;
		    exit(-1);
		}
	nume = abs(nume);
	ext2 = (tete->succ)->succ ;
	for ( i=0 ; i<4 ; ++i)  /*  On cherche "nume"  */
		{
		    if (ext2->u.e.famille[i] == 0) break ;	
		    if (abs(ext2->u.e.famille[i]) == nume) found = i+1 ;
		}
	if (found)    /* On compacte le tableau "famille"    */
		{
		    ext2->u.e.famille[found-1] = ext2->u.e.famille[i-1] ;
		    ext2->u.e.famille[i-1] = 0 ;
		}
	return (found) ;
}

/**************************************/
/* ajout arriere d'un nouveau maillon */
/**************************************/

struct maillon *nwmail_(tete,iy,ix,niveau)

struct maillon *tete;
int ix,iy,niveau;
{
	register struct maillon *maillon;

	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s nwmail :%d n'est pas une chaine pixel %d %d\n",
							xargv[0],tete,ix,iy);
		return(NIL);
		}

	if ( (maillon = core->succ) == NIL ) return(NIL);/* c'est plein */ 

	core->succ = maillon->succ;
	maillon->succ = NIL;

	maillon->u.m.data[0] = ix;
	maillon->u.m.data[1] = iy;
	maillon->u.m.data[2] = niveau ;

	tete->u.t.dernier = (tete->u.t.dernier)->succ = maillon ;

	return(maillon);
}


/* recuperation du premier maillon */

fimail_(tete,ix,iy,niveau,err)

struct maillon **tete;
int *ix,*iy,*niveau,*err;
{
	register struct maillon *maillon;

	*err = 0;

	if( (*tete)->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s fimail :%d n'est pas une chaine pixel %d %d\n",
							xargv[0],*tete,*ix,*iy);
	
		*err = -1;
		return;

		}

	maillon = (((*tete)->succ)->succ)->succ ;

	*ix = maillon->u.m.data[0] ;
	*iy = maillon->u.m.data[1] ;
	*niveau = maillon->u.m.data[2];

}


/* recuperation du dernier maillon */

remail_(tete,ix,iy,niveau,err)

struct maillon **tete;
int *ix,*iy,*niveau,*err;
{
	register struct maillon *maillon;

	*err = 0;

	if( (*tete)->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s remail :%d n'est pas une chaine pixel %d %d\n",
							xargv[0],*tete,*ix,*iy);
	
		*err = -1;
		return;

		}

	maillon = (*tete)->u.t.dernier ;

	*ix = maillon->u.m.data[0] ;
	*iy = maillon->u.m.data[1] ;
	*niveau = maillon->u.m.data[2];

}

/* restitution d'une chaine */

free_chain(tete)

struct maillon *tete;
{
	
	(tete->u.t.dernier)->succ = core->succ ;
	tete->u.t.magic = 0;
	core->succ = tete;
}

/* ecriture d'une chaine */

wrtchn_(fd,tete,number,taille,dimy,vicom,tek)

int *fd,*taille,*vicom,*tek,*dimy, *number ;
struct maillon **tete;
{
	int ici;
	register struct maillon *ext1,*ext2,*maillon;
	register int i;
	int x,y;
	float rx,ry;
	char cmd[80];

	if( (*tete)->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s wrtchn:%d n'est pas une chaine\n",
							xargv[0],*tete);
		return;

		}

	if ( *taille < 0 )
		{
		fprintf(stdout,"%s:wrtchn taille du maillon %d\n",xargv[0],

								*taille);
		return;

		}

	ici = lseek(*fd,0L,1);
	lseek(*fd,sizeof(struct ext_chaine),1);

/*	ext_chaine.no = (*tete)->u.t.numero; */
	ext_chaine.no = *number ;

	ext_chaine.taille = *taille;
	ext_chaine.contour = (*tete)->u.t.mode & CONTOUR ;

	ext1 = (*tete)->succ;
	ext2 = ext1->succ;
	for ( i=0 ; i<4 ; i++)
		{
		ext_chaine.entree[i] = ext1->u.e.famille[i];
		ext_chaine.sortie[i] = ext2->u.e.famille[i];
		}


	maillon = ext2 ;
	i = 0;
	while ( (maillon = maillon->succ) != NIL )
		{
		i++;
		write(*fd,maillon->u.m.data,sizeof(maillon->u.m.data));
		write(*fd,pad,*taille * sizeof(float) ); /* trou pour les data */

#ifdef VICOM
		if ( *vicom )
			{
			x = maillon->u.m.data[0];
			y = maillon->u.m.data[1];
			sprintf(cmd,"vec 1(%d,%d,%d,%d,1,1)",y,x,y,x);
			cmdvic_(cmd);
			}
#endif VICOM

#ifdef TEK
		if ( *tek )
			{
			rx = maillon->u.m.data[0];
			ry = *dimy-maillon->u.m.data[1] + 1 ;
			movea_(&rx,&ry) ;
			drawa_(&rx,&ry) ;
			}
#endif TEK
		}		


	ext_chaine.nbm = i;
	lseek(*fd,ici,0);
	write(*fd,&ext_chaine,sizeof(struct ext_chaine));
	lseek(*fd,0L,2);
	free_chain(*tete);
}

/* ------------------------------------- */
/*                                       */
/* calcul du nb de maillons d'une chaine */
/*                                       */
/* ------------------------------------- */

nbmail_(tete)

struct maillon *tete;
{
	register struct maillon *ext1,*ext2,*maillon;
	register int i;

	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s nbmail:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}

	ext1 = tete->succ;
	ext2 = ext1->succ;

	maillon = ext2 ;
	i = 0;
	while ( (maillon = maillon->succ) != NIL ) i++;

	return(i);
}

/*****************************************************************************/
/*                                                                           */
/*   FUSCHN :  fusionne la chaine 'suite' en queue de la chaine 'tete'       */
/*             la chaine 'tete' herite des fils de 'suite'                   */
/*                                                                           */
/*****************************************************************************/


fuschn_(tete,suite)
struct maillon *tete,*suite ;

{	
	register int i;

	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s fuschn:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}

	if( suite->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s fuschn:%d n'est pas une chaine\n",
                                                        xargv[0],suite);
		exit(-1);
		}

	(tete->u.t.dernier)->succ =  ((suite->succ)->succ)->succ;
	tete->u.t.dernier = suite->u.t.dernier ;

	for ( i=0 ; i<4 ; ++i)
		{
		((tete->succ)->succ)->u.e.famille[i] = 
			((suite->succ)->succ)->u.e.famille[i] ;
		} 
}


/*****************************************************************************/
/*                                                                           */
/*      MIROIR   :   change l'orientation de la chaine 'tete'                */
/*                                                                           */
/*****************************************************************************/
miroir_(tete)
struct maillon *tete;
{
	register struct maillon *temp,*maillon,*ext2;
	register int x,i;

	if( tete->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s miroir:%d n'est pas une chaine\n",
							xargv[0],tete);
		exit(-1);
		}

	ext2 = (tete->succ)->succ;
	tete->u.t.dernier = ext2->succ;
	maillon = (ext2->succ)->succ ; /* 2eme maillon */

	while ( maillon != NIL )
		{
		temp = maillon->succ ;
		maillon->succ = ext2->succ ;
		ext2->succ = maillon;
		maillon = temp;
		}

	for ( i=0 ; i<4 ; ++i)
		{
			x = ext2->u.e.famille[i] ;
			ext2->u.e.famille[i] =(tete->succ)->u.e.famille[i];
			(tete->succ)->u.e.famille[i] = x;
		} 

	(tete->u.t.dernier)->succ = NIL;
}

/***********************************/
/* ecriture d'une chaine sur stdout*/
/***********************************/
dump_(tete) 
	struct maillon *tete;
{
	register struct maillon *ext1,*ext2,*maillon;
	register i;
	char rep[3];
	printf("visualisation chaine : %d ...",tete->u.t.numero);
	ext1 = tete->succ;
	ext2 = ext1->succ;
	printf("aieuls =");
	for ( i=0 ; i<4 ; i++)
		{
		printf("%d,",ext1->u.e.famille[i]);
		}
	printf("fils =");
	for ( i=0 ; i<4 ; i++)
		{
		printf("%d,",ext2->u.e.famille[i]);
		}
	printf("\n");
	printf("   Liste des maillons ? (o/n) ");
	scanf("%s",rep);
	if (rep[0] != 'o' ) return;
	maillon = ext2 ;
	while ( (maillon = maillon->succ) != NIL )
		{
		printf("(%d,%d)",maillon->u.m.data[0],	maillon->u.m.data[1]);
		}		
	printf("\n");
}

/* remplissage d'un vecteur avec les maillons */

getmail_(tete,buf) 
struct maillon **tete;
register struct ms *buf;
{

	register struct maillon *ext1,*ext2,*maillon;
	register int i = 0 ;

	ext1 = (*tete)->succ;
	ext2 = ext1->succ;
	
	maillon = ext2 ;
	while ( (maillon = maillon->succ) != NIL )
		{
		buf->data[0] = maillon->u.m.data[0];
		buf->data[1] = maillon->u.m.data[1];
		(buf++)->data[2] = maillon->u.m.data[2];
		i++ ;
		}		

	return(i) ;
}




/***********************************************************/
/* renvoi la moyenne des niveaux de gris de la chaine tete */
/***********************************************************/

float getmoy_(tete,nb) 
struct maillon *tete;
int *nb ;
{

	register struct maillon *ext1,*ext2,*maillon;
	register int i = 0 ;
	float moyenne = 0. ;

	ext1 = (tete)->succ;
	ext2 = ext1->succ;
	
	maillon = ext2 ;
	while ( (maillon = maillon->succ) != NIL )
		{
		moyenne = moyenne + maillon->u.m.data[2];
		i++ ;
		}		
	moyenne = moyenne / (float) i ;

	*nb = i ;
	return(moyenne) ;
}












/****************************************************************************/
/*									    */
/*	procedure qui teste les pixels du futur dans imag		    */
/*									    */
/****************************************************************************/
float ptest5668()
{ register float *p5,*p6,*p7,*p8;

  p5 = (imag + lap + 1);
  p6 = (imag + lap - 1 + mcolo);
  p7 = p6 + 1;
  p8 = p7 + 1;
  return( (*p5 && *p6) || (*p6 && *p8) );
/* Version du 25/08/87 : pour resoudre le probleme de la coordonnee ix qui
n'etait pas correcte : imag+lap au lieu de imag+lap+mcolo */
}

float ptest5()
	{ return( *(imag + lap + 1 ));
/* Version du 25/08/87 : pour resoudre le probleme de la coordonnee ix qui
n'etait pas correcte : imag+lap au lieu de imag+lap+mcolo */
	}
/****************************************************************************/
/*									    */
/*	procedure qui teste le pixel courant	: 			    */
/*		Si le pixel n'est pas significatif 		 	    */
/*			alors retourne   0 (pixel non significatif)	    */
/*		Sinon         retourne   niveau de gris du pixel	    */
/*									    */
/****************************************************************************/
float pixel_()
	{ return( *( imag + lap ));
/* Version du 25/08/87 : pour resoudre le probleme de la coordonnee ix qui
n'etait pas correcte : imag+lap au lieu de imag+lap+mcolo */
	}
/****************************************************************************/
/*									    */
/*	Initialisation de la ligne l de itab avec des 'X'                   */
/*									    */
/****************************************************************************/

initab_(l,ixint,ixbout,itab)
	int l, ixint ,ixbout ;
	int itab[][2][4];
{	int k ;
	for (k= ixint; k<= ixbout; k++) itab[k][l][1] = 'X' ;	
}

/*****************************************************************************/
/*									     */
/*	A partir du tableau ITAB determine la configuration du pixel traite  */
/*	en retournant l'entier V					     */
/*									     */
/*****************************************************************************/

int configur_( j ,itab)
	int j;
	int itab[][2][4];
{
	register int ncov = 0 ; 
	register int ncfv = 0 ;
	static int poids[] = {1,8,4,2} ;
	register int k ,vitab;
	char deb_ch[10];
	if (idebug ==2) printf("configur : entre dans configur\n");

	/*--------- traitement de la ligne l1 -------------*/
	for (k=0 ; k<3; k++)
	     {   
	      if ((vitab=itab[j+k][l1][1]) <= 'F') ncfv += poids[k] ;
	      else if (vitab <= 'P') ncov += poids[k] ;
	     }

	/*--------- Traitement de la lgne l2   ------------*/
	if ((vitab=itab[j][l2][1]) <= 'F') ncfv += poids[3] ;
	else if (vitab <= 'P') ncov += poids[3] ;	

	if (idebug ==2) 
	  { printf("configur : itab = \n");
	    for (k=0;k<3; k++) 
		printf("%c",itab[j+k][l1][1]);
	    printf("\n") ;
	    for (k=0;k<3; k++) 
		printf("%c",itab[j+k][l2][1]);
	    printf("\n") ;
	  }
	return(( ncfv*10 + ncov)) ;
}


/* la chaine est un contour */

contch_(tete)
struct maillon **tete;
{
	if( (*tete)->u.t.magic == MAGIC )
		(*tete)->u.t.mode |= CONTOUR;
	else
		{
		fprintf(stdout,"%s contch:%d n'est pas une chaine\n",
							xargv[0],*tete);
		return;
		}
}

/* test de la fermeture d'une chaine */

isclos_(tete)
struct maillon **tete;
{
	if( (*tete)->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s isclos:%d n'est pas une chaine\n",
							xargv[0],*tete);
		return(OPEN);

		}

	return((*tete)->u.t.mode & CLOSE);
}

/* test de contour */

iscont_(tete)
struct maillon **tete;
{
	if( (*tete)->u.t.magic != MAGIC )
		{
		fprintf(stdout,"%s iscont:%d n'est pas une chaine\n",
							xargv[0],*tete);
		return(OPEN);

		}

	return((*tete)->u.t.mode & CONTOUR);
}
