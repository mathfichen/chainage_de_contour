#include <stdio.h>
#include <include/chaine/maillon.h>
#define NIL 0

extern int *lfils_();
extern int *laieul_();
extern int supp_ds_famille();

extern int *kbruit;
extern struct maillon **kfus;

extern double hypot();      /* utilise dans distance() */

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
		printf (" refiliat : entree;  etablissement des liaisons: \n");
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


/*19/04/1989**************************************************************/
/*                                                                       */
/*   DISTANCE :  Cette fonction calcule la distance euclidienne entre    */
/*               les deux chaines abs(numch1) et abs(numch2).            */
/*               Le signe de numch1 et numch2 servant a indiquer         */
/*               la tete (positif) ou la queue (negatif).                */
/*                                                                       */
/*************************************************************************/

double distance(kbuf, numch1, numch2)
     
     struct maillon *kbuf[];
     int numch1, numch2;
{
  struct maillon *aux1, *aux2;
  int x1, x2, y1, y2;
  double p1, p2, resu;

  aux1 = kbuf[abs(numch1)-1];
  aux2 = kbuf[abs(numch2)-1];
  if (numch1 > 0) {
    x1=aux1->succ->succ->succ->u.m.data[0];
    y1=aux1->succ->succ->succ->u.m.data[1];
   }
  if (numch1 < 0) {
    x1=aux1->u.t.dernier->u.m.data[0];
    y1=aux1->u.t.dernier->u.m.data[1];
  }
  if (numch2 > 0) {
    x2=aux2->succ->succ->succ->u.m.data[0];
    y2=aux2->succ->succ->succ->u.m.data[1];
  }
  if (numch2 < 0) {
    x2=aux2->u.t.dernier->u.m.data[0];
    y2=aux2->u.t.dernier->u.m.data[1];
  }
  return(hypot((double)(x1-x2),(double)(y1-y2)));
}


/*03/05/1989**************************************************************** */
/*                                                                           */
/*    REFIL_CHAINE_UNIT :   Cette fonction retablit la filiation apres la    */
/*                          suppression d'une chaine de longueur 1.          */
/*                                                                           */
/*                   Les liaisons sont retablies entre les chaines dont      */
/*                   les distances entre extremitees sont les plus courtes.  */
/*                   Il n'y a que 2 liaisons, au maximum, retablies.         */
/*                   De plus, avec la nouvelle version de Fusion, la         */
/*                   fonction s'attend a avoir numprogen=3 ou 4.             */
/*                                                                           */
/* ************************************************************************* */

refil_chaine_unit(kbuf,progen,numprogen,idebug)

     struct maillon *kbuf[] ;
     int progen[] ;
     int numprogen ;  
     int idebug ;   
     
