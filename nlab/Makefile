CFLAGS := -Wall -Wextra -Wpedantic -Wfloat-equal -Wvla -std=c99 -Werror -O2
SANITIZE := $(CFLAGS) -fsanitize=address -fsanitize=undefined -g3
NLBS := nlb
RSTS := rst

all: parse interp parse_s interp_s

parse: general.c nlab.c stack.c variabledict.c test.c
	clang general.c nlab.c stack.c variabledict.c test.c $(CFLAGS) -o parse -lm

parse_s: general.c nlab.c stack.c variabledict.c test.c
	clang general.c nlab.c stack.c variabledict.c test.c $(SANITIZE) -o parse_s -lm

interp: general.c nlab.c stack.c variabledict.c test.c
	clang general.c nlab.c stack.c variabledict.c test.c $(CFLAGS) -DINTERP -o interp -lm

interp_s: general.c nlab.c stack.c variabledict.c test.c
	clang general.c nlab.c stack.c variabledict.c test.c $(SANITIZE) -DINTERP -o interp_s -lm


testparse: parse 
	./parse_s ./.nlbfile/trivial.$(NLBS)
	./parse_s ./.nlbfile/setprinta.$(NLBS)
	./parse_s ./.nlbfile/onesprint.$(NLBS)
	./parse_s ./.nlbfile/setprintb.$(NLBS)
	./parse_s ./.nlbfile/loopa.$(NLBS)
	./parse_s ./.nlbfile/setprintc.$(NLBS)
	./parse_s ./.nlbfile/nestedloop.$(NLBS)
	./parse_s ./.nlbfile/loopb.$(NLBS)
	./parse_s ./.nlbfile/lifeb3s23.$(NLBS)

testinterp: interp
	./interp_s ./.nlbfile/trivial.$(NLBS) 
	./interp_s ./.nlbfile/setprinta.$(NLBS) 
	./interp_s ./.nlbfile/onesprint.$(NLBS)
	./interp_s ./.nlbfile/setprintb.$(NLBS)
	./interp_s ./.nlbfile/loopa.$(NLBS)
	./interp_s ./.nlbfile/setprintc.$(NLBS)
	./interp_s ./.nlbfile/nestedloop.$(NLBS)
	./interp_s ./.nlbfile/loopb.$(NLBS)
	./interp_s ./.nlbfile/lifeb3s23.$(NLBS)

# Save the result into the .rst file
savetestinterp: interp
	./interp_s ./.nlbfile/trivial.$(NLBS) > trivial.$(RSTS)
	./interp_s ./.nlbfile/setprinta.$(NLBS) > setprinta.$(RSTS)
	./interp_s ./.nlbfile/onesprint.$(NLBS) > onesprint.$(RSTS)
	./interp_s ./.nlbfile/setprintb.$(NLBS) > setprintb.$(RSTS)
	./interp_s ./.nlbfile/loopa.$(NLBS) > loopa.$(RSTS)
	./interp_s ./.nlbfile/setprintc.$(NLBS) > setprintc.$(RSTS)
	./interp_s ./.nlbfile/nestedloop.$(NLBS) > nestedloop.$(RSTS)
	./interp_s ./.nlbfile/loopb.$(NLBS) > loopb.$(RSTS)
	./interp_s ./.nlbfile/lifeb3s23.$(NLBS) > lifeb3s23.$(RSTS)	

autotest: autotest.c general.c
	clang autotest.c general.c $(CFLAGS) -o autotest -lm


clean: 
	rm -f parse interp parse_s interp_s testparse testinterp


#You may also be willing to try these .nlb files which have some errors.
errorexample1: 
	./parse ./.nlbfile/noBEGIN.$(NLBS)
errorexample2: 
	./parse ./.nlbfile/trivial_noRBrace.$(NLBS)
errorexample3: 
	./parse ./.nlbfile/set_missVARNAME.$(NLBS)
errorexample4: 
	./parse ./.nlbfile/set_noSEMICOLON.$(NLBS)
errorexample5: 
	./parse ./.nlbfile/invalidPRINT.$(NLBS)
errorexample6: 
	./parse ./.nlbfile/nullSTRING.$(NLBS)
errorexample7: 
	./interp ./.nlbfile/lackelements.$(NLBS)
errorexample8: 
	./parse ./.nlbfile/invalidVARNAME1.$(NLBS)
errorexample9: 
	./parse ./.nlbfile/invalidVARNAME2.$(NLBS)
errorexample10: 
	./interp ./.nlbfile/differentsizedarr.$(NLBS)

