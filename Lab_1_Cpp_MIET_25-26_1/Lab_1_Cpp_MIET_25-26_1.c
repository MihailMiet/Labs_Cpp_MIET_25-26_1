#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person {
	char* name;
	char* fam;
	int status;
} PERSON;

void mem_alloc(PERSON** persons, int* pers_vol) {
	persons = (PERSON**)realloc(persons, (*pers_vol + 1) * sizeof(PERSON*));
	(persons[*pers_vol]->name) = (char*)malloc(80 * sizeof(char));
	(persons[*pers_vol]->fam) = (char*)malloc(80 * sizeof(char));
	(persons[*pers_vol]->status) = (int*)malloc(sizeof(int));
}

void add_pers(PERSON** persons, int* pers_vol) {
	mem_alloc(persons, pers_vol);
	printf("First name: ");
	scanf_s("%s", persons[*pers_vol]->name, 80);
	printf("\n");
	printf("Last name: ");
	scanf_s("%s", persons[*pers_vol]->fam, 80);
	printf("\n");
	(*pers_vol)++;
}

void remove_pers(PERSON** persons, int* pers_vol) {
	int cur_pers = 0, matching_pers = 0, needed_pers = 0, matching_pers_vol = 0;
	PERSON** matching_pers = (PERSON**)malloc(sizeof(PERSON*));
	PERSON* cur_pers_buffer = (PERSON*)malloc(sizeof(PERSON));
	char* var = (char*)malloc(80);
	printf("First name or Last name or Degree: ");
	scanf_s("%s", var, 80);
	printf("\n");
	printf("          First name        Last name        Degree\n\n");
	while (cur_pers < *pers_vol) {
		if (strcmp(persons[cur_pers]->name, var) == 0 || strcmp(persons[cur_pers]->fam, var) == 0 || persons[cur_pers]->status == strtod(var, NULL)) {
			matching_pers = (PERSON**)realloc(persons, (matching_pers_vol + 1) * sizeof(PERSON*));
		}
	}
}
int main(void) {
	int exit_flag = 0, option, pers_vol = 0;
	PERSON** persons = (PERSON**)malloc(sizeof(PERSON*));
	while (exit_flag == 0) {
		system("cls");
		printf("\n\n\tAdd a new person ................ 1\n\n\tRemove a person ................ 2\n\n\t"
			"List all staff ................ 3\n\n\tSearch for a person (name) ................ 4\n\n\t"
			"Search for a person (degree) ................ 5\n\n\tExit ................................ 6\n\n");
		scanf_s("%d", &option);
		system("cls");
		printf("\n");
		switch (option) {
		case 1: add_pers(persons, &pers_vol); break;
		case 2: remove_pers(persons, &pers_vol); break;
		case 3: list_pers(persons, &pers_vol); break;
		case 4: search_pers_name(persons, &pers_vol); break;
		case 5: search_pers_deg(persons, &pers_vol); break;
		default: printf("Invalid option\n\n"); break;
		}
	}
}