{
  int j, i, nombre, ecart, mini, lien1[2], lien2[2];
  double min1, min2, dist;

  if (idebug == 8) {
    printf(" refil_chaine_unit : entree;  etablissement des liaisons:\n");
    for (j=0;j<numprogen;j++) printf("      progen[%d] = %d ",j,progen[j]);
    printf(" \n");
    for (j=0;j<numprogen;j++) {
      for (i=j+1;i<numprogen;i++) {
	printf("  distance entre chaine %d et %d : %5f\n",
	       progen[j],progen[i],distance(kbuf, progen[j], progen[i]));
      }
    }
  }

  min1=min2=1.0e+6;   /* On est sur qu'une distance entre 2 chaines */
                      /* sera plus petite que 1.0e+6 */
  mini=1000000;
  lien1[0]=lien1[1]=lien2[0]=lien2[1]=0;

  if (numprogen==4) { /* 4 chaines presentes */
    /* calcul de la plus petite distance 'min1' entre chaine */
    for (i=0;i<numprogen;i++) {
      for (j=i+1;j<numprogen;j++) {
	if ((dist=distance(kbuf, progen[i], progen[j]))<min1) min1=dist;
      }
    }

    nombre=0;
    /* 'nombre' contient le nombre de distances egales a 'min1' */
    for (i=0;i<numprogen;i++) {
      for (j=i+1;j<numprogen;j++) {
	if (distance(kbuf, progen[i], progen[j])==min1) nombre++;
      }
    }
    min1=1.0e+6;

    if (nombre<4) {	/* les distances entre les 4 chaines sont */
			/* differentes, on applique le critere de longueur */

      /* recherche du premier couple de chaines */
      for (i=0;i<numprogen;i++) {
	for (j=i+1;j<numprogen;j++) {
	  if ((dist=distance(kbuf, progen[i], progen[j]))<min1) {
	    min1=dist;
	    lien1[0]=progen[i];
	    lien1[1]=progen[j];
	  }
	}
      }

      /* recherche du second couple */
      for (i=0;i<numprogen;i++) {
	for (j=i+1;j<numprogen;j++) {
	  if (((progen[i]!=lien1[0]) && (progen[i]!=lien1[1])) &&
	      ((progen[j]!=lien1[0]) && (progen[j]!=lien1[1]))) {
	    /* different du premier couple */
	    lien2[0]=progen[i];
	    lien2[1]=progen[j];
	  }
	}
      }
    }
    else {  /* application du critere sur l'ecart entre numero de chaines */

      /* recherche du premier couple de chaines horizontales */
      for (i=0;i<numprogen;i++) {
	for (j=i+1;j<numprogen;j++) {
	  if ((ecart=abs(abs(progen[i])-abs(progen[j])))<mini) {
	    mini=ecart;
	    lien1[0]=progen[i];
	    lien1[1]=progen[j];
	  }
	}
      }

      /* recherche du second couple */
      for (i=0;i<numprogen;i++) {
	for (j=i+1;j<numprogen;j++) {
	  if (idebug == 8) {
	    printf("refil_chaine_unit:");
	    printf("progen[%d]=%d, lien1[0]=%d; progen[%d]=%d, lien1[1]=%d\n",
		   i,progen[i],lien1[0],j,progen[j],lien1[1]);
	  }
	  if (((progen[i]!=lien1[0]) && (progen[i]!=lien1[1])) &&
	      ((progen[j]!=lien1[0]) && (progen[j]!=lien1[1]))) {
	    /* different du premier couple */
	    lien2[0]=progen[i];
	    lien2[1]=progen[j];
	  }
	}
      }
    }

    /* il faut refilier lien1[] et lien2[]. */
    if (idebug == 8) {
      printf("refil_chaine_unit:");
      printf("  appel de refiliat sur lien1: %d & %d\n",lien1[0],lien1[1]);
    }
    refiliat(kbuf, lien1, 2, idebug);

    if (idebug == 8) {
      printf("refil_chaine_unit:");
      printf("  appel de refiliat sur lien2: %d & %d\n",lien2[0],lien2[1]);
    }
    refiliat(kbuf, lien2, 2, idebug);

  }	/* fin de numprogen==4 */
  else {
    if (numprogen==3) {

      /* recherche des couples de chaines */
      for (i=0;i<numprogen;i++) {
	for (j=i+1;j<numprogen;j++) {
	  dist=distance(kbuf, progen[i], progen[j]);
	  if (dist<min1) {
	    min2=min1;
	    lien2[0]=lien1[0];
	    lien2[1]=lien1[1];
	    min1=dist;
	    lien1[0]=progen[i];
	    lien1[1]=progen[j];
	  }
	  else {
	    if (dist<min2) {
	      min2=dist;
	      lien2[0]=progen[i];
	      lien2[1]=progen[j];
	    }
	  }
	}
      }

      /* il faut refilier lien1[] et lien2[]. */
      if (idebug == 8) {
	printf("refil_chaine_unit:");
	printf("  appel de refiliat\n");
      }
      refiliat(kbuf, lien1, 2, idebug);
      
      if (idebug == 8) {
	printf("refil_chaine_unit:");
	printf("  appel de refiliat\n");
      }
      refiliat(kbuf, lien2, 2, idebug);
    }
    else {
      if (numprogen==2) {
	if (idebug == 8) {
	  printf("refil_chaine_unit:");
	  printf("  appel de refiliat\n");
	}
	refiliat(kbuf, progen, numprogen, idebug);
      }
    }
  }

} /* fin de refil_chaine_unit */


/* ***********************************************************************  */
/*                                                                          */
/*    SUPP_BRUIT    :  Cette fonction elimine la chaine  kbuf[i]            */
/*                     Elle met a jour la filiation des 'nb' chaines de     */
/*		       'famille'					    */
/*									    */
/*		Remarque: Tsuptest a ete enleve car dans tous les	    */
/*			  cas, on elimine les chaines inferieures	    */
/*			  au seuil fixe'.				    */
/*                                                                          */
/* ***********************************************************************  */


