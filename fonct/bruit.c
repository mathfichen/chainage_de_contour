#include <stdio.h>
#include <include/chaine/maillon.h>
#define NIL 0

extern int *lfils_();
extern int *laieul_();
extern int supp_ds_famille();

extern int *kbruit;
extern struct maillon **kfus;

/* *********************************************************************** */
/*                  							   */
/*    TSUPTEST   :   Cette fonction permet de tester si la chaine "tete"   */
/*                   qui est de la forme suivante peut etre supprimee      */ 
/*                                                /                        */
/*                                     .________./___                      */
/*                                       tete    \                         */
/*                                                \                        */
/*                                                                         */
/*                  Elle retourne "vrai" si c'est possible.                */
/*                   Hypotheses :  "nb" > 0                               */
/*                                 "famille"  famille de "tete"            */
/*                  							   */
/* *********************************************************************** */

TsupTest (tete,kbuf,nb,famille,idebug) 

	struct maillon *tete , *kbuf[] ; 
	int nb ;
	int famille[] ;
	int idebug ;   

{	char type  ;
	int i ;

	if (idebug == 8) 
	     {
              printf (" TsupTest  : entree \n") ;
	      printf("              tete = %d , nb = %d \n", tete , nb);
	      for (i=0;i<nb;i++) printf(" famille[%d] = %d \n",i,famille[i]);
	     }

	if (nb >= 2) return (1) ;
	
        /*--------------   ici nb == 1  -----------------------------------*/
	if (famille[0] > 0)
		return( ( !(un_seul_lien( tete->u.t.numero,
				 kbuf[famille[0] -1 ],&type, idebug) ) ) );
	else	
		return( ( !(un_seul_lien( -( tete->u.t.numero),
				 kbuf[abs(famille[0]) -1 ],&type, idebug) ) ) );
}

/* ************************************************************************* */
/*                                                                           */
/*    REFILIAT   :   Cette fonction retablit la filiation apres la           */
/*                   suppression d'un lien                                   */
/*                                                                           */
/*                                                                           */
/* ************************************************************************* */

refiliat (kbuf,progen,numprogen,idebug)

	struct maillon *kbuf[] ;
	int progen[] ;
	int numprogen ;  
	int idebug ;   

{
	struct maillon *x ;
	int j ;

	if (idebug == 8) 
	     {
		printf (" refiliat  : entree \n") ;
		for (j=0;j<numprogen;j++) printf("      progen[%d] = %d ",j,
								progen[j]);
		printf(" \n") ;
	     }

	x = kbuf[abs(progen[0])-1] ;
	if (progen[0] > 0)
	     {
		for (j=1 ; j< numprogen ; j++)
		    {
			if (progen[j] >0)
			    {
			    if (aieul_b ( x,progen[j] ) )
				    aieul_b ( kbuf[progen[j]-1],progen[0] ) ;
			    }
			if (progen[j] < 0)
			    {
			    if (aieul_b  ( x,progen[j] ) )
			         fils_b  ( kbuf[abs(progen[j])-1],progen[0] ) ;
			    }
		    }
	     }

	if (progen[0] < 0)
	     {
		for (j=1 ; j< numprogen ; j++)
		    {
			if (progen[j] > 0)
			    {
			    if (aieul_b( kbuf[progen[j]-1],progen[0] ) )
				fils_b ( x,progen[j] );
			    }
			if (progen[j] < 0)
			    {
			    if (fils_b ( kbuf[abs(progen[j])-1],progen[0] ) )
				fils_b ( x,progen[j] );
			    }
		    }
	     }
}


/* ***********************************************************************  */
/*                                                                          */
/*    SUPP_BRUIT    :  Cette fonction elinime la chaine  kbuf[i]            */
/*                     Elle met a jour la filiation des 'nb' chaines de     */
/*		       'famille'					    */
/*                                                                          */
/* ***********************************************************************  */


supp_bruit (kbuf,i,nb,famille,nbsuppres,idebug) 

	struct maillon *kbuf[] ;
	int i,nb,*nbsuppres,idebug;
	int famille[];
	{

	int j,number;
	struct maillon *x;
        if (idebug == 8) 
		              {
			      printf("supp_bruit : appel de TsupTest\n");
			      printf("kbuf[%d] = %d   ",i,kbuf[i]);
			      printf("nb = %d \n",nb) ; 
	      		      }
        if(TsupTest(kbuf[i],kbuf,nb,famille,idebug))
		{
		 supp_ds_famille(kbuf,famille,kbuf[i]->u.t.numero,idebug);

		   if (idebug == 8) 
			{
			 printf("supp_bruit :");
			 printf("  appel de refiliat\n");
			}
	           refiliat (kbuf,famille,nb,idebug) ;
	           kbuf[i] = NIL ;

		   kbruit[i] =0 ;kfus[i] = NIL;
		   kfus[ abs(famille[0]) -1] = kbuf[ abs( famille[0] ) -1];

		   (*nbsuppres)++ ;
	        }
	else kbruit[i] = -kbruit[i];
	}

