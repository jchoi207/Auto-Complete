#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct term {
  char term[200]; //assume terms are no longer than 200;
  double weight;
} term;

int compare_terms1(const void * a, const void * b) {
  const term* p_a = (const term *)a;
  const term* p_b = (const term *)b;
  return strcmp(p_a->term, p_b->term);
}

int compare_terms2(const void * a, const void * b) {
  const term* p_a = (const term *)a;
  const term* p_b = (const term *)b;  
  if (p_b->weight > p_a->weight) {
    return 1;
  } else if (p_b->weight < p_a->weight) {
    return -1;
  } else {
    return 0;
  }
}

void read_in_terms(term **terms, int *pnterms, char *filename){
  char line[200];

  FILE *fp = fopen(filename, "r");  
  fgets(line, 200, fp);
  *pnterms = atoi(line);

  *terms = (term*)malloc(*pnterms * sizeof(term)); 

for (int i = 0; i < *pnterms; i++) {
    double population;
    char location[200];
    char pop_str[200];
    fgets(line, sizeof(line), fp);
    sscanf(line, "%lf %[^\n]s", &population, location);
  
    (*terms)[i].weight = population;
    strcpy((*terms)[i].term, location);

}

  fclose(fp);
  qsort(*terms, *pnterms, sizeof(term), compare_terms1);

}


int lowest_match(term *terms, int nterms, char* substr) {
  
  int left = 0;
  int right = nterms - 1;
  int mid = 0;

  while (left <= right) {

    mid = (left + right)/2;

    if (strcmp(terms[mid].term, substr) == 0) {
      return mid;
    } else if (strcmp(terms[mid].term, substr) < 0) {
      left = mid + 1;
    } else if (strcmp(terms[mid].term, substr) > 0) { 
      right = mid - 1;
    } 
  }
    return left;
}

int highest_match(term *terms, int nterms, char* substr) {
  int length = strlen(substr);
  
  int left = 0;
  int right = nterms - 1;
  int mid = 0;

  while (left <= right) {

    mid = (left + right)/2; 

    if (strncmp(terms[mid].term, substr, length) < 0) {
      left = mid + 1;
    } else if (strncmp(terms[mid].term, substr, length) > 0) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  return right;
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr){
  int start = lowest_match(terms, nterms, substr);
  int end = highest_match(terms, nterms, substr) + 1;
  int index = 0;
  *n_answer = (end - start);

  *answer = (term*)malloc(*n_answer * sizeof(term));

  for (int i = start; i < end; i++) {
    (*answer)[index].weight = terms[i].weight; //[0][i]
    strcpy((*answer)[index].term, terms[i].term);
    index = index + 1;
  }

  qsort(*answer, *n_answer, sizeof(term), compare_terms2);

}