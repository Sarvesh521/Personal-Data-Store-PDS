// Following code is written by Sarvesh Kumar.A IMT2022521

#include <stdio.h>
#include "pds.h"
#include <string.h>
#include <stdlib.h>
#include "bst.h"
#include <stdbool.h>

struct PDS_RepoInfo repo_handle;

// pds_create
// Open the data file and index file in "wb" mode
// Initialize index file by storing "0" to indicate there are zero entries in index file
// close the files
int pds_create(char *repo_name, char *linked_repo_name)
{
	char *repo_name_data = malloc(sizeof(char) * 30);
	strcpy(repo_name_data, repo_name);
	strcat(repo_name_data, ".dat");
	FILE *data_file_ptr = fopen(repo_name_data, "wb");
	if (data_file_ptr == NULL)
	{
		return PDS_FILE_ERROR;
	}
	fclose(data_file_ptr);
	char *repo_name_index = malloc(sizeof(char) * 30);
	strcpy(repo_name_index, repo_name);
	strcat(repo_name_index, ".ndx");
	FILE *index_file_ptr = fopen(repo_name_index, "wb");
	if (index_file_ptr == NULL)
	{
		return PDS_FILE_ERROR;
	}
	int zero = 0;
	fwrite(&zero, sizeof(int), 1, index_file_ptr);
	fclose(index_file_ptr);

	if (linked_repo_name != NULL)
	{
		char* repo_name_linked_data = malloc(sizeof(char) * 30);
		strcpy(repo_name_linked_data, linked_repo_name);
		strcat(repo_name_linked_data, ".dat");
		FILE *linked_data_file_ptr = fopen(repo_name_linked_data, "wb");
		if (linked_data_file_ptr == NULL)
		{
			return PDS_FILE_ERROR;
		}
		fclose(linked_data_file_ptr);

		char* repo_name_linked = malloc(sizeof(char) * 30);
		strcpy(repo_name_linked, repo_name);
		strcat(repo_name_linked, "_");
		strcat(repo_name_linked, linked_repo_name);
		strcat(repo_name_linked, ".dat");
		FILE *linked_file_ptr = fopen(repo_name_linked, "wb");
		if (linked_file_ptr == NULL)
		{
			return PDS_FILE_ERROR;
		}
		fclose(linked_file_ptr);
	}

	return PDS_SUCCESS;
}

// pds_open
// Open the data file and index file in rb+ mode
// Update the fields of PDS_RepoInfo appropriately
// Read the number of records form the index file
// Load the index into the array and store in ndx_array by reading index entries from the index file
// Close only the index file
int pds_open( char *repo_name, char *linked_repo_name, int rec_size, int linked_rec_size )
{
	if (repo_handle.repo_status == PDS_REPO_OPEN)
	{
		return PDS_REPO_ALREADY_OPEN;
	}
	else
	{
		repo_handle.pds_data_fp=NULL;
		repo_handle.pds_ndx_fp=NULL;
		repo_handle.pds_linked_data_fp=NULL;
		repo_handle.pds_link_fp=NULL;
		char *repo_name_data = malloc(sizeof(char) * 30);
		strcpy(repo_name_data, repo_name);
		strcat(repo_name_data, ".dat");
		repo_handle.pds_data_fp = fopen(repo_name_data, "rb+");
		if (repo_handle.pds_data_fp == NULL)
		{
			return PDS_FILE_ERROR;
		}

		char *repo_name_index = malloc(sizeof(char) * 30);
		strcpy(repo_name_index, repo_name);
		strcat(repo_name_index, ".ndx");
		repo_handle.pds_ndx_fp = fopen(repo_name_index, "rb+");
		if (repo_handle.pds_ndx_fp == NULL)
		{
			return PDS_FILE_ERROR;
		}

		char *repo_handle_pds_name = malloc(sizeof(char) * 30);
		strcpy(repo_handle.pds_name, repo_name);
		repo_handle.repo_status = PDS_REPO_OPEN;
		repo_handle.rec_size = rec_size;
		repo_handle.rec_count = 0;
		repo_handle.ndx_root = NULL;
		fread(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp);
		// printf("rec_count: %d\n", repo_handle.rec_count);
		pds_load_ndx();
		fclose(repo_handle.pds_ndx_fp);

		if (linked_repo_name != NULL)
		{
			char *repo_name_linked_data = malloc(sizeof(char) * 30);
			strcpy(repo_name_linked_data, linked_repo_name);
			strcat(repo_name_linked_data, ".dat");
			repo_handle.pds_linked_data_fp = fopen(repo_name_linked_data, "rb+");
			if (repo_handle.pds_linked_data_fp == NULL)
			{
				return PDS_FILE_ERROR;
			}

			char *repo_name_linked = malloc(sizeof(char) * 30);
			strcpy(repo_name_linked, repo_name);
			strcat(repo_name_linked, "_");
			strcat(repo_name_linked, linked_repo_name);
			strcat(repo_name_linked, ".dat");
			repo_handle.pds_link_fp = fopen(repo_name_linked, "rb+");
			if (repo_handle.pds_link_fp == NULL)
			{
				return PDS_FILE_ERROR;
			}
			repo_handle.linked_rec_size = linked_rec_size;
		}
		

		return PDS_SUCCESS;
	}
}

