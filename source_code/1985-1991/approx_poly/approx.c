/*approx
+===============================================================+
!								!
!	Giraudon  -  INRIA . SOPHIA	:   2/05/85		!
!								!
+===============================================================+
!								!
!	Routine d'appel du programme d'approximation polygonale !
!	avec ou sans approximation au moindres carres		!
================================================================+
*/

#include <stdio.h>
#include <dzv.h>
#include <math.h>

approx(res,numero,nb_mai,ta_mai,maillon,ferme,ang_cou,bavre,identif,Icar,numseg) 
int res, numero, nb_mai, ta_mai, maillon[] , Icar, *numseg;
short ferme , identif[];
float ang_cou , bavre[] ;
{
	int ier,abs(), ix ,iy, ix1, iy1, niv, niv1 , nomb;
	int *marc , *rest1, *rest2 ;
	float *frest1, *frest2 , s=0. ,s1=0. , longu ;
	register int gog , iso = 0  , k=0 , lop ;
	register int  ngo, nsom , nsom1 ;
	float ongu, ranh, cosw, sinw , erreur , gaillons[14] , delx, dely, theta;
	float xm ,ym ,xd ,yd ,xf ,yf  ;

	int *sommet1[300] , *sommet[300] ;


/* Appel de la routine d'approximation polygonale */

		nsom1 = apang2( maillon, sommet1 , nb_mai, ta_mai) ; 
	if (nsom1 > 300) fprintf(stderr," Buffer sommet1 trop petit \n");
		nsom = strip( maillon, sommet , nb_mai, ta_mai , ang_cou) ; 

	if ( (nsom1 + 1) < nsom) /*Il faut que apang2 soit vraiment meilleur*/
	{
		for (gog =0 ; gog < nsom1 ; gog++ )
		{
			sommet[gog] = sommet1[gog] ;
		}
	nsom = nsom1 ;
	}
 
	for (gog = 0 ; gog < (nsom-1) ; gog++)
	{
	*numseg += 1 ;
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
		rest1 = rest1 + 1;
		} ;
	s = s / iso ;
	s1 = ( (s1 / iso) - (s * s)) / (float) iso ;
		
	frest1 = gaillons ;
	frest2 = gaillons + ta_mai ;

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
		carre (marc,iso,ta_mai,&xm,&ym,&cosw,&sinw,&xd,&yd,&xf,&yf,&erreur) ;
		*(frest1++)=  xd  ; *(frest1++) =  yd  ; /* approx avec moindres carres*/
		*(frest1) = (float) *(sommet[gog] +2) ; /* niveau de gris */
		*(frest2++) = xf  ; *(frest2++) = yf  ;
		*(frest2) = (float) *(sommet[gog+1] +2) ;/* niveau de gris */
		}

	delx = *(gaillons + ta_mai) - *(gaillons) ;
	dely = *(gaillons + ta_mai + 1) - *(gaillons + 1) ;
	theta = atan2(dely,delx) ;            /* calcul de l'angle du segment */
        theta = (theta*180.)/3.141592 ;
	
	if (sinw < 0) theta = 360 + theta ; /* theta entre 0 et 360 deg */

	bavre[0] = longu ; /* longueur du segment */
	bavre[1] = s ;  /* moyenne des niveaux de gris */
	bavre[2] = s1 ; /* variance */
	bavre[3] = xm ; /* milieu x */
	bavre[4] = ym ; /*   "    y */
	bavre[5] = cosw ; /* cosinus */
	bavre[6] = sinw ; /* sinus */
	bavre[7] = theta ; /*orientation en degre */
	bavre[8] = erreur/longu ; /* erreur a la droite */

	identif[0] = numero ; /* numero de la chaine mere */
	identif[1] = nb_mai ; /* nbre de chain. de cette chaine*/
	identif[2] = ferme ;  /* contour ferme */
	identif[3] = nsom - 1 ; /* nb de segm. conte. ds cett. cha */
	identif[4] = 0 ; /* slot vide */

	wr_elt( res , *numseg , 2 , bavre, identif ) ;
					de_bug("wr_elt mal passe") ;
	wr_mai ( res , gaillons ) ;
					de_bug("wr_mai mal passe") ;
	wr_mai ( res , (gaillons+ta_mai));
					de_bug("wr_mai mal passe") ;
	}

}


