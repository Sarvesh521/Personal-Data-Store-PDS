# Personal Data Store (PDS)

This project contains the implementation of various functionalities related to a Player-sports database.

Parent is Player
Child is Sport

## Files

- `IMT2022521_pds.c`: Contains the implementation of the primary data store.
- `IMT2022521_player.c`: Contains the implementation of player-related functionalities.
- `IMT2022521_sports.c`: Contains the implementation of sports-related functionalities.
- `bst.c`: Contains the implementation of a binary search tree used in the project.
- `IMT2022521_pds_tester.c`: Contains the test cases for the project.

## Compilation

You can manually compile the project using the following command for Non-Interactive Demo:

```bash
gcc -o lab6 IMT2022521_pds.c IMT2022521_player.c IMT2022521_sports.c bst.c IMT2022521_pds_tester.c 
./lab6 testcase.in
```

You can manually compile the project using the following command for Interactive Demo:

```bash
gcc -o lab6 IMT2022521_pds.c IMT2022521_player.c IMT2022521_sports.c bst.c linked_data_demo.c
./lab6
```
