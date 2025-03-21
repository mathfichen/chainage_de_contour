p=$(HOME)/lib/dzv.a
v=$(HOME)/lib/chaine_V0.a
w= -lcore -lsuntool -lsunwindow -lpixrect 

i=/u/gandalf/0/pastis/lib/libarg.a

LIBS = $(i) $(v) $(p) -lX11 -linrimage -lm

OPTION = -O3
#OPTION = -g 
INCLUDE = -I. -I$(HOME)/dzv/h -I$(HOME) -I$(INRIMAGE)

#*********** REGLE PAR DEFAUT DE GENERATION DES FICHIERS .O ************
.c.o:; cc ${OPTION} ${INCLUDE} -c $*.c -o $*.o
 

apoly : apoly.o approx.o
	cc $(OPTION) apoly.o approx.o $(LIBS) -o apoly
	/bin/rm *.o
	/bin/mv apoly $(HOME)/bin/

seg_ori : seg_ori.o 
	cc $(OPTION) seg_ori.o $(LIBS) -o seg_ori
	/bin/rm *.o
	/bin/mv seg_ori $(HOME)/bin/

