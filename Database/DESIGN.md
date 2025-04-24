## Design Document

 * Nathan Yee
 * December 4, 2024

## Introduction
In this design document I will plan out the implementation for the Business Customer Database along with its functions

## File Structure
For this project, I will have a main file called "database" that the user can run in order to interact with the customers.tsv file. In this main file, I will include a header file named "customer_lib.h" that will contain the function declarations for the functions defined in the library file "customer_lib.c". The header file will also contain the struct definitions that I will need to build the hash table and linked lists. I will use a makefile in order to combine and update these files for compilation. 

## Function 1: Add
When the user calls the add funciton, I will take the hash of the email address they input, along with the name, shoe size, and favorite food. I will iterate through the bucket to see if a node with that email address. If there is an existing node, I will simply update the information. If the node does not already exist, I will add a new node onto the linked list contained in that bucket. 

## Function 2: Lookup
For this function I will take the hash of the email that the user is trying to lookup. If a node for that email already exists in the specific bucket, I will display the information contained in that node. If there is no matching node found, I will display a message saying that the lookup was unsuccessful. 

## Function 3: Delete
I will simply use the email to find the bucket the node exists in and delete it, freeing the associated memory. I will update the linked list pointers as well. If the node is not found, I will display an error message. 

## Function 4: List
I will do a linear scan of the hashtable, listing all the information found within it. 

## Function 5: Save 
After opening the tsv file for writing, I will iterate through the hash table and write in every node that is found into the tsv file. 

## Function 6: Quit
I will free all the memory used for the hash table and its associated buckets and linked lists and exit the program. 
