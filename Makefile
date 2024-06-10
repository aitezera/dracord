ECC = g++
FLAGS = -ljsoncpp -lSDL2_image -lcpr -I/usr/include/SDL2
NAME = dracord
SRC = src
CPPFILES = $(foreach dir,$(SRC),$(wildcard $(dir)/*.cpp))

requirements:
	@echo [!] Checking required libraries are installed
	@$(if $(shell ldconfig -p | grep SDL2), echo '[!] Found SDL2', echo '[X] SDL2 was not found ' ;FORCEFAIL)
	@$(if $(shell ldconfig -p | grep SDL2_image), echo '[!] Found SDL2_image', echo '[X] SDL2_image was not found ' ;FORCEFAIL)
	@$(if $(shell ldconfig -p | grep libjsoncpp), echo '[!] Found libjsoncpp', echo '[X] libjsoncpp was not found ' ;FORCEFAIL)
	@$(if $(shell ldconfig -p | grep libcpr), echo '[!] Found libcpr', echo '[X] libcpr was not found ' ;FORCEFAIL)

all:
	$(MAKE) requirements
	$(info [!] Compiling all files within src)
	$(ECC) -o $(NAME) $(CPPFILES) $(FLAGS)

# This is here to only test SDL2 rather than requesting to the Discord API
debug:
	$(ECC) -o $(NAME) src/main.cpp src/sdlinput.cpp src/sdlinput.h $(FLAGS)