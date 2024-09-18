#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_SUCCESS 0
#define PLAYER_FAILURE 1
 
struct Player{
	int player_id;
	char player_name[30];
};

extern struct PDS_RepoInfo *repoHandle;

// Add the given contact into the repository by calling put_rec_by_key
int add_player( struct Player *p );

// Display contact info in a single line as a CSV without any spaces
void print_player( struct Player *p );

// Use get_rec_by_key function to retrieve contact
int search_player( int player_id, struct Player *p );

// Load all the contacts from a CSV file
int store_players( char *player_data_file );

int search_player_by_name( char *player_name, struct Player *p, int *io_count );

/* Return 0 if phone of the contact matches with phone parameter */
/* Return 1 if phone of the contact does NOT match */
/* Return > 1 in case of any other error */

int match_player_name( void *rec, void *key );

int delete_player( int player_id );

#endif
