#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person {
	char* name;
	char* fam;
	int* name_len;
	int* fam_len;
	int* status;
} PERSON;

void mem_alloc(PERSON** pers_arr, int* pers_vol) {
	pers_arr[*pers_vol] = (PERSON*)malloc(sizeof(PERSON));
	pers_arr[*pers_vol]->name = (char*)malloc(30 * sizeof(char));
	pers_arr[*pers_vol]->fam = (char*)malloc(30 * sizeof(char));
	pers_arr[*pers_vol]->name_len = (int*)malloc(sizeof(int));
	pers_arr[*pers_vol]->fam_len = (int*)malloc(sizeof(int));
	pers_arr[*pers_vol]->status = (int*)malloc(sizeof(int));
}

void mem_clear(PERSON** pers_arr, int* pers_vol) {
	int cur_pers_ind = 0;
	if (*pers_vol == 0) { return; }
	while (cur_pers_ind < *pers_vol) {
		free(pers_arr[cur_pers_ind]->name);
		free(pers_arr[cur_pers_ind]->fam);
		free(pers_arr[cur_pers_ind]->name_len);
		free(pers_arr[cur_pers_ind]->fam_len);
		free(pers_arr[cur_pers_ind]->status);
		free(pers_arr[cur_pers_ind]);
		cur_pers_ind++;
	}
	free(pers_arr);
}

void push(PERSON** persons, int* pers_vol) {
	int cur_pers_ind = 0, * struct_size;
	FILE* fp = fopen("persons_data.bin", "wb");
	if (!fp) { return; }
	if (*pers_vol == 0) { fclose(fp); return; }
	fwrite(pers_vol, sizeof(int), 1, fp);
	while (cur_pers_ind < *pers_vol) {
		fwrite(persons[cur_pers_ind]->name_len, sizeof(int), 1, fp);
		fwrite(persons[cur_pers_ind]->fam_len, sizeof(int), 1, fp);
		fwrite(persons[cur_pers_ind]->name, sizeof(char), strlen(persons[cur_pers_ind]->name) + 1, fp);
		fwrite(persons[cur_pers_ind]->fam, sizeof(char), strlen(persons[cur_pers_ind]->fam) + 1, fp);
		fwrite(persons[cur_pers_ind]->status, sizeof(int), 1, fp);
		cur_pers_ind++;
	}
	fclose(fp);
}

void pull(PERSON*** persons, int* pers_vol) {
	int cur_pers_ind = 0;
	FILE* fp = fopen("persons_data.bin", "rb");
	if (!fp) { return; }
	fread(pers_vol, sizeof(int), 1, fp);
	if (*pers_vol == 0) { fclose(fp); return; }
	*persons = (PERSON**)realloc(*persons , *pers_vol * sizeof(PERSON*));
	while (cur_pers_ind < *pers_vol) {
		mem_alloc(*persons, &cur_pers_ind);
		fread((*persons)[cur_pers_ind]->name_len, sizeof(int), 1, fp);
		fread((*persons)[cur_pers_ind]->fam_len, sizeof(int), 1, fp);
		fread((*persons)[cur_pers_ind]->name, sizeof(char), *((*persons)[cur_pers_ind]->name_len) + 1, fp);
		fread((*persons)[cur_pers_ind]->fam, sizeof(char), *((*persons)[cur_pers_ind]->fam_len) + 1, fp);
		fread((*persons)[cur_pers_ind]->status, sizeof(int), 1, fp);
		cur_pers_ind++;
	}
	fclose(fp);
}

void add_pers(PERSON** persons, int* pers_vol) {
	mem_alloc(persons, pers_vol);
	printf("First name: ");
	scanf_s("%s", persons[*pers_vol]->name, 30);
	printf("\n");
	printf("Last name: ");
	scanf_s("%s", persons[*pers_vol]->fam, 30);
	printf("\n");
	printf("Status: ");
	scanf_s("%d", persons[*pers_vol]->status);
	printf("\n");
	*(persons[*pers_vol]->name_len) = strlen(persons[*pers_vol]->name);
	*(persons[*pers_vol]->fam_len) = strlen(persons[*pers_vol]->fam);
	(*pers_vol)++;
}