supp_bruit (kbuf,i,nb,famille,nbsuppres,idebug) 

     struct maillon *kbuf[] ;
     int i,nb,*nbsuppres,idebug;
     int famille[];
{
  
  int jj,ii,number;
  struct maillon *x;
  if (idebug == 8) {
    printf("supp_bruit:\n");
    printf("  kbuf[%d] = %d   ",i,kbuf[i]);
    printf("nb = %d \n",nb) ; 
  }
  supp_ds_famille(kbuf,famille,kbuf[i]->u.t.numero,idebug);
  
  if (idebug == 8) {
    printf("supp_bruit:");
    printf("  appel de refiliat\n");
  }
  refiliat (kbuf,famille,nb,idebug) ;
  
  if ( idebug == 8) { 
    printf("resultat de refiliat:");
    printf(")\n");
    jj=0;
    while ((famille[jj]!=0)&&(jj<4)) {
      printf("    chaine %5d : enfants=(",famille[jj]);
      x=kbuf[abs(famille[jj])-1];
      for (ii=0;ii<4;ii++) {
	printf("%5d",((x->succ)->succ)->u.e.famille[ii]);
	if (ii<3) printf(", ");
      }
      printf(")\n");
      printf("                     aieux=(");
      for (ii=0;ii<4;ii++) {
	printf("%5d",(x->succ)->u.e.famille[ii]);
	if (ii<3) printf(", ");
      }
      printf(")\n");
      jj++;
    }
  }
  
  kbuf[i] = NIL ;
  
  if (idebug == 8) {
    printf("supp_bruit:");
    printf("  elimination de la chaine %d\n\n",i+1);
  }	
  
  kbruit[i] =0 ;kfus[i] = NIL;
  kfus[ abs(famille[0]) -1] = kbuf[ abs( famille[0] ) -1];
  
  (*nbsuppres)++ ;

}	/* fin de supp_bruit */


/*19/04/1989**************************************************************  */
/*                                                                          */
/*  SUPP_CHAINE_UNIT : Cette fonction elimine la chaine kbuf[i] de taille 1 */
/*		       fortement liee.					    */
/*                     Elle met a jour la filiation des 'nbrea' chaines     */
/*		       de 'parent'.                              	    */
/*									    */
/* ***********************************************************************  */

supp_chaine_unit(kbuf,i,nbrea,parent,nbsuppres,idebug)

     struct maillon *kbuf[];
     int i,nbrea,*nbsuppres,idebug;
     int parent[];
{
  int jj,ii;
  struct maillon *x;
  if (idebug == 8) {
    printf("supp_chaine_unit:\n");
    printf("kbuf[%d] = %d   ",i,kbuf[i]);
    printf("nbrea = %d\n",nbrea);
  }
  
  supp_ds_famille(kbuf,parent,kbuf[i]->u.t.numero,idebug);
  
  
  if (idebug == 8) {
    printf("supp_chaine_unit:\n");
    printf("  appel de refil_chaine_unit\n");
  }
  
  refil_chaine_unit(kbuf,parent,nbrea,idebug);
  
  if ( idebug == 8) { 
    printf("resultat de refil_chaine_unit:\n");
    jj=0;
    while ((parent[jj]!=0)&&(jj<4)) {
      printf("    chaine %5d : enfants=(",parent[jj]);
      x=kbuf[abs(parent[jj])-1];
      for (ii=0;ii<4;ii++) {
	printf("%5d",((x->succ)->succ)->u.e.famille[ii]);
	if (ii<3) printf(", ");
      }
      printf(")\n");
      printf("                     aieux=(");
      for (ii=0;ii<4;ii++) {
	printf("%5d",(x->succ)->u.e.famille[ii]);
	if (ii<3) printf(", ");
      }
      printf(")\n");
      jj++;
    }
  }
  
  if (idebug == 8) {
    printf("supp_chaine_unit:");
    printf("  elimination de la chaine %d\n",i+1);
  }	
  
  kbuf[i] = NIL ;
  
  kbruit[i] = 0; kfus[i] = NIL;
  kfus[ abs(parent[0]) -1 ] = kbuf[ abs(parent[0]) -1];
  
  (*nbsuppres)++;
  
}	/* fin de supp_chaine_unit */


/*xx/02/1989**************************************************************  */
/*                                                                          */
/*    SUPP_LIAISON :   Cette fonction elimine la chaine kbuf[i] fortement   */
/*		       liee.						    */
/*                     Elle met a jour la filiation des 'nbref' chaines     */
/*		       de 'enfant' et les 'nbrea' chaines de 'parent'.	    */
/*									    */
/* ***********************************************************************  */

