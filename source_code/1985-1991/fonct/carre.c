/* fonction d'approximation aux moindres carres	*/
/* d'une chaine de points			*/
/* en entree: la suite des maillons inxy	*/
/* (x, y, ta-2 parametres) ou x et y sont	*/
/* entiers					*/
/* n le nombre de points			*/
/* en sortie: (parametres passes par adresses)	*/
/*   xm, ym (centre de gravite)			*/
/*   cosw, sinw (cos directeurs de la droite)	*/
/*   (xd,yd) (xf,yf) coord. des projections	*/
/*   sur le segment du 1er et dernier point	*/
/*  erreur: l'erreur (somme des carres des	*/
/*     distances des points a la droite		*/
/*   */
#define DEBUG
#include <math.h>
#include <stdio.h>
extern int debug_;
carre (inxy, n, ta, xm, ym, cosw, sinw, xd, yd, xf, yf, erreur)
int *inxy, n, ta;
float *xm, *ym, *cosw, *sinw, *xd, *yd, *xf, *yf, *erreur;
{
	int *i, *ifin;
	float sumx, sumy, sumx2, sumy2, sumxy;
	float tempx, tempy, a;
	sumx = sumy = sumx2 = sumy2 = sumxy = 0.;
#ifdef DEBUG
	if (debug_ >= 2) {
		fprintf(stderr, "--- %d points\n", n);
		affich (inxy, n, ta);
	}
#endif
	if (n == 1) {
		*xm = *xf = *xd = *inxy;
		*ym = *yf = *yd = *(inxy+1);
		*cosw = 1.;
		*sinw = *erreur = 0.;
	}
	else if (n == 2) {
		*xd = *inxy;
		*yd = *(inxy + 1);
		*xf = *(inxy + ta);
		*yf = *(inxy + ta + 1);
		*xm = (*xd + *xf)/2.;
		*ym = (*yd + *yf)/2.;
		tempx = *xf - *xd;
		tempy = *yf - *yd;
		a = sqrt((double)(tempx*tempx + tempy*tempy) ) ;
		*erreur = 0.;
		if (a) {
			*cosw = tempx/a;
			*sinw = tempy/a;
		}
		else {
			*cosw = 1.;
			*sinw = 0.;
		}			
	}
	else {
		ifin = inxy + n*ta;
		for (i = inxy; i < ifin; i += ta) {
			sumx  += (tempx = *i);
			sumx2 += tempx*tempx;
			sumy  += (tempy = *(i+1));
			sumy2 += tempy*tempy;
			sumxy += tempx*tempy;
		}
		sumx2 -= sumx*sumx/n;
		sumy2 -= sumy*sumy/n;
		sumxy -= sumx*sumy/n;
#ifdef DEBUG
		if (debug_ >= 2) {
			fprintf(stderr, "sommes: %8.3f %8.3f %8.3f\n",
				sumx2, sumy2, sumxy);
		}
#endif
		a = sumx2 - sumy2;
		if (sumxy == 0.) 
			if (a < 0.) {
				*sinw = 1.;
				*cosw = 0.;
			}
			else {
				*sinw = 0.;
				*cosw = 1.;
			}
		else {
			tempx = .5*a/sqrt( (double)(a*a + 4.*sumxy*sumxy) ) ;
			*cosw = sqrt ( (double) (.5 + tempx) ) ;
			*sinw = sqrt ((double)(.5 - tempx) ) ;
			if (sumxy < 0.) *sinw = -*sinw;
		}
		*xm = sumx/n;
		*ym = sumy/n;
		tempx = (*inxy - *xm)*(*cosw) + (*(inxy+1) - *ym)*(*sinw);
#ifdef DEBUG
		if (debug_ >= 2) {
			fprintf(stderr,"dir:%8.3f %8.3f   temp1: %8.3f\n",
				*cosw, *sinw, tempx);
		}
#endif
		*xd = *xm + tempx*(*cosw);
		*yd = *ym + tempx*(*sinw);
		ifin -= ta;
		tempy = (*ifin - *xm)*(*cosw) + (*(ifin+1) - *ym)*(*sinw);
		*xf = *xm + tempy*(*cosw);
		*yf = *ym + tempy*(*sinw);
#ifdef DEBUG
		if (debug_ >= 2) {
			fprintf(stderr,"dir:%8.3f %8.3f   temp2: %8.3f\n",
				*cosw, *sinw, tempy);
		}
#endif
		*erreur = (*cosw)*(*cosw)*sumy2 + (*sinw)*(*sinw)*sumx2
			- 2.*(*sinw)*(*cosw)*sumxy;
		if (*xd > *xf) {
			*cosw = - *cosw;
			*sinw = - *sinw;
		}
		else if ((*xd == *xf) && (*yd > *yf)) *sinw = -1.;		
#ifdef DEBUG
	if (debug_ >= 1) {
		fprintf(stderr,"segment de: %8.3f %8.3f\n", *xd, *yd);
		fprintf(stderr,"         a: %8.3f %8.3f\n", *xf, *yf);
		fprintf(stderr,"\nbarycentre: %8.3f %8.3f\n",*xm,*ym);
		fprintf(stderr,"direction:  %8.3f %8.3f\n",*cosw,*sinw);
		fprintf(stderr,"erreur:   %10.3f%\n\n\n",*erreur);
	}
#endif
	}
	return;
}
