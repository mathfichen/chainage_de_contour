/********************************************************/
/*							*/
/*    Parametres en entree :				*/
/*							*/
/*	pi 	: ligne du pixel courant		*/
/*	pj	: colonne du pixel courant		*/
/*	pk	: niveau  du pixel courant		*/
/*	pl1	: 1 ere ligne logique de itab		*/
/*	pl2	: 2 eme ligne logique de itab		*/
/*	pmcolo	: nbre de colonne d'une image		*/
/*	pmco2	: 2 * pmcolo				*/
/*	imag	: Buffer de l'image			*/
/*	plap  	: indice courant dans imag		*/
/*	idebug  : Option debug				*/
/*	maxch	: nbre maxi de chaines possible		*/
/*							*/
/*    Parametres en entree/sortie			*/
/*							*/
/*	nomcha 	: nbre de chaines creees		*/
/*	kbuf[0..maxch]	:				*/
/*		  contient les adresses des chaines     */
/*		  indexees par leurs numeros		*/
/*	itab[1..2,1..pmcolo,1..4]  :			*/
/*		  permet de coonnaitre le passe du 	*/
/*		  pixel courant				*/
/*		  - le champ 1 :  pl1 et pl2		*/
/*		  - le champ 2 :  colonne dans l'image  */
/*		  - le champ 3 :  2 => type du pixel    */
/*		   		  3 => adresse du pixel */
/*				  1,4 => inutilises	*/
/*							*/
/********************************************************/
#include <include/chaine/common_ext.h>
#include <include/chaine/maillon.h>
#define NIL  0
/* #define val_max 150    Modif de gg le 24/11/89 : ffff = 150 */
#define val_max 151     /* valeur max de la configuration passe */

struct maillon *opench();
  int (*(gtotab[val_max]))();

float ptest5();
float ptest5668();

/****************************************************************************/
P_XXXX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XXXX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Mettre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   printf("     Mettre une option <k> superieure\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
}

/****************************************************************************/
P_XXXO( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XXXO\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l2 ][ 1 ] == 'P') { itab[ j-1][l2 ][ 1 ] = 'D';
/*                                   sipremier = 1; */
/* Correction du 20/02/89 : Il faut tester le futur dans toutes
les configurations */
                                 }
                         else    itab[ j-1][l2 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
/*if (!(sipremier))*/
/* Correction du 20/02/89 : Il faut tester le futur dans toutes
les configurations */

if ( ptest5668() )
{
	itab[ j][l2 ][ 1 ] = 'F';
}
}

/****************************************************************************/
P_XXXF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XXXF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Metttre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j-1][l2 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]))
        aieul_( kbuf[itab[ j-1][l2 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j-1][l2 ][ 2 ]))
        fils_ (  kbuf[ itab[ j-1][l2 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}

/****************************************************************************/
P_XXOX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XXOX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j+1][l1 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j+1][l1 ][ 1 ] == 'P') { itab[ j+1][l1 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j+1][l1 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if ( ptest5668() )
{
itab[ j][l2 ][ 1 ] = 'F';
}
}

/****************************************************************************/
P_XXOO( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XXOO\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l2 ][ 1 ] == 'P') { itab[ j-1][l2 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l2 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j+1][l1 ][ 1 ] == 'P')      itab[ j+1][l1 ][ 1 ] = 'D';
else itab[ j+1][l1 ][ 1 ] = 'F';
itab[ j][l2 ][ 1 ] = 'F';
if (!ptest5())
{
if (itab[ j+1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j+1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j+1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j+1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j+1][l1 ][ 2 ]);
    }
}
}

/****************************************************************************/
P_XXOF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XXOF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j+1][l1 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j+1][l1 ][ 1 ] == 'P') { itab[ j+1][l1 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j+1][l1 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
itab[ j][l2 ][ 1 ] = 'F';
if (itab[ j-1][l2 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l2 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l2 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l2 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]);
    }
}