supp_liaison (kbuf,i,nbref,nbrea,enfant,parent,nbsuppres,idebug)

     struct maillon *kbuf[];
     int i,nbref,nbrea,*nbsuppres,idebug;
     int enfant[],parent[];
{
  int jj,ii;
  struct maillon *x;
  if (idebug == 8) {
    printf("supp_liaison:\n");
    printf("kbuf[%d] = %d   ",i,kbuf[i]);
    printf("nbrea = %d     nbref = %d \n",nbrea,nbref);
  }
  
  supp_ds_famille(kbuf,enfant,kbuf[i]->u.t.numero,idebug);
  
  if (idebug == 8) {
    printf("supp_liaison: ");
    printf("  appel de refiliat\n");
  }
  refiliat(kbuf,enfant,nbref,idebug);

  if ( idebug == 8) { 
    printf("resultat de refiliat:\n");
    jj=0;
    while ((enfant[jj]!=0)&&(jj<4)) {
      printf("    chaine %5d : enfants=(",enfant[jj]);
      x=kbuf[abs(enfant[jj])-1];
      for (ii=0;ii<4;ii++) {
	printf("%5d",((x->succ)->succ)->u.e.famille[ii]);
	if (ii<3) printf(", ");
      }
      printf(")\n");
      printf("                     aieux=(");
      for (ii=0;ii<4;ii++) {
	printf("%5d",(x->succ)->u.e.famille[ii]);
	if (ii<3) printf(", ");
      }
      printf(")\n");
      jj++;
    }
  }
  
  supp_ds_famille(kbuf,parent,kbuf[i]->u.t.numero,idebug);
  
  if (idebug == 8) {
    printf("supp_liaison: ");
    printf("  appel de refiliat\n");
  }
  refiliat(kbuf,parent,nbrea,idebug);
  
  if ( idebug == 8) { 
    printf("resultat de refiliat:\n");
    jj=0;
    while ((enfant[jj]!=0)&&(jj<4)) {
      printf("    chaine %5d : enfants=(",enfant[jj]);
      x=kbuf[abs(enfant[jj])-1];
      for (ii=0;ii<4;ii++) {
	printf("%5d",((x->succ)->succ)->u.e.famille[ii]);
	if (ii<3) printf(", ");
      }
      printf(")\n");
      printf("                     aieux=(");
      for (ii=0;ii<4;ii++) {
	printf("%5d",(x->succ)->u.e.famille[ii]);
	if (ii<3) printf(", ");
      }
      printf(")\n");
      jj++;
    }
  }
  
  if (idebug == 8) {
    printf("supp_liaison:");
    printf("  elimination de la chaine %d\n",i+1);
  }	
  
  kbuf[i] = NIL ;
  
  kbruit[i] = 0; kfus[i] = NIL;
  kfus[ abs(parent[0]) -1 ] = kbuf[ abs(parent[0]) -1];
  kfus[ abs(enfant[0]) -1 ] = kbuf[ abs(enfant[0]) -1];
  
  (*nbsuppres)++;
  
}	/* fin de supp_liaison */


/*19/04/1989**************************************************************  */
/*                                                                          */
/*    BRUITAGE    :    Cette fonction elimine toutes les chaines            */
/*                     de longueur <= "s" y ccompris les jonctions.         */
/*                     fortement liees.                                     */
/*                     Elle rend le nombre "nbsuppres" de chaines           */
/*                     supprimees                                           */
/* ***********************************************************************  */


bruitage (kbuf,nomcha,s,nbsuppres,idebug) 

	struct maillon *kbuf[] ;
	int nomcha ,
	    s ,
	    *nbsuppres ;
	int idebug ;   

{
  struct maillon *x;
  int i ;
  int taille ,     /* longueur de la chaine actuelle */
      nbref ,      /* nombre de fils                 */
      nbrea ;      /* nombre d'aieuls                */
  int *parent ,    /*  tableau des aieuls            */
      *enfant ;    /*  tableau des fils 		   */

  for (i=0 ; i<nomcha ; i++) 
    if (kbruit[i] > 0) {
      if (idebug == 8) {
	printf("BRUITAGE : chaine =%d;",i+1);
	printf("    kbuf[%d]=%d\n",i,kbuf[i]);
      }
      if (kbuf[i] != NIL) if ((taille=kbruit[i]) <= s) {
	enfant = lfils_(kbuf[i],&nbref) ;
	parent = laieul_(kbuf[i],&nbrea) ;
	if (idebug ==8) {
	  printf("....nbrea=%d ;nbref=%d",nbrea,nbref);
	  printf("; taille=%d\n",nbmail_(kbuf[i]));
	}
	if ( (nbref==0) && (nbrea==0) ) {
	  kbuf[i] = NIL ;
	  kbruit[i] =0 ;kfus[i] = NIL;
	  (*nbsuppres)++ ;
	}
	else
	  if (nbmail_(kbuf[i])==1) { /* elimination d'une chaine de taille 1 */
	    supp_chaine_unit(kbuf,i,nbrea,parent,nbsuppres,idebug);
	  }
	  else {
	    if (nbref==0)
	      supp_bruit(kbuf,i,nbrea,parent,nbsuppres,idebug); 
	    else 
	      if (nbrea==0)
		supp_bruit(kbuf,i,nbref,enfant,nbsuppres,idebug);
	      else
		if (nbrea==1 && nbref==1 && 
		    (abs(enfant[0])==(kbuf[i])->u.t.numero) &&
		    (abs(parent[0])==(kbuf[i])->u.t.numero) )
		  { kbuf[i] = NIL;
		    kbruit[i] =0 ;kfus[i] = NIL;
		    (*nbsuppres)++;
		  }
		else supp_liaison(kbuf,i,nbref,nbrea,enfant,
				  parent,nbsuppres,idebug);
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

