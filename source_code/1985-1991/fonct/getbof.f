C*GETBOF
C===============================================================!
C								!
C	Giraudon  -  SOPHIA     :      3/05/84			!
C								!
C===============================================================!
C								!
C	Routine qui calcule le nombre de lignes maximum pouvant !
C	etre lue a la fois, en fonction du format de l'image    !
C	et de la taille du buffer lorsque l'on veut filtrer une !
C	image par bout (probleme du recouvrement)		!
C								!
C	Les parametres d'entrees sont :				!
C	- IFMT   :  format de l'image				!
C	- LONG   :  taille du buffer de stockage		!
C	- IDIM   :  dimension du filtre				!
C								!
C	Les parametres de sorties sont :			!
C	- NLIGN  : nombre de lignes images pouvant etre lues	!
C		   a la fois					!
C	- NREST  : nombre de lignes a lire au dernier coup	!
C	- NTOUR  : nombre de tour pour lire toute l'image       !
C	- NDIMA  : le nombre exact de lignes utiles		!
C								!
C===============================================================!
C-
	subroutine getbof(ifmt,long,idim,nlign,nrest,ntour,ndima)
	dimension ifmt(1)
	index=0
	inf=1
	nlign=long/ifmt(1)
	max=nlign
	do 5 i=1,600
	index=index+1
	if (max.ge.ifmt(2)) goto 200
	inf=max-idim+2
	max=inf+nlign-1
5	continue
200	nrest=nlign-(max-ifmt(2))
	if (nlign.ge.ifmt(2)) nlign=ifmt(2)
	ntour=index
	ndima=nlign-idim+1
	return
	end



