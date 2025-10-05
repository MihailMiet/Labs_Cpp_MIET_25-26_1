#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person {
	char* name;
	char* fam;
	int* status;
} PERSON;

void mem_alloc(PERSON** pers_arr, int* pers_vol) {
	pers_arr[*pers_vol] = (PERSON*)malloc(sizeof(PERSON));
	pers_arr[*pers_vol]->name = (char*)malloc(30 * sizeof(char));
	pers_arr[*pers_vol]->fam = (char*)malloc(30 * sizeof(char));
	pers_arr[*pers_vol]->status = (int*)malloc(sizeof(int));
}

void mem_clear(PERSON** pers_arr, int* pers_vol) {
	int cur_pers_ind = 0;
	if (*pers_vol == 0) { return; }
	while (cur_pers_ind < *pers_vol - 1) {
		free(pers_arr[cur_pers_ind]->name);
		free(pers_arr[cur_pers_ind]->fam);
		free(pers_arr[cur_pers_ind]->status);
		cur_pers_ind++;
	}
	free(pers_arr);
}

void push(PERSON** persons, int* pers_vol) {
	int cur_bytes_vol = 0, cur_pers_ind = 0;
	char* byte_pointer;
	FILE* fp = fopen("persons_data.dat", "wb");
	if (!fp) { return; }
	while (cur_pers_ind < *pers_vol) {
		byte_pointer = persons[cur_pers_ind]->name;
		while (cur_bytes_vol < sizeof(*(persons[cur_pers_ind]->name))) {
			putc(*byte_pointer++, fp);
			cur_bytes_vol++;
		}
		cur_bytes_vol = 0;
		byte_pointer = persons[cur_pers_ind]->fam;
		while (cur_bytes_vol < sizeof(*(persons[cur_pers_ind]->fam))) {
			putc(*byte_pointer++, fp);
			cur_bytes_vol++;
		}
		cur_bytes_vol = 0;
		byte_pointer = persons[cur_pers_ind]->status;
		while (cur_bytes_vol < sizeof(int)) {
			putc(*byte_pointer++, fp);
			cur_bytes_vol++;
		}
		cur_bytes_vol = 0;
		cur_pers_ind++;
	}
	fclose(fp);
	*pers_vol = cur_pers_ind;
}

long long int get_file_length() {
	long long int file_size = 0;
	FILE* fp = fopen("persons_data.dat", "rb");
	if (!fp) { return; }
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);
	fclose(fp);
	return file_size;
}

void pull(PERSON*** persons, int* pers_vol) {
	int byte_data = 0, cur_bytes_vol = 0, cur_pers_ind = 0;
	long long int file_size = 0;
	file_size = get_file_length() * 4;
	FILE* fp = fopen("persons_data.dat", "rb");
	if (!fp) { return; }
	*pers_vol = (int)(file_size / sizeof(PERSON));
	//free(*persons);
	*persons = (PERSON**)realloc(*persons, file_size / sizeof(PERSON) * sizeof(PERSON*));
	char* byte_pointer = NULL;
	do {
		mem_alloc(*persons, &cur_pers_ind);
		byte_pointer = (*persons)[cur_pers_ind]->name;
		while (cur_bytes_vol < sizeof(*((*persons)[cur_pers_ind]->name))) {
			*byte_pointer++ = getc(fp);
			cur_bytes_vol++;
		}
		cur_bytes_vol = 0;
		byte_pointer = (*persons)[cur_pers_ind]->fam;
		while (cur_bytes_vol < sizeof(*((*persons)[cur_pers_ind]->fam))) {
			*byte_pointer++ = getc(fp);
			cur_bytes_vol++;
		}
		cur_bytes_vol = 0;
		byte_pointer = (*persons)[cur_pers_ind]->status;
		while (cur_bytes_vol < sizeof(int)) {
			*byte_pointer++ = getc(fp);
			cur_bytes_vol++;
		}
		cur_bytes_vol = 0;
		cur_pers_ind++;
	} while (cur_pers_ind < (file_size / sizeof(PERSON) * sizeof(PERSON*)));
	fclose(fp);
	*pers_vol = cur_pers_ind;
}

void add_pers(PERSON** persons, int* pers_vol) {
	char** name_2adr = &(persons[*pers_vol]->name), ** fam_2adr = &(persons[*pers_vol]->name);
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
	printf("%ud", sizeof(*(persons[*pers_vol]->name)));
	*name_2adr = (char*)realloc(persons[*pers_vol]->name, sizeof(*(persons[*pers_vol]->name)));
	persons[*pers_vol]->name = *name_2adr;
	*fam_2adr = (char*)realloc(persons[*pers_vol]->fam, sizeof(*(persons[*pers_vol]->fam)));
	persons[*pers_vol]->fam = *fam_2adr;
	(*pers_vol)++;
}

void remove_pers(PERSON** persons, int* pers_vol) {
	int cur_pers_ind = 0, needed_pers = 0, matching_pers_vol = 0;
	PERSON*** matching_pers = (PERSON***)malloc(sizeof(PERSON**));
	*matching_pers = (PERSON**)malloc(sizeof(PERSON*));
	PERSON* pers_void_plug = (PERSON*)malloc(sizeof(PERSON));
	char* var = (char*)malloc(80);
	printf("First name or Last name or Status: ");
	scanf_s("%s", var, 80);
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
		matching_pers = (PERSON**)realloc(*matching_pers, (matching_pers_vol + 1) * sizeof(PERSON*));
		cur_pers_ind++;
	}
	cur_pers_ind = 0;
	if (matching_pers_vol != 0) {
		printf("Enter the needed person number: ");
		scanf_s("%d", &needed_pers);
		while (cur_pers_ind < matching_pers_vol) {
			if (persons[cur_pers_ind] == (*matching_pers)[needed_pers - 1]) {
				while (cur_pers_ind < *pers_vol - 1) {
					persons[cur_pers_ind] = persons[cur_pers_ind + 1];
					persons[cur_pers_ind + 1] = pers_void_plug;
					cur_pers_ind++;
				}
			break;				
			}
		}
		//mem_clear(persons[cur_pers_ind + 1], pers_vol);
	}
	mem_clear(matching_pers, pers_vol);
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
	char* var = (char*)malloc(80);
	printf("First name or Last name or Status: ");
	scanf_s("%s", var, 80);
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
}

void search_pers_status(PERSON** persons, int* pers_vol) {
	int cur_pers_ind = 0, needed_pers = 0, matching_pers_vol = 0;
	PERSON** matching_pers = (PERSON**)malloc(sizeof(PERSON*));
	int var = 0;
	printf("Status (1/2/3): ");
	scanf_s("%d", var);
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
}

int main(void) {
	int exit_flag = 0, option = 0, pers_ind = 0, pers_vol = 0;
	PERSON*** persons = (PERSON***)malloc(sizeof(PERSON**));
	*persons = (PERSON**)malloc(2 * sizeof(PERSON*));
	pull(persons, &pers_ind);
	while (exit_flag == 0) {
		system("cls");
		printf("\n\n\tAdd a new person ................ 1\n\n\tRemove a person ................ 2\n\n\t"
			"List all staff ................ 3\n\n\tSearch for a person (name) ................ 4\n\n\t"
			"Search for a person (status) ................ 5\n\n\tExit ................................ 6\n\n");
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
		default: printf("Invalid option\n\n"); break;
		}
		persons = (PERSON**)realloc(persons, (pers_ind + 1) * sizeof(PERSON*));
		push((*persons), &pers_ind);
	}
	mem_clear(persons, &pers_ind);
	return 0;
}