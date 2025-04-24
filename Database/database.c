#include "customers_lib.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_help(void) {
  printf("valid commands are...\n");
  printf("- help: display this message\n");
  printf("- list: list all customers in database\n");
  printf("- lookup: lookup a specfic customer in database\n");
  printf("- add: add a customer to the database\n");
  printf("- delete: delete a customer from the database\n");
  printf("- save: write current database to file\n");
  printf("- quit: exit the program\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "USAGE: database <filename>\n");
    return 0;
  }

  const char *filename = argv[1];

  printf("**************************************\n");
  printf("* Editing Customer Database  *\n");
  printf("**************************************\n");
  printf("editing %s\n", filename);

  const int BUFSIZE = 1024;
  char buf[BUFSIZE];
  char *result;

  customer_table *table = make_table(1000);
  if (access(filename, F_OK) == 0) {
    load_table(table, filename);
    printf("loaded %s\n", filename);
  }

 
  while (true) {
    printf("command (type help for list of commands): ");
    result = fgets(buf, BUFSIZE, stdin);
    if (!result) {
      break; 
    }
    chomp(result);

    if (!strcmp(buf, "quit") || !strcmp(buf, "q")) {
      break;
    } else if (!strcmp(buf, "help") || !strcmp(buf, "?")) {
      print_help(); 
    } else if (!strcmp(buf, "list")) {
      list_customers(table); 
    } else if (!strcmp(buf, "save")) {
      save_to_file(table, filename);
    } else if (!strcmp(buf, "lookup")) {
      printf("Type the email of the customer you are looking for: \n");
      result = fgets(buf, BUFSIZE, stdin);
      chomp(result);

      ll_customers *temp = lookup_customer(table, result);
      if (temp) {
        printf("\n");
        printf("Printing customer information: \n");
        display_customer(temp);
      } else {
        printf("Customer not found.\n"); 
      }
    } else if (!strcmp(buf, "add")) {
      printf("Type customer email: ");
      char *email = strdup(fgets(buf, BUFSIZE, stdin));
      chomp(email);
  
      printf("Type customer name: ");
      char *name = strdup(fgets(buf, BUFSIZE, stdin));
      chomp(name);
   
      printf("Type customer shoe size: ");
      result = fgets(buf, BUFSIZE, stdin);
      chomp(result);
      int size = atoi(result);

      printf("Type customer favorite food: ");
      char *food = strdup(fgets(buf, BUFSIZE, stdin));
      chomp(food);

      add_customer(table, email, name, size, food);
      printf("Customer successfully added!\n"); 
      free(email);
      free(name);
      free(food);
    } else if (!strcmp(buf, "delete")) {
      printf("Type email of customer you wish to delete: ");
      result = fgets(buf, BUFSIZE, stdin);
      chomp(result);
    
      bool successful = delete_customer(table, result);
      if (successful) {
        printf("Successfully deleted customer\n");
      } else {
        printf("Customer was not found\n");
      } 
    }

    else{
      printf("Unknown command: %s\n", buf);
    }
    
  }

  quit_program(table);
  printf("Quitting program! Bye Bye!\n");
  return 0;
  
}
