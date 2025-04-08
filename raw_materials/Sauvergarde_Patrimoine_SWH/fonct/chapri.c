/*==============================================================*/
/*								*/
/*	Giraudon  -  INRIA . SOPHIA	:   17/09/84		*/
/*								*/
/*	modif du 1/03/89 :					*/
/*	   plus de passage de 'imag' a "ecrcha" et "segec2_"	*/
/*	modif du 16/06/89 :					*/
/*	   utilisation de lecflt_ au lieu de lect_		*/
/*								*/
/*==============================================================*/
/*								*/
/*	Programme de creation de chaines sur une image binaire  */
/*	de bits ou de bytes (nombre de colonnes max 512).	*/
/*	Il faut forcement que les contours soit d'epaisseur 1   */
/*	Si ce n'est pas le cas faire passer d'abord le program- */
/*	me "AFF".						*/
/*	Le resultat est l'ensemble des chaines ecrites sur 	*/
/*	disques.						*/
/*	La tache se fait en deux passages :			*/
/*	- lecture de l'image et creation de toutes les chaines  */
/*	- fusion des chaines qui peuvent l'etre et 		*/
/*	  renumerotation de la filiation			*/
/*	- Puis au choix : 					*/
/*	  ecriture sur disque des chaines			*/
/*	  approximation polygonale et ecriture sur disque des   */
/*	  segments						*/
/*								*/
/*==============================================================*/

#include <stdio.h>
#include <include/chaine/common.h>
#include <include/chaine/maillon.h>


