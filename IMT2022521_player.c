//Following code is written by Sarvesh Kumar.A IMT2022521

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "player.h"

int store_players( char *player_data_file )
{
	FILE *cfptr;
	char player_line[500], token;
	struct Player p, dummy;

	cfptr = (FILE *) fopen(player_data_file, "r");
	while(fgets(player_line, sizeof(player_line)-1, cfptr)){
		//printf("line:%s",contact_line);
		sscanf(player_line, "%d%s", &(p.player_id),p.player_name);
		print_player( &p );
		add_player( &p );
	}
}

void print_player( struct Player *p )
{
	printf("%d,%s\n", p->player_id,p->player_name);
}


int search_player( int player_id, struct Player *p )
{
	return get_rec_by_key( player_id, p );
}


int add_player( struct Player *p )
{
	int status;

	status = put_rec_by_key( p->player_id, p );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add player with key %d. Error %d", p->player_id, status );
		return PLAYER_FAILURE;
	}
	return status;
}

int search_player_by_name( char *player_name, struct Player *p, int *io_count )
{
	int result = get_rec_by_non_ndx_key(player_name, p, match_player_name, io_count);
	if (result == 0) {
		return PLAYER_SUCCESS;
	} else {
		return PLAYER_FAILURE;
	}
}

int match_player_name( void *rec, void *key )
{
	// Store the rec in a struct contact pointer
	// Store the key in a char pointer
	// Compare the phone values in key and record
	// Return 0,1,>1 based on above condition
	struct Player *player = (struct Player *)rec;
	char *player_name = (char *)key;
	// printf("Comparing %s with %s\n", player->player_name, player_name);
	if (strcmp(player->player_name, player_name) == 0) {
		return 0;
	} else {
		return 1;
	}
}

int delete_player ( int player_id )
{
  int status = delete_rec_by_ndx_key(player_id);
  if(status != PDS_SUCCESS)
	return PLAYER_FAILURE;
  return status;
}