// pds_load_ndx - Internal function
// Load the index entries into the BST ndx_root by calling bst_add_node repeatedly for each
// index entry. Unlike array, for BST, you need read the index file one by one in a loop
int pds_load_ndx()
{
	// Internal function used by pds_open to read index entries into BST
	int beginOfFile = fseek(repo_handle.pds_ndx_fp, 0, SEEK_SET);
	if (beginOfFile)
		return PDS_FILE_ERROR;
	struct PDS_NdxInfo *key_offset_info = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo *));
	while (fread(key_offset_info, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp) == 1)
	{
		int isInserted = bst_add_node(&repo_handle.ndx_root, key_offset_info->key, key_offset_info);
		if (isInserted == BST_DUP_KEY || isInserted == BST_NULL)
			return PDS_NDX_SAVE_FAILED;
		key_offset_info = (struct PDS_NdxInfo *)malloc(1 * sizeof(struct PDS_NdxInfo *));
	}
	return PDS_SUCCESS;
}

// put_rec_by_key
// Seek to the end of the data file
// Create an index entry with the current data file location using ftell
// Add index entry to BST by calling bst_add_node
// Increment record count
// Write the record at the end of the file
// Return failure in case of duplicate key
// int is_deleted; // 0 - not deleted, 1 - deleted
int put_rec_by_key(int key, void *rec)
{
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
		return PDS_REPO_NOT_OPEN;
	int endOfFile = fseek(repo_handle.pds_data_fp, 0, SEEK_END);
	if (endOfFile != 0)
		return PDS_FILE_ERROR;
	int byteOffset = (int)ftell(repo_handle.pds_data_fp);
	if (byteOffset == -1)
		return PDS_FILE_ERROR;
	struct BST_Node *bstSearchResult = bst_search(repo_handle.ndx_root, key);
	if (bstSearchResult)
	{
		struct PDS_NdxInfo *ndxInfo = bstSearchResult->data;
		if (!ndxInfo || ndxInfo->is_deleted == 1)
		{
			if (fseek(repo_handle.pds_data_fp, ndxInfo->offset, SEEK_SET) != 0)
				return PDS_FILE_ERROR;
			if (fwrite(&key, sizeof(int), 1, repo_handle.pds_data_fp) != 1 && fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp) != 1)
				return PDS_ADD_FAILED;
			ndxInfo->is_deleted = 0;
			return PDS_SUCCESS;
		}
	}
	struct PDS_NdxInfo *ndxInfo = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
	ndxInfo->key = key;
	ndxInfo->offset = byteOffset;
	ndxInfo->is_deleted = 0;											  
	if (bst_add_node(&repo_handle.ndx_root, key, ndxInfo) != BST_SUCCESS) 
		return PDS_ADD_FAILED;
	// Write the key at the current data file location.
	if (fwrite(&key, sizeof(int), 1, repo_handle.pds_data_fp) == 1 && fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp) == 1)
		return PDS_SUCCESS;
	return PDS_ADD_FAILED;
}

