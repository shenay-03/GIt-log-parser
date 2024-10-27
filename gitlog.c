// Alan Shen
// Shenay

#include "hw2.h"
#include "helpers2.h"
#include "linkedlist.h"

// Part 0 Function to implement
char* myStrCpy(char* str, char* delimiters) {
	if (str == NULL)
	{
		return NULL;
	}
	int length = 0;
	//int get_all = (delimiters == NULL);
	char* s = str;

	// checks if we reach end or delim
	while (1)
	{
		if((isin(*s, delimiters) == 1) || (*s == '\0'))
		{
			break;
		}
		else
		{
			++length;
			++s;
		}
	}

	
	void* tmp = calloc(length+2, sizeof(char));
	
	if(tmp == NULL)
	{
		return NULL;
	}
	char *new_str = (char*)tmp;
	char *new_cpy = new_str;
	// mallocs enough memory
	// else error
	
	// copies string
	char *st = str;
	//*(new_str + length) = '\0'; 
	for(int i = 0; i < length ; ++i)
	{
		*new_str = *st;
		++new_str;
		++st;
	}
	//*new_str = '\0'; 
	//for(int i = 0; i < length ; ++i)
	return new_cpy;
}

// Part 1 Functions to implement
ModFile* PutModFile(int ins, int dels, char* filename, ModFile* mf) {
    if (ins < 0 || dels < 0 || filename == NULL)
	{
		return NULL;
	}
	else if(mf == NULL)
	{	
		//printf("here\n");
		ModFile *new_file = malloc(sizeof(ModFile));
		if(new_file == NULL)
		{
			return NULL;
		}
		new_file->inserts = ins;
		new_file->deletes = dels;
		new_file->filename = myStrCpy(filename,NULL);
		

		return new_file;
	}
	else
	{
		// printf("here\n");
		// checks if names match
		if (myStrCmp(filename, mf->filename) == 0)
		{
			mf->inserts += ins;
			mf->deletes += dels;
			return mf;
		}
		else
		{
			return NULL;
		}
	}
	
}

