/* !*  Functions for Color Image Subtraction
 *
 * \author Sebastian Dichler <el16b032@technikum-wien.at><sedi343@gmail.com>
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
    printf("Format input:\t"BOLD"subtract.out <-f FILENAME> <-s FILENAME> <-o FILENAME>\n\n"RESET);
    printf(ITALIC"Required Parameters:\n"RESET);
    printf(BOLD"<-f FILENAME>"RESET"\tIt specifies the first input file.\n");
    printf(BOLD"<-s FILENAME>"RESET"\tIt specifies the second input file.\n");
    printf(BOLD"<-o FILENAME>"RESET"\tIt specifies the output file. ("BOLD"colored"RESET")\n\n");
    printf(ITALIC"Optional Parameters:\n"RESET);
    printf(BOLD"[-n FILENAME]"RESET"\tIt specifies the second output file. ("BOLD"black"RESET")\n");
    printf(BOLD"[-a NUMBER]"RESET"\tChange output algorithm "BOLD"0"RESET" is default, "BOLD"1"RESET" or "BOLD"2"RESET"\n");
    printf("\t\tUse "BOLD"0"RESET" for changed object white(depends on RGB input) and everything else colored/black. (justcolor)\n");
    printf("\t\tUse "BOLD"1"RESET" for changed object colored and everything else white(depends on RGB input)/black. (justcolor)\n");
    printf("\t\tUse "BOLD"2"RESET" for changed object white(depends on RGB input) and everything else colored/black. (fullpixel)\n");
    printf(BOLD"[-t THRESHOLD]"RESET"\tValue between "BOLD"0"RESET" and "BOLD"100"RESET".\n");
    printf(BOLD"[-r RED]"RESET"\tSpecify the "BOLD"RED"RESET" color value for the difference area to be filled in.\n");
    printf(BOLD"[-g GREEN]"RESET"\tSpecify the "BOLD"GREEN"RESET" color value for the difference area to be filled in.\n");
    printf(BOLD"[-b BLUE]"RESET"\tSpecify the "BOLD"BLUE"RESET" color value for the difference area to be filled in.\n");
    printf(BOLD"[-h]"RESET"\t\tPrints a help message.\n\n");
    
#if DEBUG
    printf(BOLDRED ITALIC"*** DEBUG MODE ACTIVE ***\n\n"RESET);
#endif
}

/* ---- REMOVE COMMENT FUNCTION ---- */ //Umprogrammieren mit fgetc ein zeichen einlesen wenn # solang einlesen bis \n

int removecomment(FILE *pFin, FILE *pFin2, FILE *pFout, FILE *pFout2, struct picture *Pointer_1, struct picture *Pointer_2, struct picture *Pointer_3, int second_file)
{
    char comment[500];
    
    fpos_t position;
    
    fgetpos(pFin,&position);
    
    fgets(comment, 500, pFin);
    
    if (comment[strlen(comment)-1] != '\n' && comment[0] == '#')
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
        
        return -1;
    }
    
    if (comment[strlen(comment)-1] != '\n' && comment[0] == '#')
    {
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
        
        return -1;
    }
    
    if (comment[0] != '#')
    {
        fsetpos(pFin,&position);
    }
    else
    {
        while (comment[0] == '#')
        {
            fgetpos(pFin,&position);
            fgets(comment, 500, pFin);
            
            if (comment[strlen(comment)-1] != '\n' && comment[0] == '#')
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
                
                return -1;
            }
            
            if (comment[strlen(comment)-1] != '\n')
            {
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
                
                return -1;
            }
            
            if (comment[0] != '#')
            {
                break;
            }
        }
    }
    
    fsetpos(pFin,&position);
    
    return 0;
}

/* ---- ONLY NUMBERS FUNCTION ---- */

int check_number(char *number)
{
    int i;
    
    for (i = 0; i < strlen(number); i++)
    {
        if (number[i] == '.' || number[i] == ',')
        {
            printf(BOLD"\nERROR: No floating-point numbers allowed.\n"RESET);
            return 1;
        }
        
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
    
    for (i = 0; i < strlen(input); i++)
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
    strncpy(string, input, strlen(input));
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

void closefiles(FILE *pFin1, FILE *pFin2, FILE *pFout1, FILE *pFout2, int second_file)
{
    if (pFin1 != NULL)
    {
        fclose(pFin1);
    }
    if (pFin2 != NULL)
    {
        fclose(pFin2);
    }
    if (pFout1 != NULL)
    {
        fclose(pFout1);
    }
    if (second_file == 1)
    {
        if (pFout2 != NULL)
        {
            fclose(pFout2);
        }
    }
}
