/*apoly_or
+-----------------------------------------------------------------------+
!									!
!	Giraudon -INRIA Sophia     :      Avril 1988			!
!                                     Nlle version : Fevrier 1990       !
!									!
+-----------------------------------------------------------------------+
!									!
!	Ce programme permet de realiser l'approximation polygonale	!
!	d'un fichier de chaine.						!
!	Lecture des chaines avec le nouveau standart.			!
!	Differents filtres sur les chaines peuvent etre utilises 	!
!	La lecture des maillons se fait en short. 			!
!       Si l'image brute est presente alors on effectue en plus         !
!       le calcul de l'orientation vrai du segment (ex prog seg_ori)    !
!       Le codage de la direction de segment est calcule a partir       !
!       du contraste local de l'image. Le principe est le suivant :     !
!   	- Le cote fonce du contour est toujours a droite dans le sens   !
!	  de parcours du segment.				  	!
!   Il rend l'orientation vrai (comprise entre 0-180 degre et -180-0)	!
!   le cosinus et le sinus vrai et reoriente le segment si necessaire   !
!									!
!	Si l'image n'est pas presente on met la valeur 1000 dans theta  !
!									!
!	En entree :							!
!	chaine		:	nom du fichier de chaine en entree	!
!	segment		:	nom du fichier de segment en sortie	!
!	im              :       donner le nom de l'image brute          !
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
! - s      :  seuil en flottant  sur le rapport des valeurs             !
!	      par defaut = 4.						!
! - f      :  taille de la fenetre dans laquelle on va observer l'image !
!	      brute (typiquement 5 <.< 11. Par defaut 7			!
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

extern debug_ , koctet_ ;

#define MA 7   /* on ne peut creer des maillons a plus de 7 elements */
#define CHA 2   /* nombre de valeur flottante de l'entete chaine */
#define JO 5   /* nombre short par famille */

#define BA 9   /* nombre de valeur flottante de l'entete segment*/
#define ID 5   /* nombre de "short" de l'entete segment  */

int flag_im=0 ;
int mcolo , mlign ;
int first =1;
struct image *nf ;
float *buff ;

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

	char *typage , chaine[80]  ,  segment[80] , picture[80], *calloc() ;

	int fen = 7 ,  lfmt[9] ;
	float seuil=4. ;

	int *taillz , nb_zone , ta_mai, ta_jo , ta_mo ;
	int head ,lgmax , lgmin , numero , fd_c , fd_s, nomb , vfer , Icar=2 , nb_seg;
	short  *jonct , *joncq , *identif;
	char *TYP, COMM[80] ;
	float *moye , nvgmax , nvgmin, varmax , varmin , ANGLE = 2.2 , *bavre;


