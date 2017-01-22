#H1-Image-Subtraction
IMAGE SUBTRACTION @ v1.0  
Created by Sebastian Dichler, 2017  
Format input:	subtract.out <-f FILENAME> <-s FILENAME> <-o FILENAME>  

#####Required Parameters:  
<pre>
<-f FILENAME>   It specifies the first input file.
<-s FILENAME>   It specifies the second input file.
<-o FILENAME>   It specifies the output file. (colored)
</pre>

#####Optional Parameters:  
<pre>
[-n FILENAME]   It specifies the second output file. (black)
[-a NUMBER]     Change output algorithm 0 is default, 1 or 2
                Use 0 for changed object white(depends on RGB input) and everything else
                colored/black. (justcolor)
                Use 1 for changed object colored and everything else white
                (depends on RGB input)/black. (justcolor)
                Use 2 for changed object white(depends on RGB input) and everything else
                colored/black. (fullpixel)
[-t THRESHOLD]  Value between 0 and 100.
[-r RED]        Specify the RED color value for the difference area to be filled in.
[-g GREEN]      Specify the GREEN color value for the difference area to be filled in.
[-b BLUE]       Specify the BLUE color value for the difference area to be filled in.
[-h]            Prints a help message.
</pre>
