	
#include <stdio.h>
#include "../include/maillon.h"

#define NIL     0
#define CONTOUR 2

extern int *lfils_();
extern int *laieul_();

int elime_(kbuf,&nomcha,seuil,idebug) 
struct maillon *kbuf[];
int  nomcha , idebug ;
float seuil ;
{
	int i , nb , famille[4];

	for(i=0 ; i < nomcha ; i++)
	{
		if (kbuf[i] != NIL)
		{
			res = moyenne(kbuf[i],seuil) ;
			if (res)
			{
				famille = lfils(kbuf[i],&nb) ;
				supp_ds_famille(kbuf[i],famille);
				refiliat(kbuf[i],famille) ;
				famille = laieul(kbuf[i],&nb) ;
				supp_ds_famille(kbuf[i],famille);
				refiliat(kbuf[i],famille) ;
				kbuf[i] = NIL ;
			}
		}
	}
	return ;
}
				
	
int moyenne(point,seuil)
struct maillon *point ;
float seuil ;
{
	int res = 0 , *nb ;
	float moye ;
	nb =(int*) malloc(sizeof(int) ) ;
	moye = getmoy_(point,nb) ;
	if (moye >= seuil) res = 1 ;
	return(res) ;
}
