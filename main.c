/* main.c - main function file
 * Copyright (C) 2014 Adrian Alonso <aalonso00@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with self library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>

#define   CHAR_LIMIT   16u    //Used for limit on char read  

int
main(int argc, char **argv)
{
    int fd;
    char *file = NULL;
    unsigned char buff[16];

    /*Extra variables*/
    unsigned int rub_Offset = 0u;       //Offset to read the bytes
    unsigned int rub_index;             //Index to print the string
    struct stat S_FILE_INFO;            //Structure to determine size of file
    size_t* rub_length;		        //Contains size of the file
    unsigned long rub_Counter = 0u;     //Counter for bytes printed

    if(argc != 2)
    {
        printf ("Usage: %s <file>\n", argv[0]);
        exit (1);
    }

    file = argv[1];

    fd = open(file, O_RDONLY);

    if(fd < 0)
    {
        printf ("Error");
        exit(1);
    }

    /* Print complete file content following od hex format */
    /* od -A x -t x1z -v file.dat */

    /*Obtain file stats*/
    fstat(fd,&S_FILE_INFO);
    *rub_length = S_FILE_INFO.st_size;		//Pass lenght of file

    /*Print until counter will reach file size*/
    while(rub_Offset < *rub_length)
    {
	    /*Read 16 symbols and save into buffer*/
	    pread(fd,buff,CHAR_LIMIT,rub_Offset);
	    /*Print actual start address*/
	    printf("%06x ",rub_Offset);

	    /*Read file and save 16 bytes by time in the buffer*/
	    for(rub_index = 0; rub_index < CHAR_LIMIT; rub_index++)
	    {
		/*If the symbols finished, finish the bucle*/
		if(rub_Counter >= *rub_length)
		{
			/*Print empty space*/
			printf("   ");
			/*Clear buffer position*/
			buff[rub_index] = ' ';
		}

		/*If not, print the buffer*/
		else
		{
			/*Print the hexa converted symbols*/
			printf("%02x ",buff[rub_index]);

			/*Check if symbol is next line*/
			if(buff[rub_index] == 0x0A)
			{
				/*Print . instead on next line*/
				buff[rub_index] = '.';
			}

			/*Increase Counter*/
			rub_Counter++;
		}
	    }

	    /*Print the saved string*/
	    printf(">%s<\n",buff);
	    /*Increase the offset value to read the next line*/
	    rub_Offset = rub_Offset + CHAR_LIMIT;
    }

    close (fd);

    return 0;
}