void remove_pers(PERSON** persons, int* pers_vol) {
	int cur_pers_ind = 0, needed_pers = 0, matching_pers_vol = 0, plug_vol = 1;
	PERSON*** matching_pers = (PERSON***)malloc(sizeof(PERSON**));
	*matching_pers = (PERSON**)malloc(sizeof(PERSON*));
	PERSON* pers_void_plug = (PERSON*)malloc(sizeof(PERSON));
	pers_void_plug->name = (char*)malloc(1);
	pers_void_plug->fam = (char*)malloc(1);
	pers_void_plug->name_len = (char*)malloc(4);
	pers_void_plug->fam_len = (char*)malloc(4);
	pers_void_plug->status = (char*)malloc(4);
	char* var = (char*)malloc(30);
	printf("First name or Last name or Status: ");
	scanf_s("%s", var, 30);
	printf("\n\n");
	printf("%*s", 4, "");
	printf("Pers");
	printf("%*s", 4, "");
	printf("First name");
	printf("%*s", 24, "");
	printf("Last name");
	printf("%*s", 25, "");
	printf("Status\n\n");
	while (cur_pers_ind < *pers_vol) {
		if (strcmp(persons[cur_pers_ind]->name, var) == 0 || strcmp(persons[cur_pers_ind]->fam, var) == 0 || *(persons[cur_pers_ind]->status) == atoi(var)) {
			mem_alloc(*matching_pers, &matching_pers_vol);
			(*matching_pers)[matching_pers_vol] = persons[cur_pers_ind];
			printf("%*s", 4, "");
			printf("%3d.", matching_pers_vol + 1);
			printf("%*s", 4, "");
			printf("%-30s", (*matching_pers)[matching_pers_vol]->name);
			printf("%*s", 4, "");
			printf("%-30s", (*matching_pers)[matching_pers_vol]->fam);
			printf("%*s", 4, "");
			printf("%-30d\n\n", *((*matching_pers)[matching_pers_vol]->status));
			matching_pers_vol++;
		}
		*matching_pers = (PERSON**)realloc(*matching_pers, (matching_pers_vol + 1) * sizeof(PERSON*));
		cur_pers_ind++;
	}
	cur_pers_ind = 0;
	if (matching_pers_vol != 0) {
		printf("Enter the needed person number (Press 0 to exit): ");
		scanf_s("%d", &needed_pers);
		if (invalid_or_escape_option_loop(needed_pers, *pers_vol)) { return; }
		while (cur_pers_ind < *pers_vol) {
			if (persons[cur_pers_ind] == (*matching_pers)[needed_pers - 1]) {
				while (cur_pers_ind < *pers_vol - 1) {
					persons[cur_pers_ind] = persons[cur_pers_ind + 1];
					persons[cur_pers_ind + 1] = pers_void_plug;
					cur_pers_ind++;
					printf("%d", cur_pers_ind);
				}
				free(pers_void_plug->name);
				free(pers_void_plug->fam);
				free(pers_void_plug);
				break;				
			}
			cur_pers_ind++;
		}
	}
	mem_clear(*matching_pers, &matching_pers_vol);
	free(matching_pers);
	free(var);
	(*pers_vol)--;
}

void list_pers(PERSON** persons, int* pers_vol) {
	int cur_pers_ind = 0;
	char ch[4];
	printf("\n\n");
	printf("%*s", 4, "");
	printf("Pers");
	printf("%*s", 4, "");
	printf("First name");
	printf("%*s", 24, "");
	printf("Last name");
	printf("%*s", 25, "");
	printf("Status\n\n");
	while (cur_pers_ind < *pers_vol) {
		printf("%*s", 4, "");
		printf("%3d.", cur_pers_ind + 1);
		printf("%*s", 4, "");
		printf("%-30s", persons[cur_pers_ind]->name);
		printf("%*s", 4, "");
		printf("%-30s", persons[cur_pers_ind]->fam);
		printf("%*s", 4, "");
		printf("%-30d\n\n", *(persons[cur_pers_ind]->status));
		cur_pers_ind++;
	}
	printf("Press any key to continue: ");
	scanf_s("%s", &ch, 4);
}