/*********************************************************************************\
*								       *
* Initialisation des variables par defaut     *
*									       *
\*********************************************************************************/
	head =  lgmin = vfer =  0 ;
	nb_seg = numero = 0 ;
	lgmax = 10000 ; nvgmax = 1000. ; varmax = 10000. ; nvgmin = 1. - 1000. ;
	varmin = 1. - 1000.;


	getopt("-nu","",0    ,"%d",&numero  ,"",0);

	flag_im = getopt("-im","",0    ,"%s",picture  ,"",0);

	getopt("-s","",0    ,"%f",&seuil  ,"",0);
	getopt("-f","",0    ,"%d",&fen  ,"",0);

	getopt("-app","",0    ,"%d",&Icar  ,"",0);
	head = getopt("-sent","",0   ,"",0    ,"",0);
	vfer = getopt("-FER","",0   ,"",0    ,"",0);
	debug_ = getopt("-D","",0   ,"",0    ,"",0);
	getopt("-lcmax","",0    ,"%d",&lgmax  ,"",0);
	getopt("-lcmin","",0    ,"%d",&lgmin  ,"",0);
	getopt("-ngmax","",0    ,"%f",&nvgmax ,"",0);
	getopt("-ngmin","",0    ,"%f",&nvgmin  ,"",0);
	getopt("-vamax","",0    ,"%f",&varmax ,"",0);
	getopt("-vamin","",0    ,"%f",&varmin ,"",0);
	getopt("-ang","",0    ,"%f",&ANGLE ,"",0);


   if ( getopt("-","",0,"",0,"",0 ) || !getopt("","%s",chaine,"",0,"",0) )
		{
		strcpy(chaine,"<"); /* standart input */
		}

	if ( !getopt("","%s",segment,"",0,"",0) )
		{
		strcpy(segment,">"); /* standart output */
		}

	if ( !testopt() ) exit(-1);


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
	lfcars(fd_c,fd_s,ta_mai,jonct,joncq,moye,head,vfer,lgmax,lgmin,nvgmax,nvgmin,varmax,varmin,Icar,ANGLE,bavre,identif,&nb_seg,picture,fen,seuil) ;
		goto fin ;
		}
	

	if (numero == -1)
		{
laber : 
 	fprintf(stderr," \n Donner le numero de la chaine (-1 pour fin) : ") ;
	scanf("%d",&numero) ;
	if (numero <= -1) goto fin ;
	if (numero == 0) goto labes ;
	lfcarr(fd_c,fd_s,numero,ta_mai,jonct,joncq,moye,head,Icar,ANGLE,bavre,identif,&nb_seg,picture,fen,seuil) ;  
	goto laber ;
		}

/* ici on lit un seul segment donne par numero et on sort */
	lfcarr(fd_c,fd_s,numero,ta_mai,jonct,joncq,moye,head,Icar,ANGLE,bavre,identif,&nb_seg,picture,fen,seuil) ;  

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

lfcars(fd_c,fd_s,ta_mai,jonct,joncq,moye,head,vfer,lgmax,lgmin,nvgmax,nvgmin,varmax,varmin,Icar,ANGLE,bavre,identif,nb_seg,picture,fen,seuil) 

int fd_c, fd_s, ta_mai , vfer , Icar , head ,  lgmax , lgmin  , *nb_seg, fen ;
float  bavre[] , moye[]  , nvgmax, nvgmin , varmax , varmin ,ANGLE , seuil;
short jonct[] ,joncq[] , identif[] ;
char picture[] ;
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
			approx_or(fd_s,numero,nb_mai,ta_mai,MAILLONS,ferme,ANGLE,bavre,identif,Icar,nb_seg,picture,fen,seuil) ;
			if (head) imprhead(numero,nb_mai,jonct,joncq,ferme,moye) ; 

		}
  		if (VALI && FAR)
		{
			approx_or(fd_s,numero,nb_mai,ta_mai,MAILLONS,ferme,ANGLE,bavre,identif,Icar,nb_seg,picture,fen,seuil) ;
			if (head) imprhead(numero,nb_mai,jonct,joncq,ferme,moye) ; 
		}


	}

	  return(0) ;
  }



	/*******************************************
	*  lecture directe d'un element du fichier *
	********************************************/

lfcarr(fd_c,fd_s,num_elt,ta_mai,jonct,joncq,moye,head,Icar,ANGLE,bavre,identif,nb_seg,picture,fen,seuil) 
int fd_c, fd_s, num_elt , ta_mai , head ,Icar  , *nb_seg , fen ;
float  moye[] , ANGLE , bavre[] , seuil ;
short  jonct[] , joncq[] , identif[];
char picture[] ; 
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

		approx_or(fd_s,numero,nb_mai,ta_mai,MAILLONS,ferme,ANGLE,bavre,identif,Icar,nb_seg,picture,fen,seuil) ;
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





/*approx_or
+===============================================================+
!								!
!	Giraudon  -  INRIA . SOPHIA	:   2/05/85		!
!				nlle version Fevrier 1990	!
!								!
+===============================================================+
!								!
!	Routine d'appel du programme d'approximation polygonale !
!	avec ou sans approximation au moindres carres		!
================================================================+
*/


