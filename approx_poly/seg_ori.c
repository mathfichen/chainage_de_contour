/*
**************************************************************************
*									 *
*   Giraudon - INRIA sophia     :      Juin  1987		         *
*									 *
**************************************************************************
*									 *
*   Ce programme permet le codage de la direction de segment a partir    *
*   du contraste local de l'image. Le principe est le suivant :		 *
*   	- Le cote fonce du contour est toujours a droite dans le sens    *
*	  de parcours du segment.				  	 *
*   Il rend l'orientation vrai (comprise entre 0-180 degre et -180-0)	 *
*   le cosinus et le sinus vrai et reoriente le segment si necessaire    *
* 									 *
* - D	   :  debug							 *
* - d      :  numero du vecteur a debugger                               *
* - k      :  nombre de kilo octets                                      *
* - s      :  seuil en flottant  sur le rapport des valeurs              *
*	      par defaut = 4.						 *
* - f      :  taille de la fenetre dans laquelle on va observer l'image  *
*	      brute (typiquement 5 <.< 11. Par defaut 7			 *
*									 *
*   inname      nom de l'image d'entree 				 *
*		(codage possible octet, entier long,  flottant)		 *
*   innam1	nom du fichier de segments en entree et en sortie        *
*									 *
**************************************************************************
*/


#include <math.h>
#include <inrimage/image.h>
#include <dzv.h>
#include <stdio.h>
extern debug_ , koctet_ ;

double seuil_d ;

int xargc;
char **xargv;
main(argc,argv)
int argc;
char *argv[];
{

	char *inname , *innam1 , *calloc() ;
	float *buff , seuil = 4. ;
	int  lfmt[9] ,  ipoids , logcha = 2 , logseg =1 , ndeb = -1;
	int  koctet_ = 100 , fen=7 ;
        int dzv_dimx, dzv_dimy;
	struct image *nf , *image_() ; 

	int fd ;

	DZV *dzv_fic , *dzv_elt , *dzv_mai ;

	int bid,nomb;
	int *taillz , nb_zone , ta_mai, ta_ba , ta_id ;
	char *typage ;
	float *mail_o, *mail_e , *bavre ;
	short *identif ;
	char *TYP, COMM[80] ;

	xargc = argc ;
	xargv = argv ;

	{
                              /********************************/
                               /* Initialisation des arguments */
 
 
 arg_init(xargc,xargv,"  Calcul des orientations des segments de fichier Segment (DZV)" ,
                                "15/11/93 (inrimage version 4.3)",
                                " G. Giraudon ") ;
 
      arg_get("%S","Nom de l'image (inrimage) initiale",&inname) ;
      arg_get("%S","Nom du fichier des segments (entree et sortie (DZV))",&innam1) ;

      arg_get(""," ");
      arg_get("","/* *********************** Parametres ********************** */");

arg_get("-s %lf 4.","Seuil sur la difference des niveaux de gris acceptable",&seuil_d) ;
arg_get("-f %d 7","Fenetre image a partir de laquelle on calcule l'orientation \n le calcul est fait uniquement sur le point milieu du segment",&fen) ;
        arg_get(""," ");
        arg_get("","/* *********************** Options ********************** */");
arg_get("-d %d -1","Options : \n -1 Traitement de tous les segments \n -x traitement du segment x \n ",&ndeb) ;
debug_ = arg_get("-D 0","option de debugging inrimage (facultative) sans argument");
         arg_end() ;

	seuil = (float) seuil_d ;

	nf = image_(inname,"e","",lfmt) ;
	fd = G_open(innam1,&TYP,&nomb,&dzv_fic,&dzv_elt,&dzv_mai) ;
	re_fic(fd,COMM, &dzv_dimx, &dzv_dimy );
	de_bug("re_fic:  mal passee") ;

	if ( strcmp("segments",TYP) )
		{
  printf(" Le fichier n'est pas du type segments mais du type %s \n",TYP);
		exit(-1) ;
		}

/* Definition du fichier sur disque auquel on s'attend */
	re_dzv(dzv_elt,&nb_zone,&taillz,&typage) ;
	ta_ba = taillz[1] ;
	ta_id = taillz[2] ;
	if ( nb_zone != 2 && (ta_ba != 9 && ta_ba != 3) && ta_id != 5 )
		{
 printf(" La definition de dzv_elt n'est pas celle du fichier lu \n") ;
		exit(-1) ;
		}
	bavre = (float*) calloc( ta_ba , sizeof(float) ) ;
	identif = (short*) calloc( ta_id , sizeof(short) ) ;


	re_dzv(dzv_mai,&nb_zone,&taillz,&typage) ;
	ta_mai = taillz[1] ;
	if (nb_zone != 1 && ta_mai != 3  )
		{
 printf(" La definition de dzv_mai n'est pas celle du fichier lu \n") ;
		exit(-1) ;
		}
	mail_o = (float*) calloc( ta_mai , sizeof(float) ) ;
	mail_e = (float*) calloc( ta_mai , sizeof(float) ) ;

	ipoids = 9 * lfmt[0] * 4 ;
	buff = (float*) calloc( ipoids , sizeof(float) ) ;

    lfcars(nf,buff,lfmt,fd,nomb,mail_o,mail_e,ta_mai,bavre,ta_ba,identif
	  ,ta_id,seuil,ndeb,fen) ;
	
	close( fd ) ;

	}
}



	/************************************************
	*  lecture sequentielle d'un element du fichier *
	*************************************************/

