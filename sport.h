#ifndef SPORT_H
#define SPORT_H

#define SPORT_SUCCESS 0
#define SPORT_FAILURE 1
 
struct Sport{
    int sport_id;
    char sport_name[30];
};

extern struct PDS_RepoInfo *repoHandle;

// Add the given contact into the repository by calling put_rec_by_key
// int add_contact( struct Contact *c );
int add_sport( struct Sport *s );

// Display contact info in a single line as a CSV without any spaces
// void print_contact( struct Contact *c );
void print_sport( struct Sport *s );

// Use get_rec_by_key function to retrieve contact
// int search_contact( int contact_id, struct Contact *c );
int search_sport( int sport_id, struct Sport *s );

// Load all the contacts from a CSV file
// int store_contacts( char *contact_data_file );
int store_sports( char *sport_data_file );

// int search_contact_by_phone( char *phone, struct Contact *c, int *io_count );

/* Return 0 if phone of the contact matches with phone parameter */
/* Return 1 if phone of the contact does NOT match */
/* Return > 1 in case of any other error */
// int match_contact_phone( void *rec, void *key );
// int delete_contact( int contact_id );

#endif