approx_or(res,numero,nb_mai,ta_mai,maillon,ferme,ang_cou,bavre,identif,Icar,numseg,picture,fen,seuil) 
int res, numero, nb_mai, ta_mai, maillon[] , Icar, *numseg, fen;
short ferme , identif[];
float ang_cou , bavre[] , seuil ;
char picture[] ;

{
	int ier,abs(), ix ,iy, ix1, iy1, niv, niv1 , nomb;
	int *marc , *rest1, *rest2 , kk ;
	float *frest1, *frest2 , s=0. ,s1=0. , longu ;
	register int gog , iso = 0  , k=0 , lop ;
	register int  ngo, nsom , nsom1 ;
	float ongu, ranh, cosw, sinw , erreur , gaillons[14] , delx, dely, theta;
	float xm ,ym ,xd ,yd ,xf ,yf  , mail_o[7] , mail_e[7];

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

	for (kk=0;kk<ta_mai;kk++) {
			mail_o[kk] = gaillons[kk] ;
			mail_e[kk] = gaillons[kk+ ta_mai] ;
			}


	bavre[0] = longu ; /* longueur du segment */
	bavre[1] = s ;  /* moyenne des niveaux de gris */
	bavre[2] = s1 ; /* variance */
	bavre[3] = xm ; /* milieu x */
	bavre[4] = ym ; /*   "    y */
	bavre[5] = cosw ; /* cosinus */
	bavre[6] = sinw ; /* sinus */
	bavre[7] = 1000 ; /*orientation en degre */
	bavre[8] = erreur/longu ; /* erreur a la droite */

	identif[0] = numero ; /* numero de la chaine mere */
	identif[1] = nb_mai ; /* nbre de chain. de cette chaine*/
	identif[2] = ferme ;  /* contour ferme */
	identif[3] = nsom - 1 ; /* nb de segm. conte. ds cett. cha */
	identif[4] = 0 ; /* slot vide */

	if (flag_im) orient(picture,mail_o,mail_e,ta_mai,seuil,bavre,identif,0,fen) ;

	wr_elt( res , *numseg , 2 , bavre, identif ) ;
					de_bug("wr_elt mal passe") ;
	wr_mai ( res , mail_o ) ;
					de_bug("wr_mai mal passe") ;
	wr_mai ( res , mail_e );
					de_bug("wr_mai mal passe") ;
	}

}




orient(picture,mail_o,mail_e,ta_ma,seuil,bavre,identif,ntest,fen)
int ta_ma , ntest, fen;
short identif[] ;
float bavre[] , seuil, mail_o[]  , mail_e[]  ;
char picture[] ;
	{
	int deltax , deltay , ix ,iy , ma , fla ,flo, iy1 , m_mili 
	, m_lig , lfmt[9] ;
	int zebul() , abs() , idebug = 0 , ici;
	struct image *image_()  ;
	float  s , s1 , ngxy , Ix , Iy , cos , sin , theta, angle()  ;
	

	if (first) { 	if (flag_im) nf = image_(picture,"e","",lfmt) ;
			mcolo = lfmt[0] ;
			mlign = lfmt[1] ;
			buff = (float*) calloc((fen+2)*mcolo ,sizeof(float) ) ;
			first = 0 ;
		   }

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

	if ( (iy+m_mili) > mlign ) iy = mlign - m_mili ;
	if (ix < m_mili ) ix = m_mili ;
	if ( (ix+m_mili) > mcolo ) ix = mcolo ;
	iy1 = iy - m_mili ;

	lptset_(&nf,&iy1) ;
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
	if (delx == 0) delx = 0.00001 ;
	res = atan2(dely,delx) ;
	*cosi = cos(res) ;
	*sinu = sin(res) ;
	res = (res*180.)/3.141592 ;
	return(res) ;
}