lfcars(nf,buff,lfmt,fd,nomb,mail_o,mail_e,ta_mai,bavre,ta_ba,identif
,ta_id,seuil,ndeb,fen)

int fd, ta_mai, ta_ba , ta_id , lfmt[] , nomb , fen ;
float  mail_o[] , mail_e[], bavre[] , buff[] , seuil;
short identif[] ;
struct image *nf ;
	{
	int numero=1, nb_mai , k , first , nomtot , ntest=0;

	nomtot =nomb+1 ;
	nomtot = get_max_elt( fd );

	while( numero <= nomtot)
		{
/*		if ( ra_fic(fd,numero,0)) */
		if ( G_seek_elt(fd,numero)) /* Modif du 15/11/93 */
			{
			S_read_elt(fd,&numero,&nb_mai,bavre,identif)  ;
			de_bug("erreur sur la lecture !!!!!!!!") ;
			if (nb_mai != 2 )
				{
				fprintf(stderr," erreur nb_mai != 2 \n") ;
				return(-1) ;
				}
			S_read_mai(fd,mail_o)  ;
			S_read_mai(fd,mail_e)  ;
			if (numero == ndeb) ntest = 1 ;
	
orient(nf,buff,lfmt,mail_o,mail_e,ta_mai,seuil,bavre,identif,ntest,fen) ;

			G_seek_elt(fd,numero) ; 
			S_write_elt(fd,numero,nb_mai,bavre,identif);
			G_seek_mai(fd,numero,1) ;
			S_write_mai(fd,mail_o)  ;
			G_seek_mai(fd,numero,2) ;
			S_write_mai(fd,mail_e)  ;
			}
		ntest = 0 ;
		numero += 1;
		}
	return(1) ;
	}



