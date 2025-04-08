#include <stdio.h>
#include <include/chaine/maillon.h>

#define NIL     0
#define CONTOUR 2

extern int *lfils_();
extern int *laieul_();

/*-------tableaux permettant de rendre plus efficace la fusion et bruit ----*/
int *kbruit;
struct maillon **kfus;
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
/*									     */
/*	REMP_DS_FAMILLE : pour chaque chaine j de "famille" on remplace      */
/*		  	  "n_ancien" dans le lien de filiation ou il         */
/*                         apparait par "n_new" .		             */
/*									     */
/*****************************************************************************/
remp_ds_famille(kbuf,famille,n_new,n_ancien,idebug)
	int famille[];
	struct maillon *kbuf[];
	int idebug;
	int n_ancien, n_new;
	{ 
	struct maillon *x;
	int j;
	if ( idebug == 7 )
		{ 
		  printf("....remp_ds_famille : ");
		  printf("n_ancien=%d; n_new=%d; famille=",n_ancien,n_new);
		  for ( j=0 ; j<4 ; j++) printf("%d,",famille[j]);
		  printf("\n");
		}
        for ( j=0 ; j<4 ; j++) 
		{
		if (famille[j] == 0) break;
		if (famille[j] > 0)
			{
			 x = kbuf[ famille[j] -1 ];
			 if ( !(sraieul_(x,n_ancien,n_new))) 
						ssaieul_(x,abs(n_new));
			}
		 else   {
			 x = kbuf[ -(famille[j]) -1 ]; 
			 if (!(srfils_ (x,n_ancien,n_new)))  
						ssfils_(x,abs(n_new)) ; 
			}
		}
	}
/*****************************************************************************/
/*									     */
/*	SUPP_DS_FAMILLE : pour chaque chaine j de "famille" on supprime      */
/*		  	  "number" dans le lien de filiation ou il apparait  */
/*									     */	
/*****************************************************************************/
supp_ds_famille(kbuf,famille,number,idebug)
	int famille[];
	struct maillon *kbuf[];
	int idebug;
	int number;
	{ 
	struct maillon *x;
	int j;

	if ( idebug == 8)
		{ 
		  printf("... supp_ds_famille : ");
		  printf("number=%d, famille=",number);
		  for (j=0;j<4;j++) printf("%d,",famille[j]);
		  printf("\n");
		}
        for ( j=0 ; j<4 ; j++) 
		{
		if (famille[j] == 0) break;
		if (famille[j] > 0)
			{
			 x = kbuf[ famille[j] -1 ];
			 if ( !(ssaieul_(x,number)))
				printf("SUPP_DS_FAMILLE :erreur Algo aieul\n");
			}
		 else   {
			 x = kbuf[ -(famille[j]) -1 ];
		         if (!(ssfils_ (x,number)))
				printf("SUPP_DS_FAMILLE :erreur Algo aieul\n");
			}
		}
	}
/*****************************************************************************/
/*									     */
/*	NBFILS,NBAIEUL : 						     */
/*		  							     */
/*		- En entree : une chaine x				     */
/*									     */
/*		- En sortie : Si la chaine x a un seul fils 		     */
/*				 (respectivement aieuls) retourne ce fils    */
/*			      sinon NIL					     */
/*									     */
/*****************************************************************************/

struct maillon *nbfils(x,kbuf,idebug) 

  struct maillon *x,*kbuf[];
  int    idebug;
{ 
  int nb,i,*fils;
  if (idebug == 7) 
		{
		printf("..nbfils : x=%d; fils=",x->u.t.numero);
		}
  fils = (int *) lfils_(x,&nb);
  if (idebug ==7)
	{ 
	  for (i=0; i<nb ; i++) printf("%d,",fils[i]);
	  printf("\n");
	}
  for ( i=0 ; i < nb ; i++ )
	{ 
	if (abs( fils[i]) == x->u.t.numero)
		{
		x->u.t.mode = CONTOUR;
		if (idebug == 200) 
			{printf("la chaine %d est un contour\n",x->u.t.numero);
			 printf("                Detection dans nbfils\n");
			}
		return(NIL);
		}
	}
  if ( nb == 1) return( kbuf[ abs(fils[0]) -1] );
  else return(NIL);
}

/*...........................................................................*/

