/* !*  Functions for Color Image Subtraction
 *
 * \author Sebastian Dichler <el16b032@technikum-wien.at> <sedi343@gmail.com>
 *
 */

#include "myHeader.h"

/*------------------------------------------------------------------*/
/* F U N C T I O N   D E F I N I T I O N                            */
/*------------------------------------------------------------------*/

/* ---- CLEAR FUNCTION ---- */

void clear(void)
{
#if defined(__APPLE__) && defined(__MACH__) || defined(__linux__)
    system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
    helpdesk_1();
}

/* ---- CLEAR WITH NO HELP FUNCTION ---- */

void clearNoHelp(void)
{
#if defined(__APPLE__) && defined(__MACH__) || defined(__linux__)
    system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

/* ---- HELPDESK FUNCTION ---- */

void helpdesk_1(void)
{
    printf("\n");
    printf(BOLD"IMAGE SUBTRACTION @ v1.0\n"RESET);
    printf(BOLD"Created by Sebastian Dichler, 2017\n"RESET);
    printf("Format input:\t"BOLD"subtract.out <-f FILENAME> <-s FILENAME> <-o FILENAME>\n"RESET);
    printf("Use \""BOLD"-h"RESET"\" for more information.\n\n");
    
#if DEBUG
    printf(BOLDRED ITALIC"*** DEBUG MODE ACTIVE ***\n\n"RESET);
#endif
}

void helpdesk_2(void)
{
    printf("\n");
    printf(BOLD"IMAGE SUBTRACTION @ v1.0\n"RESET);
    printf(BOLD"Created by Sebastian Dichler, 2017\n"RESET);
    printf("Format input:\t"BOLD"subtract.out <-f FILENAME> <-s FILENAME> <-o FILENAME>\n"RESET);
    printf("Picture 2 ("BOLD"<-s FILENAME>"RESET") should contain more information, specially for Algorithm "BOLD"1"RESET".\n\n");
    printf(ITALIC"Required Parameters:\n"RESET);
    printf(BOLD"<-f FILENAME>"RESET"\tIt specifies the first input file.\n");
    printf(BOLD"<-s FILENAME>"RESET"\tIt specifies the second input file.\n");
    printf(BOLD"<-o FILENAME>"RESET"\tIt specifies the output file. ("BOLD"colored"RESET")\n\n");
    printf(ITALIC"Optional Parameters:\n"RESET);
    printf(BOLD"[-n FILENAME]"RESET"\tIt specifies the second output file. ("BOLD"black"RESET")\n");
    printf(BOLD"[-a NUMBER]"RESET"\tChange output algorithm "BOLD"0"RESET" is default, "BOLD"1"RESET", "BOLD"2"RESET", "BOLD"3"RESET" or "BOLD"4"RESET"\n");
    printf("\t\tUse "BOLD"0"RESET" for changed object white(depends on RGB Input) and everything else\n\t\tcolored/black. (justcolor)\n");
    printf("\t\tUse "BOLD"1"RESET" for changed object colored and everything else white\n\t\t(depends on RGB Input)/black. (justcolor)\n");
    printf("\t\tUse "BOLD"2"RESET" for changed object white(depends on RGB Input) and everything else\n\t\tcolored/black. (fullpixel)\n");
    printf("\t\tUse "BOLD"3"RESET" for changed object white(depends on RGB Input) and everything else\n\t\tcolored/black. (Gray value unweighted)\n");
    printf("\t\tUse "BOLD"4"RESET" for changed object white(depends on RGB Input) and everything else\n\t\tcolored/black. (Gray value weighted)\n");
    printf(BOLD"[-t THRESHOLD]"RESET"\tValue between "BOLD"0"RESET" and "BOLD"255"RESET"(default, depends on maxcolor value).\n");
    printf(BOLD"[-r RED]"RESET"\tSpecify the "BOLD"RED"RESET" color value for the difference area to be filled with.\n");
    printf(BOLD"[-g GREEN]"RESET"\tSpecify the "BOLD"GREEN"RESET" color value for the difference area to be filled with.\n");
    printf(BOLD"[-b BLUE]"RESET"\tSpecify the "BOLD"BLUE"RESET" color value for the difference area to be filled with.\n");
    printf(BOLD"[-h]"RESET"\t\tPrints a help message.\n\n");
    
#if DEBUG
    printf(BOLDRED ITALIC"*** DEBUG MODE ACTIVE ***\n\n"RESET);
#endif
}

/* ---- REMOVE COMMENT FUNCTION ---- */

int removecomment(FILE *pFin, FILE *pFin2, FILE *pFout, FILE *pFout2, struct picture *Pointer_1, struct picture *Pointer_2, struct picture *Pointer_3, int second_file)
{
    char comment[STRINGLENGTH+1];
    
    fpos_t position;
    
    fgetpos(pFin, &position);
    
    fgets(comment, STRINGLENGTH, pFin);
    
    if (comment[strnlen(comment, STRINGLENGTH)-1] != '\n' && comment[0] == '#')
    {
        printf(BOLD"\nERROR: Header Comment in file to long.\n"RESET);
        
        if (Pointer_1 != NULL || Pointer_2 != NULL || Pointer_3 != NULL)
        {
            free(Pointer_1);
            free(Pointer_2);
            free(Pointer_3);
        }
        
        fclose(pFin);
        fclose(pFin2);
        fclose(pFout);
        if (second_file == 1)
        {
            fclose(pFout2);
        }
        
        return 1;
    }
    
    if (comment[0] != '#')
    {
        fsetpos(pFin, &position);
    }
    else
    {
        while (comment[0] == '#')
        {
            fgetpos(pFin, &position);
            fgets(comment, STRINGLENGTH, pFin);
            
            if (comment[strnlen(comment, STRINGLENGTH)-1] != '\n' && comment[0] == '#')
            {
                printf(BOLD"\nERROR: Header Comment in file to long.\n"RESET);
                
                if (Pointer_1 != NULL || Pointer_2 != NULL || Pointer_3 != NULL)
                {
                    free(Pointer_1);
                    free(Pointer_2);
                    free(Pointer_3);
                }
                
                fclose(pFin);
                fclose(pFin2);
                fclose(pFout);
                if (second_file == 1)
                {
                    fclose(pFout2);
                }
                
                return 1;
            }
            
            if (comment[0] != '#')
            {
                break;
            }
        }
    }
    
    fsetpos(pFin, &position);
    
    return 0;
}

/* ---- ONLY NUMBERS FUNCTION ---- */

int check_number(char *number)
{
    char * pch;
    int i;
    
    pch = strchr(number, '.');
    if (pch != NULL)
    {
        printf(BOLD"\nERROR: No floating-point numbers allowed.\n"RESET);
        return 1;
    }
    
    pch = strchr(number, ',');
    if (pch != NULL)
    {
        printf(BOLD"\nERROR: No floating-point numbers allowed.\n"RESET);
        return 1;
    }
    
    for (i = 0; i < strnlen(number, STRINGLENGTH); i++)
    {
        if (isdigit(number[i]) == 0)
        {
            printf(BOLD"\nERROR: Parameter is not a number.\n"RESET);
            return 1;
        }
    }
    
    return 0;
}

/* ---- CLEAR STRING FUNCTION ---- */
/* ---- FUNCTION BY HELMUT RESCH ---- */

unsigned int clearString(char *input)
{
    int i;
    
    for (i = 0; i < strnlen(input, STRINGLENGTH); i++)
    {
        if (input[i] == '\r')
        {
            input[i] = '\0';
        }
        if (input[i] == '\n')
        {
            input[i] = '\0';
        }
        if (input[i] == '\t')
        {
            input[i] = '\0';
        }
    }
    
    return 0;
}

/* ---- FUNCTION TO AVOID STRINGLEAKS IN MAIN FILE ---- */

int clearOptarg(char *string, char *input)
{
    strncpy(string, input, strnlen(input, STRINGLENGTH));
    string[strlen(input)] = '\0';
    
    if (strlen(string) >= STRINGLENGTH)
    {
        printf(BOLD"\nERROR: Parameterinput is too long!\n"RESET);
        return 1;
    }
    else
    {
        return 0;
    }
}

/* ---- FUNCTION TO CLOSE FILES IF PROGRAM JUMPS INTO ERROR MESSAGE ---- */

int closefiles(FILE *pFin1, FILE *pFin2, FILE *pFout1, FILE *pFout2, int second_file)
{
    int check = 0;
    
    if (pFin1 != NULL)
    {
        check = fclose(pFin1);
        if (check == EOF)
        {
            printf(BOLD"\nERROR: Can't close Inputfile 1!\n"RESET);
            return -1;
        }
    }
    if (pFin2 != NULL)
    {
        check = fclose(pFin2);
        if (check == EOF)
        {
            printf(BOLD"\nERROR: Can't close Inputfile 2!\n"RESET);
            return -1;
        }
    }
    if (pFout1 != NULL)
    {
        check = fclose(pFout1);
        if (check == EOF)
        {
            printf(BOLD"\nERROR: Can't close Outputfile 1!\n"RESET);
            return -1;
        }
    }
    if (second_file == 1)
    {
        if (pFout2 != NULL)
        {
            check = fclose(pFout2);
            if (check == EOF)
            {
                printf(BOLD"\nERROR: Can't close Outputfile 2!\n"RESET);
                return -1;
            }
        }
    }
    return 0;
}
