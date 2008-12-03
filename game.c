
#include "config.h"
#include "logging.h"
#include "game.h"

void game_assign()
{

	int players_per_table;
	int concurrent_games;
	int games_in_progress;
	table *tble;

	if (!config_get_int("players_per_table", &players_per_table))
	{
		logging.critical("config file doesn't define 'players_per_table'");
		players_per_table = 6;
	}

	if (!config_get_int("concurrent_games", concurrent_games))
	{
		logging.critical("config file doesn't define 'concurrent_games'");
		concurrent_games = 3;
	}

	games_in_progress = 0;
	while (games_in_progress < concurrent_games)
	{
		tble = table_new();

		//listen for players

		//ask for table
		
		//assign to table
		

		games_in_progress++;
	}
}
