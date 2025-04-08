/*segec2_
+===============================================================+
!								!
!	Giraudon  -  INRIA . SOPHIA	:   2/05/85		!
!								!
!	modif du 1/03/89 : allocation dynamique de maillon	!
!								!
+===============================================================+
!								!
!	Routine d'appel du programme d'approximation polygonale !
!	avec approximation au moindres carres			!
!	et routine d'ecriture sur disque des segments		!
!	On traite une chaine en allant chercher son pointeur	!
!	de tete dans le vecteur KBUF				!
!								!
!	Parametre d'entree :					!
!	-------------------					!
!	outanme =  Nom du fichier disque ou l'on ecrit		!
!		  les segments					!
!	KBUF   =  buffer qui contient tous les pointeurs de     !
!		  tete de chaines				!
!	SOMMET =  vecteur de sortir de apang2 qui contient les  !
!		  les  sommets de l'approx. polyg.		!
!	NOMCHA =  le nombre de chaines a ecrire	 		!
!	ITAIL  =  taille supplementaire des maillons (0-4)	!
!								!
+===============================================================+
*/

#include <math.h>
#include <stdio.h>
#include <dzv.h>
#include <include/chaine/maillon.h>


#define MA 7   /* on ne peut creer des maillons a plus de 7 elements */
#define BA 9   /* nombre de valeur flottante de l'entete */
#define ID 5   /* nombre de "short" de l'entete */
#define PLUS_MAIL 50 /* nombre arbitraire de maillon rajoutes lors d'un
			malloc de maillon => evite malloc intempestif */


/* structure des segments sur support externe 
   BA float : 	longueur du segment
		moyenne des niveaux de gris
		variance
		x milieu
		y milieu
		cosinus
		sinus
		orientation en degre (a completer)
		erreur relative a la droite
   ID short :	numero de la chaine mere
		nombre de maillons de cette chaine
		si cette chaine est un contour ferme
		nombre de segments autre qui composent cette chaine
		libre : doit etre utilise par segori
 */