struct maillon *nbaieul(x,kbuf,idebug) 

  struct maillon *x,*kbuf[];
  int    idebug;
{ 
  int nb,i,*aieul;
  if (idebug == 7) 
		{
		printf("..nbaieul : x=%d; aieul=",x->u.t.numero);
		}
  aieul = (int *) laieul_(x,&nb);
  if (idebug ==7)
	{ 
	  for (i=0; i<nb ; i++) printf("%d,",aieul[i]);
	  printf("\n");
	}
  for ( i=0 ; i < nb ; i++ )
	{ 
	  if ( abs(aieul[i]) == x->u.t.numero)
		{
		x->u.t.mode = CONTOUR;
		if (idebug == 200) 
			{printf("la chaine %d est un contour\n",x->u.t.numero);
			 printf("                Detection dans nbaieul\n");
			}
		return(NIL);
		}
	}
  if ( nb == 1) return( kbuf[ abs(aieul[0]) -1] );
  else return(NIL);
}

/****************************************************************************/
/*									    */
/*    INVERS_ORIENT : Cette procedure modifie pour chacune des chaines i de */
/*		      "famille" , le "numero" apparaissant dans la famille  */
/*			de tete ou de queue de la chaine i .		    */
/*			La modification consiste a inverser le signe de     */
/*			"numero".					    */
/*									    */
/****************************************************************************/
int invers_orient(kbuf,famille,numero,idebug)
	int famille[],numero,idebug;
	struct maillon *kbuf[];
{ int i,j,lolo=0;
  struct maillon *x;
  if (idebug == 7)
	{
	printf("........invers : numero=%d; famille =",numero);
	for (i=0 ; i<4 ; i++) printf("%d;",famille[i]);
	printf("\n");
	}
  for (i=0 ; i<4 ; i++)
	{ 
	if (famille[i] == 0) return(1) ; 
	lolo = 0;
	if ( famille[i] > 0)
		{
		x = kbuf[ famille[i] -1 ];
		if (idebug ==7)
		    { printf(".....ds invers, famille de %d =",famille[i]);
			  for (j=0; j<4 ;j++) 
				printf("%d,",(x->succ)->u.e.famille[j]);
			  printf("\n");
			}
		for (j=0 ; j<4 ; j++)
			if ((x->succ)->u.e.famille[j] == numero) 
				{(x->succ)->u.e.famille[j]= -numero;
/*			    	 return(1); gg : mod du 16/04/89*/
				lolo = 1 ;
				break ;
			   	}
/*		return(0);  gg : mod du 16/04/89 on retournait trop tot*/
		}
	else    {
		x = kbuf[ -(famille[i]) -1 ] ; 
		if (idebug ==7)
		       { printf(".....ds invers, famille de %d =",famille[i]);
			 for (j=0; j<4 ;j++) 
			      printf("%d,",((x->succ)->succ)->u.e.famille[j]);
			 printf("\n");
		       }
		for (j=0 ; j<4 ; j++)
			if (((x->succ)->succ)->u.e.famille[j] == numero) 
			   	{((x->succ)->succ)->u.e.famille[j]= -numero;
/*			    	 return(1); gg : mod du 16/04/89*/
				lolo = 1 ;
				break ;
			   	}
/*		 return(0);*/
		}
	if (lolo == 0) return(0) ;
	}
	return(1) ;
}
/****************************************************************************/
/*									    */
/*	INCLURE :  fonction qui doit inclure la chaine p dans la chaine x . */
/*		  							    */
/*			._______x________. ._______p_______.		    */
/*				     typex typep			    */
/*									    */
/*      type_  =  'F' pour une fin de chaine.				    */
/*	          'D' pour un debut de chaine.				    */
/*      Si typep = typex alors il est necessaire de retourner une des chaine*/
/*									    */
/*      la fonction retourne un pointeur sur la chaine resultat		    */
/*									    */
/****************************************************************************/