// put_linked_rec_by_key - NEW
// Seek to the end of the linked data file
// No need to create index entry
// Write the key at the current data file location
// Write the record after writing the key
int put_linked_rec_by_key( int key, void *rec ){
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
		return PDS_REPO_NOT_OPEN;
	int endOfFile = fseek(repo_handle.pds_linked_data_fp, 0, SEEK_END);
	if (endOfFile != 0)
		return PDS_FILE_ERROR;
	if (fwrite(&key, sizeof(int), 1, repo_handle.pds_linked_data_fp) == 1 && fwrite(rec, repo_handle.linked_rec_size, 1, repo_handle.pds_linked_data_fp) == 1)
		return PDS_SUCCESS;
	return PDS_ADD_FAILED;
}


// get_rec_by_key
// Search for index entry in BST by calling bst_search
// Seek to the file location based on offset in index entry
// Read the record from the current location
int get_rec_by_key(int key, void *rec)
{
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
		return PDS_REPO_NOT_OPEN;

	struct BST_Node *bstSearchResult = bst_search(repo_handle.ndx_root, key);

	if (!bstSearchResult)
		return PDS_REC_NOT_FOUND;
	if (((struct PDS_NdxInfo *)bstSearchResult->data)->is_deleted == 1)
		return PDS_REC_NOT_FOUND;
	struct PDS_NdxInfo *key_offset_info = (struct PDS_NdxInfo *)malloc(1 * sizeof(struct PDS_NdxInfo *));
	key_offset_info = bstSearchResult->data;
	int fileLocation = fseek(repo_handle.pds_data_fp, key_offset_info->offset, SEEK_SET);

	if (fileLocation)
		return PDS_FILE_ERROR;

	int readKey = fread(&key, sizeof(int), 1, repo_handle.pds_data_fp);

	if (readKey != 1)
		return PDS_FILE_ERROR;

	int readRecord = fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);

	if (readRecord != 1)
		return PDS_FILE_ERROR;

	return PDS_SUCCESS;
}

int get_rec_by_non_ndx_key(void *non_ndx_key, void *rec, int (*matcher)(void *rec, void *non_ndx_key), int *io_count)
{
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
		return PDS_REPO_NOT_OPEN;
	*io_count = 0;
	fseek(repo_handle.pds_data_fp, 0, SEEK_SET);
	int key;
	while (fread(&key, sizeof(int), 1, repo_handle.pds_data_fp) == 1)
	{
		(*io_count)++;
		fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
		if (matcher(rec, non_ndx_key) == 0 && (((struct PDS_NdxInfo *)bst_search(repo_handle.ndx_root, key)->data)->is_deleted == 0))
		{
			return PDS_SUCCESS;
		}
	}
	return PDS_REC_NOT_FOUND;
}
// get_linked_rec_by_key - NEW
// Do a linear search of the given key in the linked data file
int get_linked_rec_by_key( int key, void *rec ){
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
		return PDS_REPO_NOT_OPEN;
	fseek(repo_handle.pds_linked_data_fp, 0, SEEK_SET);
	int readKey;
	while (fread(&readKey, sizeof(int), 1, repo_handle.pds_linked_data_fp) == 1)
	{
		if (readKey == key)
		{
			fread(rec, repo_handle.linked_rec_size, 1, repo_handle.pds_linked_data_fp);
			return PDS_SUCCESS;
		}
		fseek(repo_handle.pds_linked_data_fp, repo_handle.linked_rec_size, SEEK_CUR);
	}
	return PDS_REC_NOT_FOUND;

}

