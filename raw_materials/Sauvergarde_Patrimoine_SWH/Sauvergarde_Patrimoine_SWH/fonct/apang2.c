/* fonction d'echantillonnage utilisant		*/
/* la variation angulaire			*/
/* la direction du trace en un point est	*/
/* approchee par un vecteur partant de ce point */
/* et joignant un point ulterieur, tel que ce	*/
/* vecteur soit de longueur = dmin1		*/
/* (la longueur est la norme L2			*/
/*						*/
/* soit ref le 1er tel vecteur			*/
/* on considere le vecteur courant associe aux	*/
/* points successifs: s'il fait un angle > eps1	*/
/* avec Ref, ou un angle > eps2 avec un qconque */
/* des vecteurs entre Ref et lui, on		*/
/* decide d'echantillonner au voisinage de	*/
/* l'origine de ce vecteur			*/
/* pour ca: on scanne avant et apres (dans	*/
/* un "champ" de largeur champ), et on choisit	*/
/* le point de courbure max (la courbure etant  */
/* l'angle entre les vecteurs de longueur	*/
/* dmin2 arrivant et partant en ce point)  	*/
/* NB: l'angle entre 2 vecteurs est represente	*/
/* par le couple (prod. scal., prod. vector.)	*/
/*						*/
/* en entree:					*/
/* - un tableau de coordonnees			*/
/* comprenant n fois (x,y,ng, ..ta parametres..)*/
/* avec les x et y en entier			*/
/* - le nombre de points n (en entier)		*/
/* - dmin1 longueur du vecteur courant		*/
/* - dmin2 longueur des vecteurs pour courbure	*/
/* - champ demi-largeur de l'intervalle ou on	*/
/*   cherche la courbure max			*/
/*     (dmin2, dmin2, champ: reels)		*/
/*     (eps1 eps2 en degres)			*/
/* un tableau de pointeurs sur les points	*/
/* echantillonnes est rendu (outp)		*/
/* et la valeur retournee			*/
/* est le nombre de points echantillonnes	*/
/*  */
/*  */
/*  */
/* #define DEBUG  */
#define EPS1 25
#define EPS2 40
#define DMIN1 4.
#define DMIN2 4.
#define CHAMP 3.
#include <math.h>
#include <stdio.h>
/*   */
float DEV;
/*   */
float dmin1 = DMIN1, champ = CHAMP, dmin2 = DMIN2;
int eps1 = EPS1, eps2 = EPS2;
extern int debug_;
apang2(inxy, outp, n, ta)
int *inxy, **outp;
register int ta;
{
	double tan();
	struct vect {
		int dx;
		int dy;
	}
	vmin, vmax, vcour, ref, vsuiv;
	register int *ic, *iext, *ifin;
	int **nech;
	int *finco, *pmax;
	register int temp, r1;
	int temp2, e2;
	int e1;
	int r2, rch;
/*   */
/* le 1er point est toujours echantillonne	*/
/* on cherche la 1ere reference			*/
/* si elle n'existe pas, on echantillonne aussi	*/
/* le dernier point				*/
	e1 = tan(eps1*3.14159/180.) * 100;
	e2 = tan(eps2*3.14159/180.) * 100;
	r1 = dmin1 * dmin1;
	r2 = dmin2 * dmin2;
	rch = champ * champ;
	ic = inxy;
	ifin = inxy + n*ta;
	nech = outp;
	*outp = inxy;
/*   */
#ifdef DEBUG
	if (debug_ >= 1) {
		fprintf(stderr,"dmins: %f  %f  %f\n",dmin1, dmin2, champ);
		fprintf(stderr,"rmins: %d  %d  %d\n",r1,r2,rch);
		fprintf(stderr, "eps:  %d   %d  \n", eps1, eps2);
		fprintf(stderr, "e1= %d   e2= %d\n", e1, e2);
		fprintf(stderr,"ifin %d\n", (ifin-inxy)/ta);
	}
	if (debug_ >= 2) affich(inxy, n, ta);
#endif
/*   */
/* pour chaque reference (ref.dx, ref.dy)	*/
/* et on va lui comparer les vecteurs successifs*/
/* on va en meme temps mettre a jour les	*/
/* directions extremes (vmin vmax) et		*/
/* aussi calculer au passage la "courbure" en	*/
/* chaque point					*/
/* on initialise les vecteurs	*/
/* d'ecart maximum vmin et vmax			*/
/*   */
/*   */
/* on boucle sur les points echantillonnes	*/
/* (et donc sur les references)			*/
/*   */
	while (ic < ifin) {
		iext = ic;
		while ((iext += ta) < ifin) {
			ref.dx = *iext - *ic;
			ref.dy = *(iext+1) - *(ic+1);
			if ((ref.dx*ref.dx + ref.dy*ref.dy) >= r1) break;
		}
/*   */
#ifdef DEBUG
		if (debug_ >= 1) {
			fprintf(stderr,"A  ref: %d  a %d\n", (ic-inxy)/ta,
			(iext-inxy)/ta);
			affi2("ref",ref.dx,ref.dy);
		}
#endif
/*   */
		if (iext >= ifin) {
#ifdef DEBUG
			if (debug_ >= 1) fprintf(stderr, "sortie B\n");
#endif
			break;
		}
/*   */
		vmax.dx = vmin.dx = 1;
		vmax.dy = vmin.dy = 0;
/*   */
/* on boucle sur les vecteurs courants		*/
/*   */
		for (ic = iext - ta;; iext = (ic += ta)) {
			while ((iext += ta) < ifin) {
				vcour.dx = *iext - *ic;
				vcour.dy = *(iext+1) - *(ic+1);
				if ((vcour.dx*vcour.dx + vcour.dy*vcour.dy)
				    >= r1) break;
			}
/*   */
#ifdef DEBUG
			if (debug_ >= 2) {
				fprintf(stderr,"C courant: %d a %d\n",
				(ic-inxy)/ta, (iext-inxy)/ta);
				affi2("vcour",vcour.dx,vcour.dy);
			}
#endif
/*   */
			if (iext >= ifin) {
#ifdef DEBUG
				if (debug_ >= 1) fprintf(stderr, "sortie D\n");
#endif
				break;
			}
/*   */
/* on oriente en prenant ref comme axe des x	*/
/*   */
			temp = vcour.dx*ref.dx + vcour.dy*ref.dy;
			vcour.dy = ref.dx*vcour.dy - ref.dy*vcour.dx;
			vcour.dx = temp;
/*   */
#ifdef DEBUG
			if (debug_ >= 2) affi2("courabs",vcour.dx,vcour.dy);
#endif
/*   */
/*   */
/* 1er cas :l'angle (ref,vcour) est > 0		*/
/*   */
			if (vcour.dy > 0) {
#ifdef DEBUG
				if (debug_ >= 2)
					fprintf(stderr,"E   ref,vcour >0\n");
#endif
/*   */
/* on compare l'angle avec eps1			*/
/*   */
				if (100*vcour.dy - e1*vcour.dx >= 0)
#ifdef DEBUG
				{
					if (debug_ >= 2) 
						fprintf(stderr,"F ref,vcour >eps1\n");
#endif
					break;  
#ifdef DEBUG
				}
#endif
/*   */
/* on compare la direction avec celle de vmax	*/
/* et on met a jour vmax si necessaire		*/
/*   */
#ifdef DEBUG
				if (debug_ >= 2) affi2("vmax",vmax.dx,vmax.dy);
#endif
/*  */
				if (vmax.dx*vcour.dy
				    - vmax.dy*vcour.dx <= 0)
					continue;
				else {
/*   */
#ifdef DEBUG
					if (debug_ >= 2) 
						fprintf(stderr,"G vmax mis a jour\n");
#endif
/*   */
					vmax.dx = vcour.dx;
					vmax.dy = vcour.dy;
				}
			}
			else {
/*   */
/* 2eme cas: l'angle ref, vcour est <0	*/
/*   */
				if (debug_ >= 2) fprintf(stderr, "ref,cour <0\n");
/*   */
/*   */
/* on compare l'angle a eps1		*/
/*   */
				if (100*vcour.dy + e1*vcour.dx <= 0)
#ifdef DEBUG
				{
					if (debug_ >= 2) 
						fprintf(stderr,"H ref,vcour >eps1\n");
#endif
					break;  
#ifdef DEBUG
				}
#endif
/*   */
/* on compare a vmin, et on met vmin	*/
/* a jour si necessaire			*/
/*   */
#ifdef DEBUG
				if (debug_ >= 2) affi2("vmin",vmin.dx,vmin.dy);
#endif
/*   */
				if (vmin.dx*vcour.dy
				    - vmin.dy*vcour.dx >= 0)
					continue;
				else {
/*   */
#ifdef DEBUG
					if (debug_ >= 2)
						fprintf(stderr,"J  vmin mis a jour\n");
#endif
/*   */
					vmin.dx = vcour.dx;
					vmin.dy = vcour.dy;
				}
			}
/*   */
/* on compare l'angle (vmin, vmax) a eps2	*/
/*   */
			vcour.dx = vmax.dx*vmin.dx + vmax.dy*vmin.dy;
			vcour.dy = vmin.dx*vmax.dy - vmin.dy*vmax.dx;
/*   */
#ifdef DEBUG
			if (debug_ >= 2) affi2("maxmin",vcour.dx,vcour.dy);
#endif
/*   */
			if ((vcour.dy < 0) ||
			    (100*vcour.dy - e2*vcour.dx) >= 0) { 
/*   */
#ifdef DEBUG
				if (debug_ >= 2)
					fprintf(stderr,"K   vmin,vmax >eps2\n");
#endif
/*   */
				break;     
			}
		}
/*   */
/* on doit echantillonner			*/
/* on va chercher le point de "courbure"	*/
/* maximum, entre icc-champ et icc+champ	*/
/* en tronquant champ si on est pres d'un bord	*/
/* si d'autre part un vecteur deborde, il est	*/
/* remplace par le + proche vecteur qui ne	*/
/* deborde pas (ou ref)				*/
/* (NB on suppose champ et dmin2 <= dmin1)	*/
/*   */
/* a partir d'ici, vcour contient le vecteur	*/
/* arrivant sur le pt				*/
/*   puis la courbure en ce point		*/
/*   vmin celui qui en part			*/
/*   vmax la courbure max			*/
/*   */
		if (iext >= ifin) break;
#ifdef DEBUG
		if (debug_ >= 1)
			fprintf(stderr,"L   on echantillonne:  %d\n", (ic-inxy)/ta);
#endif
/*   */
		finco = ic;
		while ((finco += ta) < ifin) {
			temp = *finco - *ic;
			temp2 = *(finco+1) - *(ic+1);
			if ((temp*temp + temp2*temp2) > rch) break;
		}
		iext = ic;
		while ((iext -= ta) > *nech) {
			temp = *ic - *iext;
			temp2 = *(ic+1) - *(iext+1);
			if ((temp*temp + temp2*temp2) > rch) break;
		}
		if ((ic = iext) <= *nech) ic = *nech + ta;
		if (ic >= finco) {
#ifdef DEBUG
			if (debug_ >= 1) fprintf(stderr, "sortie M\n");
#endif
			break;
		}
/*   */
#ifdef DEBUG
		if (debug_ >= 2)
			fprintf(stderr,"P   debco finco: %d   %d\n",(ic-inxy)/ta,
			(finco-inxy)/ta);
#endif
		vmin.dx = ref.dx;
		vmin.dy = ref.dy;
		vmax.dx = 1;
		vmax.dy = 0;
		pmax = ic;
		while (ic < finco) {
/*   */
#ifdef DEBUG
			if (debug_ >= 2) fprintf(stderr,"Q   icox %d\n", (ic-inxy)/ta);
#endif
/*   */
			iext = ic;
			while ((iext -= ta) >= *nech) {
				vcour.dx = *ic - *iext;
				vcour.dy = *(ic+1) - *(iext+1);
				if ((vcour.dx*vcour.dx + vcour.dy*vcour.dy)
				    >= r2) break;
			}
			if (iext < *nech) {
#ifdef DEBUG
				if (debug_ >= 2)
					fprintf(stderr,"R vprec < nech\n");
#endif
				vcour.dx = ref.dx;
				vcour.dy = ref.dy;
			}
#ifdef DEBUG
			else if (debug_ >= 2) fprintf(stderr,
			"vprec: %d  a %d\n",(ic - inxy)/ta,(iext-inxy)/ta);
#endif
			iext = ic;
			while ((iext += ta) < ifin) {
				vsuiv.dx = *iext - *ic;
				vsuiv.dy = *(iext+1) - *(ic+1);
				if ((vsuiv.dx*vsuiv.dx + vsuiv.dy*vsuiv.dy)
				    >= r2) break;
			}
			if (iext < ifin) {
/*   */
#ifdef DEBUG
				if (debug_ >= 2)
					fprintf(stderr,"S suiv: %d  a %d\n",

					(ic-inxy)/ta, (iext-inxy)/ta);
#endif
/*   */
				vmin.dx = vsuiv.dx;
				vmin.dy = vsuiv.dy;
			}
#ifdef DEBUG
			else	if (debug_ >= 2) fprintf(stderr, "suiv >fin\n");
			if (debug_ >= 2) affi2("vprec",vcour.dx,vcour.dy);
			if (debug_ >= 2) affi2("vsuiv",vmin.dx,vmin.dy);
#endif
/*   */
			temp = vcour.dx*vmin.dx + vcour.dy*vmin.dy;
			vcour.dy = vcour.dx*vmin.dy - vcour.dy*vmin.dx;
			vcour.dx = temp;
/*   */
#ifdef DEBUG
			if (debug_ >= 2) affi2("courbure",vcour.dx,vcour.dy);
#endif
/*   */
			if (vcour.dy < 0) vcour.dy = -vcour.dy;
			if (vmax.dx*vcour.dy
			    - vmax.dy*vcour.dx > 0) {
				pmax = ic;
/*   */
#ifdef DEBUG
				if (debug_ >= 2)
					fprintf(stderr,"T   pmax %d\n",
					(pmax-inxy)/ta);
#endif
/*   */
				vmax.dx = vcour.dx;
				vmax.dy = vcour.dy;
			}
			ic += ta;
		}
/*   */
		if ((finco < ifin) ||
		    ((*(ifin-ta) - *pmax)*(*(ifin-ta) - *pmax) +
		    (*(ifin-ta+1)-*(pmax+1))*(*(ifin-ta+1)
		    -*(pmax+1)) >= rch))
			*(++nech) = ic = pmax;
		else {
#ifdef DEBUG
			if (debug_ >= 1)
				fprintf(stderr, "sortie N\n");
#endif
			break;
		}
#ifdef DEBUG
		if (debug_ >= 1)
			fprintf(stderr," point retenu: %d -> %d\n",
			nech-outp+1, (ic-inxy)/ta);
#endif
	}
	if ((*(nech++) < (ifin - 1)) && (ifin > (inxy + 1))) {
/*   */
		*(nech++) = ifin - ta;
#ifdef DEBUG
		if (debug_ >= 1) fprintf(stderr,"U dernier nech %d -> %d\n",
		nech-outp, (*(nech-1)-inxy)/ta);
#endif
	}
	n = nech - outp;
/*   */
#ifdef DEBUG
	if (debug_ >= 1) affich(inxy, n, ta);
#endif
/*   */
	return (n);
}
/*   */
#ifdef DEBUG
affich (inxy, n, ta)
int *inxy, n, ta;
{
	int i, *p, j;
	fprintf(stderr, "nbre de points = %d\n", n);
	p = inxy;
	for (i = 0; i<n; i++) {
		for (j = 0; j<ta; j++)
			fprintf(stderr,"%d   ",*(p++));
		fprintf(stderr, "\n");
	}
}
/*   */
affi2(texte, x, y)
char *texte;
int x,y;
{
	float l;
	l = x*x + y*y;
	l = sqrt((double)l);
	if (l == 0.) l = 1.;
	fprintf(stderr, "%s   %f   %f\n", texte, x/l, y/l);
}
#endif
