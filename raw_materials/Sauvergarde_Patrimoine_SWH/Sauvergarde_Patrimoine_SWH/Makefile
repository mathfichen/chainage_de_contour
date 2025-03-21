v=$(HOME)/lib/chaine_V0_diff.a
v1=$(HOME)/lib/chaine_V1_diff.a
v2=$(HOME)/lib/chaine_fort_diff.a
v3=$(HOME)/lib/chaine_fort_V1_diff.a

p=$(HOME)/lib/dzv.a

a=$(INRIMAGE)/lib/imaglb7
b=$(INRIMAGE)/lib/vsysl7
c=$(INRIMAGE)/lib/entsorl7
e=$(INRIMAGE)/lib/vsyslc

w= -lcore -lsuntool -lsunwindow -lpixrect 
%q= -lF77 -lI77 -lm
q= -lm

.SUFFIXES : .o .c .f

.f.o :
	f77 -c -g $*.f
.c.o :
	cc -c -O3 -I. -I$(HOME)/dzv/h -I$(HOME) -I$(INRIMAGE) $*.c

newk_V0_diff : chaines.o chaic.o  $v $p $a $c $b $e 
	cc -g chaines.o chaic.o  $v $p $a $c $b $e $q -o newk_V0_diff
	/bin/mv newk_V0_diff $(HOME)/bin/

newk_V1_diff : chaines.o chaic.o  $(v1) $p $a $c $b $e 
	cc -g chaines.o chaic.o  $(v1) $p $a $c $b $e $q -o newk_V1_diff
	/bin/mv newk_V1_diff $(HOME)/bin/

newk_fort_diff : chaines.o chaic.o  $(v2) $p $a $c $b $e 
	cc -g chaines.o chaic.o  $(v2) $p $a $c $b $e $q -o newk_fort_diff
	/bin/mv newk_fort_diff $(HOME)/bin/