struct maillon *inclure(kbuf,p,typep,x,typex,idebug)

  char typep,typex;
  struct maillon *p, *x, *kbuf[];
  int idebug;
{ 
  int numero_x;
  if (idebug == 7) 
		{
		printf("....inclure  p=%d (typep=%c) dans x=%d (typex=%c)\n",
				p->u.t.numero,typep,x->u.t.numero,typex);
		}

  numero_x = x->u.t.numero;

  if (typex == typep)
	{
	  miroir_(p);
	  if ( !(invers_orient(kbuf,(p->succ)->u.e.famille,-(p->u.t.numero),
						idebug )) )
		printf("INCLURE : erreur algo p->succ\n"); 
	  if ( !(invers_orient(kbuf,((p->succ)->succ)->u.e.famille,
							p->u.t.numero,
						idebug )) )
		printf("INCLURE : erreur algo p->succ->succ\n"); 
	}

  if (typex == 'D' )
		   	{
			fuschn_(p,x);
			p->u.t.numero = numero_x;
			kbuf[ numero_x -1 ] =p;
			return( p );
			}
   else			{
     if (idebug==8)
       printf(" je passe ici numero x %d  numero p %d\n",numero_x,p->u.t.numero);
			fuschn_(x,p);
		        kbuf[ numero_x -1 ]=x;
			return( x );
			}
}

/****************************************************************************/
/*									    */
/*	UN_SEUL_LIEN   : fonction  qui recherche dans la liste des fils ou  */
/*			 des aieuls de la chaine p , la chaine ayant pour   */
/*			 identificateur 'numero'.			    */
/*	En sortie : 							    */
/*		   typep = 'D' si le numero appartient a la liste des aieuls*/
/*		         = 'F' si le numero appartient a la liste des fils  */
/*		   							    */
/*		   la fonction retourne 'vrai' si il n'y a qu'un seul elt   */
/*			      			  dans la famille	    */
/*			                 'faux' sinon .			    */
/*									    */
/****************************************************************************/

int un_seul_lien( numero,p,typep,idebug)
  struct maillon *p;
  int    numero ;
  char   *typep ;
  int    idebug ;
{ struct maillon *ext;

  if (numero > 0 )
	{ *typep = 'D';
	  ext    = p->succ;
	}
  else  { *typep = 'F';
	  ext    = (p->succ)->succ;
	}
  if ( idebug==7 || idebug ==8 )
	{ printf("..un_seul : numero =%d ;typep = %c ;",numero,*typep);
	  printf(" ext->u.e.fa[0]=%d;",ext->u.e.famille[0]);
	  printf(" ext->u.e.fa[1]=%d\n",ext->u.e.famille[1]);
	}
  return( !(ext->u.e.famille[1] ) );

}

/****************************************************************************/
/*									    */
/*	FUSION : procedure qui fusionne toutes les chaines pouvant l'etre . */
/*		 Deux chaines peuvent fusionner si elles ont un lien de     */
/*		 filiation unique entre elles.				    */
/*									    */
/****************************************************************************/

fusion(kbuf,nomcha,nbsuppres,s,idebug)
	
int nomcha, *nbsuppres, s ,idebug ;

struct maillon *kbuf[];
 
{ 
  int    i ,bruit_total ;
  char   bout[4];
  struct maillon *x,*p ;
  char   typep;
  int j , numberp, *parents , *parents_p ,nb_parents ;

  for (i=0 ; i < nomcha ; i++ )
    if (kfus[i] != NIL)
	{
	if (idebug == 7) 
	       {
               printf("********** fusion : chaine %d ****************\n",i+1);
	       }
	x = kbuf[i];
	if ( x != NIL )
	       {

	       while ((p=nbfils(x,kbuf,idebug)) != NIL &&
		   	un_seul_lien(((x->succ)->succ)->u.e.famille[0],
							p,&typep,idebug))
			{
			numberp =p->u.t.numero;				
			kbuf[ numberp -1 ] = NIL;
			x = inclure(kbuf,p,typep,x,'F',idebug);
			(*nbsuppres)++;
			remp_ds_famille(kbuf,((x->succ)->succ)->u.e.famille,
					  -(x->u.t.numero),numberp,idebug);

			bruit_total = abs(kbruit[i]) + abs(kbruit[numberp-1]);
			if ( bruit_total >s )
				 kbruit[i] = - bruit_total;
			else     kbruit[i] =   bruit_total;
			kbruit[ numberp -1 ]=0;
			kfus  [ numberp -1 ]=NIL;
			}

	       while ((p=nbaieul(x,kbuf,idebug)) != NIL &&
				 un_seul_lien((x->succ)->u.e.famille[0],
							p,&typep,idebug))
			{
			numberp =p->u.t.numero;	
			kbuf[p->u.t.numero -1] = NIL;
			x = inclure(kbuf,p,typep,x,'D',idebug);
			(*nbsuppres)++;
			remp_ds_famille(kbuf,(x->succ)->u.e.famille,
				           x->u.t.numero,numberp,idebug);

			bruit_total = abs(kbruit[i]) + abs(kbruit[numberp-1]);
			if ( bruit_total >s )
				 kbruit[i] = - bruit_total;
			else     kbruit[i] =   bruit_total;
			kbruit[ numberp -1 ]=0;
			kfus  [ numberp -1 ]=NIL;
			}
		}
	       kfus[i] = NIL;
	       }
	}


