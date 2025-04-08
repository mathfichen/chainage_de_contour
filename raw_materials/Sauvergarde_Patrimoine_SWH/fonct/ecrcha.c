/*ecrcha
+===============================================================+
!								!
!	Giraudon  -  INRIA . SOPHIA	:   17/06/86		!
!								!
!	modif du 1/03/89 : allocation dynamique de maillon	!
!								!
+===============================================================+
!								!
!	Routine d'ecriture sur disque des chaines		!
!	On ecrit une chaine sur disque en allant chercher son	!
!	pointeur de tete dans le vecteur KBUF			!
!								!
!	Parametre d'entree :					!
!	-------------------					!
!	outname  :   	nom du fichier de chaines		!
!	kbuf	 :	vecteur contenant les pointeurs de tete !
!			de chaines				!
!	nomcha	 :	nombre ce chaines crees.		!
!	itail	 :	taille supplementaire par maillon	!
!								!
+===============================================================+
*/

#include <math.h>
#include <stdio.h>
#include <dzv.h>
#include <include/chaine/maillon.h>


#define MA 7   /* on ne peut creer des maillons a plus de 7 elements */
#define BA 2   /* nombre de valeur flottante de l'entete */
#define JO 5   /* nombre short par famille */
#define PLUS_MAIL 50 /* nombre arbitraire de maillon rajoutes lors d'un
			malloc de maillon => evite malloc intempestif */

/* structure des chaines sur support externe 

   JO short :    nombre et liste des numeros des chaines connectees en tete
   JO short :    nombre et liste des numeros des chaines connectees en queue
   1 short  :	si cette chaine est un contour ferme

   BA float : 	moyenne des niveaux de gris
		variance
 */

ecrcha(outname,kbuf,nomcha,itail,ifmt)

	int nomcha, itail, ifmt[];
	int kbuf[];
	char outname[];
	{

	DZV *dzv_fic , *dzv_elt , *dzv_mai ;
	int fd ;

	char *calloc() ;
	int ta , numero = 0 , j , k=0 , i , *Fils , *Aieul , *r_mail ;
	int ipoi , totalm , *lfils_() , *laieul_() , nb , nb1 ;
	short Sfils[5], Saieul[5] , *Smaillon , Ferm , *rest1 ;
	float s ,*s1 ,*s2 , Resul[BA] ;
	int ix1, iy1, niv1, ier, ix, iy, niv, dimension[2];
	int nb_mail, tail_mail = 0;
	struct ms *maillon;

	dimension[0]=ifmt[0] ; dimension[1]=ifmt[1] ;
	ta = 3 + itail ;
	if (ta > MA ) 
		{
		fprintf(stderr," Vous avez donne une taille de maillons > 7, je ne peut traiter \n") ;
		return(-1) ;
		}		

	Smaillon = (short*) calloc( ta , sizeof(short) ) ;
	*(Smaillon) =0 ;

/* Definition du fichier sur disque */
	dzv_fic = cr_dzv(3,20,"char","commentaire",1,"int","dimx",1,"int","dimy");
		de_bug("dzv_fic mal passe") ;
	dzv_elt = cr_dzv(4,JO,"short","jonct tete",JO,"short","jonct queue",1,"short","fermeture",BA,"float","moyenne variance") ;
	de_bug("dzv_elt mal passe") ;
	dzv_mai = cr_dzv(1,ta,"short","x y ng") ;
	de_bug("dzv_mai mal passe") ;

/**** 24/2/88 ****/
	fd = cr_fic_base(outname,"chaines",nomcha,dzv_fic,dzv_elt,dzv_mai);
	de_bug("creation mal passee") ;

	wr_fic(fd, "chainage avec newkchain" ,ifmt,ifmt+1 );
	de_bug("wr_fic :  mal passee") ;

/*fprintf(stderr,"\n Creation du fichier effectue avec le format suivant : \n") ;
	pr_fic("stderr",outname) ;
	printf(" nombre de chaines %d \n",nomcha) ; */

	while ( k < nomcha)
	{
		ipoi = *( kbuf + k++) ;
		if (ipoi == 0) continue ;
		numero += 1 ;
		Ferm = iscont_(&ipoi);
		nb_mail = nbmail_(ipoi);
		if ( ( Ferm == 0) && (nb_mail > 4) ) 
		{       
/* si la chaine est trop petite on ne teste pas */

/* on teste si c'est une chaine marquee fermee . Sinon on regarde le 
   1er et le dernier maillon , si ils sont a cote on ferme la chaine 
   on rajoutant le 1er maillon a la fin */
			fimail_(&ipoi,&ix,&iy,&niv,&ier) ;
			remail_(&ipoi,&ix1,&iy1,&niv1,&ier) ;

			if (abs(ix-ix1) <= 1 && abs(iy-iy1)<= 1 )
			{
				nwmail_(ipoi,iy,ix,niv) ;
     				contch_(&ipoi) ;
				Ferm = 1 ;
				nb_mail++;
			}			

		}

/* Allocation dynamique de maillon pour 'nb_mail' si necessaire */

		if (nb_mail > tail_mail) {
			free(maillon);
			tail_mail = nb_mail + PLUS_MAIL;
			maillon = (struct ms *)
				  malloc(tail_mail * sizeof(struct ms));
		}

/* Maintenant on va recuperer tous les maillons de la chaine ipoi ainsi que
   le nombre de maillons */
		totalm = getmail_(&ipoi,maillon) ; 

		s1 = Resul ; s2 = Resul + 1 ;
		*s1 = 0 ; *s2 = 0 ;
		r_mail = maillon ;
		for (i=0 ; i < totalm ; i++)
		{
			r_mail += 2;
			s = (float) *(r_mail++);
				/* car il faut avancer de trois */
			*(s2) = *(s2) + s * s ;
			*(s1) = *(s1) + s ;
		}
		*(s1) = *(s1) / (float)totalm ;
		*(s2) = (*(s2) / (float) totalm) - (*(s1) * *(s1) );

		Fils = lfils_(ipoi,&nb) ;
		Sfils[0] = (short) nb ;
		for (i=0 ; i<nb ;i++) 
			Sfils[i+1] = (short) Fils[i] ;
		for (i=nb ; i<4 ; i++)
			Sfils[i+1] = 0 ;

		Aieul = laieul_(ipoi,&nb1) ;
		Saieul[0] = (short) nb1 ;
		for (i=0 ; i<nb1 ;i++) 
			Saieul[i+1] = (short) Aieul[i] ;
		for (i=nb1 ; i<4 ; i++)
			Saieul[i+1] = 0 ;


	reorg_chain(maillon,totalm,Saieul[0],Sfils[0]); /* amelioration de la chaine */

	wr_elt( fd , numero , totalm ,  Saieul, Sfils ,&Ferm , Resul) ;
		de_bug("wr_elt mal passe") ;

		i = 0 ;
		r_mail = maillon ;
		while ( i++ < totalm)
		{
		rest1 = Smaillon ;
			j=0;
			while (j++ < ta)
				*(rest1++) = (short) *(r_mail++) ;
			wr_mai ( fd , Smaillon ) ;
			de_bug("wr_mai mal passe") ;
		}
/*	free_chain(ipoi); */
	}
	cl_fic(fd) ;
	return ;
}