/****************************************************************************/
P_XXFX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XXFX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Metttre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (!ptest5())
{
if (itab[ j+1][l1 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j+1][l1 ][ 2 ]))
        aieul_( kbuf[itab[ j+1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j+1][l1 ][ 2 ]))
        fils_ (  kbuf[ itab[ j+1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}
}

/****************************************************************************/
P_XXFO( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XXFO\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l2 ][ 1 ] == 'P') { itab[ j-1][l2 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l2 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
itab[ j][l2 ][ 1 ] = 'F';
if (!ptest5())
{
if (itab[ j+1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j+1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j+1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j+1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j+1][l1 ][ 2 ]);
    }
}
}

/****************************************************************************/
P_XXFF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XXFF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Metttre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (!ptest5())
{
if (itab[ j+1][l1 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j+1][l1 ][ 2 ]))
        aieul_( kbuf[itab[ j+1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j+1][l1 ][ 2 ]))
        fils_ (  kbuf[ itab[ j+1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}
if (itab[ j-1][l2 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]))
        aieul_( kbuf[itab[ j-1][l2 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j-1][l2 ][ 2 ]))
        fils_ (  kbuf[ itab[ j-1][l2 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}

/****************************************************************************/
P_XOXX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XOXX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j][l1   ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j][l1   ][ 1 ] == 'P') { itab[ j][l1   ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j][l1   ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if ( ptest5668() )
{
itab[ j][l2 ][ 1 ] = 'F';
}
}

/****************************************************************************/
P_XFXX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XFXX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Metttre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j][l1   ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j][l1   ][ 2 ]))
        aieul_( kbuf[itab[ j][l1   ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j][l1   ][ 2 ]))
        fils_ (  kbuf[ itab[ j][l1   ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}

/****************************************************************************/
P_XFXO( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XFXO\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l2 ][ 1 ] == 'P') { itab[ j-1][l2 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l2 ][ 1 ] = 'X';
itab[ j][l2 ][ 1 ] = 'F';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j][l1   ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j][l1   ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j][l1   ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j][l1   ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j][l1   ][ 2 ]);
    }
}

/****************************************************************************/
P_XFXF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XFXF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2][ 2 ];
itab[ j][l2 ][ 1 ] = 'F';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j][l1   ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j][l1   ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j][l1   ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j][l1   ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j][l1   ][ 2 ]);
    }
if (itab[ j-1][l2 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l2 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l2 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l2 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]);
    }
}

/****************************************************************************/
P_XFOX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XFOX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j+1][l1 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j+1][l1 ][ 1 ] == 'P') { itab[ j+1][l1 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j+1][l1 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if ( ptest5668() )
{
itab[ j][l2 ][ 1 ] = 'F';
if (itab[ j][l1   ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j][l1   ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j][l1   ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j][l1   ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j][l1   ][ 2 ]);
    }
}
}

/****************************************************************************/
P_XFOO( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XFOO\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l2 ][ 1 ] == 'P') { itab[ j-1][l2 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l2 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j+1][l1 ][ 1 ] == 'P')      itab[ j+1][l1 ][ 1 ] = 'D';
else itab[ j+1][l1 ][ 1 ] = 'F';
itab[ j][l2 ][ 1 ] = 'F';
if (itab[ j][l1   ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j][l1   ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j][l1   ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j][l1   ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j][l1   ][ 2 ]);
    }
}

/****************************************************************************/
P_XFOF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XFOF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
if (itab[ j+1][l1 ][ 1 ] == 'P')      itab[ j+1][l1 ][ 1 ] = 'D';
else itab[ j+1][l1 ][ 1 ] = 'F';
itab[ j][l2 ][ 2 ] = itab[ j-1][l2][ 2 ];
itab[ j][l2 ][ 1 ] = 'F';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j][l1   ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j][l1   ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j][l1   ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j][l1   ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j][l1   ][ 2 ]);
    }
if (itab[ j-1][l2 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l2 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l2 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l2 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]);
    }
}

/****************************************************************************/
P_XFFO( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XFFO\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l2 ][ 1 ] == 'P') { itab[ j-1][l2 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l2 ][ 1 ] = 'X';
itab[ j][l2 ][ 1 ] = 'F';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j][l1   ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j][l1   ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j][l1   ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j][l1   ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j][l1   ][ 2 ]);
    }
}

