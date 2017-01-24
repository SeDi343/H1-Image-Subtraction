/*! Image Subtraction
 *
 * \description a program to subtrace one image with the information of another
 *              picture
 *
 * \author Sebastian Dichler <el16b032@technikum-wien.at> <sedi343@gmail.com>
 *
 * \version Rev.: 01, 19.01.2017 - Created
 *          Rev.: 02, 19.01.2017 - Writing code for image input into buffer.
 *          Rev.: 03, 19.01.2017 - Using 1 function.c file main.c and header.h
 *                                 File for this project.
 *          Rev.: 04, 19.01.2017 - Adding removecomment_2p function
 *                                 for save free() of both pointers.
 *          Rev.: 05, 19.01.2017 - New structure of the code, changing functions
 *                                 eg. the removecomment
 *          Rev.: 06, 19.01.2017 - Adding first thoughts to the algorithm
 *          Rev.: 07, 20.01.2017 - Adding 2nd output, everything is black, without
 *                                 the changed object.
 *          Rev.: 07, 20.01.2017 - Changing full removecomment function and
 *                                 added function to clear the oparg string
 *                                 as well added if to check if you use both output
 *                                 files to close files you realy just use
 *          Rev.: 08, 21.01.2017 - Changed algorithm
 *          Rev.: 09, 21.01.2017 - Adding some algorythms
 *          Rev.: 10, 21.01.2017 - Adding check of return value of fscanf reading string
 *                                 and changing removecomment function
 *          Rev.: 11, 21.01.2017 - Removing major bug in removecomment function,
 *                                 somehow return -1 isn't canceling program,
 *                                 checking return value of every removecomment()
 *                                 request
 *          Rev.: 12, 22.01.2017 - Checking every return value of every function
 *          Rev.: 13, 22.01.2017 - Changing functions, theyre saver now
 *          Rev.: 14, 24.01.2017 - Found many bugs with return value of functions
 *                                 fixing them now
 *          Rev.: 15, 24.01.2017 - somehow the closefile function is not working
 *                                 files dont get closed with
 *                                 FILE ** input value and also with FILE *.
 *                                 also not working if you put function code directly into
 *                                 main program
 *          Rev.: 16, 24.01.2017 - Added algorithm Gray value unweighted (3)
 *                                 Added algorithm Gray value weighted (4)
 *          Rev.: 17, 24.01.2017 - THRESHOLD input is now absolute value not percentage
 *
 * \note could also use diffcolor = diffcolor&0x7FFFFFFF;
 *
 * \information Tested on macOS Sierra 10.12.2, ubuntu 12.04, raspi3pixel 4.4.38-v7+
 *
 */

#include "myHeader.h"

int main (int argc, char *argv[])
{
    FILE* pFin_1 = NULL;
    FILE* pFin_2 = NULL;
    FILE* pFout_1 = NULL;
    FILE* pFout_2 = NULL;
    
    char inputFile1[STRINGLENGTH];
    char inputFile2[STRINGLENGTH];
    char outputFile1[STRINGLENGTH];
    char outputFile2[STRINGLENGTH];
    char thresholdString[STRINGLENGTH];
    char redString[STRINGLENGTH];
    char greenString[STRINGLENGTH];
    char blueString[STRINGLENGTH];
    
    int counterParameter = 0;

    char id_1[STRINGLENGTH];
    char id_2[STRINGLENGTH];
    unsigned int width_pic_1;
    unsigned int width_pic_2;
    unsigned int height_pic_1;
    unsigned int height_pic_2;
    unsigned int max_color_1;
    unsigned int max_color_2;
    struct picture *picture_1_Pointer = NULL;
    struct picture *picture_2_Pointer = NULL;
    struct picture *picture_edit_Pointer = NULL;
    
    int i = 0;
    int opt;
    
    int redChannel = 255;
    int blueChannel = 255;
    int greenChannel = 255;
    
    int reddiffcolor = 0;
    int greendiffcolor = 0;
    int bluediffcolor = 0;
    float diffcolor = 0;
    
    int threshold = 0;
    int threshold_percent = 20;
    
    char algorithm_code[STRINGLENGTH];
    int algorithm_code_number = 0;
    
    int error = 0;
    
    int second_file = 0;
    
    int returnvalue = 0;
    
    char *pEnd;
    
/*------------------------------------------------------------------*/
/* P A R A M E T E R   C H E C K                                    */
/*------------------------------------------------------------------*/
    
    
/* ---- CHECK FOR INPUT PARAMETERS ---- */
    
    while ( (opt = getopt (argc, argv, "f:s:o:n:t:r:g:b:a:h?")) != -1)
    {
        switch (opt)
        {
                
/* ---- FIRST FILE INPUT ---- */
                
            case 'f':
                error = clearOptarg(inputFile1, optarg);
                
                pFin_1 = fopen(inputFile1, "rb");
                if (pFin_1 == NULL)
                {
                    clear();
                    
                    printf(BOLD"\nERROR: Can't open file %s\n"RESET, inputFile1);
                    
                    closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                    
                    return -1;
                }
                counterParameter++;
            break;
                
/* ---- SECOND FILE INPUT ---- */
                
            case 's':
                error = clearOptarg(inputFile2, optarg);
                
                pFin_2 = fopen(inputFile2, "rb");
                if (pFin_2 == NULL)
                {
                    clear();
                    
                    printf(BOLD"\nERROR: Can't open file %s\n"RESET, inputFile2);
                    
                    closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);

                    return -1;
                }
                counterParameter++;
            break;
                
/* ---- FIRST OUTPUT FILE ---- */
                
            case 'o':
                error = clearOptarg(outputFile1, optarg);
                
                pFout_1 = fopen(outputFile1, "wb");
                if (pFout_1 == NULL)
                {
                    clear();

                    printf(BOLD"\nERROR: Can't create file %s\n"RESET, outputFile1);
                    
                    closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);

                    return -1;
                }
                counterParameter++;
            break;
                
/* ---- SECOND OUTPUT FILE ---- */
                
            case 'n':
                error = clearOptarg(outputFile2, optarg);
                
                second_file = 1;
                
                pFout_2 = fopen(outputFile2, "wb");
                if (pFout_2 == NULL)
                {
                    clear();

                    printf(BOLD"\nERROR: Can't create file %s\n"RESET, outputFile2);
                    
                    closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);

                    return -1;
                }
            break;
                
