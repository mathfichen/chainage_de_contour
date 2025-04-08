/************************************************************************/
/*                                                                      */
/* Date de creation : 15 mai 86.                                        */
/*                                                                      */
/*______________________________________________________________________*/
/*                                                                      */
/*     Ce module permet d'extraire l'ensemble des metachaines d'une     */
/*     image .                                                          */
/*     Une metachaine est un ensemble de chaines connexes.              */
/*                                                                      */
/*______________________________________________________________________*/
/*                                                                      */
/*                                Auteur :  Garnesson Philippe          */
/*                                                                      */
/************************************************************************/

/*------------------------------------------------------------------------*/
/*                                                                        */
/*        Les parametres en common :                                      */
/*                                                                        */
/*        PILE         : la pile des chaines du chemin qu'on est en train */
/*                       de parcourir.                                    */
/*        PT_PILE      : pointeur sur l'element courant de PILE           */
/*        MAX_PILE     : le nombre maximum d'elements dans la PILE        */
/*        MARQUER      : les elements presents dans la pile plus ceux ou  */
/*                       on s'interdit de passer de nouveaux              */
/*        A_TESTER     : les chaines qui n'ont pas servies                */
/*        MAX_VOISIN   : nombre de voisins maximum en un sommet           */
/*        LIST_META    : structure des metachaines allouee dynamiquement  */
/*        TETE_META    : pointeur sur la liste des metachaines            */
/*        NBRE_META    : Nombre de metachaines pour un emsemble de        */
/*                       composantes connexes du graphe.                  */
/*                                                                        */
/*------------------------------------------------------------------------*/
#define NIL 0
#define MAX_VOISIN 10
int *pile,*marquer,*a_tester;
int pt_pile,max_pile;

struct list_meta
        { int nb_elt;
          int *une_meta;
          struct list_meta *suivant;
        };
struct list_meta *tete_meta;
int nbre_meta;

/*-------------------------------------------------------------------------*/
/*                                                                         */
/*        Les fonctions utilisees :                                        */
/*      =========================                                          */
/*                                                                         */
/*            Manipulation des metachaines :                               */
/*            ------------------------------                               */
/*                ajout_tete  : stockage d'une metachaine.                 */
/*                visu_meta   : visualisation sur ecran                    */
/*                chemin      : extraction d'une metachaine de la pile     */
/*            Parcourt du graphe :                                         */
/*            --------------------                                         */
/*                all_voisins : donne les voisins d'un sommet              */
/*                empiler     : memorise un sommet dans la pile            */
/*                depiler     : retourne le sommet de la pille             */
/*            Recherche de metachaines :                                   */
/*            --------------------------                                   */
/*                meta        : dans une partie du graphe                  */
/*                cons_meta   : dans l'ensemble du graphe                  */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/****************************************************************/
/*                                                              */
/*        AJOUT_TETE :  Procedure qui ajoute une metachaine dans*/
/*                      la liste des metachaines                */
/*                                                              */
/****************************************************************/
int ajout_tete(tete_meta,nbij,i)
	struct list_meta **tete_meta;
	int i,nbij;
{
 int k;
 struct list_meta *sauve_tete;

 nbre_meta++;
 sauve_tete = *tete_meta;
 *tete_meta = (struct list_meta *) malloc (sizeof( struct list_meta) ) ;
 (*tete_meta)->nb_elt = nbij;
 (*tete_meta)->une_meta = (int *) malloc( sizeof(int)* nbij);
 for (k=0; k< nbij ;k++)
		{ (*tete_meta)->une_meta[k] = pile[ i+k ];
		}
 (*tete_meta)->suivant= sauve_tete;
}
/****************************************************************/
/*                                                              */
/*  VISU_META : procedure qui parcourt la liste des metachaines */
/*              et les visualise.                               */
/*                                                              */
/****************************************************************/
int visu_meta(tete_meta)
        struct list_meta *tete_meta; 
{ int k;

  if (tete_meta != NIL)
        { for (k=0; k< tete_meta->nb_elt; k++)
                printf("%d ",tete_meta->une_meta[k]);
          printf("\n");
          visu_meta(tete_meta->suivant);
        }
}

/************************************************************************/
/*                                                                      */
/*      CHEMIN : procedure qui construit a partir de la pile de nouveaux*/
/*               chemins . Il suffit de relire la pile du sommet        */
/*               a l'element <CH>.                                      */
/*               On verifie que le chemin n'existait pas deja           */
/*                                                                      */
/************************************************************************/
int chemin(ch)
	int ch;
{
 int i_pile,nbij,en_trop;
 int *elts;

 en_trop = 0;
 i_pile  = marquer[abs(ch)];
 if ( pile[ i_pile ] != ch )  { if (!a_tester[abs(ch)]) return;

				/* le sommet qui a permis de detecter la */
				/* metachaine n'est pas dans la pile. On */
				/* l'y met.				 */
				empiler( ch, &pt_pile);
				en_trop =1;
			      }
 nbij = pt_pile - i_pile + 1;
 ajout_tete(&tete_meta, nbij, i_pile);
 if (en_trop) ch = depiler(&pt_pile);
}
/************************************************************************/
/*                                                                      */
/*     ALL_VOISINS : procedure qui permet de determiner tous les        */
/*                   voisins d'un sommet du graphe. Tient compte        */
/*                   des liens par transitivite.                        */
/*                                                                      */
/************************************************************************/
all_voisins(ch,voisin,nb_voisin,kbuf)
        int ch;                 /* sommet initial                      */
        int *voisin;            /* table des sommets voisins           */
        int *nb_voisin;         /* Nombre d'elements dans <voisin>     */
        struct maillon *kbuf[];
{
 int *local;
 int nb_local;
 int i,j,k,snb_voisin;
 extern int *laieul_();
 extern int *lfils_();