/****************************************************************************/
P_XFFF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_XFFF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Metttre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (!ptest5())
{
if (itab[ j][l1 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j][l1 ][ 2 ]))
        aieul_( kbuf[itab[ j][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j][l1 ][ 2 ]))
        fils_ (  kbuf[ itab[ j][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
/* Modification du 7/04/89 a la palce de if(itab[j+1.. mis itab[j], ce qui
permet dans cette configuration XFFF, de creer une nouvelle chaine avec les bons 
liens */
}
if (itab[ j-1][l2 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]))
        aieul_( kbuf[itab[ j-1][l2 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j-1][l2 ][ 2 ]))
        fils_ (  kbuf[ itab[ j-1][l2 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}

/****************************************************************************/
P_OXXX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_OXXX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l1 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l1 ][ 1 ] == 'P') { itab[ j-1][l1 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l1 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if ( ptest5668() )
{
itab[ j][l2 ][ 1 ] = 'F';
}
}

/****************************************************************************/
P_OXOX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_OXOX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l1 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l1 ][ 1 ] == 'P') { itab[ j-1][l1 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l1 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j+1][l1 ][ 1 ] == 'P')      itab[ j+1][l1 ][ 1 ] = 'D';
else itab[ j+1][l1 ][ 1 ] = 'F';
itab[ j][l2 ][ 1 ] = 'F';
if (itab[j-1][l1][2] != itab[j+1][l1][2])
if (!ptest5())
{
if (itab[ j+1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j+1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j+1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j+1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j+1][l1 ][ 2 ]);
    }
}
}

/****************************************************************************/
P_OXFX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_OXFX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l1 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l1 ][ 1 ] == 'P') { itab[ j-1][l1 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l1 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
itab[ j][l2 ][ 1 ] = 'F';
if (itab[j-1][l1][2] != itab[j+1][l1][2])
if (!ptest5())
{
if (itab[ j+1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j+1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j+1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j+1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j+1][l1 ][ 2 ]);
    }
}
}

/****************************************************************************/
P_FXXX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FXXX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Metttre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j-1][l1 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j-1][l1 ][ 2 ]))
        aieul_( kbuf[itab[ j-1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j-1][l1 ][ 2 ]))
        fils_ (  kbuf[ itab[ j-1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}

/****************************************************************************/
P_FXXO( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FXXO\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l2 ][ 1 ] == 'P') { itab[ j-1][l2 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l2 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (!(sipremier))
if ( ptest5668() )
{
itab[ j][l2 ][ 1 ] = 'F';
if (itab[ j-1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l1 ][ 2 ]);
    }
}
}

/****************************************************************************/
P_FXXF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FXXF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Metttre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j-1][l2 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]))
        aieul_( kbuf[itab[ j-1][l2 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j-1][l2 ][ 2 ]))
        fils_ (  kbuf[ itab[ j-1][l2 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}

/****************************************************************************/
P_FXOX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FXOX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j+1][l1 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j+1][l1 ][ 1 ] == 'P') { itab[ j+1][l1 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j+1][l1 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
itab[ j][l2 ][ 1 ] = 'F';
if (itab[ j-1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l1 ][ 2 ]);
    }
}

/****************************************************************************/
P_FXOO( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FXOO\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l2 ][ 1 ] == 'P') { itab[ j-1][l2 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l2 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j+1][l1 ][ 1 ] == 'P')      itab[ j+1][l1 ][ 1 ] = 'D';
else itab[ j+1][l1 ][ 1 ] = 'F';
itab[ j][l2 ][ 1 ] = 'F';
if (itab[ j-1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l1 ][ 2 ]);
    }
if (itab[j-1][l1][2] != itab[j+1][l1][2])
if (!ptest5())
{
if (itab[ j+1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j+1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j+1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j+1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j+1][l1 ][ 2 ]);
    }
}
}

/****************************************************************************/
P_FXOF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FXOF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j+1][l1 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j+1][l1 ][ 1 ] == 'P') { itab[ j+1][l1 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j+1][l1 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
itab[ j][l2 ][ 1 ] = 'F';
if (itab[ j-1][l2 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l2 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l2 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l2 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]);
    }
}

