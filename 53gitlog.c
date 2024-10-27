#include "constants2.h"
#include "helpers2.h"
#include "hw2.h"
#include <time.h>

int main(int argc, char* argv[]) {
	int D_flag = 0;
	int A_flag = 0;
    int NUM_arg = 0;
    int LEVEL_arg = 0;
    char ORDER_arg = 'n';   // default no order
    char* DATE_arg = NULL;
    char* OUTFILE = NULL;
    char* INFILE = NULL;

    // Use basic getopt to parse flags and respective arguments
    int option;
    while ((option = getopt(argc, argv, "HD:A:O:I:n:ad" )) >= 0) {
        switch (option) {
            case 'H':
				fprintf(stdout, USAGE_MSG);
				return EXIT_SUCCESS;
            case 'D':
        	    D_flag = 1;
                DATE_arg = optarg;
                break;
            case 'A':
				A_flag = 1;
                LEVEL_arg = atoi(optarg);
                break;
            case 'n':
				NUM_arg = atoi(optarg);
                break;
            case 'a':
            case 'd':
				ORDER_arg = option;
                break;
            case 'O':
				OUTFILE = optarg;
                break;
            case 'I':
				INFILE = optarg;
                break;
            default:
                fprintf(stderr, USAGE_MSG);
                return EXIT_FAILURE;
        }
    }

    // validate a required option was specified - Does not check for more than 1
    if ( ! (A_flag | D_flag) )
    {
        fprintf(stderr, "ERROR: Search mode was not specified.\n\n" USAGE_MSG);
        return EXIT_FAILURE;
    }
    
    // INSERT YOUR IMPLEMENTATION HERE
    // getopts only stored the arguments and performed basic checks. More error checking is still needed!!!!
	
	// checks if both flags are tripped
	if (D_flag && A_flag)
	{
		fprintf(stderr, USAGE_MSG);
		return 1;
	}
	
	// checks if num is < 0
	if (NUM_arg < 0)
	{
		fprintf(stderr, USAGE_MSG);
		return 1;
	}


	// D argument
	if(D_flag)
	{

		// TODO free and delete when error and output

		// initalizes a buffer
		char *buffer = NULL;
		size_t len = 0;
		Author *tmp = NULL;
		// TODO
		// if(!A_flag)
		list_t *modf = CreateList(ModFileABC_Comparator, ModFile_Printer, ModFile_Deleter);
		// checks if the date matches
		struct tm t;
		time_t t_of_day;
	
		// parse DATE_arg
		int day, month, year;
		sscanf(DATE_arg, "%d/%d/%d", &day, &month, &year);
		// from https://www.epochconverter.com/programming/c
		t.tm_year = year-1900;  // Year - 1900
		t.tm_mon = month-1;           // Month, where 0 = jan
		t.tm_mday = day;          // Day of the month
		t.tm_hour = 23;
		t.tm_min = 59;
		t.tm_sec = 59;
		t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
		t_of_day = mktime(&t);

		if(t_of_day == -1 || year <= 1900 || day < 1 || day > 31 || month < 1 || month > 12)
		{
			// invalid date
			DestroyList(&modf);
			fprintf(stderr, USAGE_MSG);
			return 1;
		}
		long int timestamp = 0;
		// searches for a line with correct timestamp
		int success = 0;

		FILE *new_file = stdin;
		if (INFILE != NULL)
		{
			if ((new_file = fopen(INFILE, "r")) == NULL)
			{
				DestroyList(&modf);
				return 2;
			}
		}

		while(success = getline(&buffer, &len, new_file) != -1)	
		{
			if (has_4_commas(buffer))
			{
				tmp = CreateAuthor(buffer, &timestamp);
				AuthorDeleter(tmp);
				free(tmp);
				// CHANGE WHEN -O and -I LATER TODO
				// WHEN -a exitst change char
				// ProcessModFile(stdin, modf, 'f');
				// checks if timestamp > DATE
				if (timestamp >= t_of_day)
				{
					if(ORDER_arg == 'n')
					{
						ProcessModFile(new_file, modf, 'f');
					}
					else if (ORDER_arg == 'a')
					{
						ProcessModFile(new_file, modf, 'a');
					}
					else
					{
						fprintf(stderr, USAGE_MSG);
						if (new_file != stdin)
						{
							free(buffer);
							DestroyList(&modf);
							fclose(new_file);
						}
						return 1;
					}
					//PrintNLinkedList(modf, stdout, NUM_arg);
					
				}
				
			}

			//clear_buffer(buffer);
		}
		if (new_file != stdin)
		{
			fclose(new_file);
		}
		FILE *new_out = stdout;
		if(OUTFILE != NULL)
		{
			if((new_out = fopen(OUTFILE, "w")) == NULL)
			{
				DestroyList(&modf);
				free(buffer);
				return 2;
				
			}
		}
		PrintNLinkedList(modf, new_out, NUM_arg);
		DestroyList(&modf);
		free(buffer);
		if (new_out != stdout)
		{
			fclose(new_out);
		}
		// if nothing is found?
		if (success == -1)
		{
			fprintf(stderr, USAGE_MSG);
			return 1;
		}
		
	}
	// if A flag
	/*
	else
	{		
		if(LEVEL_arg != 0 || LEVEL_arg != 1)
		{
			fprintf(stderr, USAGE_MSG);
		}
		// initalizes a buffer
		char *buffer = NULL;
		size_t len = 0;
		Author *tmp = NULL;
		// TODO
		// if(!A_flag)
		list_t *autf = CreateList(AuthorCommitComparator, AuthorPrinter, AuthorDeleter);
		// checks if the date matches
	
		
		// searches for a line with correct timestamp
		int success = 0;

		FILE *new_file = stdin;
		if (INFILE != NULL)
		{
			if ((new_file = fopen(INFILE, "r")) == NULL)
			{
				DestroyList(&modf);
				return 2;
			}
		}

		while(success = getline(&buffer, &len, new_file) != -1)	
		{
			if (has_4_commas(buffer))
			{
				tmp = CreateAuthor(buffer, &timestamp);
				
				free(tmp);
				// CHANGE WHEN -O and -I LATER TODO
				// WHEN -a exitst change char
				// ProcessModFile(stdin, modf, 'f');
				// checks if timestamp > DATE
				
				if(LEVEL_arg == 0)
				{
					ProcessModFile(new_file, modf, 'f');
				}
				else if (ORDER_arg == 'a')
				{
					ProcessModFile(new_file, modf, 'a');
				}
				else
				{
					fprintf(stderr, USAGE_MSG);
					if (new_file != stdin)
					{
						free(buffer);
						DestroyList(&modf);
						fclose(new_file);
					}
					return 1;
				}
			//PrintNLinkedList(modf, stdout, NUM_arg);
			
				
				
			}

			//clear_buffer(buffer);
		}
		if (new_file != stdin)
		{
			fclose(new_file);
		}
		FILE *new_out = stdout;
		if(OUTFILE != NULL)
		{
			if((new_out = fopen(OUTFILE, "w")) == NULL)
			{
				DestroyList(&modf);
				free(buffer);
				return 2;
				
			}
		}
		PrintNLinkedList(modf, new_out, NUM_arg);
		DestroyList(&modf);
		free(buffer);
		if (new_out != stdout)
		{
			fclose(new_out);
		}
		// if nothing is found?
		if (success == -1)
		{
			fprintf(stderr, USAGE_MSG);
			return 1;
		}
	}
	*/
    return 0;
}


