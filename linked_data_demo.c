//Following code is written by Sarvesh Kumar.A IMT2022521

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "sport.h"
#include "pds.h"
#include "player.h"
#include "bst.h"

// Include the header file that contains the struct declarations for parent and child records
struct Player player_arr[10];
struct Sport sport_arr[10];
// Declare global variables for 10 parent and 10 child records

void show_menu();
void setup_data();
void process_option( int option );
void process_option1();
void process_option2();
void process_option3();
void process_option4();


int main()
{
    int option;
    setup_data();
    do{
        show_menu();
        scanf("%d",&option);
        process_option(option);
    } while (option != 0);
    pds_close();
    printf("Exiting... \n");
    return 0;
}

void show_menu()
{
    printf("\nLINKED DATA DEMO\n\n");
    printf("0. Exit\n");
    printf("1. Add linked data\n");
    printf("2. Get linked data\n");
    printf("3. Get Player by name\n");
    printf("4. Delete Player\n");
    printf("\nEnter option: ");  
}

void setup_data()
{
    // Call appropriate PDS functions to create the repository
    int status;
    status=pds_create("player_data","sport_data");
    if (status == PDS_SUCCESS) {
        printf("Repository created successfully\n");
    } else {
        printf("Error creating repository\n");
    }
    // Call appropriate PDS functions to open the repository
    status=pds_open("player_data","sport_data",sizeof(struct Player),sizeof(struct Sport));
    if (status == PDS_SUCCESS) {
        printf("Repository opened successfully\n");
    } else {
        printf("Error opening repository\n");
    }
    // statically create 10 parent and 10 child records individually
    for (int i=0;i<10;i++){
        player_arr[i].player_id = 100+i;
        strcpy(player_arr[i].player_name, "Player");
        sport_arr[i].sport_id = 200+i;
        strcpy(sport_arr[i].sport_name, "Sport");
    }
    // Call appropriate PDS functions to store the parent and child records
    for (int i=0;i<10;i++){
        status=add_player(&player_arr[i]);
        if (status == PDS_SUCCESS) {
            printf("Player added successfully\n");
        } else {
            printf("Error adding player\n");
        }

        status=add_sport(&sport_arr[i]);
        if (status == PDS_SUCCESS) {
            printf("Sport added successfully\n");
        } else {
            printf("Error adding sport\n");
        }
    }
}

void process_option( int option )
{
    switch(option){
        case 0: 
            // do nothing
            break;
        case 1:
            process_option1();
            break;
        case 2:
            process_option2();
            break;
        case 3:
            process_option3();
            break;
        case 4:
            process_option4();
            break;
        default:
            printf("Invalid option\n");
            break;
    }
}

void process_option1()
{
    int parent_key, child_key;
    printf("Enter parent key and child key for linking: ");
    scanf("%d%d", &parent_key, &child_key);
    int status;
    int flag1=0;
    int flag2=0;
    status = search_player(parent_key, &player_arr[0]);
    if (status == PDS_SUCCESS) {
        printf("Parent key found\n");
        flag1=1;
    } else {
        printf("Parent key not found\n");
    }
    status = search_sport(child_key, &sport_arr[0]);
    if (status == PDS_SUCCESS) {
        printf("Child key found\n");
        flag2=1;
    } else {
        printf("Child key not found\n");
    }
    if (flag1==0 || flag2==0){
        printf("Linking failed\n");
        return;
    }
    status = pds_link_rec (parent_key, child_key);
    if (status == PDS_SUCCESS) {
        printf("Linking successful\n");
    } else {
        printf("Linking failed\n");
    }
    // TODO: Call appropriate PDS functions here to link the two records
}

void process_option2()
{
    int parent_key;
    printf("Enter parent key for linking: ");
    scanf("%d", &parent_key);
    int status;
    status = search_player(parent_key, &player_arr[0]);
    if (status == PDS_SUCCESS) {
        printf("Parent key found\n");
    } else {
        printf("Parent key not found\n");
        return;
    }
    // TODO: Call appropriate PDS functions here to fetch arrach of child keys
    // Call your PDS functions to display the list of child keys retrieved for the given parent key
    // Display all field values of each child record in a nicely formatted manner
    int linked_keys_result[10];
    int *result_set_size = (int *)malloc(sizeof(int));
    pds_get_linked_rec(parent_key,linked_keys_result,result_set_size);
    for (int i=0;i<(*result_set_size);i++){
        printf("Linked key: %d\n",linked_keys_result[i]);
    }
}


void process_option3(){
    char player_name[30];
    printf("Enter player name: ");
    scanf("%s", player_name);
    int status;
    struct Player p;
    int io_count;
    status = search_player_by_name(player_name, &p, &io_count);
    if (status == PLAYER_SUCCESS) {
        printf("Player found\n");
        print_player(&p);
    } else {
        printf("Player not found\n");
    }
}

void process_option4(){
    int player_id;
    printf("Enter player id: ");
    scanf("%d", &player_id);
    int status;
    status = delete_player(player_id);
    if (status == PLAYER_SUCCESS) {
        printf("Player deleted\n");
    } else {
        printf("Player not found\n");
    }
}