/* ---- THRESHOLD ---- */
                
            case 't':
                error = clearOptarg(thresholdString, optarg);
                
                error = check_number(thresholdString);

                threshold = strtod(thresholdString, &pEnd);
                
                threshold_percent = 0;
            break;
                
/* ---- RED COLOR ---- */
                
            case 'r':
                error = clearOptarg(redString, optarg);
                
                error = check_number(redString);

                redChannel = strtod(redString, &pEnd);
            break;
                
/* ---- GREEN COLOR ---- */
                
            case 'g':
                error = clearOptarg(greenString, optarg);
                
                error = check_number(greenString);
                
                greenChannel = strtod(greenString, &pEnd);
            break;
                
/* ---- BLUE COLOR ---- */
                
            case 'b':
                error = clearOptarg(blueString, optarg);
                
                error = check_number(blueString);
                
                blueChannel = strtod(blueString, &pEnd);
            break;
                
/* ---- SUPPORTING MORE ALGORITHMS ---- */
                
            case 'a':
                error = clearOptarg(algorithm_code, optarg);
                
                error = check_number(algorithm_code);
                
                algorithm_code_number = strtod(algorithm_code, &pEnd);
            break;
                
/* ---- HELPDESK ---- */
                
            case 'h':
                clearNoHelp();
                helpdesk_2();
                
                return 0;
            break;
                
/* ---- HELPDESK ---- */
                
            case '?':
                clearNoHelp();
                helpdesk_2();
                
                return 0;
            break;
        }
    }
    
/*------------------------------------------------------------------*/
/* E R R O R   H A N D L I N G                                      */
/*------------------------------------------------------------------*/
    
    clear();
    
/* ---- CHECK FOR NO PARAMETER INPUT ---- */
    
    if (argc == 1)
    {
        clearNoHelp();
        helpdesk_2();
        printf(BOLD"\nERROR: No Parameters given.\n"RESET);
        
        return -1;
    }
    
/* ---- IF ONE PARAMETER INPUT FAILED OR IS NOT CORRECT ---- */
    
    if (error == 1)
    {
        printf(BOLD"\nERROR: One or more Parameters are not correct.\n"RESET);
        
        closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
        
        return -1;
    }
    
/* ---- CHECK IF ONE FILE IS NOT PRESENT ---- */
    
    if (pFin_1 == NULL || pFin_2 == NULL || counterParameter != 3)
    {
        printf(BOLD"\nERROR: Both Inputfiles and Outputfile must be present.\n"RESET);
        
#if DEBUG
        printf(RED" * IN1 missing(1missing, 0present): %d\n * IN2 missing(1missing, 0present): %d\n"RESET,
               (pFin_1==NULL)?1:0, (pFin_2==NULL)?1:0);
#endif
        
        closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);

        return -1;
    }
    
/* ---- PARAMETER -O NEEDS AN ARGUMENT ---- */
    
    if (pFout_1 == NULL)
    {
        printf(BOLD"\nERROR: Parameter -o requires an argument.\n"RESET);
        
        closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);

        return -1;
    }
    
/* ---- PARAMETER -N NEEDS AN ARGUMENT ---- */
    
    if (second_file == 1)
    {
        if (pFout_2 == NULL)
        {
            printf(BOLD"\nERROR: Parameter -n requires an argument.\n"RESET);
            
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);

            return -1;
        }
    }
    