chapri_(ichn,buf_image,pkbuf,ipoids,nf,ifmt,pmcolo,pix,piy,pixfin,piyfin,
			pitail,pibruit,meta,pidebug,piseg,VAL,outname)
	int ichn[],ipoids[],ifmt[];
	char outname[] ;
	int *pmcolo,*pix,*piy,*pixfin,*piyfin,*pitail,*pidebug,*piseg;
        int *pibruit,meta;
        struct maillon *pkbuf[] ;
	struct image *nf;
	float *VAL;
        float *buf_image;

	{
	int ifd, ix= *pix, iy= *piy, ixfin= *pixfin;
	int iyfin= *piyfin, itail= *pitail, iseg= *piseg, ibruit= *pibruit;

	float *imag_indexd, *imag_xf, pixel_();
	int nlign,nrest,ntour,nlit,nmot,indexd,indexf;
	int i,j,k,l3,ival,ito,kl,idimy,lop,Tets=1;
	int ifict[2];
	int nc, ilong, mlign,  ixint;
	int ixbout, iybout, idim, ivico	, itek;

/*----- Initialisation des variables externes --------------*/
	mcolo   = *pmcolo;
	abandon = 0;
	imag    = buf_image;
	mco2    = mcolo * 2;
	maxch	= ipoids[2];
	nomcha 	= 0;
	kbuf	= pkbuf;
	idebug  = *pidebug;
	
/*----------------------------------------------------------*/	
	nc     	= 0;
	ilong   = ipoids[1];
	mlign	= ifmt[1];
	ixint 	= ix + 1;
	ixbout 	= ix + ixfin - 2;
	iybout 	= iy + iyfin - 2;
	idim 	= 3;
	ivico	= 0;
	itek 	= 0;
	                 /*mod gg du 20/07/89 voir plus loin */
	if (ifmt[8] == 200 && (ifmt[2] == 1 || ifmt[2] == -8) ) Tets = 256 ;

	itab = (int ***)(malloc( sizeof(int) * mcolo * 8) ); 
	if ( ilong < (mcolo * 3) ) 
		{ printf(" PAS ASSEZ DE PLACE EN MEMOIRE :\n");
		  printf("        mettre un -k plus grand . ( k=%d)\n",ilong);
		  exit(-1);
		}
	if (idebug >= 1 ) printf("   .......creation de %d chaines au maximum........\n",maxch);

	if (((ixfin + ix - 1) > mcolo) || ((iyfin + iy - 1) > mlign) )
		{ printf("Erreur sur les parametres ... recommencez.\n");
		  exit(-1);
		}

/*---- Initialisation du compteur de chaines et du buffer de chaines ----*/
	initch_(ichn,ipoids[0]);

/*---- on veut lire une image qui a mcolo colonnes et iyfin lignes   ----*/
 	ifict[0] = mcolo;
 	ifict[1] = iyfin; 
	getbof(ifict,ilong,idim,&nlign,&nrest,&ntour,&nlit);
if (idebug > 6) printf(" nlign %d, ntour %d, nrest %d, nlit %d\n",nlign,ntour,nrest,nlit) ;

	nmot   = mcolo * (idim - 1);
	indexd = nmot  + 1;
	indexf = mcolo * nlit + 1;
/*-------------------------------------------------------------------*/
/*              Debut de la boucle de calcul			     */	
/*-------------------------------------------------------------------*/
	lptset_(&nf,&iy);

	i  = iy;
	l1 = 0;
	l2 = 1;

/*------initialisation de Itab avec 'X'------*/
        initab_(l1,ixint-2,ixbout,itab);

/*------Initialisation du tableau de branchement sur configuration-----*/
	initgtot_();
    
	if (idebug >= 1) printf("Etape numero 1 : le chainage\n");
	for (ito=1;ito<=ntour;ito++)
		{
		if ((i <= iybout ) && (!(ito == ntour && nrest == 0)))
			{
			if (ito == ntour) 
				{
	 			  nlign = nrest;
			 	  nlit = nlign - idim + 1;
				}

			if (ito == 1)
				{ 
				  lecflt_(&nf,&nlign,imag);
				}
		      	else    { imag_indexd = imag + indexd - 1;
				  lecflt_(&nf,&nlit,imag_indexd);
				}
			for (kl=1; kl<=nlit ;kl++)
				 { i = i + 1;
/* Version du 25/08/87 : pour resoudre le probleme de la coordonnee ix qui
n'etait pas correcte : attention modification de pixel_() */
				   lop = kl*mcolo ;
				   /*----initialisation de Itab avec 'X'---*/
				   initab_( l2,ixint -2,ixbout,itab);
				   for ( j=ixint ; j <= ixbout ; j++)
					{
/* Position du pixel a teste donne par lap dans
   le buffer imag (0 a (mcolo-1)) */
					lap = lop + j - 1 ;

/* modification du 16 Juin 1989: lecture du fichier image par lecflt */
/* pour l'instant, (pixel_() * 256) evite la modification des DZV 'chaines'*/
/* Pb si l'image est en exposant 8 : alors modif gg du 20/07/89 */
					if (k=(int)(pixel_() * Tets )) 
					   {
					   ival = configur_(j-2,itab);
					   selectio_(ival,i,j-1,k,itab);
	     				   nc = nc + 1;
					   }
					}
				  l3 = l1;
				  l1 = l2;
				  l2 = l3;
				 }

			}
		if (i !=ntour)  {
				 imag_xf = imag + indexf -1;
				 mvmots_(imag_xf,imag,&nmot);
				}	
			/*----copie des idim-1 dernieres lignes en   -----*/
			/*----debut de buffer de maniere a permettre -----*/
			/*----a l'executable d'etre pipable en entree-----*/
		}
if (idebug >= 1) {
	printf("        nombre de chaines est       : %d\n",nomcha);
	printf("        nombre de pixels total      : %d\n",nc+abandon);
	printf("        nombre de pixels traites    : %d\n",nc);
	printf("        nombre de pixels abandonnes : %d\n",abandon);
}

/* liberation du pointeur image 'imag' */
	free(imag);

	lienc7_(kbuf,&nomcha,ibruit,idebug);

	if (meta) cons_meta(kbuf,nomcha);

        /*------ecriture de chaines ----------------------------------*/
	if (iseg==0) 
		{
if (idebug >= 1) printf("Etape no 4 : ecriture sur disque des chaines  \n");
		ecrcha(outname,kbuf,nomcha,itail,ifmt);
		}

	/*------Approximation polygonale et ecriture sur disque-------*/

	if (iseg !=0) 
		{
		if (idebug >= 1) {
			printf("Etape numero 4 : approximation polygonale");
			if (iseg == 1) 
			{
				printf(" sans moindre carre \n");
			}	
			else
			{
				printf(" avec moindre carre \n");
			}

		printf("                 et ecriture sur disque des segments \n");
			}
		segec2_(outname,kbuf,itab,&nomcha,&itail,VAL,ifmt,iseg,idebug); 
		}
	}
