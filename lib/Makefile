# Nom du microcontroleur cible
MCU=atmega324pa

# Nom de votre projet
PROJECTNAME=test

# Fichiers sources
# Utilisez le suffixe .cpp pour les fichiers C++
# Listez tous les fichiers a compiler, separes par
# un espace. exemple: 'tp1.c tp2.cpp':
# Liste des dossiers a regarder
SRCS1 = $(wildcard Bouton/*.cpp)
SRCS2 = $(wildcard Can/*.cpp)
SRCS3 = $(wildcard Led/*.cpp)
SRCS4 = $(wildcard PWM/*.cpp)
SRCS5 = $(wildcard RS232/*.cpp)
SRCS6 = $(wildcard Timer/*.cpp)
SRCS7 = $(wildcard Debug/*.cpp)
SRCS8 = $(wildcard Piezo/*.cpp)
PRJSRC= $(SRCS1) $(SRCS2) $(SRCS3) $(SRCS4) $(SRCS5) $(SRCS6) $(SRCS7) $(SRCS8) 

# Niveau d'optimization
OPTLEVEL=s

# Liste complete des IDs disponible avec avrdude
AVRDUDE_PROGRAMMERID=usbasp

#compilateur utilise
CC=avr-gcc
#pour copier le contenu d'un fichier objet vers un autre
OBJCOPY=avr-objcopy
#pour permettre le transfert vers le microcontroleur
AVRDUDE=avrdude
#pour supprimer les fichiers lorsque l'on appel make clean
REMOVE=rm -f
# HEXFORMAT -- format pour les fichiers produient .hex
HEXFORMAT=ihex
#AVR-AR
LIBCC=avr-ar


# Flags pour le compilateur en C
CFLAGS=-I. -I/usr/include/simavr  -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-std=c++14 -fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall                                        

# Flags pour le compilateur en C++
CXXFLAGS=-fno-exceptions     

# Linker pour lier les librairies utilisees
LDFLAGS=-Wl,-Map,$(TRG).map -mmcu=$(MCU)

#Nom des cibles par defaut
TRG=$(PROJECTNAME).elf
HEXROMTRG=$(PROJECTNAME).hex
HEXTRG=$(HEXROMTRG) $(PROJECTNAME).ee.hex

# Cette fonction permet de differencier les fichiers .c
# des fichiers .cpp
# Fichier C
CFILES=$(filter %.c, $(PRJSRC))
# Fichier C++
CPPFILES=$(filter %.cpp, $(PRJSRC))

# Liste de tout les fichiers objet que nous devons creer
OBJDEPS= $(patsubst %.cpp, ./%.o, $(PRJSRC))

#Nom de la librairie
LIBNAME = libpackage.a
	

# Creation des cibles Phony (Phony Target)
.PHONY: all clean 

# Make all permet simplement de compiler le projet
#
all: $(LIBNAME)

# Creation du fichier .a avec avr-ar crs 
$(LIBNAME): $(OBJDEPS)
	$(LIBCC) $(ARFLAGS) $(LIBNAME) $(OBJDEPS)

# Production des fichiers object
# De C a objet
%.o: %.c
	$(CC) $(CFLAGS) -c $<
# De C++ a objet
%.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

# Verification des dependances (header dependencies)
-include *.d

# Make clean permet d'effacer tout les fichiers generes
clean:
	$(REMOVE) $(OBJDEPS) $(shell find . -name "*.d") $(LIBNAME)