  if (ch > 0) local = laieul_( kbuf[abs(ch)-1], &nb_local);
       else   local = lfils_ ( kbuf[abs(ch)-1], &nb_local);
  k = *nb_voisin;
  for ( i=0; i < nb_local; i++)
        {
        for (j=0; j < k ; j++)
		if (voisin[j] == local[i] || 
			((voisin[j] == -local[i]) && (j != 0)) ) break;
        if  (j == k)
                {
                if (k > MAX_VOISIN ) 
                        { printf(" Le nombre de liens par transitivite ");
                          printf(" a ete sous-estime ...\n");
                          printf(" modifie MAX_VOISIN =%d\n",MAX_VOISIN);
                          exit(-1);
                        }
                voisin[k] = local[i];
                k +=1;
                }
        }
  if (k != *nb_voisin) 
        {
        snb_voisin = *nb_voisin;
        *nb_voisin = k;
        for (i=snb_voisin; i<k; i++)
                all_voisins( voisin[i], voisin, nb_voisin  , kbuf);
        }        

}
/************************************************************************/
/*                                                                      */
/*        EMPILER : procedure qui permet d'empiler le parametre dans    */
/*                  la pile <PILE> a la hauteur <pt_pile>               */
/*                                                                      */
/************************************************************************/
int empiler(ch,pt_pile)
        int ch, *pt_pile;
{ if ( (*pt_pile < 0) || ( ((*pt_pile) + 1 ) > max_pile) )
        { printf(" erreur dans <EMPILER> =%d\n",*pt_pile);
          exit( -1);
        }
  (*pt_pile)++;
  pile[ *pt_pile ] = ch;
}
/************************************************************************/
/*                                                                      */
/*        DEPILER : fonction qui retourne le sommet de                  */
/*                  la pile <PILE> a la hauteur <pt_pile> et decremente */
/*                  <pt_pile>                                           */
/*                                                                      */
/************************************************************************/
int depiler(pt_pile)
        int *pt_pile;
{ if (*pt_pile == 0)
        { printf(" erreur dans <DEPILER> =%d\n",*pt_pile);
          exit( -1);
        }
  *pt_pile += -1;
  return( pile[ (*pt_pile) +1 ] );

}

/************************************************************************/
/*                                                                      */
/*   META :     Procedure qui a partir de la chaine donnee <CH>         */
/*              parcourt toutes les chaines qui y sont liees afin de    */
/*              trouver les metachaines .                               */
/*              Les chaines par lesquelles ont est deja passe sont      */
/*              stockees dans la <PILE>. Le tableau <MARQUER> permet de */
/*              savoir quelles sont les chaines presentes dans la <PILE>*/
/*              Le graphe est parcouru en profondeur d'abord            */
/*                                                                      */
/************************************************************************/
int meta(ch,kbuf)
	int ch;
        struct maillon *kbuf[];
{
 int ich, nb_voisin;
 int voisin[ MAX_VOISIN ], test_voisin[ MAX_VOISIN ];
 int cha;

 empiler( ch, &pt_pile);
 marquer [ abs(ch) ] = pt_pile;
 a_tester[ abs(ch) ] = 0;
 ch = -ch;

 voisin[0] =ch;
 nb_voisin =1;
 all_voisins( ch, voisin, &nb_voisin, kbuf);
 nb_voisin += -1;
 voisin[0] = voisin[ nb_voisin ];

 for( ich=0; ich < nb_voisin; ich++)
	{
	cha = voisin[ ich ];
	if (marquer[ abs(cha) ] )
		{ chemin( cha );
		  test_voisin[ ich] =0;
		}
	else    { test_voisin[ ich] =1;
		  marquer[ abs( cha ) ] = pt_pile + 1;
	        }
	}
 for( ich=0; ich < nb_voisin; ich++)
	{
	cha = voisin[ ich ];
	if ( test_voisin[ich] )	meta( cha, kbuf );
	 /*...else..............il est inutile de reprendre ................*/
	 		/*......ce chemin car il n'appartient pas a ........*/
	 		/*......une metachaine et on l'a deja visite .......*/
	 }
 for( ich=0; ich < nb_voisin; ich++)
	if (test_voisin[ich]) marquer[ abs(voisin[ich]) ] = 0;

 cha = depiler( &pt_pile );
 }

/************************************************************************/
/*                                                                      */
/*  CONS_META : Procedure qui pour l'ensemble des chaines de l'image    */
/*              construit l'ensemble des metachaines.                   */
/*              Le tableau a_tester permet de savoir quelles sont les   */
/*              chaines qui restent a tester                            */
/*                                                                      */
/************************************************************************/
int cons_meta( kbuf, nomcha)
        int nomcha;
        struct maillon *kbuf[];

{
 int i;

 pile      = (int *) malloc( (sizeof( int ) * nomcha) +1);
 marquer   = (int *) malloc( (sizeof( int ) * nomcha) +1 );
 a_tester  = (int *) malloc( (sizeof( int ) * nomcha) +1 );
 tete_meta = (struct list_meta *) NIL;

 max_pile = nomcha;
 pt_pile  = 0;

 for ( i=1; i<= nomcha; i++)
        {
        marquer[i]  =0;
        a_tester[i] =1;
        }
 for ( i=1; i<= nomcha; i++)
        if ( a_tester[i] )
                { nbre_meta =0;
                  meta(i,kbuf);
                }
 printf("\nvisualisation des metachaines\n");
 printf("=============================\n");
 visu_meta(tete_meta);
}


