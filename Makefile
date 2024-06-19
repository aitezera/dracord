ECC = g++
FLAGS = -ljsoncpp -lSDL2_image -lSDL2_ttf -lcpr -lSDL2 -I/usr/include/SDL2
NAME = dracord
SRC = src
INCLUDE = include

CPPFILES = $(foreach dir,$(SRC),$(wildcard $(dir)/*.cpp))
HPPFILES = $(foreach dir,$(INCLUDE),$(wildcard $(dir)/*.hpp))

requirements:
	@echo [!] Checking required libraries are installed
	@$(if $(shell ldconfig -p | grep SDL2), echo '[!] Found SDL2', echo '[X] SDL2 was not found ' ;FORCEFAIL)
	@$(if $(shell ldconfig -p | grep SDL2_image), echo '[!] Found SDL2_image', echo '[X] SDL2_image was not found ' ;FORCEFAIL)
	@$(if $(shell ldconfig -p | grep libjsoncpp), echo '[!] Found libjsoncpp', echo '[X] libjsoncpp was not found ' ;FORCEFAIL)
	@$(if $(shell ldconfig -p | grep libcpr), echo '[!] Found libcpr', echo '[X] libcpr was not found ' ;FORCEFAIL)

all: requirements
	$(info [!] Compiling all files within src)
	$(ECC) -o $(NAME) $(CPPFILES) -I $(INCLUDE) $(FLAGS)

# This is here to only test SDL2 rather than requesting to the Discord API
# This will be removed upon actually having them linked together.
client:
	$(ECC) -o $(NAME) src/main.cpp src/window.cpp src/ui.cpp src/logging.cpp -I $(INCLUDE) $(FLAGS)

# This is to test the requests being made to the Discord API
requests:
	$(ECC) -o $(NAME) src/requests.cpp src/logging.cpp -I $(INCLUDE) $(FLAGS)

# As it gets more bigger, it will eventually be split into multiple directories and such. 
# If that happens, this will be updated to reflect that.
clean:
	$(info [!] Cleaning up)
	@rm -f $(NAME)
	$(info [!] Cleaned up the folder)
