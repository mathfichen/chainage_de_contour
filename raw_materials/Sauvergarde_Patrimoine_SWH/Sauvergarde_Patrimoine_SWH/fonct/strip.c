/************************************************/
/* fonction d'echantillonnage utilisant la	*/
/* distance max. d'un point a la droite de	*/
/* reference					*/
/* en entree:					*/
/* -inxy tableau de coordonnees (x, y, ta-2	*/
/*	nombres)				*/
/* -n nombre de points e entree			*/
/* -ta taille d'un maillon			*/
/* en sortie:					*/
/* -outp indices (dans inxy) des points		*/
/* echantillonnes				*/
/* -valeur retournee: le nombre d'echantillons	*/
/*   						*/
/* en outre: dev (deviation maximum) et reb	*/
/* (rebroussement max) sont en variables	*/
/* externe					*/
/* enfin, une recherche (simple) de courbure	*/
/* max. est faite en cas d'echantilonnage	*/
/************************************************/

#define DEBUG
#define REBRO
#include <stdio.h>
#include <math.h>
extern int debug_;


strip (inxy, outp, n, ta , VAL )
int *inxy, **outp, n, ta;
float VAL ;
{
	float devsq, dev;
	int *j, *ifin;
	float rsq;
#ifdef REBRO
	float rsqmax, rsqmin;
#endif
	int **nech, *candidat;
	float dx, dy;
	float oldx, oldy;
	float cosad1, sinad1, cosad2, sinad2;
	float cosl1, sinl1, cosl2, sinl2;
	float marge, h1, h2, h;
/*   */
	if (VAL > 0. ) dev = VAL ; /* Modif gerard le 18/06/86 : passage en*/
	devsq = dev*dev;            /* parametre de coupure anglulaire */
	ifin = inxy + ta*n;
	*(nech = outp) = inxy;
#ifdef DEBUG
	if (debug_ >= 1) {
		fprintf (stderr, "A  dev= %-3.0f (%-4.0f)\n",
		dev, devsq);
		fprintf (stderr, "A  pts en entree: %d\n", (ifin-inxy)/ta);
		fprintf (stderr, "A  1er echantillon %d  x= %d y= %d\n",
		(*nech -inxy)/ta, **nech, *(*nech+1));
	}
	if (debug_ >= 3) affich (inxy, n, ta);
#endif
/*   */
/* boucle sur les echantillons			*/
/*   */
	for (j = inxy; j < ifin;) {
		oldx = *j;
		oldy = *(j+1);
		rsq = 0.;
#ifdef DEBUG
	if (debug_ >= 2) fprintf(stderr,
		"Depart %d (%7f %7f)\n", (j-inxy)/ta, oldx, oldy);
#endif
/*   */
/* mise en place du cone admissible */
/*   */
		while ((rsq <= devsq) && ((j += ta) < ifin)) {
			dx = *j - oldx;
			dy = *(j+1) - oldy;
			rsq = dx*dx + dy*dy;
		}
		if (j >= ifin) {
#ifdef DEBUG
	if (debug_ >= 3) fprintf(stderr,
		"termine (et dernier pt echantillonne)\n");

#endif
			break;
		}
		h = (float) sqrt( (double)(rsq - devsq) );
		cosad1 = (dx*h + dy*dev)/rsq;
		cosad2 = (dx*h - dy*dev)/rsq;
		sinad1 = (dy*h - dx*dev)/rsq;
		sinad2 = (dy*h + dx*dev)/rsq;
		candidat = j;
		marge = dev/h;
		rsqmin = (rsqmax = rsq) - devsq;
#ifdef DEBUG
		if (debug_ >= 2) fprintf(stderr,
			" CANDIDAT: %d (marge: %f)\n",
			(j-inxy)/ta, marge);
#endif
#ifdef DEBUG
	if (debug_ >= 2) fprintf(stderr,
		"A cone admissible en %d: (%5.2f %5.2f)  (%5.2f %5.2f)\n",
		(j - inxy)/ta, cosad1, sinad1, cosad2, sinad2);
#endif
/*   */
/* exploration successive des points			*/
/*   */
		while ((j += ta) < ifin) {
			dx = *j - oldx;
			dy = *(j+1) - oldy;
			rsq = dx*dx + dy*dy;
#ifdef DEBUG
	if (debug_ >= 2) fprintf(stderr,
		"essai: %d (%5f %5f)\n", (j-inxy)/ta, dx, dy);
#endif
/*   */
/* on teste le rebroussement			*/
/*   */
#ifdef REBRO
			if (rsq <= rsqmin) {
#endif

#ifdef DEBUG
	if (debug_ >= 2) fprintf(stderr,
		"---rebroussement\n");
#endif

#ifdef REBRO
				break;
			}
			if (rsq > rsqmax) {
				rsqmin = (rsqmax = rsq) - devsq;
			}
#endif
/*  							*/
/* On teste si le point appartient au cone admissible	*/
/* et si son centrage est meilleur que celui du		*/
/* precedent candidat					*/
/* si oui, il devient le nouveau candidat		*/
/*  							*/
			if ((h = rsq - devsq) <= 0.) continue;
			h = (float) sqrt ( (double) h);
			if (((h1 = cosad1*dy - sinad1*dx) >= 0.) &&
			    ((h2 = sinad2*dx - cosad2*dy) >= 0.)) {
				h1 = (h2 > h1) ?  h2/h : h1/h;
				if ((h1 <= marge) || (j == ifin - ta)) {
					marge = h1;
					candidat = j;
#ifdef DEBUG
				if (debug_ >= 2) fprintf(stderr,
					" candidat: %d (marge: %f)\n",
					(j-inxy)/ta, marge);
#endif
				}
			}
/*   */
/* on determine le cone local				*/
/*   */
			cosl1 = (dx*h + dy*dev)/rsq;
			cosl2 = (dx*h - dy*dev)/rsq;
			sinl1 = (dy*h - dx*dev)/rsq;
			sinl2 = (dy*h + dx*dev)/rsq;
#ifdef DEBUG
	if (debug_ >= 3) fprintf(stderr,
		"B  cone local (%5.4f  %5.4f) (%5.4f  %5.4f)\n",
		cosl1, sinl1, cosl2, sinl2);
#endif
/*   */

/* on met a jour le cone admissible: intersection	*/
/* de l'ancien cone admissible, et du cone local	*/
/*   */
			if ((sinl1*cosad1 - sinad1*cosl1) > 0.) {
				sinad1 = sinl1;
				cosad1 = cosl1;
			}
			if ((sinad2*cosl2 - sinl2*cosad2) > 0.) {
				sinad2 = sinl2;
				cosad2 = cosl2;
			}
#ifdef DEBUG
	if (debug_ >= 3) fprintf(stderr,
		"C  cone admissible: (%5.4f %5.4f)  (%5.4f %5.4f)\n",
		cosad1, sinad1, cosad2, sinad2);
#endif
/*   */
/* on teste si ce nouveau cone est vide ou non	*/
/*   */
			if ((sinad1*cosad2 - sinad2*cosad1) > 0.) break;
/*   */
		}
/*   */
/* on echantillonne				*/
/*   */
		*(++nech) = j = candidat;
#ifdef DEBUG
	if (debug_ >= 1) fprintf(stderr,
		"echantillon %d: %d (%d, %d)\n",
		nech - outp, (*nech - inxy)/ta, **nech, *(*nech+1));
#endif
	}
	if (*nech < (ifin - ta)) {
		*(++nech) = ifin - ta;
#ifdef DEBUG
	if (debug_ >= 1) fprintf(stderr,
		"dernier echant %d: %d (%d, %d)\n",
		nech - outp, (*nech - inxy)/ta, **nech, *(*nech+1));
#endif
	}
	return (nech - outp + 1);
}

#ifdef DEBUG
affich (inxy, n, ta)
int *inxy, n, ta;
{
	int i, *p, j;
	fprintf(stderr, "nbre de points = %d\n", n);
	p = inxy;
	for (i = 0; i<n; i++) {
		fprintf (stderr,"n %d : ",i);
		for (j = 0; j<ta; j++)
			fprintf(stderr,"%d   ",*(p++));
		fprintf(stderr, "\n");
	}
}

affi2(texte, x, y)
char *texte;
int x,y;
{
	float l;
	l = x*x + y*y;
	l = (float) sqrt((double)l);
	if (l == 0.) l = 1.;
	fprintf(stderr, "%s   %f   %f\n", texte, x/l, y/l);
}
#endif