/****************************************************************************/
P_FXFX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FXFX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Metttre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j-1][l1 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j-1][l1 ][ 2 ]))
        aieul_( kbuf[itab[ j-1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j-1][l1 ][ 2 ]))
        fils_ (  kbuf[ itab[ j-1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
if (itab[j-1][l1][2] != itab[j+1][l1][2])
if (!ptest5())
{
if (itab[ j+1][l1 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j+1][l1 ][ 2 ]))
        aieul_( kbuf[itab[ j+1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j+1][l1 ][ 2 ]))
        fils_ (  kbuf[ itab[ j+1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}
}

/****************************************************************************/
P_FXFO( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FXFO\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j-1][l2 ][ 1 ] == 'P') { itab[ j-1][l2 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j-1][l2 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
itab[ j][l2 ][ 1 ] = 'F';
if (itab[ j-1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l1 ][ 2 ]);
    }
if (itab[j-1][l1][2] != itab[j+1][l1][2])
if (!ptest5())
{
if (itab[ j+1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j+1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j+1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j+1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j+1][l1 ][ 2 ]);
    }
}
}

/****************************************************************************/
P_FXFF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FXFF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Metttre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[j-1][l1][2] != itab[j+1][l1][2])
if (!ptest5())
{
if (itab[ j+1][l1 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j+1][l1 ][ 2 ]))
        aieul_( kbuf[itab[ j+1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j+1][l1 ][ 2 ]))
        fils_ (  kbuf[ itab[ j+1][l1 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}
if (itab[ j-1][l2 ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]))
        aieul_( kbuf[itab[ j-1][l2 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j-1][l2 ][ 2 ]))
        fils_ (  kbuf[ itab[ j-1][l2 ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}

/****************************************************************************/
P_FOXX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FOXX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j][l1   ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j][l1   ][ 1 ] == 'P') { itab[ j][l1   ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j][l1   ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if ( ptest5668() )
{
itab[ j][l2 ][ 1 ] = 'F';
if (itab[ j-1][l1 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l1 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l1 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l1 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l1 ][ 2 ]);
    }
}
}

/****************************************************************************/
P_FFXF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FFXF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2][ 2 ];
itab[ j][l2 ][ 1 ] = 'F';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j-1][l2 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l2 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l2 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l2 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]);
    }
}

/****************************************************************************/
P_FFOX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FFOX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j+1][l1 ][ 2 ];
itab[ j][l2 ][ 1 ] = 'O' ;
if (itab[ j+1][l1 ][ 1 ] == 'P') { itab[ j+1][l1 ][ 1 ] = 'D';
                                   sipremier = 1;
                                 }
                         else    itab[ j+1][l1 ][ 1 ] = 'X';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
itab[ j][l2 ][ 1 ] = 'F';
if (itab[ j][l1   ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j][l1   ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j][l1   ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j][l1   ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j][l1   ][ 2 ]);
    }
}

/****************************************************************************/
P_FFOF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FFOF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
if (itab[ j+1][l1 ][ 1 ] == 'P')      itab[ j+1][l1 ][ 1 ] = 'D';
else itab[ j+1][l1 ][ 1 ] = 'F';
itab[ j][l2 ][ 2 ] = itab[ j-1][l2][ 2 ];
itab[ j][l2 ][ 1 ] = 'F';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j-1][l2 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l2 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l2 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l2 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]);
    }
}

/****************************************************************************/
P_FFFX( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FFFX\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
nomcha += 1;
if (nomcha >= maxch)
  {printf("Le nbre maxi de chaines a ete atteint:\n");
   printf("     Metttre une option <k> superieure\n");
   exit(-1);
  }
tete = (struct maillon *) opench_();
if (tete == NIL)
  {printf("Plus de place pour creer des chaines.\n");
   exit(-2);
  }
kbuf[ nomcha - 1] = tete ;
itab[ j][l2 ][ 2 ] = nomcha;
itab[ j][l2 ][ 1 ] = 'P'   ;
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j][l1   ][ 1 ] == 'D')
   {if (aieul_( kbuf[  itab[ j][l2][2 ] -1],itab[ j][l1   ][ 2 ]))
        aieul_( kbuf[itab[ j][l1   ][ 2 ] -1],itab[ j][l2][2 ]);
   }
