#include <stdio.h>
#include <dzv.h>

#include "def.h"
#include "com_ext.c"

int fd_dzv;	/* descripteur fichier dzv */
DZV *dzv_fic, *dzv_elt, *dzv_mai;
int dzv_dimx, dzv_dimy;


/************************************************************************
* ouvre_dzv :                                                           *
*   Rend 0 si le fichier fic_dzv n'existe pas ou n'a pas le bon format. *
*   Rend 1 sinon et met a jour les dimensions dzv_dimx et dzv_dimy      *
************************************************************************/

int ouvre_dzv(fic_dzv)
     char fic_dzv[];
{
  char *type;
  int ta_jo, ta_mo, nb_tripl, *taille;

  fd_dzv = G_open(fic_dzv, &type, &nombre_chaine,
		  &dzv_fic, &dzv_elt, &dzv_mai);
  if (!fd_dzv) {
    fprintf(stderr, "%s: le fichier %s n'existe pas\n", PROG, fic_dzv);
    return(0);
  }
  S_read_fic(fd_dzv, NULL, &dzv_dimx, &dzv_dimy);

  if (strcmp("chaine_grad", type)) {
    fprintf(stderr, "%s: le fichier %s n'est pas du type chaine_grad,\n",
	    PROG, fic_dzv);
    fprintf(stderr, "mais du type %s\n", type);
    return(0);
  }

  /* Definition du fichier sur disque auquel on s'attend */

  G_get_dzv(dzv_elt, &nb_tripl, &taille, NULL, NULL);
  ta_jo = taille[1];    /* taille du champ jonction queue */
  ta_mo = taille[4];    /* taille du champ moyenne variance */
  if (nb_tripl != 4 && ta_jo != 5 && ta_mo != 2) {
    fprintf(stderr, "%s: la definition de dzv_elt du fichier", PROG);
    fprintf(stderr, " %s n'est pas celle attendue\n", fic_dzv);
    G_close(fd_dzv);
    return(0);
  }

  G_get_dzv(dzv_mai, &nb_tripl, &taille, NULL, NULL);
  if (nb_tripl != 2 && taille[1] != 2 && taille[2] != 3) {
    fprintf(stderr, "%s: la definition de dzv_mai du fichier", PROG);
    fprintf(stderr, " %s n'est pas celle attendue\n", fic_dzv);
    G_close(fd_dzv);
    return(0);
  }

  return(1);	/* ouverture ok */
}


/************************************************************************
* charge_dzv :                                                          *
*    lit les chaines du fichier dzv pointe par fd_dzv et les stocke     *
*    en memoire.                                                        *
************************************************************************/

int charge_dzv(nb_total)
     int nb_total;
{
  int indice, cpt, num, nb_mai;
  float *tmp_x, *tmp_y, *tmp_gx, *tmp_gy, *tmp_maxs, grad[3];
  short jonct[5], joncq[5], coord[2], ferme_dzv;

  if (chaine!=NULL) {
    for (cpt=0;cpt<nombre_chaine;cpt++) {
      memfree(&(chaine[cpt].x));
      memfree(&(chaine[cpt].y));
      memfree(&(chaine[cpt].gx));
      memfree(&(chaine[cpt].gy));
    }
  }
  memfree(&chaine);
  chaine = (struct s_chaines *)   /* affectation de la VG chaine */
	   memalloc(nb_total * sizeof(struct s_chaines));
  indice = 0;

  G_seek_elt(fd_dzv, 1);
  while (S_read_elt(fd_dzv, &num, &nb_mai,
		    &jonct[0], &joncq[0], &ferme_dzv, NULL)) {
    tmp_x = (float *) memalloc(nb_mai * sizeof(float));
    tmp_y = (float *) memalloc(nb_mai * sizeof(float));
    tmp_gx = (float *) memalloc(nb_mai * sizeof(float));
    tmp_gy = (float *) memalloc(nb_mai * sizeof(float));
    for (cpt=0;cpt<nb_mai;cpt++) {
      if (S_read_mai(fd_dzv, coord, grad)) {
	tmp_x[cpt] = (float) coord[0];
	tmp_y[cpt] = (float) coord[1];
	tmp_gx[cpt] = grad[1]; /* car grad[0] = niveau de gris */
	tmp_gy[cpt] = grad[2];
      }
      else {
	fprintf(stderr, "%s: erreur sur nombre de maillons de la chaine %d\n",
		PROG, num);
      }
    }
    chaine[indice].num = num;
    chaine[indice].ferm = (int) ferme_dzv;
    chaine[indice].nbm = nb_mai;
    chaine[indice].x = tmp_x;
    chaine[indice].y = tmp_y;
    chaine[indice].gx = tmp_gx;
    chaine[indice].gy = tmp_gy;
    for (cpt=0;cpt<5;cpt++) {
      chaine[indice].jt[cpt] = jonct[cpt];
      chaine[indice].jq[cpt] = joncq[cpt];
    }
    indice++;
  }
  if (indice!=nb_total) {
    fprintf(stderr, "%s: erreur coherence Chaine / DZV !! ", PROG);
    fprintf(stderr, "(%d,%d)\n", indice, nb_total);
  }
  G_close(fd_dzv);
}
