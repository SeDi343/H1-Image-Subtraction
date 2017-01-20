/*! Image Subtraction
 *
 * \description a program to subtrace one image with the information of another
 *              picture
 *
 * \author Sebastian Dichler <el16b032@technikum-wien.at>
 *
 * \version Rev.: 01, 19.01.2017 - Created
 *          Rev.: 02, 19.01.2017 - Writing code for image input into buffer.
 *          Rev.: 03, 19.01.2017 - Using 1 function.c file main.c and header.h
 *                                 File for this project.
 *          Rev.: 04, 19.01.2017 - Adding removecomment_2p function
 *                                 for save free() of both pointers.
 *          Rev.: 05, 19.01.2017 - New structure of the code, changing functions
 *                                 eg. the removecomment
 *          Rev.: 06, 19.01.2017 - Adding first thoughts about the algorithm
 *          Rev.: 07, 20.01.2017 - Changing full removecomment function and
 *                                 added function to clear the oparg string
 *                                 as well added if to check if you use both output
 *                                 files to close files you realy just use
 
 for(int i = 0; i < ppmWidth*ppmHeight; i++)
 {
    redDiff = fabs((pixel2Pointer+i)->r - (pixel1Pointer+i)->r);
    greenDiff = fabs((pixel2Pointer+i)->g - (pixel1Pointer+i)->g);
    blueDiff = fabs((pixel2Pointer+i)->b - (pixel1Pointer+i)->b);
 
    if(redDiff < threshold)
    {
        if(greenDiff < threshold)
        {
            if(blueDiff < threshold)
            {
                redDiff = thresholdBlack;
                greenDiff = thresholdBlack;
                blueDiff = thresholdBlack;
            }
            else
            {
                redDiff = thresholdShowRed;
                greenDiff = thresholdShowGreen;
                blueDiff = thresholdShowBlue;
            }
        }
        else
        {
            redDiff = thresholdShowRed;
            greenDiff = thresholdShowGreen;
            blueDiff = thresholdShowBlue;
        }
    }
    else
    {
        redDiff = thresholdShowRed;
        greenDiff = thresholdShowGreen;
        blueDiff = thresholdShowBlue;
    }
 
    (pixeloutPointer+i)->r = redDiff;
    (pixeloutPointer+i)->g = greenDiff;
    (pixeloutPointer+i)->b = blueDiff;
 }
 
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

    char id_1[80];
    char id_2[80];
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
    
    float redChannel = 255;
    float blueChannel = 255;
    float greenChannel = 255;
    
    int diffcolor = 0;
    
    int threshold = 0;
    float threshold_percent = 10;
    
    int error = 0;
    
    int second_file = 0;
    
    char *pEnd;
    
/*------------------------------------------------------------------*/
/* P A R A M E T E R   C H E C K                                    */
/*------------------------------------------------------------------*/
    
    clear();
    
