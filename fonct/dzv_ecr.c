#include <stdio.h>
#include <dzv.h>

#include "def.h"
#include "com_ext.c"

DZV *dzv_fic_b, *dzv_elt_b, *dzv_mai_b; /* b comme bezier */
int fd_dzv_b;	/* descripteur fichier dzv */
extern int dzv_dimx, dzv_dimy;

extern int traitement_chaine_actif;

#define TM 4	/* taille tableau ident_mere:
     chaine mere,
     nombre de maillon de la cahine mere,
     fermeture chaine mere,
     nombre de bezier ou droite genere a partir de la chaine mere */

#define TG 2	/* taille general_bezier:
     longueur du bezier (pour l'instant, longueur de la courbe origine),
     erreur commise sur l'approximation */

#define TC 2	/* taille controle bezier en coordonnees: x, y */

#define TP 4	/* taille controle bezier en points: p1, p2, p3, p4 */


/************************************************************************
* cree_dzv :                                                            *
*   cree un entete de fichier fic_dzv en format bezier.                 *
************************************************************************/

int cree_dzv(fic_dzv)
     char fic_dzv[];
{
  int nombre_bezier;

  nombre_bezier = 0;	/* inconnu a priori */

  dzv_fic_b = (DZV *) G_set_dzv(3, 20, "char", "commentaire",
				1, "int", "dimx", 1, "int", "dimy");
  dzv_elt_b = (DZV *) G_set_dzv(2, TG, "float", "longueur erreur",
			    TM, "short", "mere nb_mai fermeture nb_bezier");
  dzv_mai_b = (DZV *) G_set_dzv(1, TC, "float", "x y");

  fd_dzv_b = G_create(fic_dzv, "beziers", nombre_bezier,
		      dzv_fic_b, dzv_elt_b, dzv_mai_b);
  S_write_fic(fd_dzv_b, "cree par vchaine", &dzv_dimx, &dzv_dimy);
}

  
/***********************************************************************
* sauve_dzv :                                                          *
***********************************************************************/

void sauve_dzv()
{
  int ind, i, j, nb_bezier, n, n_sel, f_sel, nm_sel, numero_bezier;
  short jt[5], jq[5];
  struct courbe_bezier *liste_bezier;
  short ident_mere[TM];
  float general_bezier[TG];
  float (*controle_bezier)[TC]; /* [TP][TC] */


  traitement_chaine_actif = TRUE;

  numero_bezier = 1;

  n = numero_chaine;
  n_sel = numero_chaine_sel;
  f_sel = ferme_sel;
  nm_sel = nb_mai_sel;

  for (ind=1;ind<=nombre_chaine;ind++) {
    numero_chaine_sel = numero_chaine = ind;
    info_chaine(&nb_mai_sel, &ferme_sel, jt, jq);
    calcul_courbure();
    approxime(&nb_bezier, &liste_bezier);
    ident_mere[0] = (short) numero_chaine_sel;
    ident_mere[1] = (short) nb_mai_sel;
    ident_mere[2] = (short) ferme_sel;
    ident_mere[3] = (short) nb_bezier;
    for (i=0;i<nb_bezier;i++) {
      general_bezier[0] = liste_bezier[i].l;
      general_bezier[1] = liste_bezier[i].e;
      S_write_elt(fd_dzv_b, numero_bezier, TP, general_bezier, ident_mere);
      numero_bezier++;
      controle_bezier = liste_bezier[i].pt;
      for (j=0;j<TP;j++) {
	S_write_mai(fd_dzv_b, controle_bezier[j]);
      }
    }
    memfree(&liste_bezier); /* libere pointeur alloue dans 'approxime()' */
  }

  numero_chaine = n;
  numero_chaine_sel = n_sel;
  ferme_sel = f_sel;
  nb_mai_sel = nm_sel;

  traitement_chaine_actif = FALSE;
  G_close(fd_dzv_b);
}