orient(nf,buff,lfmt,mail_o,mail_e,ta_ma,seuil,bavre,identif,ntest,fen)
int  lfmt[9]  , ta_ma , ntest, fen;
short identif[] ;
float bavre[], buff[] , seuil, mail_o[]  , mail_e[]  ;
struct image *nf ;

	{
	int deltax , deltay , ix ,iy , ma , fla ,flo, iy1 , m_mili 
	, m_lig ;
	int zebul() , mcolo , abs() , idebug = 0 , ici;
	float  s , s1 , ngxy , Ix , Iy , cos , sin , theta, angle() ;
	
	mcolo = lfmt[0] ;
	idebug = debug_ ;
	m_mili = fen/2 ;
	m_lig = (fen-3)/2  ;

/* Definition du point milieu du segment */
	Ix = bavre[3]  ; ix = (int) (Ix +.5) ;
	Iy = bavre[4]  ; iy = (int) (Iy +.5) ;


/* Definition du cadrant */
	deltax = (int)(mail_o[0] - Ix) ;	
	deltay = (int) (mail_o[1] - Iy) ;
	if (deltax == 0) ma = 0 ;
	if (deltay == 0) ma = 10 ;
	if ( (deltax * deltay) > 0 ) ma = 11;
	if ( (deltax * deltay) < 0 ) ma = 22 ;

	if (( abs(deltax) < 3) && (abs(deltay) > 8) ) ma = 0 ;

	if (ma == 11 )
		{
		if ( abs(deltax) >= abs(deltay) ) ma = 4 ;
		else ma = 3 ;
		}
	if (ma == 22 )
		{
		if ( abs(deltax) >= abs(deltay) ) ma = 1 ;
		else ma = 2 ;
		}

	if (idebug || ntest) 
		{
		printf(" ma = %d \n ",ma) ;
		printf(" deltax = %d  deltay = %d \n",deltax,deltay) ;
		}

/* modif provisoire de monique:	if (iy < m_mili ) iy = m_mili ;
 * remplace par: */
	if (iy <= m_mili) iy = m_mili+1;

	if ( (iy+m_mili) > lfmt[1] ) iy = lfmt[1] - m_mili ;
	if (ix < m_mili ) ix = m_mili ;
	if ( (ix+m_mili) > mcolo ) ix = mcolo ;
	iy1 = iy - m_mili ;
	c_lptset(nf,iy1) ;
	lecflt_(&nf , &fen , buff) ;
	ix = ix - 1 ;   /* car buff commence a zero  */
	ngxy = buff [ m_mili*mcolo + ix ] ;
/*	if (ngxy == 0) ngxy = bavre[1] ; cas ou l'image est de synthese */

	if (ma == 0)
		{
		fla = m_lig*mcolo + ix - m_mili ;
		flo = m_lig*mcolo + ix + m_mili ;
		s = (buff[ fla ] + buff[ fla + mcolo] + buff[fla + 2*mcolo] )
			/ 3. ;
		s1 = (buff[flo] + buff[flo + mcolo] + buff[flo + 2*mcolo ] )
			/ 3. ;
		}

	if (ma == 10) 
		{
		flo = (fen-1)*mcolo + ( fla = ix - 1) ;
		s = (buff[fla] + buff[fla+1] + buff[fla+2] ) / 3. ;
		s1 = (buff[flo] + buff[flo+1] + buff[flo+2] ) / 3. ;
		}

	if (ma == 1 )
		{
		fla = ix - m_mili ;
		flo = (fen-1)*mcolo + ix + m_mili ;
		s = (buff[fla] + buff[fla+1] + buff[fla+2] + 
				buff [mcolo + fla ] ) / 4. ;
		s1 = (buff[flo] + buff[flo - 1] + buff[flo - 2] + 
				buff [flo - mcolo] ) / 4. ;
		}
	if (ma == 2 )
		{
		fla = ix - m_mili ;
		flo = (fen-3)*mcolo + ix + m_mili ;
		s = (buff[fla] + buff [fla + 1] + buff [ fla + mcolo] +
			buff [ fla + 2*mcolo ] ) / 4. ;
		s1 = (buff[flo] + buff [ flo + mcolo ] + buff[ flo + 2*mcolo]
			+ buff[flo + 2*mcolo - 1] ) / 4. ;
		}

	if (ma == 3 )
		{
		fla = ix + m_mili ;
		flo = (fen-3)*mcolo + ix - m_mili ;
		s1 = (buff[fla] + buff [fla - 1] + buff [ fla + mcolo] +
			buff [ fla + 2*mcolo ] ) / 4. ;
		s = (buff[flo] + buff [ flo + mcolo ] + buff[ flo + 2*mcolo]
			+ buff[flo + 2*mcolo + 1] ) / 4. ;
		}

	if (ma == 4 )
		{
		fla = ix + m_mili - 2 ;
		flo = (fen-2)*mcolo + ix - m_mili ;
		s = (buff[fla] + buff [fla + 1] + buff [ fla + 2 + mcolo ] +
			buff [ fla + 2 ] ) / 4. ;
		s1 = (buff[flo] + buff [flo + mcolo ] + buff[flo + mcolo + 1]
			+ buff[flo + mcolo + 2] ) / 4. ;
		}

/* Calcul du contraste (zebul), de l'angle et inversion des maillons
 si necessaire */

		ici = zebul(s,s1,ngxy,seuil) ;

	if (idebug || ntest) 
	printf(" s  %f  s1 = %f  ngxy %f ici = %d \n",s,s1,ngxy,ici) ;


		if (ici == 1)
		{
			if ((ma == 0 || ma == 2 || ma == 3) && deltay < 0)
			inversion(mail_o,mail_e,ta_ma,ntest) ;
			if ((ma == 10 || ma == 1 || ma == 4) && deltax > 0)
			inversion(mail_o,mail_e,ta_ma,ntest) ;
		}

		if (ici == 2 )
		{
			if ((ma == 0 || ma == 2 || ma == 3) && deltay > 0)
			inversion(mail_o,mail_e,ta_ma,ntest) ;
			if ((ma == 10 || ma == 1 || ma == 4) && deltax < 0)
			inversion(mail_o,mail_e,ta_ma,ntest) ;
		}

		theta = angle(mail_o,mail_e,&cos,&sin) ;

		if (idebug || ntest) 
		{
			fprintf(stderr," s ngxy s1 ici = %f %f %f %d \n "
					,s,ngxy,s1,ici) ;
			fprintf(stderr," theta cos sin = %f %f %f \n "
					,theta,cos,sin) ;
		}

	bavre[5] = cos ; bavre[6] = sin ; 
	bavre[7] = theta ;
	identif[4] = ici ;

	return ;
}

