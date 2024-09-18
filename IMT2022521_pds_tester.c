//Following code is written by Sarvesh Kumar.A IMT2022521

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "player.h"
#include "sport.h"

#define TREPORT(a1,a2) printf("Status: %s - %s\n\n",a1,a2); fflush(stdout);

void process_line( char *test_case );

int main(int argc, char *argv[])
{
	FILE *cfptr;
	char test_case[50];

	if( argc != 2 ){
		fprintf(stderr, "Usage: %s testcasefile\n", argv[0]);
		exit(1);
	}

	cfptr = (FILE *) fopen(argv[1], "r");
	while(fgets(test_case, sizeof(test_case)-1, cfptr)){
		// printf("line:%s",test_case);
		if( !strcmp(test_case,"\n") || !strcmp(test_case,"") )
			continue;
		process_line( test_case );
	}
}

void process_line( char *test_case )
{
	char repo_name[30];
	char linked_repo_name[30];
	char command[15], param1[15], param2[15],param3[15], info[1024];
	int player_id, status, rec_size, expected_status,sport_id,link_rec_size; 
	struct Player testPlayer;
	strcpy(testPlayer.player_name, "dummy player");
	struct Sport testSport;
	strcpy(testSport.sport_name, "dummy sport");

	rec_size = sizeof(struct Player);
	link_rec_size = sizeof(struct Sport);

	sscanf(test_case, "%s%s%s%s", command, param1, param2,param3);
	printf("Test case: %s", test_case); fflush(stdout);
	if( !strcmp(command,"CREATE") ){
		strcpy(repo_name, param1);
		strcpy(linked_repo_name, param2);
		if( !strcmp(param3,"0") )
			expected_status = PLAYER_SUCCESS;
		else
			expected_status = PLAYER_FAILURE;
		if (strcmp(linked_repo_name,"0")==0)
		{
			status = pds_create( repo_name, NULL );
		}
		else
		{
			status = pds_create( repo_name,linked_repo_name );
		}
		if(status == PDS_SUCCESS)
			status = PLAYER_SUCCESS;
		else
			status = PLAYER_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_create returned status %d",status);
			TREPORT("FAIL", info);
		}
	}

	else if( !strcmp(command,"OPEN") ){
		strcpy(repo_name, param1);
		strcpy(linked_repo_name, param2);
		if( !strcmp(param3,"0") )
			expected_status = PLAYER_SUCCESS;
		else
			expected_status = PLAYER_FAILURE;

		if (strcmp(linked_repo_name,"0")==0)
		{
			status = pds_open( repo_name, NULL, rec_size, link_rec_size);
		}
		else
		{
			status = pds_open( repo_name, linked_repo_name, rec_size, link_rec_size);
		}
		if(status == PDS_SUCCESS)
			status = PLAYER_SUCCESS;
		else
			status = PLAYER_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_open returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"STORE") ){
		int che=0;
		che=strcmp(param2,"p");
		if (che==0)
		{
			sscanf(param1, "%d", &player_id);
			testPlayer.player_id = player_id;
			sprintf(testPlayer.player_name, "Name-of-%d", player_id);
			status = add_player( &testPlayer );
			if (status == PDS_SUCCESS)
			{
				status = PLAYER_SUCCESS;
			}
			else
			{
				status = PLAYER_FAILURE;
			}
			if( !strcmp(param3,"0")){
				expected_status = PLAYER_SUCCESS;
			}
			else
			{
				expected_status = PLAYER_FAILURE;
			}
			if( status == expected_status ){
				TREPORT("PASS", "");
			}
			else{
				sprintf(info,"add_player returned status %d",status);
				TREPORT("FAIL", info);
			}
		}
		else
		{
			sscanf(param1, "%d", &sport_id);
			testSport.sport_id = sport_id;
			sprintf(testSport.sport_name, "Name-of-%d", sport_id);
			status = add_sport( &testSport );
			if (status == PDS_SUCCESS)
			{
				status = SPORT_SUCCESS;
			}
			else
			{
				status = SPORT_FAILURE;
			}
			if( !strcmp(param3,"0")){
				expected_status = SPORT_SUCCESS;
			}
			else
			{
				expected_status = SPORT_FAILURE;
			}
			if( status == expected_status ){
				TREPORT("PASS", "");
			}
			else{
				sprintf(info,"add_player returned status %d",status);
				TREPORT("FAIL", info);
			}
		}
	}
	else if( !strcmp(command,"NDX_SEARCH") ){
		int che=0;
		che=strcmp(param2,"p");
		if (che==0){
			if( !strcmp(param3,"0") )
				expected_status = PLAYER_SUCCESS;
			else
				expected_status = PLAYER_FAILURE;

			sscanf(param1, "%d", &player_id);
			testPlayer.player_id = -1;
			status = search_player( player_id, &testPlayer );
			if(status == PDS_SUCCESS)
				status = PLAYER_SUCCESS;
			else
				status = PLAYER_FAILURE;
			if( status != expected_status ){
				sprintf(info,"search key: %d; Got status %d",player_id, status);
				TREPORT("FAIL", info);
			}
			else{
				// Check if the retrieved values match
				char expected_name[30];
				sprintf(expected_name, "Name-of-%d", player_id);
				if( expected_status == 0 ){
					if (testPlayer.player_id == player_id && 
						strcmp(testPlayer.player_name,expected_name) == 0){
							TREPORT("PASS", "");
					}
					else{
						sprintf(info,"Player data not matching... Expected:{%d,%s} Got:{%d,%s}\n",
							player_id, expected_name, 
							testPlayer.player_id, testPlayer.player_name
						);
						TREPORT("FAIL", info);
					}
				}
				else
					TREPORT("PASS", "");
			}
		}
		else
		{
			if( !strcmp(param3,"0") )
				expected_status = SPORT_SUCCESS;
			else
				expected_status = SPORT_FAILURE;

			sscanf(param1, "%d", &sport_id);
			testSport.sport_id = -1;
			status = search_sport( sport_id, &testSport );
			if(status == PDS_SUCCESS)
				status = SPORT_SUCCESS;
			else
				status = SPORT_FAILURE;
			if( status != expected_status ){
				sprintf(info,"search key: %d; Got status %d",sport_id, status);
				TREPORT("FAIL", info);
			}
			else{
				char expected_name[30];
				sprintf(expected_name, "Name-of-%d", sport_id);
				if( expected_status == 0 ){
					if (testSport.sport_id == sport_id && 
						strcmp(testSport.sport_name,expected_name) == 0){
							TREPORT("PASS", "");
					}
					else{
						sprintf(info,"Sport data not matching... Expected:{%d,%s} Got:{%d,%s}\n",
							sport_id, expected_name, 
							testSport.sport_id, testSport.sport_name
						);
						TREPORT("FAIL", info);
					}
				}
				else
					TREPORT("PASS", "");
			}
		}
	}

	else if( !strcmp(command,"NON_NDX_SEARCH") ){
		char player_name[30], expected_name[30];
		int expected_io, actual_io;
		if( strcmp(param2,"-1") == 0 )
			expected_status = PLAYER_FAILURE;
		else
			expected_status = PLAYER_SUCCESS;

		sscanf(param1, "%s", player_name);
		sscanf(param2, "%d", &expected_io);
		testPlayer.player_id = -1;
		actual_io = 0;
		status = search_player_by_name( player_name, &testPlayer, &actual_io );
		if(status == PDS_SUCCESS)
			status = PLAYER_SUCCESS;
		else
			status = PLAYER_FAILURE;
		if( status != expected_status ){
			sprintf(info,"search key: %d; Got status %d",player_id, status);
			TREPORT("FAIL", info);
		}
		else{
			sscanf(player_name, "%s", expected_name);
			if (expected_status == 0)
			{
				if (strcmp(testPlayer.player_name,expected_name) == 0)
				{
					if (expected_io == actual_io)
					{
						TREPORT("PASS", "");
					}
					else
					{
						sprintf(info,"Num I/O not matching for player %d... Expected:%d Got:%d\n",
							player_id, expected_io, actual_io
						);
						TREPORT("FAIL", info);
					}
				}
				else
				{
					sprintf(info,"Player data not matching... Expected:{%d,%s} Got:{%d,%s}\n",
						player_id, expected_name, 
						testPlayer.player_id, testPlayer.player_name
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
	else if( !strcmp(command,"NDX_DELETE") ){
		if( strcmp(param2,"0") == 0 )
			expected_status = PLAYER_SUCCESS;
		else
			expected_status = PLAYER_FAILURE;

		sscanf(param1, "%d", &player_id);
		testPlayer.player_id = player_id;
		status = delete_player( player_id );
		if( status != expected_status ){
			sprintf(info,"delete key: %d; Got status %d",player_id, status);
			TREPORT("FAIL", info);
		}
		else{
			TREPORT("PASS", "");
		}
	}
	else if( !strcmp(command,"CLOSE") ){
		if( !strcmp(param1,"0") )
			expected_status = PLAYER_SUCCESS;
		else
			expected_status = PLAYER_FAILURE;

		status = pds_close();
		if(status == PDS_SUCCESS)
			status = PLAYER_SUCCESS;
		else
			status = PLAYER_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_close returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if (!strcmp(command,"ADDLINK"))
	{
		int parent_key,child_key;
		sscanf(param1, "%d", &parent_key);
		sscanf(param2, "%d", &child_key);
		struct PDS_link_info link_info;
		link_info.parent_key = parent_key;
		link_info.child_key = child_key;
		int status;
		int flag1=0;
		int flag2=0;
		status = search_player(parent_key, &testPlayer);
		if (status == PDS_SUCCESS) {
			printf("Parent key found\n");
			flag1=1;
		} else {
			printf("Parent key not found\n");
		}
		status = search_sport(child_key, &testSport);
		if (status == PDS_SUCCESS) {
			printf("Child key found\n");
			flag2=1;
		} else {
			printf("Child key not found\n");
		}
		if (flag1==0 || flag2==0){
			status = PLAYER_FAILURE;
		}
		else{
			status = pds_link_rec(parent_key,child_key);
		}
		if (status == PDS_SUCCESS)
		{
			status = PLAYER_SUCCESS;
		}
		else
		{
			status = PLAYER_FAILURE;
		}
		if( !strcmp(param3,"0")){
			expected_status = PLAYER_SUCCESS;
		}
		else
		{
			expected_status = PLAYER_FAILURE;
		}
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"add_link returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if (!strcmp(command,"GETLINK"))
	{
		int parent_key;
		struct PDS_link_info link_info;
		int linked_keys_result[10];
		int *result_set_size = (int *)malloc(sizeof(int));
		sscanf(param1, "%d", &parent_key);
		status = search_player(parent_key, &testPlayer);
		if (status == PDS_SUCCESS) {
			status = pds_get_linked_rec(parent_key,linked_keys_result,result_set_size);
		} else {
			printf("Parent key not found\n");
		}
		if (status == PDS_SUCCESS)
		{
			status = PLAYER_SUCCESS;
		}
		else
		{
			status = PLAYER_FAILURE;
		}
		if( !strcmp(param2,"0")){
			expected_status = PLAYER_SUCCESS;
		}
		else
		{
			expected_status = PLAYER_FAILURE;
		}
		if( status == expected_status ){
			if (status == PLAYER_SUCCESS) {
				for (int i=0;i<(*result_set_size);i++){
					printf("Linked key: %d\n",linked_keys_result[i]);
				}
			}
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"get_link returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else{
		TREPORT("FAIL", "Invalid command");
	}
}