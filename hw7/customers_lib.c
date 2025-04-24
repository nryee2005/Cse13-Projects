#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "customers_lib.h"

// Thank you Dan Bernstein.
unsigned long djb_hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while (*str != '\0') {
    c = *str;
    hash = ((hash << 5) + hash) + (unsigned char)c; /* hash * 33 + c */
    str++;
  }
  return hash;
}

//functions for ll_customers

ll_customers *add_customer_to_list(ll_customers *list, char *email, char *name, int shoe_size, char *favorite_food){
  if (lookup_on_list(list,email)) {
    update_customer(lookup_on_list(list, email), name, shoe_size, favorite_food);
    return list;  
  }

  ll_customers *new_front = calloc(1, sizeof(ll_customers));
  new_front->email = strdup(email); 
  new_front->name = strdup(name);
  new_front->shoe_size = shoe_size;
  new_front->favorite_food = strdup(favorite_food);
  new_front->next = list; 
  
  return new_front; 
}

ll_customers *delete_customer_from_list(ll_customers *list, char *email){
  
  ll_customers *temp = list;
  ll_customers *prev; 

  if (temp != NULL && !strcmp(temp->email, email)) {
    list = temp->next;
    free(temp->email);
    free(temp->name);
    free(temp->favorite_food);
    free(temp);
    return list;
  } 

  while (temp != NULL && strcmp(temp->email, email)) {
    prev = temp;
    temp = temp->next;
  } 

  if (!temp) {
    return list;
  } else {
    prev->next = temp->next;
    free(temp->email);
    free(temp->name);
    free(temp->favorite_food);
    free(temp); 
  }
  return list;
}

ll_customers *lookup_on_list(ll_customers *list, char *email){
  if (!list){
    return NULL;
  }

  if (!strcmp(list->email, email)){
    return list;
  } else {
    return lookup_on_list(list->next, email);
  } 
}
 
void update_customer(ll_customers *customer, char *name, int shoe_size, char *favorite_food){
  free(customer->name);
  free(customer->favorite_food); 

  customer->name = strdup(name);
  customer->shoe_size = shoe_size;
  customer->favorite_food = strdup(favorite_food); 
}

void display_customer(ll_customers *customer){
  if (customer) {
    printf("Email: %s\n", customer->email);
    printf("Name: %s\n", customer->name);
    printf("Shoe size: %d\n", customer->shoe_size);
    printf("Favorite food: %s\n", customer->favorite_food);
    printf("\n");
  } else {
    printf("Not valid customer\n");
  }
}

//functions for customer_table
customer_table *make_table(size_t num_buckets){ 
  customer_table *out = calloc(1, sizeof(customer_table));
  out->buckets = calloc(num_buckets, sizeof(ll_customers*));
  out->num_buckets = num_buckets;

  return out;
} 

void load_table(customer_table *table, const char *filename){
  char buf[1024]; 
  FILE *infile = fopen(filename, "r");

  int c = fgetc(infile);
  if (c == EOF) {
    return;
  } else {
    ungetc(c, infile);
  }

  while (fgets(buf, 1024, infile)) {
    int num = 0;
    char **fields = calloc(3, sizeof(char*));
    char *temp;
    int size;


    temp = strtok(buf, "\t\n");
    while (temp) {
      if (num == 2) {
        size = atoi(temp);
      } else if (num == 3) {
        fields[num-1] = strdup(temp);
      } else {
        fields[num] = strdup(temp);
      } 
      temp = strtok(NULL, "\t\n");

      num++;
    }
    add_customer(table, fields[0], fields[1], size, fields[2]);

    free(fields[0]);
    free(fields[1]);
    free(fields[2]);
    free(fields);
    free(temp);
  }
 
  fclose(infile);
}

void add_customer(customer_table *table, char *email, char *name, int shoe_size, char *favorite_food){
  unsigned long hashval = djb_hash(email);
  size_t bucket = hashval % table->num_buckets;

  table->buckets[bucket] = add_customer_to_list(table->buckets[bucket], email, name, shoe_size, favorite_food); 
}

ll_customers *lookup_customer(customer_table *table, char *email){
  unsigned long hashval = djb_hash(email);
  size_t bucket = hashval % table->num_buckets;

  return lookup_on_list(table->buckets[bucket], email);
}

bool delete_customer(customer_table *table, char *email){
  unsigned long hashval = djb_hash(email);
  size_t bucket = hashval % table->num_buckets;
  
  if (lookup_on_list(table->buckets[bucket], email)) {
    table->buckets[bucket] = delete_customer_from_list(table->buckets[bucket], email);
    return true;
  }
  
  return false;
}

void list_customers(customer_table *table){
  printf("**** List of Customers ****\n");
  printf("\n");
  int cus = 1;
  for (size_t i = 0; i<table->num_buckets; i++) {
    if (!table->buckets[i]) {
      continue;
    }
    ll_customers *temp = table->buckets[i];
    while (temp) {
      printf("Customer %d:\n", cus);
      display_customer(temp);
      cus++;
      temp = temp->next;
    }
  } 
  
  printf("**** End of List ****\n");
}

void save_to_file(customer_table *table, const char *filename){
  FILE *infile = fopen(filename, "w");
  
  for (size_t i = 0; i<table->num_buckets; i++) {
    if (!table->buckets[i]) {
      continue;
    }

    ll_customers *temp = table->buckets[i];
    while (temp) {
      fprintf(infile, "%s\t%s\t%d\t%s\n", temp->email, temp->name, temp->shoe_size, temp->favorite_food); 
      temp = temp->next;
    }
  }

  fclose(infile);
}

void quit_program(customer_table *table){
  for (size_t i = 0; i<table->num_buckets; i++) {
    ll_customers *temp = table->buckets[i];
    if (!temp) {
      free(temp);
    } else {
      while (temp) {
        ll_customers *to_free = temp;
        temp = temp->next;
        free(to_free->email);
        free(to_free->name);
        free(to_free->favorite_food);
        free(to_free); 
      }
      free(temp);
    }
  }
  free(table->buckets);
  free(table);
}

void chomp(char *s) {
  for (int i = 0; s[i]; i++) {
    if (s[i] == '\n' || s[i] == '\r') {
      // Check for \r in case we're on Windows or a very old Mac??
      // Look, I won't stop you from doing CSE 13s on a Mac from 2001 or prior.
      s[i] = '\0';
      break;
    } else if (s[i] == '\t') {
      s[i] = ' ';
    }
  }
}