/***************************************************************************/
/*Calcul de s et de s1 : moyenne des niveaux de gris autour du point designe
*/

zebul(s,s1,ngxy,seuil)
float s, s1, ngxy , seuil ;
{
	int ici ;
	float dag , dog , nmax ,nval ;

	if ( ( s == s1) && (ngxy >= s) )
	{
		ici = 3 ;
		return(ici) ;
	}
	if ( ( s == s1) && (ngxy < s) )
	{
		ici = 4 ;
		return(ici) ;
	}

	if ( ( s >= ngxy) && ( ngxy >= s1) )
	{
		ici = 1 ;
		return(ici) ;
	}


	if ( ( s1 >= ngxy) && ( ngxy >= s) )
	{
		ici = 2 ;
		return(ici) ;
	}

	if  (ngxy > s) 
	{
		dog = ngxy - s ;
		dag = ngxy - s1 ;
		if ( dog > dag)
		{
			nmax =  dog ;
			nval = s1 ;
		}
		else
		{
			nmax = dag ;
			nval = s ;
		}

		if (ngxy - (nmax / (seuil) ) > nval )
		{
			ici = 3 ;
			return(ici) ;
		}
		else
		{
			ici = 2 ;
			if ( s == nval) ici = 1 ;

			return(ici) ;
		}
	}
	else
	{
		dog = s - ngxy  ;
		dag = s1 - ngxy  ;
		if ( dog > dag)
		{
			nmax =  dog ;
			nval = s1 ;
		}
		else
		{
			nmax = dag ;
			nval = s ;
		}

		if (ngxy + (nmax / (seuil) ) < nval )
		{
			ici = 4 ;
			return(ici) ;
		}
		else
		{
			ici = 2 ;
			if (s1 == nval) ici = 1 ;
			return(ici) ;
		}
	}
}



/**********************************************************************
	Inversion du segment d'entree pour que le cote fonce soit
	toujours a droite du segment
*/
inversion (mail_o, mail_e, ta_mai,ntest)
float mail_o[], mail_e[] ;
int ta_mai , ntest;
{
	float *mails ; int i,j ;
	mails = (float*) calloc( ta_mai , sizeof(float) ) ;

	if (ntest) printf(" Inversion des maillons \n") ;

	for( j=0 ; j< ta_mai ; j++)
	{
		mails[j] = mail_o[j] ;
		mail_o[j] = mail_e[j] ;
		mail_e[j] = mails[j] ;
	}

	free(mails) ;
}



/********************************************************************
	Calcul de l'angle (en degre 0 a 180 et -180 a -0, de sinus
	et cosinus du segment 
*/

float angle(mail_o,mail_e,cosi,sinu) 
float mail_o[], mail_e[], *cosi ,*sinu ;
{
	float delx, dely, res ;
	
	delx = mail_e[0] - mail_o[0] ;
	dely = mail_e[1] - mail_o[1] ;
	res = atan2(dely,delx) ;
	*cosi = cos(res) ;
	*sinu = sin(res) ;
	res = (res*180.)/3.141592 ;
	return(res) ;
}
