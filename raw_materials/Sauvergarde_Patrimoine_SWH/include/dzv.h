typedef struct D_DZV_POINTER
	{ int    n;
	  int	 code;
	  int    *acces;
          int    *format;
          int    *taille_format;
	}DZV_POINTER;


typedef struct D_ZV
	{ int   nb;
	  int   type;
	}ZV;

typedef struct D_CO
	{
          int   nb_commentaire;
          char  **commentaire;
	  int   *taille_commentaire;
	  int   *nb;
          int  	*position;
          int  	*type;
          int  	*taille_type;
	}CO;

typedef struct D_DZV
	{ int   nb;
	  ZV    *tab;
	  short *taille_comment;
	  char  **comment;
          CO    co;
	}DZV;

extern DZV *G_set_dzv(); 
extern DZV_POINTER *G_set_pointer_fic(); 
extern DZV_POINTER *G_set_pointer_elt(); 
extern DZV_POINTER *G_set_pointer_mai(); 

extern int dzv_protection;

#define cr_fic_base	G_create
#define op_fic_base	G_open
#define get_fic		G_read_fic
#define get_elt		G_read_elt
#define get_mai		G_read_mai
#define ra_fic(fp, elt, mai)\
	((mai)==0 ? G_seek_elt(fp, elt) : G_seek_mai(fp, elt, mai))
#define cl_fic		G_close
#define get_max_elt	G_get_max_elt
#define get_min_elt	G_get_min_elt
#define get_nb_elt	G_get_nb_elt
#define get_first_elt	G_get_first_elt
#define get_last_elt	G_get_last_elt
#define cr_dzv		G_set_dzv
#define cp_dzv		G_cmp_dzv
#define del_elt		G_del_elt
#define pr_entete	G_print_fic
#define pr_elt		G_print_elt
#define pr_mai		G_print_mai
#define wr_fic		S_write_fic
#define wr_elt		S_write_elt
#define wr_mai		S_write_mai
#define re_fic		S_read_fic
#define re_elt		S_read_elt
#define re_mai		S_read_mai

#define G_set_fast_fic G_set_pointer_fic
#define G_set_fast_elt G_set_pointer_elt
#define G_set_fast_mai G_set_pointer_mai
#define G_get_fast_fic G_pget_fic
#define G_get_fast_elt G_pget_elt
#define G_get_fast_mai G_pget_mai
#define FAST_ACCES DZV_POINTER