segec2_(outname,kbuf,sommet,nomcha,itail,VAL,ifmt,Icar,idebug)
	int *nomcha, *itail, ifmt[], Icar, idebug;
	int kbuf[], *sommet[];
	float *VAL;
	char outname[];
	{

	DZV *dzv_fic , *dzv_elt , *dzv_mai ;

	int numero=0 , *marc , *rest1, *rest2 , ta ;
	float *frest1, *frest2 , bavre[BA] , s=0. ,s1=0. , longu ;
	register int gog ,numseg = 0 , iso = 0  , k=0 , lop ;
	register int  ngo, nsom , nsom1 ;
	float ongu, ranh, cosw, sinw , erreur , gaillons[2*MA] ;
	float xm ,ym ,xd ,yd ,xf ,yf , ang_cou ;
	int ipoi, ix ,iy, ix1, iy1, niv, niv1 , nomb;
	int ier, totalm, abs() ;
	int *sommet1[300] , fd ;
	short identif[ID] ;
	int nb_mail, tail_mail = 0;
	struct ms *maillon;




	nomb = *nomcha ;
	ang_cou = *VAL ;   /* defintion pour strip du parametre de coupure*/
	ta = *itail + 3;  /* la taille minimale d'un maillon est 3 : x,y,ng*/

	if (idebug >= 1) {
			printf("\nsegec2_: version du 1/03/89\n\n");
			printf(" Valeur angulaire %f \n",ang_cou) ;
	}

	if (ta > MA ) 
		{
		fprintf(stderr," Vous avez donne une taille de maillons > 7, je ne peux traiter \n") ;
		return(-1) ;
		}		


/* Definition du fichier sur disque */

	dzv_fic = cr_dzv( 3, 20,"char","segments",1,"int","dimx",1,"int","dimy");
	de_bug("dzv_fic mal passe") ;

	dzv_elt = cr_dzv(2,BA,"float","longueur_moy_varia_xmi_ymi_cos_sin_orie_err",ID,"short","nucha_nbmai_fermeture_nbseg_vide") ;
	de_bug("dzv_elt mal passe") ;

	dzv_mai = cr_dzv(1,ta,"float","x y ng") ;
	de_bug("dzv_mai mal passe") ;

/**** 24/2/88 ****/
	fd = cr_fic_base(outname,"segments",nomb,dzv_fic,dzv_elt,dzv_mai);
	de_bug("creation mal passee") ;

	if (Icar == 1)
	{
		wr_fic(fd,"ss moind. carre",ifmt,ifmt+1 );
	}
	else
	{
		wr_fic(fd,"avec moind. carre",ifmt,ifmt+1 );
	}
	de_bug("wr_fic :  mal passee") ;

	while ( k< nomb)
	{
		ipoi = *( kbuf + k++) ;
		if (ipoi == 0) continue ;
		numero += 1 ;
/*	fprintf(stderr," chaine numero : %d\n",numero) ; */
		nb_mail = nbmail_(ipoi);
		if ( (iscont_(&ipoi) == 0) && (nb_mail > 4) ) 
		{       
/* si la chaine est trop petite on ne teste pas */

/* on teste si c'est une chaine marquee fermee . Si non on regarde le 
   1er et le dernier maillon , si ils sont a cote on ferme la chaine 
   on rajoutant le 1er maillon a la fin */
			fimail_(&ipoi,&ix,&iy,&niv,&ier) ;
			remail_(&ipoi,&ix1,&iy1,&niv1,&ier) ;

			if (abs(ix-ix1) <= 1 && abs(iy-iy1)<= 1 )
			{
				nwmail_(ipoi,iy,ix,niv) ;
     				contch_(&ipoi) ;
				nb_mail++;
			}			

		}

/* Allocation dynamique de maillon pour 'nb_mail' maillons si necessaire */

		if (nb_mail > tail_mail) {
			free(maillon);
			tail_mail = nb_mail + PLUS_MAIL;
			maillon = (struct ms *)
				  malloc(tail_mail * sizeof(struct ms));
		}
	
/* Maintenant on va recuperer tous les maillons de la chaine ipoi ainsi que
   le nombre de maillons */
		totalm = getmail_(&ipoi,maillon) ; 

/* Appel de la routine d'approximation polygonale */
		nsom1 = apang2( maillon, sommet1 , totalm, ta) ; 
	if (nsom1 > 300) fprintf(stderr," Buffer sommet1 trop petit \n");
		nsom = strip( maillon, sommet , totalm, ta , ang_cou) ; 

	if ( (nsom1 + 1) < nsom) /*Il faut que apang2 soit vraiment meilleur*/
	{
		for (gog =0 ; gog < nsom1 ; gog++ )
		{
			sommet[gog] = sommet1[gog] ;
		}
	nsom = nsom1 ;
	}
 
/*	fprintf(stderr," nombre de segment %d :\n ",nsom) ;*/
		for (gog = 0 ; gog < (nsom-1) ; gog++)
		{
		numseg += 1 ;
/*	fprintf(stderr," segment numero : %d\n",numseg) ; */
		marc = rest1 = sommet[gog] ;
		rest2 = sommet[gog+1] ;
		ongu = ( *(rest1) - *(rest2) ) ;
		ranh =  (*(rest1 + 1) - *(rest2 + 1)) ;
		longu = (float) hypot((float)ongu,(float)ranh) ; 
					/* longueur du segment */
		iso = 0; s = s1 =0. ;
		while (rest1 <= rest2)
			{
			iso += 1;
			rest1 += 2;
			ngo = *(rest1) ; /* valeur du niveau de gris */
			s = s + ngo ; 
			s1 = s1 + ngo * ngo ; 
			rest1 = rest1 + *itail + 1;
			} ;
		s = s / iso ;
		s1 = ( (s1 / iso) - (s * s)) / (float) iso ;
		
		frest1 = gaillons ;
		frest2 = gaillons + ta ;

		if (Icar == 1) 
		{
		xd = *(frest1++) = (float) *sommet[gog] ;/* approx sans moindres carres*/
		yd = *(frest1++) = (float) *(sommet[gog] + 1)  ;
		*(frest1) = (float) *(sommet[gog] +2) ; /* niveau de gris */
		xf = *(frest2++) = (float) *sommet[gog+1] ;
		yf = *(frest2++) = (float) *(sommet[gog+1] + 1 );
		*(frest2) = (float) *(sommet[gog+1] +2) ;/* niveau de gris */
		xm = (xd + xf) / 2 ;
		ym =  (yd + yf) / 2 ;
		cosw = (xf - xd ) / longu ;
		sinw = (yf - yd) / longu ;
		erreur = 0;
		}
		else
		{
		carre (marc,iso,ta,&xm,&ym,&cosw,&sinw,&xd,&yd,&xf,&yf,&erreur) ;
		*(frest1++)=  xd  ; *(frest1++) =  yd  ; /* approx avec moindres carres*/
		*(frest1) = (float) *(sommet[gog] +2) ; /* niveau de gris */
		*(frest2++) = xf  ; *(frest2++) = yf  ;
		*(frest2) = (float) *(sommet[gog+1] +2) ;/* niveau de gris */
		}

		bavre[0] = longu ; /* longueur du segment */
		bavre[1] = s ;  /* moyenne des niveaux de gris */
		bavre[2] = s1 ; /* variance */
		bavre[3] = xm ; /* milieu x */
		bavre[4] = ym ; /*   "    y */
		bavre[5] = cosw ; /* cosinus */
		bavre[6] = sinw ; /* sinus */
		bavre[7] = 1000. ; /*orientation en degre a completer */
		bavre[8] = erreur/longu ; /* erreur a la droite */

		identif[0] = numero ; /* numero de la chaine mere */
		identif[1] = totalm ; /* nbre de chain. de cette chaine*/
		identif[2] = iscont_(&ipoi) ;  /* contour ferme */
		identif[3] = nsom - 1 ; /* nb de segm. conte. ds cett. cha */
		identif[4] = 0 ; /* slot vide */

		wr_elt( fd , numseg , 2 , bavre, identif ) ;
		de_bug("wr_elt mal passe") ;
/*fprintf(stderr," ecriture du segment %d effectuee \n",numseg) ;*/



		wr_mai ( fd , gaillons ) ;
		de_bug("wr_mai mal passe") ;
		wr_mai ( fd , (gaillons+ta));
		de_bug("wr_mai mal passe") ;
		}

	}
	if (idebug >= 1) fprintf(stderr," nombre de segment %d :\n ",numseg) ;
	cl_fic( fd) ;

}