/* ---- ALGORITHM CODE NUMBER NOT 0, 1 OR 2 ---- */
    
    if (algorithm_code_number != 0 && algorithm_code_number != 1 && algorithm_code_number != 2 &&
        algorithm_code_number != 3 && algorithm_code_number != 4)
    {
        printf(BOLD"\nERROR: Algorithm code number can only be 0, 1, 2, 3 or 4\n"RESET);
        
#if DEBUG
        printf(RED" * Algorithm code number Input: %d"RESET, algorithm_code_number);
#endif
        
        closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
        
        return -1;
    }
    
/*------------------------------------------------------------------*/
/* R E A D I N G   F I L E                                          */
/*------------------------------------------------------------------*/

/* ---- CHECK FOR P3 ---- */
    
    returnvalue = fscanf(pFin_1, "%499s", id_1);
    returnvalue = fscanf(pFin_2, "%499s", id_2);
    
    if (returnvalue >= STRINGLENGTH || returnvalue == EOF)
    {
        printf(BOLD"\nERROR: Not supported data format.\n"RESET);
        
        closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
        
        return -1;
    }

/* ---- FSCANF DOESNT READ THE LAST CHAR IN A STRING EG "\n" ---- */

    returnvalue = fgetc(pFin_1);
    returnvalue = fgetc(pFin_2);
    
    if (returnvalue == EOF)
    {
        printf(BOLD"\nERROR: Failed Reading Data\n"RESET);
        
        closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
        
        return -1;
    }
    
#if DEBUG
    printf(RED ITALIC" * %s: %#01x %#01x %#01x\n"RESET, id_1, id_1[0], id_1[1], id_1[2]);
    printf(RED ITALIC" * %s: %#01x %#01x %#01x\n"RESET, id_2, id_2[0], id_2[1], id_2[2]);
