
/*
     Phillip Taylor B.Sc. (HONS) Software Engineering. Texas Hold'em Poker Software
     Copyright (C) 2008  Phillip Taylor

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/*
 * This file is where global options are accessed. These are configuration
 * options such as MAX_PLAYERS_PER_TABLE etc.
 */ 

#ifndef CONFIGURATON_H
#define CONFIGURATION_H

//flush_stdin_macro is a macro for flushing text between scanf and fgets calls.
#define flush_stdin_macro while (!fgetc(stdin))

//debug() is a macro defined here allowing you to write out debug statements
//pass in -d 5 when compiling to set the debug level to 5.
#define debug(level,fmt, ...) \
	if (debugThreshold >= level) \
		fprintf(stderr,fmt, __VA_ARGS__)

//because debug requires fprintf it then means all files with debug
//need to include <stdio.h> for convenience i'll just allow them
//to define DEBUG_SUPPORT and then I'll use that to wack in support for
#define DEBUG_SUPPORT #include <stdio.h>

#define BUILD_VERSION "0.1"
#define CONFIGURATION_FILE "poker.conf"

int debugThreshold;
int noSimultaneousTables;

//general settings

int CONFIG_MAX_TABLES;
int STARTING_SMALL_BLIND;
int TIME_TO_INCREMENT;
int INCREMENT_ON_PLAYER_DROP_OUT;
int SYSTEM_PERCENTAGE; /* how much money the server takes from the game */

void load_configuration();

#endif
