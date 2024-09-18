//Following code is written by Sarvesh Kumar.A IMT2022521

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "sport.h"

int store_sports( char *sport_data_file )
{
    FILE *cfptr;
    char sport_line[500], token;
    struct Sport s, dummy;

    cfptr = (FILE *) fopen(sport_data_file, "r");
    while(fgets(sport_line, sizeof(sport_line)-1, cfptr)){
        //printf("line:%s",contact_line);
        sscanf(sport_line, "%d%s", &(s.sport_id),s.sport_name);
        print_sport( &s );
        add_sport( &s );
    }
}


void print_sport( struct Sport *s )
{
    printf("%d,%s\n", s->sport_id,s->sport_name);
}


int search_sport( int sport_id, struct Sport *s )
{
    return get_linked_rec_by_key( sport_id, s );
}

int add_sport( struct Sport *s )
{
    int status;

    status = put_linked_rec_by_key( s->sport_id, s );

    if( status != PDS_SUCCESS ){
        fprintf(stderr, "Unable to add sport with key %d. Error %d", s->sport_id, status );
        return SPORT_FAILURE;
    }
    return status;
}