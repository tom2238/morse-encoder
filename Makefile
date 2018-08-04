# Jméno přeloženého programu
program=morse
SCRIPT=gbuildnum
B_V=$(shell cat buildnumber.h | grep STR | grep -o '[0-9]*')
# Seznam objektových souborů použijeme hned na třech místech.
OBJ=morse.o wavwrite.o main.o 
SRC=wavwrite.c morse.c main.c 
HEAD=morse.h wavwrite.h buildnumber.h
# Míra optimalizace pøekladaèe gcc
OPT=-O1 -fno-strict-aliasing
# Překladač C
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra ${OPT}
LDFLAGS=-lm
# Cílùm build, clean a distrib neodpovídá pøímo Ÿádný soubor
.PHONY: build
.PHONY: clean
.PHONY: distrib
# Cíl build nemá Ÿádnou akci, jen závislost.
build: ${program}
	
# clean smaže soubory po překladu
clean:
	-rm -f *.o ${program}

# Generování závislostí
dep: 
	$(CC) -MM *.c >dep.list

# distrib vytvoøí balíèek s kompletním zdrojovým kódem
# akce na dva øádky se napí¹e pomocí zpìtného lomítka
distrib:
#	rm  ${program}${B_V}.tar.gz
	tar -zcvf ${program}${B_V}.tar.gz ${SRC} ${HEAD} ${program} Makefile > /dev/null

packall:
#	rm ${program}${B_V}_all.tar.gz
	tar -zcvf ${program}${B_V}_all.tar.gz * > /dev/null

$(SCRIPT):
		# Increment buildnumber.h
		gbuildnum

${program}: $(SCRIPT) ${OBJ}
		${CC} ${OBJ} ${CFLAGS} ${LDFLAGS} -o ${program} ${OPT}

# Závislost v¹ech objektových souborù na v¹ech hlavièkových
${OBJ}: ${HEAD}
