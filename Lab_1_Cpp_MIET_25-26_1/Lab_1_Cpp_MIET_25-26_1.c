#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person {
	char* name;
	char* fam;
	int* status;
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
	scanf("%s", persons[*pers_vol]->name);
	printf("\n");
	printf("Last name: ");
	scanf("%s", persons[*pers_vol]->fam);
	printf("\n");
	(*pers_vol)++;
}

void remove_pers(PERSON** persons, int* pers_vol) {
	int cur_pers = 0, matching_pers_counter = 0, needed_pers = 0, matching_pers_vol = 0;
	PERSON** matching_pers = (PERSON**)malloc(sizeof(PERSON*));
	PERSON* pers_void_plug = (PERSON*)malloc(sizeof(PERSON));
	char* var = (char*)malloc(80);
	printf("First name or Last name or Degree: ");
	scanf("%s", var);
	printf("\n");
	printf("          First name        Last name        Degree\n\n");
	while (cur_pers < *pers_vol) {
		if (strcmp(*(persons[cur_pers]->name), var) == 0 || strcmp(*(persons[cur_pers]->fam), var) == 0 || *(persons[cur_pers]->status) == strtod(var, NULL)) {
			matching_pers = (PERSON**)realloc(persons, (matching_pers_vol + 1) * sizeof(PERSON*));
			matching_pers[matching_pers_vol] = (PERSON*)malloc(sizeof(PERSON));
			matching_pers[matching_pers_vol] = persons[cur_pers];
			printf("%5d.    %8s        %11.5lf            %8.2lf\n\n", matching_pers_vol + 1, *(matching_pers[matching_pers_vol]->name),
		 	 *(matching_pers[matching_pers_vol]->fam), *(matching_pers[matching_pers_vol]->status));
			matching_pers_vol++;
		}
		cur_pers++;
	}
	cur_pers = 0;
	if (matching_pers_vol != 0) {
		printf("Enter the needed person number: ");
		scanf("%d", &needed_pers);
		while (cur_pers < matching_pers_vol) {
			if (persons[cur_pers] == matching_pers[needed_pers - 1]) {
				while (cur_pers < *pers_vol - 1) {
					persons[cur_pers] = persons[cur_pers + 1];
					persons[cur_pers + 1] = pers_void_plug;
					cur_pers++;
				}
			break;				
			}
		}
		free(pers_void_plug);
	}
	cur_pers = 0;
	while (cur_pers < *pers_vol - 1) {
		free(matching_pers[cur_pers]->name);
		free(matching_pers[cur_pers]->fam);
		free(matching_pers[cur_pers]->status);
	}
	free(matching_pers);
}
int main(void) {
	int exit_flag = 0, option, pers_vol = 1;
	PERSON** persons = (PERSON**)malloc(sizeof(PERSON*));
	while (exit_flag == 0) {
		//system("cls");
		printf("\n\n\tAdd a new person ................ 1\n\n\tRemove a person ................ 2\n\n\t"
			"List all staff ................ 3\n\n\tSearch for a person name ................ 4\n\n\t"
			"Search for a person (degree) ................ 5\n\n\tExit ................................ 6\n\n");
		scanf("%d", &option);
		//system("cls");
		printf("\n");
		switch (option) {
		case 1: add_pers(persons, &pers_vol); break;
		case 2: remove_pers(persons, &pers_vol); break;
		//case 3: list_pers(persons, &pers_vol); break;
		//case 4: search_pers_name(persons, &pers_vol); break;
		//case 5: search_pers_deg(persons, &pers_vol); break;
		case 6: exit_flag = 1; break;
		default: printf("Invalid option\n\n"); break;
		}
	}
	return 0;
}