#endif
    
    if ((id_1[0] == 'P' && id_1[1] == '3') && (id_2[0] == 'P' && id_2[1] == '3'))
    {
        
/* ---- REMOVING COMMENTS ---- */
        
        error = removecomment(FILE_1);
        if (error == 1)
        {
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        error = removecomment(FILE_2);
        if (error == 1)
        {
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }

/* ---- READING WIDTH AND HEIGHT 1 ---- */
        
        returnvalue = fscanf(pFin_1, "%u", &width_pic_1);
        if (returnvalue == EOF)
        {
            printf(BOLD"\nERROR: Cant read input file.\n"RESET);
            
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
        returnvalue = fscanf(pFin_1, "%u", &height_pic_1);
        if (returnvalue == EOF)
        {
            printf(BOLD"\nERROR: Cant read input file.\n"RESET);
            
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
/* ---- READING WIDTH AND HEIGHT 2 ---- */

        returnvalue = fscanf(pFin_2, "%u", &width_pic_2);
        if (returnvalue == EOF)
        {
            printf(BOLD"\nERROR: Cant read input file.\n"RESET);
            
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
        returnvalue = fscanf(pFin_2, "%u", &height_pic_2);
        if (returnvalue == EOF)
        {
            printf(BOLD"\nERROR: Cant read input file.\n"RESET);

            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
/* ---- REMOVING COMMENT ---- */
        
        error = removecomment(FILE_1);
        if (error == 1)
        {
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        error = removecomment(FILE_2);
        if (error == 1)
        {
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
/* ---- READING MAX COLOR INDEX ---- */
        
        returnvalue = fscanf(pFin_1, "%u", &max_color_1);
        if (returnvalue == EOF)
        {
            printf(BOLD"\nERROR: Cant read input file.\n"RESET);
            
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }

        returnvalue = fscanf(pFin_2, "%u", &max_color_2);
        if (returnvalue == EOF)
        {
            printf(BOLD"\nERROR: Cant read input file.\n"RESET);

            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
/* ---- DIFFERENT PICTURES ARE NOT ALLOWED ---- */
        
        if (width_pic_1 != width_pic_2 || height_pic_1 != height_pic_2 || max_color_1 != max_color_2)
        {
            printf(BOLD"\nERROR: Pictures haven't the same main information (PIC1: %dx%d, %d; PIC2: %dx%d, %d)\n"RESET,
                   width_pic_1, height_pic_1, max_color_1, width_pic_2, height_pic_2, max_color_2);
            
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);

            return -1;
        }
        
/* ---- HIGHER RGB COLOR THAN MAX COLOR INDEX IS NOT ALLOWED ---- */
        
        if (redChannel > max_color_1 || greenChannel > max_color_1 || blueChannel > max_color_1)
        {
            printf(BOLD"\nERROR: Color Index not allowed (chose between 0 and %d)\n"RESET, max_color_1);
            printf(BOLD"Standard Value is 255 may change that manually. with -R, -G, -B\n"RESET);
            
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);

            return -1;
        }
        
        if (redChannel > max_color_2 || greenChannel > max_color_2 || blueChannel > max_color_2)
        {
            printf(BOLD"\nERROR: Color Index not allowed (chose between 0 and %d)\n"RESET, max_color_2);
            printf(BOLD"Standard Value is 255 may change that manually. with -R, -G, -B\n"RESET);
            
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);

            return -1;
        }
        
/* ---- THRESHOLD_PERCENT CAN ONLY BE 0 TO 100 ---- */
        
        if ((threshold > max_color_1 || threshold < 0) && threshold_percent == 0)
        {
            printf(BOLD"\nERROR: Threshold must be 0 to %d.\n"RESET, max_color_1);
            
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
/* ---- CALCULATING THRESHOLD WITH PERCENTAGE ---- */
        
        if (threshold_percent == 20)
        {
            threshold = (threshold_percent * max_color_1) / 100;
        }
        
#if DEBUG
        printf(RED ITALIC" * Threshold: %d\n"RESET, threshold);
        printf(RED ITALIC" * Threshold perc: %d\n\n"RESET, threshold_percent);
#endif
        
/*------------------------------------------------------------------*/
/* A L L O C A T I N G   M E M O R Y                                */
/*------------------------------------------------------------------*/
        
        picture_1_Pointer = (struct picture *)malloc(width_pic_1*height_pic_1*sizeof(struct picture));
        
        picture_2_Pointer = (struct picture *)malloc(width_pic_2*height_pic_2*sizeof(struct picture));
        
        picture_edit_Pointer = (struct picture *)malloc(width_pic_1*height_pic_1*sizeof(struct picture));
        
        if (picture_1_Pointer == NULL || picture_2_Pointer == NULL || picture_edit_Pointer == NULL)
        {
            printf(BOLD"\nERROR: Could't allocate memory.\n"RESET);
            free(picture_1_Pointer);
            free(picture_2_Pointer);
            free(picture_edit_Pointer);
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);

            return -1;
        }
        
/*------------------------------------------------------------------*/
/* R E A D   P I X E L S                                            */
/*------------------------------------------------------------------*/
        
        printf(BOLD"* Importing file....\n"RESET);
        
/* ---- READING FIRST PICTURE ---- */
        
        for (i = 0; i < width_pic_1*height_pic_1; i++)
        {
/* ---- REMOVE COMMENT ---- */
            error = removecomment(FILE_1);
            if (error == 1)
            {
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
/* ---- READ RED ---- */
            returnvalue = fscanf(pFin_1, "%u", &(picture_1_Pointer+i)->r);
            if (returnvalue == EOF)
            {
                printf(BOLD"\nERROR: Cant read input file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
/* ---- READ GREEN ---- */
            returnvalue = fscanf(pFin_1, "%u", &(picture_1_Pointer+i)->g);
            if (returnvalue == EOF)
            {
                printf(BOLD"\nERROR: Cant read input file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
/* ---- READ BLUE ---- */
            returnvalue = fscanf(pFin_1, "%u", &(picture_1_Pointer+i)->b);
            if (returnvalue == EOF)
            {
                printf(BOLD"\nERROR: Cant read input file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
        }
        
/* ---- READING SECOND PICTURE ---- */
        
        for (i = 0; i < width_pic_2*height_pic_2; i++)
        {
/* ---- REMOVE COMMENT ---- */
            error = removecomment(FILE_2);
            if (error == 1)
            {
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
/* ---- READ RED ---- */
            returnvalue = fscanf(pFin_2, "%u", &(picture_2_Pointer+i)->r);
            if (returnvalue == EOF)
            {
                printf(BOLD"\nERROR: Cant read input file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
/* ---- READ GREEN ---- */
            returnvalue = fscanf(pFin_2, "%u", &(picture_2_Pointer+i)->g);
            if (returnvalue == EOF)
            {
                printf(BOLD"\nERROR: Cant read input file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
/* ---- READ BLUE ---- */
            returnvalue = fscanf(pFin_2, "%u", &(picture_2_Pointer+i)->b);
            if (returnvalue == EOF)
            {
                printf(BOLD"\nERROR: Cant read input file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
        }
        
        printf(BOLD"* Done file Import!\n\n"RESET);
        
/*------------------------------------------------------------------*/
/* A L G O R I T H M   -   C O L O R S                              */
/*------------------------------------------------------------------*/
        
        if (second_file == 1)
        {
            printf(BOLD ITALIC"Creating Picture 1:\n"RESET);
        }
        
        printf(BOLD"* Checking Color Channels...\n"RESET);
        
/* ---- DEFAULT ALGORITHM (0) HELMUT RESCH HELPED ME WITH THIS CODE SEGMENT ---- */
        
        if (algorithm_code_number == 0)
        {
            for(i = 0; i < width_pic_1*height_pic_1; i++)
            {
                reddiffcolor = (picture_1_Pointer+i)->r - (picture_2_Pointer+i)->r;
                greendiffcolor = (picture_1_Pointer+i)->g - (picture_2_Pointer+i)->g;
                bluediffcolor = (picture_1_Pointer+i)->b - (picture_2_Pointer+i)->b;
                
                reddiffcolor = abs(reddiffcolor);
                greendiffcolor = abs(greendiffcolor);
                bluediffcolor = abs(bluediffcolor);
                
                if(reddiffcolor < threshold)
                {
                    if(greendiffcolor < threshold)
                    {
                        if(bluediffcolor < threshold)
                        {
                            reddiffcolor = (picture_1_Pointer+i)->r;
                            greendiffcolor = (picture_1_Pointer+i)->g;
                            bluediffcolor = (picture_1_Pointer+i)->b;
                        }
                        else
                        {
                            reddiffcolor = redChannel;
                            greendiffcolor = greenChannel;
                            bluediffcolor = blueChannel;
                        }
                    }
                    else
                    {
                        reddiffcolor = redChannel;
                        greendiffcolor = greenChannel;
                        bluediffcolor = blueChannel;
                    }
                }
                else
                {
                    reddiffcolor = redChannel;
                    greendiffcolor = greenChannel;
                    bluediffcolor = blueChannel;
                }
                
                (picture_edit_Pointer+i)->r = reddiffcolor;
                (picture_edit_Pointer+i)->g = greendiffcolor;
                (picture_edit_Pointer+i)->b = bluediffcolor;
            }
        }
        
/* ---- ALGORITHM (1) ---- */
        
        if (algorithm_code_number == 1)
        {
            for(i = 0; i < width_pic_1*height_pic_1; i++)
            {
                reddiffcolor = (picture_1_Pointer+i)->r - (picture_2_Pointer+i)->r;
                greendiffcolor = (picture_1_Pointer+i)->g - (picture_2_Pointer+i)->g;
                bluediffcolor = (picture_1_Pointer+i)->b - (picture_2_Pointer+i)->b;
                
                reddiffcolor = abs(reddiffcolor);
                greendiffcolor = abs(greendiffcolor);
                bluediffcolor = abs(bluediffcolor);
                
                if(reddiffcolor < threshold)
                {
                    if(greendiffcolor < threshold)
                    {
                        if(bluediffcolor < threshold)
                        {
                            reddiffcolor = redChannel;
                            greendiffcolor = greenChannel;
                            bluediffcolor = blueChannel;
                        }
                        else
                        {
                            reddiffcolor = (picture_2_Pointer+i)->r;
                            greendiffcolor = (picture_2_Pointer+i)->g;
                            bluediffcolor = (picture_2_Pointer+i)->b;
                        }
                    }
                    else
                    {
                        reddiffcolor = (picture_2_Pointer+i)->r;
                        greendiffcolor = (picture_2_Pointer+i)->g;
                        bluediffcolor = (picture_2_Pointer+i)->b;
                    }
                }
                else
                {
                    reddiffcolor = (picture_2_Pointer+i)->r;
                    greendiffcolor = (picture_2_Pointer+i)->g;
                    bluediffcolor = (picture_2_Pointer+i)->b;
                }
                
                (picture_edit_Pointer+i)->r = reddiffcolor;
                (picture_edit_Pointer+i)->g = greendiffcolor;
                (picture_edit_Pointer+i)->b = bluediffcolor;
            }
        }
        
/* ---- ALGORITHM (2) ---- */
        
        if (algorithm_code_number == 2)
        {
            for (i = 0; i < width_pic_1*height_pic_1; i++)
            {
                diffcolor = (((picture_1_Pointer+i)->r + (picture_1_Pointer+i)->g + (picture_1_Pointer+i)->b) -
                             ((picture_2_Pointer+i)->r + (picture_2_Pointer+i)->g + (picture_2_Pointer+i)->b));
                
                diffcolor = fabsf(diffcolor);
                
                if (diffcolor > threshold)
                {
                    (picture_edit_Pointer+i)->r = redChannel;
                    (picture_edit_Pointer+i)->g = greenChannel;
                    (picture_edit_Pointer+i)->b = blueChannel;
                }
                else
                {
                    (picture_edit_Pointer+i)->r = (picture_1_Pointer+i)->r;
                    (picture_edit_Pointer+i)->g = (picture_1_Pointer+i)->g;
                    (picture_edit_Pointer+i)->b = (picture_1_Pointer+i)->b;
                }
            }
        }
        
/* ---- ALGORITHM (3) ---- */
        
        if (algorithm_code_number == 3)
        {
            for (i = 0; i < width_pic_1*height_pic_1; i++)
            {
                diffcolor = ((((picture_1_Pointer+i)->r + (picture_1_Pointer+i)->g + (picture_1_Pointer+i)->b)/3) -
                             (((picture_2_Pointer+i)->r + (picture_2_Pointer+i)->g + (picture_2_Pointer+i)->b)/3));
                
                diffcolor = fabsf(diffcolor);
                
                if (diffcolor > threshold)
                {
                    (picture_edit_Pointer+i)->r = redChannel;
                    (picture_edit_Pointer+i)->g = greenChannel;
                    (picture_edit_Pointer+i)->b = blueChannel;
                }
                else
                {
                    (picture_edit_Pointer+i)->r = (picture_1_Pointer+i)->r;
                    (picture_edit_Pointer+i)->g = (picture_1_Pointer+i)->g;
                    (picture_edit_Pointer+i)->b = (picture_1_Pointer+i)->b;
                }
            }
        }
        
/* ---- ALGORITHM (4) ---- */
        
        if (algorithm_code_number == 4)
        {
            for (i = 0; i < width_pic_1*height_pic_1; i++)
            {
                diffcolor = ((0.299 * (picture_1_Pointer+i)->r + 0.587 * (picture_1_Pointer+i)->g + 0.144 * (picture_1_Pointer+i)->b) -
                             (0.299 * (picture_2_Pointer+i)->r + 0.587 * (picture_2_Pointer+i)->g + 0.144 * (picture_2_Pointer+i)->b));
                
                diffcolor = fabsf(diffcolor);
                
                if (diffcolor > threshold)
                {
                    (picture_edit_Pointer+i)->r = redChannel;
                    (picture_edit_Pointer+i)->g = greenChannel;
                    (picture_edit_Pointer+i)->b = blueChannel;
                }
                else
                {
                    (picture_edit_Pointer+i)->r = (picture_1_Pointer+i)->r;
                    (picture_edit_Pointer+i)->g = (picture_1_Pointer+i)->g;
                    (picture_edit_Pointer+i)->b = (picture_1_Pointer+i)->b;
                }
            }
        }
        
        printf(BOLD"* Done Checking Color Channels!\n\n"RESET);
        
/* ---- WRITE HEADER INTO OUTPUT FILE ---- */
        
        printf(BOLD"* Writing file....\n"RESET);
        
        returnvalue = fprintf(pFout_1, "P3\n");
        
        if (returnvalue < 2)
        {
            printf(BOLD"\nERROR: Cant write output file.\n"RESET);
            
            free(picture_1_Pointer);
            free(picture_2_Pointer);
            free(picture_edit_Pointer);
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
        returnvalue = fprintf(pFout_1, "#Color Image Subtraction by Sebastian Dichler\n");
        
        if (returnvalue < 45)
        {
            printf(BOLD"\nERROR: Cant write output file.\n"RESET);
            
            free(picture_1_Pointer);
            free(picture_2_Pointer);
            free(picture_edit_Pointer);
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
        returnvalue = fprintf(pFout_1, "%u %u\n", width_pic_1, height_pic_1);
        
        if (returnvalue < 0)
        {
            printf(BOLD"\nERROR: Cant write output file.\n"RESET);
            
            free(picture_1_Pointer);
            free(picture_2_Pointer);
            free(picture_edit_Pointer);
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
        returnvalue = fprintf(pFout_1, "%u\n", max_color_1);
        
        if (returnvalue < 0)
        {
            printf(BOLD"\nERROR: Cant write output file.\n"RESET);
            
            free(picture_1_Pointer);
            free(picture_2_Pointer);
            free(picture_edit_Pointer);
            closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
            
            return -1;
        }
        
/* ---- WRITING PIXELS INTO OUTPUT FILE ---- */
        
        for (i = 0; i < width_pic_1*height_pic_1; i++)
        {
            returnvalue = fprintf(pFout_1, "%u %u %u\n",
                                  (picture_edit_Pointer+i)->r, (picture_edit_Pointer+i)->g, (picture_edit_Pointer+i)->b);
            
            if (returnvalue < 0)
            {
                printf(BOLD"\nERROR: Cant write output file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
        }
        
        printf(BOLD"* Done writing file!\n\n"RESET);
        
/*------------------------------------------------------------------*/
/* A L G O R I T H M   -   B L A C K                                */
/*------------------------------------------------------------------*/
        
        if (second_file == 1)
        {
            printf(BOLD ITALIC"Creating Picture 2:\n"RESET);

            printf(BOLD"* Checking Color Channels...\n"RESET);
            
/* ---- DEFAULT ALGORITHM (0) HELMUT RESCH HELPED ME WITH THIS CODE SEGMENT ---- */
            
            if (algorithm_code_number == 0)
            {
                for(i = 0; i < width_pic_1*height_pic_1; i++)
                {
                    reddiffcolor = (picture_1_Pointer+i)->r - (picture_2_Pointer+i)->r;
                    greendiffcolor = (picture_1_Pointer+i)->g - (picture_2_Pointer+i)->g;
                    bluediffcolor = (picture_1_Pointer+i)->b - (picture_2_Pointer+i)->b;
                    
                    reddiffcolor = abs(reddiffcolor);
                    greendiffcolor = abs(greendiffcolor);
                    bluediffcolor = abs(bluediffcolor);
                    
                    if(reddiffcolor < threshold)
                    {
                        if(greendiffcolor < threshold)
                        {
                            if(bluediffcolor < threshold)
                            {
                                reddiffcolor = 0;
                                greendiffcolor = 0;
                                bluediffcolor = 0;
                            }
                            else
                            {
                                reddiffcolor = redChannel;
                                greendiffcolor = greenChannel;
                                bluediffcolor = blueChannel;
                            }
                        }
                        else
                        {
                            reddiffcolor = redChannel;
                            greendiffcolor = greenChannel;
                            bluediffcolor = blueChannel;
                        }
                    }
                    else
                    {
                        reddiffcolor = redChannel;
                        greendiffcolor = greenChannel;
                        bluediffcolor = blueChannel;
                    }
                    
                    (picture_edit_Pointer+i)->r = reddiffcolor;
                    (picture_edit_Pointer+i)->g = greendiffcolor;
                    (picture_edit_Pointer+i)->b = bluediffcolor;
                }

            }
            
/* ---- ALGORITHM (1) ---- */
            
            if (algorithm_code_number == 1)
            {
                for(i = 0; i < width_pic_1*height_pic_1; i++)
                {
                    reddiffcolor = (picture_1_Pointer+i)->r - (picture_2_Pointer+i)->r;
                    greendiffcolor = (picture_1_Pointer+i)->g - (picture_2_Pointer+i)->g;
                    bluediffcolor = (picture_1_Pointer+i)->b - (picture_2_Pointer+i)->b;
                    
                    reddiffcolor = abs(reddiffcolor);
                    greendiffcolor = abs(greendiffcolor);
                    bluediffcolor = abs(bluediffcolor);
                    
                    if(reddiffcolor < threshold)
                    {
                        if(greendiffcolor < threshold)
                        {
                            if(bluediffcolor < threshold)
                            {
                                reddiffcolor = 0;
                                greendiffcolor = 0;
                                bluediffcolor = 0;
                            }
                            else
                            {
                                reddiffcolor = (picture_2_Pointer+i)->r;
                                greendiffcolor = (picture_2_Pointer+i)->g;
                                bluediffcolor = (picture_2_Pointer+i)->b;
                            }
                        }
                        else
                        {
                            reddiffcolor = (picture_2_Pointer+i)->r;
                            greendiffcolor = (picture_2_Pointer+i)->g;
                            bluediffcolor = (picture_2_Pointer+i)->b;
                        }
                    }
                    else
                    {
                        reddiffcolor = (picture_2_Pointer+i)->r;
                        greendiffcolor = (picture_2_Pointer+i)->g;
                        bluediffcolor = (picture_2_Pointer+i)->b;
                    }
                    
                    (picture_edit_Pointer+i)->r = reddiffcolor;
                    (picture_edit_Pointer+i)->g = greendiffcolor;
                    (picture_edit_Pointer+i)->b = bluediffcolor;
                }
            }
            
/* ---- ALGORITHM (2) ---- */
            
            if (algorithm_code_number == 2)
            {
                for (i = 0; i < width_pic_1*height_pic_1; i++)
                {
                    diffcolor = (((picture_1_Pointer+i)->r + (picture_1_Pointer+i)->g + (picture_1_Pointer+i)->b) -
                                 ((picture_2_Pointer+i)->r + (picture_2_Pointer+i)->g + (picture_2_Pointer+i)->b));
                    
                    diffcolor = fabsf(diffcolor);
                    
                    if (diffcolor > threshold)
                    {
                        (picture_edit_Pointer+i)->r = redChannel;
                        (picture_edit_Pointer+i)->g = greenChannel;
                        (picture_edit_Pointer+i)->b = blueChannel;
                    }
                    else
                    {
                        (picture_edit_Pointer+i)->r = 0;
                        (picture_edit_Pointer+i)->g = 0;
                        (picture_edit_Pointer+i)->b = 0;
                    }
                }
            }
            
/* ---- ALGORITHM (3) ---- */
            
            if (algorithm_code_number == 3)
            {
                for (i = 0; i < width_pic_1*height_pic_1; i++)
                {
                    diffcolor = ((((picture_1_Pointer+i)->r + (picture_1_Pointer+i)->g + (picture_1_Pointer+i)->b)/3) -
                                 (((picture_2_Pointer+i)->r + (picture_2_Pointer+i)->g + (picture_2_Pointer+i)->b)/3));
                    
                    diffcolor = fabsf(diffcolor);
                    
                    if (diffcolor > threshold)
                    {
                        (picture_edit_Pointer+i)->r = redChannel;
                        (picture_edit_Pointer+i)->g = greenChannel;
                        (picture_edit_Pointer+i)->b = blueChannel;
                    }
                    else
                    {
                        (picture_edit_Pointer+i)->r = 0;
                        (picture_edit_Pointer+i)->g = 0;
                        (picture_edit_Pointer+i)->b = 0;
                    }
                }
            }
            
/* ---- ALGORITHM (4) ---- */
            
            if (algorithm_code_number == 4)
            {
                for (i = 0; i < width_pic_1*height_pic_1; i++)
                {
                    diffcolor = ((0.299 * (picture_1_Pointer+i)->r + 0.587 * (picture_1_Pointer+i)->g + 0.144 * (picture_1_Pointer+i)->b) -
                                 (0.299 * (picture_2_Pointer+i)->r + 0.587 * (picture_2_Pointer+i)->g + 0.144 * (picture_2_Pointer+i)->b));
                    
                    diffcolor = fabsf(diffcolor);
                    
                    if (diffcolor > threshold)
                    {
                        (picture_edit_Pointer+i)->r = redChannel;
                        (picture_edit_Pointer+i)->g = greenChannel;
                        (picture_edit_Pointer+i)->b = blueChannel;
                    }
                    else
                    {
                        (picture_edit_Pointer+i)->r = 0;
                        (picture_edit_Pointer+i)->g = 0;
                        (picture_edit_Pointer+i)->b = 0;
                    }
                }
            }
            
            printf(BOLD"* Done Checking Color Channels!\n\n"RESET);
            
/* ---- WRITE HEADER INTO OUTPUT FILE ---- */
            
            printf(BOLD"* Writing file....\n"RESET);
            
            returnvalue = fprintf(pFout_2, "P3\n");
            
            if (returnvalue < 2)
            {
                printf(BOLD"\nERROR: Cant write output file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
            
            returnvalue = fprintf(pFout_2, "#Color Image Subtraction by Sebastian Dichler\n");
            
            if (returnvalue < 45)
            {
                printf(BOLD"\nERROR: Cant write output file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
            
            returnvalue = fprintf(pFout_2, "%u %u\n", width_pic_1, height_pic_1);
            
            if (returnvalue < 0)
            {
                printf(BOLD"\nERROR: Cant write output file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
            
            returnvalue = fprintf(pFout_2, "%u\n", max_color_1);
            
            if (returnvalue < 0)
            {
                printf(BOLD"\nERROR: Cant write output file.\n"RESET);
                
                free(picture_1_Pointer);
                free(picture_2_Pointer);
                free(picture_edit_Pointer);
                closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                
                return -1;
            }
            
/* ---- WRITING PIXELS INTO OUTPUT FILE ---- */
            
            for (i = 0; i < width_pic_1*height_pic_1; i++)
            {
                returnvalue = fprintf(pFout_2, "%u %u %u\n",
                                          (picture_edit_Pointer+i)->r, (picture_edit_Pointer+i)->g, (picture_edit_Pointer+i)->b);
                
                if (returnvalue < 0)
                {
                    printf(BOLD"\nERROR: Cant write output file.\n"RESET);
                    
                    free(picture_1_Pointer);
                    free(picture_2_Pointer);
                    free(picture_edit_Pointer);
                    closefiles(pFin_1, pFin_2, pFout_1, pFout_2, second_file);
                    
                    return -1;
                }
            }
            
            printf(BOLD"* Done writing file!\n"RESET);
        }
    }
    else
    {
        printf(BOLD"\nERROR: Not supported data format.\n"RESET);
#if DEBUG
        printf(RED" * FILE1: %s\n"RESET, id_1);
        printf(RED" * FILE2: %s\n"RESET, id_2);
#endif
    }
    
/*------------------------------------------------------------------*/
/* F R E E   M E M O R Y   &   C L O S E   F I L E S                */
/*------------------------------------------------------------------*/
    
    free(picture_1_Pointer);
    free(picture_2_Pointer);
    free(picture_edit_Pointer);
    returnvalue = fclose(pFin_1);
    if (returnvalue == EOF)
    {
        printf(BOLD"\nERROR: Can't close Inputfile 1!\n"RESET);
        
        return -1;
    }
    returnvalue = fclose(pFin_2);
    if (returnvalue == EOF)
    {
        printf(BOLD"\nERROR: Can't close Inputfile 2!\n"RESET);
        
        return -1;
    }
    returnvalue = fclose(pFout_1);
    if (returnvalue == EOF)
    {
        printf(BOLD"\nERROR: Can't close Outputfile 1!\n"RESET);
        
        return -1;
    }
    if (second_file == 1)
    {
        returnvalue = fclose(pFout_2);
        if (returnvalue == EOF)
        {
            printf(BOLD"\nERROR: Can't close Outputfile 2!\n"RESET);
            
            return -1;
        }
    }
    
    return 0;
}
