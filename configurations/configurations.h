#ifndef CONFIGURATIONS_HEADER
#define CONFIGURATIONS_HEADER

#include "../constants/constants.h"
#include "../cell/cell.h"
#include "../game/game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Loads configuration from a file into the game
void loadConfiguration(game **g, char *path); 

// Loads configuration giving the user option to choose current configurations
void loadConfigurationWithList(game **g);

// Saves the current configuration in a file
void saveConfiguration(game **g);

#endif