else
   {if (aieul_( kbuf[ itab[ j][l2][2 ] -1],-itab[ j][l1   ][ 2 ]))
        fils_ (  kbuf[ itab[ j][l1   ][ 2 ] -1],itab[ j][l2][2 ]);
   }
}

/****************************************************************************/
P_FFFF( i,j,k,itab)
int i,j,k,itab[][2][4];
{
struct maillon *tete;

int sipremier=0;
if (idebug==2)    printf("Configuration P_FFFF\n");
if (idebug==2)  { printf(" ligne  :%d\n",i);
                  printf(" colonne:%d\n",j+1);
                  printf(" niveau :%d\n",k);
                  printf(" nomcha :%d\n",nomcha);
                }
itab[ j][l2 ][ 2 ] = itab[ j-1][l2][ 2 ];
itab[ j][l2 ][ 1 ] = 'F';
nwmail_( kbuf[ itab[ j][l2][2 ] -1 ],  i, j+1, k );
if (itab[ j-1][l2 ][ 1 ] == 'F')
    {if (fils_( kbuf [itab[ j-1][l2 ][ 2 ] -1] ,-itab[ j][l2][2 ]))
         fils_( kbuf [itab[ j][l2][2 ] -1] ,-itab[ j-1][l2 ][ 2 ]);
    }
else
    {if (aieul_( kbuf [ itab[ j-1][l2 ][ 2 ]-1],-itab[ j][l2][2 ]))
         fils_ ( kbuf [itab[ j][l2][2 ] -1],itab[ j-1][l2 ][ 2 ]);
    }
}




/****************************************************/
/*   Init_gtotab : Cette procedure initialise la    */
/*   		   table qui permet de  se brancher */
/*   		   directement a la fonction        */
/*   		   qui traite une configuration     */
/****************************************************/
initgtot_()
  {
  gtotab[  0] = P_XXXX;
  gtotab[  2] = P_XXXO;
  gtotab[ 20] = P_XXXF;
  gtotab[  4] = P_XXOX;
  gtotab[  6] = P_XXOO;
  gtotab[ 24] = P_XXOF;
  gtotab[ 40] = P_XXFX;
  gtotab[ 42] = P_XXFO;
  gtotab[ 60] = P_XXFF;
  gtotab[  8] = P_XOXX;
  gtotab[ 80] = P_XFXX;
  gtotab[ 82] = P_XFXO;
  gtotab[100] = P_XFXF;
  gtotab[ 84] = P_XFOX;
  gtotab[ 86] = P_XFOO;
  gtotab[104] = P_XFOF;
  gtotab[120] = P_XFXX;
  gtotab[122] = P_XFFO;
  gtotab[140] = P_XFFF;
  gtotab[  1] = P_OXXX;
  gtotab[  5] = P_OXOX;
  gtotab[ 41] = P_OXFX;
  gtotab[ 10] = P_FXXX;
  gtotab[ 12] = P_FXXO;
  gtotab[ 30] = P_FXXF;
  gtotab[ 14] = P_FXOX;
  gtotab[ 16] = P_FXOO;
  gtotab[ 34] = P_FXOF;
  gtotab[ 50] = P_FXFX;
  gtotab[ 52] = P_FXFO;
  gtotab[ 70] = P_FXFF;
  gtotab[ 18] = P_FOXX;
  gtotab[ 90] = P_XFXX;
  gtotab[ 92] = P_XFXO;
  gtotab[110] = P_FFXF;
  gtotab[ 94] = P_FFOX;
  gtotab[ 96] = P_XFOO;
  gtotab[114] = P_FFOF;
  gtotab[130] = P_FFFX;
  gtotab[132] = P_XFFO;
  gtotab[150] = P_FFFF;
  }

/****************************************************/
/*   Etant donne la valeur V qui correspond         */
/*   a la configuration du passe                    */
/*   la procedure selectio_ permet de se            */
/*   se brancher directement a la fonction          */
/*   qui traite ce cas                              */
/****************************************************/
int selectio_ (V,i,j,k,itab)
   int V,i,j,k,itab[][2][4];
{
   if (idebug == 2) 
      {
      printf("selectio : V = %d\n",V);
      }
  (*(gtotab[V]))(i,j,k,itab);
}