/* ***********************************************************************  */
/*                                                                          */
/*    BRUITAGE    :    Cette fonction elimine toutes les chaines            */
/*                     de longueur <= "s" y ccompris les jonctions.         */
/*                                                                          */
/*                     Elle rend le nombre "nbsuppres" de chaines           */
/*                     supprimees                                           */
/* ***********************************************************************  */


bruitage (kbuf,nomcha,s,nbsuppres,idebug) 

	struct maillon *kbuf[] ;
	int nomcha ,
	    s ,
	    *nbsuppres ;
	int idebug ;   

{	struct maillon *x;
	int i ;
	int taille ,     /* longueur de la chaine actuelle */
	    nbref ,      /* nombre de fils                 */
	    nbrea ;      /* nombre d'aieuls                */
	int *parent ,    /*  tableau des aieuls            */
	    *enfant ;    /*  tableau des fils 		   */

	for (i=0 ; i<nomcha ; i++) 
	    if (kbruit[i] > 0)
	    {
		if (idebug == 8)
			{ printf("BRUITAGE : chaine =%d;",i+1);
			  printf("    kbuf[%d]=%d\n",i,kbuf[i]);
			}
		if (kbuf[i] != NIL) if ((taille=kbruit[i]) <= s) 
		     {
			enfant = lfils_(kbuf[i],&nbref) ;
			parent = laieul_(kbuf[i],&nbrea) ;
			if (idebug ==8)
				{
				printf("....nbrea=%d ;nbref=%d",nbrea,nbref);
				printf("; taille=%d\n",nbmail_(kbuf[i]));
				}
			if ( (nbref==0) && (nbrea==0) )
			     {
	       		        kbuf[i] = NIL ;
				kbruit[i] =0 ;kfus[i] = NIL;
                                (*nbsuppres)++ ;
			     }
			else
                         if (nbref==0)
                           supp_bruit(kbuf,i,nbrea,parent,nbsuppres,idebug); 
			else 
   		         if (nbrea==0)
			   supp_bruit (kbuf,i,nbref,enfant,nbsuppres,idebug);
			else
			   if (nbrea==1 && nbref==1 && 
				   (abs(enfant[0])==(kbuf[i])->u.t.numero) &&
				   (abs(parent[0])==(kbuf[i])->u.t.numero) )
				{ kbuf[i] = NIL;
				  kbruit[i] =0 ;kfus[i] = NIL;
				  (*nbsuppres)++;
			   	}
		     }
	    }
}


/************************************************************************** */
/*                                                                          */
/*   TASSAGE   :   Cette fonction compacte le tableau "kbuf" de facon       */
/*                 a ce qu'il ne contienne aucun pointeur NIL entre         */
/*                 1 et "nomcha - nbsuppress"                               */
/*                                                                          */
/*                 Elle rend le nombre "nbsupress" de pointeurs NIL         */
/*                 enleves                                                  */
/*                 La numerotation de la filiation de chaque chaine         */
/*                 est mise a jour                                          */
/*                                                                          */
/************************************************************************** */


tassage (kbuf,nomcha,idebug) 

	struct maillon *kbuf[] ;
	int nomcha ;
	int idebug ;   

{
	int *adrkbuf ;
	int i ,j ,numb ;
	struct maillon *x ,
	               *ext1 ,
		       *ext2 ; 

	if (idebug == 8) 
	     {
		printf (" tassage  : entree \n") ;
		printf (" nomcha = %d  \n",nomcha);
	     }

	j = 0 ;
        adrkbuf = (int*) malloc( sizeof(int)*nomcha ) ;
	for (i=0 ; i < nomcha ; i++)
	     {
		if (kbuf[i] != NIL) adrkbuf[i] = ++j ;
	     }

	for (i=0 ; i < nomcha ; i++)	
	     {
		x = (struct maillon *) kbuf[i] ;
		if (x!=NIL) 
		     {
			x->u.t.numero = adrkbuf[i] ;
			ext1 = x->succ ;
			ext2 = (x->succ)->succ ;
			for (j=0; j<4; j++) 
			     {
				numb = ext1->u.e.famille[j] ;
				if (numb == 0) break ; 
				if (numb < 0)
				   ext1->u.e.famille[j]= -(adrkbuf[(-numb)-1]);
				else
				   ext1->u.e.famille[j]= adrkbuf[numb-1];
			     }
			for (j=0; j<4; j++) 
			     {
				numb = ext2->u.e.famille[j] ;
				if (numb == 0) break ; 
				if (numb < 0)
			         ext2->u.e.famille[j]= -(adrkbuf[(-numb)-1]);
				else
				 ext2->u.e.famille[j]= adrkbuf[numb-1];
			     }
			kbuf[ adrkbuf[i] -1 ] = x ;
		     }
	     }	

}