void search_pers_name(PERSON** persons, int* pers_vol) {
	int cur_pers_ind = 0, needed_pers = 0, matching_pers_vol = 0;
	PERSON** matching_pers = (PERSON**)malloc(sizeof(PERSON*));
	char* var = (char*)malloc(30);
	char ch[4];
	printf("First name: ");
	scanf_s("%s", var, 30);
	if (invalid_or_escape_option_loop(strlen(var), 30)) {
		printf("Invalid option. Please try again: ");
	}
	printf("\n\n");
	printf("%*s", 4, "");
	printf("Pers");
	printf("%*s", 4, "");
	printf("First name");
	printf("%*s", 24, "");
	printf("Last name");
	printf("%*s", 25, "");
	printf("Status\n\n");
	while (cur_pers_ind < *pers_vol) {
		if (strcmp(persons[cur_pers_ind]->name, var) == 0) {
			printf("%*s", 4, "");
			printf("%3d.", cur_pers_ind + 1);
			printf("%*s", 4, "");
			printf("%-30s", persons[cur_pers_ind]->name);
			printf("%*s", 4, "");
			printf("%-30s", persons[cur_pers_ind]->fam);
			printf("%*s", 4, "");
			printf("%-30d\n\n", *(persons[cur_pers_ind]->status));
		}
		cur_pers_ind++;
	}
	free(var);
	printf("Press any key to continue: ");
	scanf_s("%s", &ch, 4);
}

void search_pers_status(PERSON** persons, int* pers_vol) {
	int cur_pers_ind = 0, needed_pers = 0, matching_pers_vol = 0;
	PERSON** matching_pers = (PERSON**)malloc(sizeof(PERSON*));
	int var = 0;
	char ch[4];
	printf("Status (1/2/3): ");
	scanf_s("%d", &var);
	if (invalid_or_escape_option_loop(var, 3)) { return; }
	printf("\n\n");
	printf("%*s", 4, "");
	printf("Pers");
	printf("%*s", 4, "");
	printf("First name");
	printf("%*s", 24, "");
	printf("Last name");
	printf("%*s", 25, "");
	printf("Status\n\n");
	while (cur_pers_ind < *pers_vol) {
		if (*(persons[cur_pers_ind]->status) == var) {
			printf("%*s", 4, "");
			printf("%3d.", cur_pers_ind + 1);
			printf("%*s", 4, "");
			printf("%-30s", persons[cur_pers_ind]->name);
			printf("%*s", 4, "");
			printf("%-30s", persons[cur_pers_ind]->fam);
			printf("%*s", 4, "");
			printf("%-30d\n\n", *(persons[cur_pers_ind]->status));
		}
		cur_pers_ind++;
	}
	printf("Press any key to continue: ");
	scanf_s("%s", &ch, 4);
}

int invalid_or_escape_option_loop(int option, int limit) {
	while (1) {
		if (option < 0 || option > limit) {
			printf("Invalid option. Please try again: ");
			scanf_s("%d", &option);
		}
		else if (option == 0) { return 1; }
		else { return 0; }
	}
}

int main(void) {
	int exit_flag = 0, option = 0, pers_ind = 0;
	PERSON*** persons = (PERSON***)malloc(sizeof(PERSON**));
	*persons = (PERSON**)malloc(sizeof(PERSON*));
	pull(persons, &pers_ind);
	while (exit_flag == 0) {
		system("cls");
		printf("\n\n\tAdd a new person ............................ 1");
		printf("\n\n\tRemove a person ........................... 2");
		printf("\n\n\tList all persons ........................ 3"); 
		printf("\n\n\tSearch for a person(name) ............. 4");
		printf("\n\n\tSearch for a person (status) ........ 5");
		printf("\n\n\tExit .............................. 6\n\n");
		printf("\n\n\n\tOption: ");
		scanf_s("%d", &option);
		system("cls");
		printf("\n");
		switch (option) {
			case 1: add_pers((*persons), &pers_ind); break;
			case 2: remove_pers((*persons), &pers_ind); break;
			case 3: list_pers((*persons), &pers_ind); break;
			case 4: search_pers_name((*persons), &pers_ind); break;
			case 5: search_pers_status((*persons), &pers_ind); break;
			case 6: exit_flag = 1; break;
			default: if (invalid_or_escape_option_loop(option, 6)) { break; }
		}
		if (pers_ind != 0) { *persons = (PERSON**)realloc(*persons, pers_ind * sizeof(PERSON*)); }
		push((*persons), &pers_ind);
	}
	if (pers_ind != 0) {
		mem_clear(*persons, &pers_ind);
		free(persons);
	}
	return 0;
}