int ModFileABC_Comparator(const void* file1, const void* file2) {
 	ModFile *f1 = (ModFile*)file1;
	ModFile *f2 = (ModFile*)file2;
	
	// DELETE
	//printf("%s, %s",f1->filename, f2->filename);

 	int n = myStrCmp(f1->filename, f2->filename);
	if (n < 0)
	{
		return -1;
	}
	else if (n == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int ModFileTotal_Comparator(const void* file1, const void* file2) {
 	ModFile *f1 = (ModFile*)file1;
	ModFile *f2 = (ModFile*)file2;
    int n = (f1->inserts + f1->deletes) - (f2->inserts + f2->deletes);
	if (n == 0)
	{
		return 0;
	}
	else if (n < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

// Part 2 Functions to implement
void ModFile_Printer(void* data, void* fp, int flag) {
	if (data == NULL || fp == NULL)
	{
		return;
	}
	ModFile *f = (ModFile*)data;
	fprintf(fp, "Total Inserts:%i\tTotal Deletes:%i\t%s\n", f->inserts, f->deletes, f->filename);
}

void ModFile_Deleter(void* data) {
	if (data == NULL)
	{
		return;
	}
	else
	{
		ModFile *f = (ModFile*)data;
		free(f->filename);
		//free(f);
	}
}

node_t* FindInList(list_t* list, void* token)  {
    if (list == NULL || token == NULL)
	{
		// printf("here\n");
		// NEED TO CHECK IF "empty token"?
		return NULL;
	}
	node_t *tmp = list->head;
	//if (tmp != NULL)
	//{
		//printf("%s, %s\n", ((ModFile*)token)->filename, ((ModFile*)(tmp->data))->filename);
	//}
	for (int i = 0; i < list->length; ++i)
	{
		// printf("%s, %s\n", ((ModFile*)token)->filename, ((ModFile*)(tmp->data))->filename);	
		if(list->comparator(token, tmp->data) == 0)
		{
			return tmp;
		}
		else
		{
			//printf("%s, %s\n", ((ModFile*)token)->filename, ((ModFile*)(tmp->data))->filename);
			tmp = tmp->next;
		}
	}
	
	return NULL;
}

void DestroyList(list_t** list) {
	if (list == NULL || *list == NULL)
	{
		return;
	}
	node_t *tmp = (*list)->head;
	node_t *cur = tmp;
	for (int i = 0; i < (*list)->length; ++i)
	{
		tmp = tmp->next;
		(*list)->deleter(cur->data);
		free(cur->data);
		free(cur);
		cur = tmp;
	}
	free(*list);
}

void ProcessModFile(FILE* fp, list_t* list, char ordering) {
	// allocates a buffer
	char *buffer = NULL;
	size_t bufsize = 0;
	
	// author line
	ModFile *modf;

	while(getline(&buffer, &bufsize, fp) != -1)
	{
		// checks if line is just newline
		int a = myStrLen(buffer);
		if(a == 1 && *buffer == '\n')
		{
			break;
		}
		else
		{
			// parse the data
			unsigned int ins = 0;
			unsigned int del = 0;
			

			// char* fname = calloc(a+1, sizeof(char));
			int pos = 0;
			
			sscanf(buffer, "%u %u %n", &ins, &del, &pos);
			char* fname = myStrCpy(buffer + pos, "\n");
			//sscanf(buffer, "%u %u %s", &ins, &del, fname);


			// TODO
			// printf("%s ", fname);
			

			ModFile *tmpfile = PutModFile(ins, del, fname, NULL);
			
			// printf("%s\n", tmpfile->filename);
			// checks if it is in the modfile
			node_t *tmp = FindInList(list, tmpfile);
			
			//free(tmpfile->filename);
			//free(tmpfile);
			if (tmp == NULL)
			{
				// TODO MAYBE CHECK IF NULL?
				modf = tmpfile;
				if(ordering == 'a')
				{
					InsertInOrder(list, modf);
				}
				else if (ordering == 'f')
				{
					InsertAtTail(list, modf);
				}
			}
			else
			{
				// printf("here\n");
				// TODO MAYBE CHECK IF NULL	
				modf = PutModFile(ins, del, fname, tmp->data);
				free(tmpfile->filename);
				free(tmpfile);
			}
			free(fname);

		}
	}
	
	free(buffer);

}


// Part 3 Functions to implement
void AuthorPrinter(void* data, void *fp, int flag) {
	if (data == NULL)
	{
		return;
	}
	else if (flag == 0)
	{
		Author *aut = (Author*)data;
		fprintf(fp, "%s <%s>,%i,%i\n", aut->fullname, aut->email, aut->commitCount, aut->modFileList->length);
	}
	else 
	{
		Author *aut = (Author*)data;
		node_t *aut_node = (aut->modFileList->head);
		fprintf(fp,"%s <%s>,%i\n", aut->fullname, aut->email, aut->commitCount);
		for (int i = 0; i < aut->modFileList->length; ++i)
		{
			fprintf(fp, "\tTotal Inserts:%u\tTotal Deletes:%u\t%s\n", 
				((ModFile*)(aut_node->data))->inserts, ((ModFile*)(aut_node->data))->deletes, ((ModFile*)(aut_node->data))->filename);

			aut_node = aut_node->next;
		}
	}
}

int AuthorEmailComparator(const void* lhs, const void* rhs)  {
    if (lhs == NULL || rhs == NULL)
	{
		return 0;
	}

	int val = myStrCmp(((Author*)lhs)->email,((Author*)rhs)->email);
	if (val < 0)
	{
		return -1;
	}
	else if (val == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int AuthorCommitComparator(const void* lhs, const void* rhs) {
    if (lhs == NULL || rhs == NULL)
	{
		return 0;
	}

	int val = ((Author*)lhs)->commitCount - ((Author*)rhs)->commitCount;
	if (val > 0)
	{
		return -1;
	}
	else if (val == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void AuthorDeleter(void* data)  {
	if (data == NULL)
	{
		return;
	}
	Author *aut = (Author*)data;
	free(aut->fullname);
	free(aut->email);
	DestroyList(&(aut->modFileList));
}

Author* CreateAuthor(char* line, long int *timestamp)  {
    // checks if line is null or no hash
	if (line == NULL || *line == ',' || *line == '\0')
	{
		return NULL;
	}
	char *buffer = NULL;
	//int name = 0;

	char *myline = myStrCpy(line, NULL);
	char *mylineHead = myline;
	buffer = myStrCpy(myline, ",");
	if (*buffer == '\0')
	{	
		free(mylineHead);
		free(buffer);
		return NULL;
	}
	free(buffer);
	
	// Finished hash looking for timestamp
	while(1)
	{
		if (*myline == '\0')
		{
			free(mylineHead);
			return NULL;
		}
		else if(*myline == ',')
		{
			myline += 1;
			break;
		}
		else
		{	
			myline +=1;
		}
	}
	// gets the timestamp
	char* tmp_time = myStrCpy(myline, ",");
	// checks if no timestamp
	if (*tmp_time == '\0')
	{
		free(mylineHead);
		free(tmp_time);
		return NULL;
	}

	*timestamp = atol(tmp_time);
	free(tmp_time);
	
	while(1)
	{
		if (*myline == '\0')
		{
			free(mylineHead);
			return NULL;
		}
		else if(*myline == ',')
		{
			myline += 1;
			break;
		}
		else
		{	
			myline +=1;
		}
	}
	//char *cur = line + name;
	//char *tmp_buf = buffer;
	// goes to next comma for the name
	char *art_name = myStrCpy(myline, ",");
	while(1)
	{
		if (*myline == '\0')
		{
			free(mylineHead);
			return NULL;
		}
		else if(*myline == ',')
		{
			myline += 1;
			break;
		}
		else
		{	
			myline +=1;
		}
	}
	// checks if there is an email
	if (*myline == ',' || *myline == '\0')
	{
		free(mylineHead);
		return NULL;	
	}
	
	char *art_email = myStrCpy(myline, ",");
	
	
	Author *new_author = malloc(sizeof(Author));
	
	//list_t *new_modf = malloc(sizeof(list_t));
	list_t *new_modf = CreateList(ModFileABC_Comparator, ModFile_Printer, ModFile_Deleter);

	new_author->fullname = art_name;
	new_author->email = art_email;
	new_author->modFileList = new_modf;
	new_author->commitCount = 1;
	
	// new_modf->head == NULL;
	
	free(mylineHead);
	return new_author;
}


// Part 4 Functions to implement
void PrintNLinkedList(list_t* list, FILE* fp, int NUM) {
	if (list == NULL || fp == NULL || NUM < 0)
	{
		return;
	}

	node_t *h = list->head;
	
	unsigned i = 0;
	//for(unsigned i = 0; i < NUM; ++i)
	while(i < NUM || NUM == 0)
	{
		if (h == NULL)
		{
			return;
		}
		fprintf(fp, "Total Inserts:%u\tTotal Deletes:%u\t%s\n",((ModFile*)(h->data))->inserts, ((ModFile*)(h->data))->deletes, ((ModFile*)(h->data))->filename);
		h = h->next; 
		++i;
	}
}





