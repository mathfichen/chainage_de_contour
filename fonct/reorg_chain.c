/*
***************************************************************************
*									  *
*                         AMELIORATION DU CHAINAGE                        *
*									  *
*									  *
*  parametres en entree :						  *
*									  *
*	tab : table contenant les coordonnees et niveaux de gris 	  *
*	      des maillons de la chaine					  *
*									  *
*	totalm : nombre de maillons de la chaine			  *
*									  *
*	jonctete : si jonctete = 0 alors pas de chaines connexes en tete  *
*									  *
*	joncqueue : si joncqueue =0 alors pas de chaines connexes en queue*
*									  *
*									  *
***************************************************************************
*/
#define	 xi    tab[i]
#define	 yi    tab[i+1]
#define	 ngi   tab[i+2]

#define	 xi_1  tab[i+3]
#define	 yi_1  tab[i+4]
#define	 ngi_1 tab[i+5]

#define	 xi_2  tab[i+6]
#define	 yi_2  tab[i+7]
#define	 ngi_2 tab[i+8]

#define	 xi_3  tab[i+9]
#define	 yi_3  tab[i+10]

#include <stdio.h>
#include <dzv.h>

reorg_chain(tab,totalm,jonctete,joncqueue)
int totalm;
int tab[];
short  jonctete ;
short  joncqueue ;
{
int  maillons[3] ;
int    i ;
int    totaltrue ;

	i = 0 ;
	if(totalm >= 3)
	{
	if(jonctete == 0)	/* traitement des 3 premiers maillons */
		{
		if(((xi == xi_1)&&(yi == yi_2))||
		   ((yi == yi_1)&&(xi == xi_2)))
			{
			maillons[0] = xi ;
			maillons[1] = yi ;
			maillons[2] = ngi ;
			xi = xi_1 ;
			yi = yi_1 ;
			ngi = ngi_1 ;
			xi_1 = maillons[0] ;
			yi_1 = maillons[1] ;
			ngi_1 = maillons[2] ;
			i = 6 ;
			}
		}
	totaltrue = (totalm * 3) - 9 ;	
	while(i < totaltrue)
		{
		if((xi < xi_1)&&(yi > yi_1)) 
			{
			if((yi_1 == yi_2)&&(xi_1 > xi_2))
				{
				if((xi_2 < xi_3)&&(yi_2 > yi_3))
					{
					maillons[0] = xi_1 ;
					maillons[1] = yi_1 ;
					maillons[2] = ngi_1 ;
					xi_1 = xi_2 ;
					yi_1 = yi_2 ;
					ngi_1 = ngi_2 ;
					xi_2 = maillons[0] ;
					yi_2 = maillons[1] ;
					ngi_2 = maillons[2] ;
					i += 6 ;
					}
				}
			}
		if((xi > xi_1)&&(yi < yi_1)) 
			{
			if((yi_1 == yi_2)&&(xi_1 < xi_2))
				{
				if((xi_2 > xi_3)&&(yi_2 < yi_3))
					{
					maillons[0] = xi_1 ;
					maillons[1] = yi_1 ;
					maillons[2] = ngi_1 ;
					xi_1 = xi_2 ;
					yi_1 = yi_2 ;
					ngi_1 = ngi_2 ;
					xi_2 = maillons[0] ;
					yi_2 = maillons[1] ;
					ngi_2 = maillons[2] ;
					i += 6 ;
					}
				}
			}
		i += 3 ;
		}
	if((joncqueue == 0)&&(i == totaltrue))	/* traitement des 3 derniers maillons */
		{
		if(((xi_2 == xi_1)&&(yi == yi_2))
		 ||((yi_2 == yi_1)&&(xi == xi_2)))
			{
			maillons[0] = xi_2 ;
			maillons[1] = yi_2 ;
			maillons[2] = ngi_2 ;
			xi_2 = xi_1 ;
			yi_2 = yi_1 ;
			ngi_2 = ngi_1 ;
			xi_1 = maillons[0] ;
			yi_1 = maillons[1] ;
			ngi_1 = maillons[2] ;
			}
		}
	}
}