/***************************************************************************/
/*									   */
/*    INIT_KBRUIT_KFUS  : 						   */
/*		Cette fonction initialise les tableaux kbruit et kfus	   */
/*	qui permettent de rendre plus efficace la fusio et l'elimination   */
/*	du bruit dans le cas d'un bruit de taille superieur a 1 .	   */
/*	Ces deux tableaux sont indexes par des numeros de chaines	   */
/*									   */
/*	KBRUIT : pour les chaines qui existent ce tableau contient en	   */
/*		 valeur absolue la taille de ces chaines sinon 0.	   */
/*		 Les chaines devant etre traitees par l'elimination du     */
/*		 bruit sont celles de taille superieur a 0 ,les autres     */
/*		 ont une taille inferieur ou egale a 0.			   */
/*		 A l'initialisation 					   */
/*			Si taille( chaine_i) <= taile_bruit		   */
/*				kbruit[ chaine_i ] = taille( chaine_i) 	   */
/*			sinon   kbruit[ chaine_i ] = - taille( chaine_i)   */
/*	KFUS   : pour les chaines qui ne peuvent pas etre fusionner	   */
/*			kfus[ chaine_i ] != 0				   */
/*		  sinon kfus[ chaine_i ] == 0				   */
/*		 A l'initialisation toutes les chaines sont supposees etre */
/*		 fusionnables						   */
/*									   */
/***************************************************************************/
init_kbruit_kfus(kbuf,nomcha,ibruit)
	struct maillon *kbuf[];
	int nomcha,ibruit;
	{
	  int taille,i;

	  kbruit = (int *) malloc(nomcha * sizeof(int));
	  kfus = (struct maillon **) malloc(nomcha * sizeof(struct maillon *));
          for (i=0 ; i<nomcha ;++i)
	       { 
	
		  if (kbuf[i] == NIL) 
		  {
			kbruit[i] = 0 ; 
			kfus[i] = NIL ;
		  }
		  else
		  {
		  if ((taille=nbmail_(kbuf[i])) <= ibruit) kbruit[i]=taille; 
                 	 else kbruit[i]= -taille;
	          kfus[i] = kbuf[i];
                  }
	       }
	}

/***************************************************************************/
/*									   */
/*    LIENC7  : procedure qui est chargee 				   */
/*									   */
/*		  - d'eliminer le bruit .				   */
/*		  - de fusionner les chaines pouvant l'etre		   */
/*		  - de tasser les chaines obtenues .			   */
/*									   */
/***************************************************************************/

lienc7_(kbuf,nomcha,ibruit,idebug)
  struct maillon *kbuf[]  ;
  int	 *nomcha ,ibruit,idebug ; {

  int nbsuppres=0,s_nbsup ;
  int s,i ;
  int choix=5 , n_chaine ;
  int nb_bruit=0, nb_fusion=0;

  if (idebug == 30) return(1);

  if (idebug >= 1) printf("Etape numero 2  : Fusion + bruit=%d\n",ibruit);
	         
/* Initialisation de "kbruit" et "kfus"     */
  init_kbruit_kfus(kbuf,*nomcha,ibruit);

  for ( i=1 ; i <= ibruit ; i++) {

	s_nbsup = nbsuppres;

	fusion(kbuf,*nomcha,&nbsuppres,ibruit,idebug);
	nb_fusion += nbsuppres - s_nbsup;
	s_nbsup = nbsuppres ;
	bruitage(kbuf,*nomcha,i,&nbsuppres,idebug) ;
	nb_bruit +=  nbsuppres - s_nbsup ;
  }   
  s_nbsup = nbsuppres ;
  fusion(kbuf,*nomcha,&nbsuppres,ibruit,idebug) ;
  nb_fusion += nbsuppres - s_nbsup;		
  if (idebug >= 1) {
		 printf("        nombre de bruits  = %d \n",nb_bruit);
		 printf("        nombre de fusion  = %d \n",nb_fusion);
	         printf("        nombre de chaines = %d \n",
						(*nomcha) - nbsuppres);
  }
  }

	

