/**GETBOF */
/*===============================================================*/
/*								*/
/*	Giraudon  -  SOPHIA     :      3/05/84			*/
/*								*/
/*===============================================================*/
/*								*/
/*	Routine qui calcule le nombre de lignes maximum pouvant */
/*	etre lue a la fois, en fonction du format de l'image    */
/*	et de la taille du buffer lorsque l'on veut filtrer une */
/*	image par bout (probleme du recouvrement)		*/
/*								*/
/*	Les parametres d'entrees sont :				*/
/*	- IFMT   :  format de l'image				*/
/*	- LONG   :  taille du buffer de stockage		*/
/*	- IDIM   :  dimension du filtre				*/
/*								*/
/*	Les parametres de sorties sont :			*/
/*	- NLIGN  : nombre de lignes images pouvant etre lues	*/
/*		   a la fois					*/
/*	- NREST  : nombre de lignes a lire au dernier coup	*/
/*	- NTOUR  : nombre de tour pour lire toute l'image       */
/*	- NLIT   : le nombre exact de lignes utiles		*/
/*								*/
/*===============================================================*/


getbof(ifmt,ilong,idim,nlign,nrest,ntour,nlit)
int ifmt[], ilong, idim, *nlign, *nrest, *ntour, *nlit ; {

	int index=0,inf=1 , max ,i ;
	*nlign = ilong / ifmt[0] ;
	max = *nlign ;
	for( i = 0 ; i< ifmt[1] ; i++) {
		index = index + 1 ;
		if (max >= ifmt[1]) goto LABEL ;
		inf = max - idim + 2 ;
		max = inf + *nlign - 1 ;
	}

LABEL:	*nrest = *nlign - (max-ifmt[1] ) ;
	if (*nlign >= ifmt[1] ) *nlign=ifmt[1] ;
	*ntour = index ;
	*nlit = *nlign - idim + 1 ;
	return(1) ;

}
