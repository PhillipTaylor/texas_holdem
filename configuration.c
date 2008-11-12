
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

#include <stdio.h>
#include <stdlib.h>

#include "configuration.h"

#define BUFFER_SIZE 255

void load_configuration()
{
	FILE *fp = NULL;
	char *buff = NULL;
	int value = 0;

	buff = (char*) malloc ((sizeof(char)) * BUFFER_SIZE);
	fp = fopen(CONFIGURATION_FILE,"r");

	if (!fp)
	{
		fprintf(stderr, "Cannot open configuration file: %s\n",CONFIGURATION_FILE);
		exit; //Bail out with the configuration file. It's early so their is no danger of data corruption and so forth.
	}

	debug(1,"loading configuration file....\n","");

	while (!feof(fp))
	{
		fgets(buff, BUFFER_SIZE, fp);
		debug(5, "CONFIG FILE LINE = %s",buff);

		//I parse by using !strncmp(buff,"search",len("search"))
		//then i use atoi(strlen("search" +1))

		//if (!strncmp(buff,"MAX_SIMULTANEOUS_GAMES="))
			//CONFIG_MAX_TABLES = atoi(*(char*)(buff+23));
		
	}

	free(buff);
	fclose(fp);

	debug(4, "CONFIG_MAX_TABLES = %i\n",CONFIG_MAX_TABLES);

	debug(1, "finished loading configuration file\n","");
}