/* ---- CHECK FOR INPUT PARAMETERS ---- */
    
    while ( (opt = getopt (argc, argv, "f:s:o:n:t:r:g:b:h:?")) != -1)
    {
        switch (opt)
        {
                
/* ---- FIRST FILE INPUT ---- */
                
            case 'f':
                error = clearOptarg(inputFile1, optarg);
                
                pFin_1 = fopen(inputFile1, "rb");
                
                if (pFin_1 == NULL)
                {
                    printf(BOLD"\nERROR: Can't open file %s\n"RESET, inputFile1);
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
                    printf(BOLD"\nERROR: Can't open file %s\n"RESET, inputFile2);
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
                    printf(BOLD"\nERROR: Can't create file %s\n"RESET, outputFile1);
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
                    printf(BOLD"\nERROR: Can't create file %s\n"RESET, outputFile2);
                    return -1;
                }
            break;
                
/* ---- THRESHOLD ---- */
                
            case 't':
                error = clearOptarg(thresholdString, optarg);
                
                error = check_number(thresholdString);

                threshold_percent = strtod(thresholdString, &pEnd);
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
/* S T A R T   O F   P R O G R A M                                  */
/*------------------------------------------------------------------*/
    
    if (error == 1)
    {
        printf(BOLD"\nERROR: One or more Parameters are not correct.\n"RESET);
        return -1;
    }
    
/* ---- CHECK IF ONE FILE IS NOT PRESENT ---- */
    
    if (pFin_1 == NULL || pFin_2 == NULL || counterParameter != 3)
    {
        printf(BOLD"\nERROR: Both Inputfiles and Outputfile must be present.\n"RESET);
        return -1;
    }
    
    if (pFout_1 == NULL)
    {
        printf(BOLD"\nERROR: Parameter -o requires an argument.\n");
    }
    
    if (second_file == 1)
    {
        if (pFout_2 == NULL)
        {
            printf(BOLD"\nERROR: Parameter -n requires an argument.\n");
        }
    }
    
/*------------------------------------------------------------------*/
/* R E A D I N G   F I L E                                          */
/*------------------------------------------------------------------*/

/* ---- CHECK FOR P3 ---- */
    
    fscanf(pFin_1, "%s", id_1);
    fscanf(pFin_2, "%s", id_2);

/* ---- FSCANF DOESNT READ THE LAST CHAR IN A STRING EG "\n" ---- */

    fgetc(pFin_1);
    fgetc(pFin_2);
    
    //fgets(id_1,80,pFin_1);
    //fgets(id_2,80,pFin_2);
    
#if DEBUG
    printf(RED"%s: %#01x %#01x %#01x\n"RESET, id_1, id_1[0], id_1[1], id_1[2]);
    printf(RED"%s: %#01x %#01x %#01x\n"RESET, id_2, id_2[0], id_2[1], id_2[2]);
#endif
    
    if ((id_1[0] == 'P' && id_1[1] == '3') && (id_2[0] == 'P' && id_2[1] == '3'))
    {
        removecomment(FILE_1);
        removecomment(FILE_2);

/* ---- READING WIDTH AND HEIGHT ---- */
        
        fscanf(pFin_1, "%u", &width_pic_1);
        fscanf(pFin_1, "%u", &height_pic_1);
        
        fscanf(pFin_2, "%u", &width_pic_2);
        fscanf(pFin_2, "%u", &height_pic_2);
        
        removecomment(FILE_1);
        removecomment(FILE_2);
        
/* ---- READING MAX COLOR INDEX ---- */
        
        fscanf(pFin_1, "%u", &max_color_1);
        
        fscanf(pFin_2, "%u", &max_color_2);
        
/* ---- DIFFERENT PICTURES ARE NOT ALLOWED ---- */
        
        if (width_pic_1 != width_pic_2 || height_pic_1 != height_pic_2 || max_color_1 != max_color_2)
        {
            printf(BOLD"\nERROR: Pictures haven't the same main information (PIC1: %dx%d, %d; PIC2: %dx%d, %d)\n"RESET,
                   width_pic_1, height_pic_1, max_color_1, width_pic_2, height_pic_2, max_color_2);
            return -1;
        }
        
/* ---- HIGHER RGB COLOR THAN MAX COLOR INDEX IS NOT ALLOWED ---- */
        
        if (redChannel > max_color_1 || greenChannel > max_color_1 || blueChannel > max_color_1)
        {
            printf(BOLD"\nERROR: Color Index not allowed (chose between 0 and %d"RESET, max_color_1);
            return -1;
        }
        
        if (redChannel > max_color_2 || greenChannel > max_color_2 || blueChannel > max_color_2)
        {
            printf(BOLD"\nERROR: Color Index not allowed (chose between 0 and %d"RESET, max_color_2);
            return -1;
        }
        
/* ---- ALLOCATING MEMORY ---- */
        
        picture_1_Pointer = (struct picture *)malloc(width_pic_1*height_pic_1*sizeof(struct picture));
        
        picture_2_Pointer = (struct picture *)malloc(width_pic_2*height_pic_2*sizeof(struct picture));
        
        picture_edit_Pointer = (struct picture *)malloc(width_pic_1*height_pic_1*sizeof(struct picture));
        
        if (picture_1_Pointer == NULL || picture_2_Pointer == NULL || picture_edit_Pointer == NULL)
        {
            printf(BOLD"\nERROR: Could't allocate memory.\n"RESET);
            free(picture_1_Pointer);
            free(picture_2_Pointer);
            free(picture_edit_Pointer);
            return -1;
        }
        
/* ---- READ PIXELS ---- */
        
        printf(BOLD"\nImporting file....\n"RESET);
        
        for (i = 0; i < width_pic_1*height_pic_1; i++)
        {
            removecomment(FILE_1);
            
            fscanf(pFin_1, "%u", &(picture_1_Pointer+i)->r);
            fscanf(pFin_1, "%u", &(picture_1_Pointer+i)->g);
            fscanf(pFin_1, "%u", &(picture_1_Pointer+i)->b);
        }
        
        for (i = 0; i < width_pic_2*height_pic_2; i++)
        {
            removecomment(FILE_2);

            fscanf(pFin_2, "%u", &(picture_2_Pointer+i)->r);
            fscanf(pFin_2, "%u", &(picture_2_Pointer+i)->g);
            fscanf(pFin_2, "%u", &(picture_2_Pointer+i)->b);
        }
        
        printf(BOLD"Done file Import!\n\n"RESET);
        
        
/*------------------------------------------------------------------*/
/* A L G O R I T H M   1   -   C O L O R S                          */
/*------------------------------------------------------------------*/
        if (second_file == 1)
        {
            printf(BOLD"Creating Picture 1\n\n"RESET);
        }
        
        printf(BOLD"Checking Color Channels...\n"RESET);
        
        threshold = (threshold_percent * max_color_1) / 100;
        
#if DEBUG
        printf(RED"THRESHOLD: %d\n"RESET, threshold);
        printf(RED"THRESHOLD%%: %f%%\n"RESET, threshold_percent);
#endif
    
        for (i = 0; i < width_pic_1*height_pic_1; i++)
        {
            diffcolor = (((picture_1_Pointer+i)->r + (picture_1_Pointer+i)->g + (picture_1_Pointer+i)->b) -
                         ((picture_2_Pointer+i)->r + (picture_2_Pointer+i)->g + (picture_2_Pointer+i)->b));
            
            diffcolor = abs(diffcolor);
            //diffcolor = diffcolor&0x7FFFFFFF;
            
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
        
        printf(BOLD"Done Checking Color Channels!\n\n"RESET);
        
/* ---- WRITE HEADER INTO OUTPUT FILE ---- */
        
        printf(BOLD"Writing file....\n"RESET);
        
        fprintf(pFout_1, "P3\n");
        fprintf(pFout_1, "#Color Image Subtraction by Sebastian Dichler\n");
        fprintf(pFout_1, "%u %u\n", width_pic_1, height_pic_1);
        fprintf(pFout_1, "%u\n", max_color_1);
        
/* ---- WRITING PIXELS INTO OUTPUT FILE ---- */
        
        for (i = 0; i < width_pic_1*height_pic_1; i++)
        {
            fprintf(pFout_1, "%u %u %u\n",
                    (picture_edit_Pointer+i)->r, (picture_edit_Pointer+i)->g, (picture_edit_Pointer+i)->b);
        }
        
        printf(BOLD"Done writing file!\n\n"RESET);
        
/*------------------------------------------------------------------*/
/* A L G O R I T H M   2   -   B L A C K                            */
/*------------------------------------------------------------------*/
        
        if (second_file == 1)
        {
            printf(BOLD"Creating Picture 2\n\n"RESET);

            printf(BOLD"Checking Color Channels...\n"RESET);
            
            for (i = 0; i < width_pic_1*height_pic_1; i++)
            {
                diffcolor = (((picture_1_Pointer+i)->r + (picture_1_Pointer+i)->g + (picture_1_Pointer+i)->b) -
                             ((picture_2_Pointer+i)->r + (picture_2_Pointer+i)->g + (picture_2_Pointer+i)->b));
                
                diffcolor = abs(diffcolor);
                //diffcolor = diffcolor&0x7FFFFFFF;
                
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
            
            printf(BOLD"Done Checking Color Channels!\n\n"RESET);
            
/* ---- WRITE HEADER INTO OUTPUT FILE ---- */
            
            printf(BOLD"Writing file....\n"RESET);
            
            fprintf(pFout_2, "P3\n");
            fprintf(pFout_2, "#Color Image Subtraction by Sebastian Dichler\n");
            fprintf(pFout_2, "%u %u\n", width_pic_1, height_pic_1);
            fprintf(pFout_2, "%u\n", max_color_1);
            
/* ---- WRITING PIXELS INTO OUTPUT FILE ---- */
            
            for (i = 0; i < width_pic_1*height_pic_1; i++)
            {
                fprintf(pFout_2, "%u %u %u\n",
                        (picture_edit_Pointer+i)->r, (picture_edit_Pointer+i)->g, (picture_edit_Pointer+i)->b);
            }
            
            printf(BOLD"Done writing file!\n\n"RESET);
        }
    }
    else
    {
        printf(BOLD"\nERROR: Not supported data format.\n"RESET);
    }
    
    free(picture_1_Pointer);
    free(picture_2_Pointer);
    free(picture_edit_Pointer);
    fclose(pFin_1);
    fclose(pFin_2);
    fclose(pFout_1);
    if (second_file == 1)
    {
        fclose(pFout_2);
    }
    return 0;
}
