
#include <stdio.h>
#include <stdlib.h>

#include "logging.h"
#include "config.h"

int *output_level;
FILE *log_file;
bool *write_to_screen;
bool *write_to_log;

void logging_init()
{
	char *log_filename;

	output_level = malloc(sizeof(int));
	write_to_screen = malloc(sizeof(bool));
	write_to_log = malloc(sizeof(bool));

	if (!config_get_int("logging_output_level", &output_level))
	{
		printf("EARLY APPLICATION ABORT. CONFIG DOESN'T SUPPLY logging_output_level\n");
		exit(0);
	}

	if (!config_get_bool("write_to_log", &write_to_log))
	{
		printf("EARLY APPLICATION ABORT. CONFIG DOESN'T SUPPLY write_to_log\n");
		exit(0);
	}
	else
	{
		if (*write_to_log)
		{
			if (!config_get_string("log_filename", &log_filename))
			{
				printf("EARLY APPLICATION ABORT. CONFIG DOESN'T SUPPLY log_filename\n");
				exit(0);
			}
			else
			{
				log_file = fopen(log_filename, "w");

				if (!log_file)
				{
					printf("unable to open %s for writing. aborting\n", log_filename);
					exit(0);
				}
			}
		}
	}

	if (!config_get_bool("write_to_screen", &write_to_screen))
	{
		printf("EARLY APPLICATION ABORT. CONFIG DOESN'T SUPPLY WRITE_TO_SCREEN\n");
		exit(0);
	}

	printf("output set to %d\n", *output_level);

	if (*write_to_screen)
		printf("writing to screen: on\n");
	else
		printf("writing to screen: off\n");

	if (*write_to_log)
		printf("writing to log: on [%s]\n", log_filename);
	else
		printf("writing to log: off\n");

}

void logging_critical(char *message)
{
	if (*write_to_screen)
		printf("CRITICAL:\t%s\n", message);

	if (*write_to_log)
		fprintf(log_file, "CRITICAL:\t%s\n", message);
}

void logging_warning(char *message)
{
	if (*output_level < 1) return;

	if (*write_to_screen)
		printf("WARNING:\t%s\n", message);

	if (*write_to_log)
		fprintf(log_file, "WARNING:\t%s\n", message);
}

void logging_debug_high(char *message)
{
	if (*output_level < 2) return;

	if (*write_to_screen)
		printf("DEBUG H:\t%s\n", message);

	if (*write_to_log)
		fprintf(log_file, "DEBUG H:\t%s\n", message);
}

void logging_debug_low(char *message)
{
	if (*output_level < 3) return;

	if (*write_to_screen)
		printf("DEBUG L:\t%s\n", message);

	if (*write_to_log)
		fprintf(log_file, "DEBUG L:\t%s\n", message);
}

void logging_shutdown()
{
	free(output_level);
	free(write_to_screen);
	free(write_to_log);

	if (log_file != NULL)
	{
		fclose(log_file);
		log_file = NULL;
	}

	printf("logging shutdown\n");
}