bool Preorder(struct BST_Node *node)
{
	if (!node)
		return true;
	struct PDS_NdxInfo *key_offset_info = (struct PDS_NdxInfo *)malloc(1 * sizeof(struct PDS_NdxInfo *));
	key_offset_info = node->data;
	int putkey_offset_info = fwrite(key_offset_info, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp);
	if (putkey_offset_info != 1)
		return false;
	return Preorder(node->left_child) && Preorder(node->right_child);
}
// pds_close
// Open the index file in wb mode (write mode, not append mode)
// Store the number of records
// Unload the ndx_array into the index file by traversing the BST in pre-order mode (overwrite the entire index file)
// Think why should it NOT be in-order?
// Close the index file and data file
int pds_close()
{
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
		return PDS_REPO_NOT_OPEN;
	char *repo_name_index = malloc(sizeof(char) * 30);
	strcpy(repo_name_index, repo_handle.pds_name);
	strcat(repo_name_index, ".ndx");
	repo_handle.pds_ndx_fp = fopen(repo_name_index, "wb");
	if (repo_handle.pds_ndx_fp == NULL)
	{
		return PDS_FILE_ERROR;
	}
	if (!Preorder(repo_handle.ndx_root))
		return PDS_NDX_SAVE_FAILED;
	bst_destroy(repo_handle.ndx_root);
	fclose(repo_handle.pds_ndx_fp);
	fclose(repo_handle.pds_data_fp);
	if (repo_handle.pds_linked_data_fp != NULL){
		fseek(repo_handle.pds_linked_data_fp, 0, SEEK_SET);
		fclose(repo_handle.pds_linked_data_fp);
	}
	if (repo_handle.pds_link_fp != NULL){
		fseek(repo_handle.pds_link_fp, 0, SEEK_SET);
		fclose(repo_handle.pds_link_fp);
	}
	repo_handle.repo_status = PDS_REPO_CLOSED;
	return PDS_SUCCESS;
}

int delete_rec_by_ndx_key(int key) // New Function
{
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
		return PDS_REPO_NOT_OPEN;
	struct BST_Node *bstSearchResult = bst_search(repo_handle.ndx_root, key);
	if (!bstSearchResult)
		return PDS_DELETE_FAILED;
	struct PDS_NdxInfo *ndxInfo = bstSearchResult->data;
	if (!ndxInfo || ndxInfo->is_deleted == 1) 
		return PDS_DELETE_FAILED;
	ndxInfo->is_deleted = 1;
	return PDS_SUCCESS;
}

// pds_link_rec - NEW
// Create PDS_link_info instance based on key1 and key2
// Go to the end of the link file
// Store the PDS_link_info record
int pds_link_rec (int key1, int key2){
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
		return PDS_REPO_NOT_OPEN;
	struct PDS_link_info *link_info = (struct PDS_link_info *)malloc(sizeof(struct PDS_link_info));
	link_info->parent_key = key1;
	link_info->child_key = key2;
	//check for duplicate link 
	fseek(repo_handle.pds_link_fp, 0, SEEK_SET);
	struct PDS_link_info *link_info_check = (struct PDS_link_info *)malloc(sizeof(struct PDS_link_info));
	while (fread(link_info_check, sizeof(struct PDS_link_info), 1, repo_handle.pds_link_fp) == 1)
	{
		if (link_info_check->parent_key == key1 && link_info_check->child_key == key2)
		{
			return PDS_ADD_FAILED;
		}
	}
	fseek(repo_handle.pds_link_fp, 0, SEEK_END);
	if (fwrite(link_info, sizeof(struct PDS_link_info), 1, repo_handle.pds_link_fp) != 1)
		return PDS_ADD_FAILED;
	return PDS_SUCCESS;
}


// pds_get_linked_rec
// Go to the beginning of the link file
// Reset result_set_size to 0
// Do a linear search of all link_info records for matching the given parent_key
// Store the matching linked key in linked_keys_result array
// Keep updating the result_set_size
int pds_get_linked_rec(int parent_key, int linked_keys_result[], int * result_set_size){
	if (repo_handle.repo_status == PDS_REPO_CLOSED)
		return PDS_REPO_NOT_OPEN;
	*result_set_size = 0;
	fseek(repo_handle.pds_link_fp, 0, SEEK_SET);
	struct PDS_link_info *link_info = (struct PDS_link_info *)malloc(sizeof(struct PDS_link_info));
	while (fread(link_info, sizeof(struct PDS_link_info), 1, repo_handle.pds_link_fp) == 1)
	{
		if (link_info->parent_key == parent_key)
		{
			linked_keys_result[*result_set_size] = link_info->child_key;
			(*result_set_size)++;
		}
	}
	return PDS_SUCCESS;
}