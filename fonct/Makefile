FLAGS=-c  -O3 -I. -I$(HOME)/dzv/h -I$(HOME) -I$(INRIMAGE)

SFILES=\
	getopt.c\
	res.c\
	apang2.c\
	carre.c\
	strip.c\
	strip2.c\
	reorg_chain.c\
	getbof.c\
	segec2_.c\
	chapri.c\
	meta.c\
	ecrcha.c


BRUIT=\
	afus_V0.c\
	bruit.c

BRUIT1=\
	afus_V1.c\
	bruit.c

BRUIT2=\
	afus_V0.c\
	bruit2.c

BRUIT3=\
	afus_V1.c\
	bruit2.c

chaine_V0_diff.a : $(SFILES) $(BRUIT)
	cc $(FLAGS) $?
	ar uv chaine_V0_diff.a *.o
	ranlib chaine_V0_diff.a
	/bin/mv chaine_V0_diff.a $(HOME)/lib/

chaine_V1_diff.a : $(SFILES) $(BRUIT1)
	cc $(FLAGS) $?
	ar uv chaine_V1_diff.a *.o
	ranlib chaine_V1_diff.a
	/bin/mv chaine_V1_diff.a $(HOME)/lib/

chaine_fort.a : $(SFILES) $(BRUIT2)
	cc $(FLAGS) $?
	ar uv chaine_fort_diff.a *.o
	ranlib chaine_fort_diff.a
	/bin/rm -f *.o
	/bin/mv chaine_fort_diff.a $(HOME)/lib/


chaine_fort_V1_diff.a : $(SFILES) $(BRUIT3)
	cc $(FLAGS) $?
	ar uv chaine_fort_V1_diff.a *.o
	ranlib chaine_fort_V1_diff.a
	/bin/rm -f *.o
	/bin/mv chaine_fort_V1_diff.a $(HOME)/lib/


