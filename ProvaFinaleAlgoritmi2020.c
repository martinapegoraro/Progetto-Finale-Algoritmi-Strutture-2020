#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 1025

///////////////////////////////////////GLOBAL VARIABLES///////////////////////////////////////////////////////////////

typedef struct {

    int in1;
    int in2;
    int lengthbefore;
    int lengthafter;
    char comm;
    int changelen;
    char **changeundo;
    char **changeredo;

} State;
char **output = NULL;  //output is the output string
int outlen = 1 ; //lenght of the output array
int freeplaces = 0; //keeps count of the number of cells already allocated in the array, not to call the realloc too many times


State **memory = NULL; //memory memorizes the output for undo and redo functions
int memorylen = 0; //length of the memory array
int freemem = 0; // free places in the states array
int undo = 0; //number of redo to do
int oldundo = 0; //keeps in memory how many times I can do the redo
int redo = 0; //number of undo to do
int lastaccess = 1 ; //keep track of the last access to the memory array USED FOR THE REDO
int firstchange = 1; //boolean
int firstprint = 1;
int firstdelete = 1;
int therewasanundo = 0; // boolean
////////////////////////////////////////////FUNCTIONS///////////////////////////////

int comma( char* string)
{
    int length = strlen(string);
    for(int i=0; i < length; i++)
    {
        if(string[i]==',') return i;

    }
}


void cFunction(int ind1, int ind2)
{
    int numLines; //number of fgets to read the entire text
    char buffer[MAX_SIZE];
    int i, j;
    int  oldoutlen;


    numLines = ind2 - ind1 + 1;

    i = ind1 - 1 ;
    j = 0;

    if (oldundo != 0)
    {
        freemem = freemem + oldundo;
        memorylen = memorylen - oldundo;
        for(int k= memorylen; k < memorylen + oldundo; k++)
        {
            for(int q = 0; q < memory[k]->changelen; q++)

            {
                memory[k]->changeundo[q]= NULL;
                free(memory[k]->changeundo[q]);
                memory[k]->changeredo[q]= NULL;
                free(memory[k]->changeredo[q]);

            }
        }
        oldundo = 0;
    }
    if(freemem > 0 );
    if(freemem == 0 || firstchange == 1)
    {
        memory = (State**)realloc(memory, sizeof(State*) * memorylen + 1);

    }

    memory[memorylen] = (State*) malloc( sizeof(State));

    memory [memorylen]->in1 = ind1;
    memory[memorylen ]->in2 = ind2;
    memory[memorylen ]->comm = 'c';
    memory[memorylen]->changelen = numLines;
    memory[memorylen]->changeundo = (char**) malloc( sizeof(char*) * (numLines ));
    memory[memorylen]->changeredo = (char**) malloc(sizeof(char*)* (numLines));

    if (ind1 <=  outlen + 1) //the change can take place just if the position in the array
    {                        // already exists or if the new line is the next position in the array

        if (firstchange == 1)
        {
            oldoutlen = 0;
        }
        else oldoutlen = outlen ;
        memory[memorylen ]->lengthbefore = oldoutlen ;

        if (freeplaces == 0 && ind2 > outlen || firstchange == 1) {

            if (ind1 > outlen) freeplaces = numLines;
            if (ind1 == outlen && firstchange == 0) freeplaces = freeplaces + numLines - 1;
            if (ind1 == outlen && firstchange == 1 ) freeplaces = freeplaces + numLines;
            if ((ind1 + numLines) < outlen) freeplaces = freeplaces;
            if (ind1 >= outlen || (ind1 + numLines) > outlen) outlen = ind1 + numLines - 1;
            if ((ind1 + numLines) < outlen) outlen = outlen;
            if (firstchange==1) output = (char **) malloc(sizeof(char *) * (outlen ));
            else output = (char **) realloc(output, sizeof(char *) * (outlen));
        }
        else
        {
            if (ind1 >= outlen || (ind1 + numLines) > outlen) outlen = ind1 + numLines - 1;
            if ((ind1 + numLines) < outlen) outlen = outlen;

        }


        memory[memorylen ]->lengthafter = outlen;


        while (numLines > 0 && i <= (ind2 - 1)) {

            if (fgets(buffer, 1025, stdin) != NULL);
            {
                memory[memorylen]->changeundo[j]= output[i];

                if (i <= oldoutlen - 1)//per sovrascrivere
                {
                    output[i]= NULL;
                    free(output[i]);
                    freeplaces++;
                }



                output[i] = strdup(buffer); //the first line of the dynamic array has index 0

                memory[memorylen]-> changeredo[j] = output[i];
                //output[i] = malloc(strlen(buffer) + 1 );
                //strcpy(output[i], buffer);
                i++;
                j++;
                numLines--;
                freeplaces--;
                if (freeplaces == 0 && i > oldoutlen - 1 && i < ind2)
                {
                    output = (char **) realloc(output, sizeof(char *) * (outlen ));
                    freeplaces = outlen - i;
                }

            }
        }
        memorylen++;
        if (freemem!= 0)freemem --;
    }
    else outlen = outlen;
    firstchange = 0;
    lastaccess = memorylen;
}



void pFunction(int ind1, int ind2)
{
    int i = 0;


    if(outlen == 0 || ind1 > outlen || firstchange == 1)
    {
        while(i <= ind2 - ind1)
        {
            fputs(".\n", stdout);
            i++;
        }
        if (firstprint == 1) firstprint = 0;

    }

    else
    {
        if (ind1 == 0 && ind2 != 0)
        {
            fputs(".\n", stdout);
            ind1 = 1;
        }
        if (ind1 == 0 && ind2 == 0)    fputs(".\n", stdout);
        else
        {
            i = ind1-1;

            if (ind2 > outlen)
            {
                while(i < outlen)
                {
                    fputs( output[i], stdout);
                    i++;
                }
                i = outlen -1;
                while (i < ind2 -1)
                {
                    fputs(".\n", stdout);
                    i++;
                }
            }
            else
            {
                while(i < ind2)
                {
                    fputs(output[i], stdout);
                    i++;
                }
            }

        }
    }

    firstprint = 0;

}

void dFunction (int ind1, int ind2)
{
    int counter = 0;
    int i ;
    int k = 0;

    if (oldundo != 0)
    {
        freemem = freemem + oldundo;
        memorylen = memorylen - oldundo;
        oldundo = 0;
    }
    if(freemem > 0);
    else
    {
        memory = realloc(memory, sizeof(struct State*) * memorylen + 1);

    }
    memory[memorylen] = (State*) malloc(sizeof(State));
    memory[memorylen]->in1 = ind1;
    memory[memorylen]->in2 = ind2;
    memory[memorylen]->comm = 'd';
    if (firstdelete == 1) memory[memorylen ]->lengthbefore = outlen - 1;
    else memory[memorylen ]->lengthbefore = outlen;
    memory[memorylen]->changelen = ind2 - ind1 + 1;
    memory[memorylen ]->changeundo = (char**) malloc( sizeof(char*) * (ind2 - ind1 + 1));
    memory[memorylen ]->changeredo = (char**) malloc( sizeof(char*) * (ind2 - ind1 + 1));

    if(ind1 > outlen || outlen == 0 || firstchange == 1)
    {
        for(int q = 0; q < ind2 - ind1 + 1; q++ )
        {
            memory[memorylen]->changeundo[q]= NULL;
            memory[memorylen]->changeredo[q]= NULL;
        }
        freeplaces = freeplaces;
        counter = 0;
    }
    if (ind1 == 0 && ind2 == 0) counter = 0;
    if (ind1== 0 && ind2 != 0)  ind1 = 1;
    if(ind1<= outlen && ind2 > outlen && ind1 != 0 )
    {

        int j  = 0;

        for(int i = ind1 -1; i < ind2 && i < outlen ; i++)
        {


            counter++;
            memory[memorylen]->changeundo[j]= output[i];
            memory[memorylen]->changeredo[j]= NULL;
            output[i] = NULL;
            freeplaces++;
            j++;

            //free(output [i]);
        }
    }
    if (ind1 <= outlen && ind2 <= outlen && ind1 != 0  && ind2 != 0)
    {

        i = 0;
        int q = 0;

        while ( i < ind2 - ind1 + 1) //&& k < outlen - ind2 )
        {
            if (output[ind1 - 1 + i] == NULL)
            {
                memory[memorylen]->changeundo[q] = NULL;
                memory[memorylen]->changeredo[q] = NULL;
            }
            else
            {
                memory[memorylen]->changeundo[q] = output[ind1 - 1 + i];
                memory[memorylen]->changeredo[q] = output[ind1 - 1 + i];
                freeplaces++;
                counter++;
            }
            output[ind1 - 1 + i] = NULL;
            //free(output[i]);
            //counter++;
            i++;
            q ++;

        }

        i = 0;
        while (k < outlen - ind2)
        {
            if (output[ind2 + k] != NULL)
            {
                output[ind1 - 1 + i ] = output[ind2 + k];
                output[ind2 + k] = NULL;
                //free(output[ind2 + k]);

            }
            //else counter --;

            i++;
            k++;

        }
    }


    firstdelete = 0;
    outlen = outlen - counter;
    memory[memorylen]-> lengthafter = outlen;
    memorylen++;
    if (freemem!= 0)freemem--;
    lastaccess = memorylen ;

}


void urFunction()
{
    int numoperations;
    int start ;
    int end ;


    numoperations = undo - redo;
    if (numoperations < 0 || numoperations == 0)
    {
        redo = redo - undo;
        undo = 0;
        if (lastaccess - numoperations > memorylen) redo = memorylen - lastaccess;


    }

    if (redo!= 0 && undo == 0  && oldundo!= 0) //JUST redo
    {

        numoperations = redo;

        while (numoperations > 0)
        {


            start = memory[lastaccess]->in1;
            end = memory[lastaccess]->in2;

            if(memory[lastaccess]->comm == 'd')
            {
                if (start== 0 && end != 0)  start = 1;
                if(start<= outlen && end > outlen && start != 0 )
                {

                    for(int i = start - 1; i < outlen ; i++)
                    {
                        output[i] = NULL;
                        freeplaces++;
                    }
                }
                if (start < outlen && end <= outlen && start != 0 && end != 0)
                {

                    int i = 0;
                    int k = 0;
                    while ( i < end - start + 1)
                    {

                        output[start - 1 + i] = NULL;
                        //free(output[start - 1 +i]);
                        freeplaces++;
                        i++;
                    }

                    i = 0;
                    while (k < outlen - end)
                    {
                        if (output[end + k] != NULL)
                        {


                            output[start - 1 + i] = output[end + k];
                            output[end + k] = NULL;
                            //free(output[end + k]);

                        }
                        //else counter --;
                        i++;
                        k++;

                    }
                }
                if(start == 1 && end == outlen )
                {

                    for(int i = 0; i < outlen; i++)
                    {

                        output[i] = NULL;
                        //free(output[i]);
                    }
                }
            }
            if(memory[lastaccess]->comm == 'c')
            {
                int j = 0;
                while(start -1 < end)
                {
                    if(output[start - 1]== NULL)freeplaces--;
                    output[start - 1] = memory[lastaccess] -> changeredo[j];

                    start++;
                    j++;
                }

            }
            outlen = memory[lastaccess] -> lengthafter;
            numoperations--;
            if (lastaccess  == memorylen );
            else lastaccess ++;
            oldundo--;

        }

    }
    else  //for undo and redo or just undo
    {
        numoperations = undo - redo;
        if (lastaccess == 0) numoperations = 0;
        if (numoperations >= memorylen ) numoperations = memorylen - oldundo;
        if (oldundo + numoperations == memorylen)
        {
            for (int q = outlen - 1; q >= 0; q--  )
            {
                output[q] = NULL;
                freeplaces++;
            }
            oldundo = oldundo + numoperations;
            outlen = 0;
            lastaccess = 0;
        }
        else {

            if (numoperations != 0)lastaccess--;

            if (lastaccess < 0) lastaccess = 0;
            oldundo = oldundo + numoperations;


            if (oldundo > memorylen) oldundo = memorylen;

            while (numoperations > 0)
            {
                start = memory[lastaccess]->in1;
                end = memory[lastaccess]->in2;
                int i = 0;

                if(start== 0) start = 1;

                if (memory[lastaccess]->comm == 'd')
                {


                    if (start > outlen)
                    {
                        while (start - 1 < memory[lastaccess]->lengthbefore)
                        {

                            output[start - 1] = memory[lastaccess]->changeundo[i];
                            if (memory[lastaccess]->changeundo[i] != NULL) freeplaces--;
                            start++;
                            i++;
                        }
                    }
                    if (start <= outlen && (outlen - start + 1) > (end - start + 1)) //pezzi da spostare > pezzi da aggiungere
                    {
                        int q = 0;
                        while (outlen + end - start - q >= end) //traslo tutto
                        {
                            if(output[outlen + end - start - q] ==NULL) output[outlen + end - start - q] = strdup(output[outlen - 1 - q]);
                            else output[outlen + end - start - q] = output[outlen - 1 - q];

                            q++;

                        }
                        start = memory[lastaccess]->in1;
                        if(start== 0) start = 1;

                        while (start - 1 < end) // copio tutto
                        {
                            if (memory[lastaccess]->changeundo[i] == NULL);
                            if(memory[lastaccess]->changeundo[i] != NULL)freeplaces--;
                            output[start - 1] = memory[lastaccess]->changeundo[i];
                            i++;

                            start++;
                        }

                    }
                    if (start <= outlen && (outlen - start + 1) <= (end - start + 1)) {
                        int j = end; // variable to transpose elements

                        start = memory[lastaccess]->in1;
                        if(start== 0) start = 1;

                        while (j <= (j + outlen - start)) //traslo tutto
                        {
                            output[j] = output[start - 1];
                            start++;
                            j++;

                        }
                        start = memory[lastaccess]->in1;
                        if(start== 0) start = 1;

                        while (start - 1 < end) //copio tutto
                        {
                            output[start - 1] = memory[lastaccess]->changeundo[i];
                            if(memory[lastaccess]->changeundo[i]!= NULL) freeplaces--;
                            start++;
                            i++;
                        }
                    }
                    outlen = memory[lastaccess]->lengthbefore;

                }
                if (memory[lastaccess]->comm == 'c') {


                    while (start - 1 < end)
                    {

                        output[start - 1] = memory[lastaccess]->changeundo[i];
                        if(memory[lastaccess]->changeundo[i]== NULL) freeplaces++;
                        start++;
                        i++;
                    }

                    outlen = memory[lastaccess]->lengthbefore;

                }

                numoperations--;
                if(numoperations > 0) lastaccess--;

            }

        }

    }

    undo = 0;
    redo = 0;
}

////////////////////////////////////////////////////MAIN/////////////////////////////////////////////////////////////

int main()
{
    char buffer[MAX_SIZE];  //buffer is used to parse the input
    int len;  //value used to measure the lengthbefore of the buffer
    int ind1; //used to memorize the start line
    int ind2; //used to memorize the finish line
    char command; //used to save the letter that identifies the command

    /*if (freopen("progettoAPI.txt", "r", stdin) ==NULL)
    {
        printf("error in file opening");
    }*/


    //output = (char**)malloc(sizeof(char*)) ;


    while(fgets(buffer, 1025, stdin) != NULL)
    {
        /*if (i>= 100) break;
        output[i] = malloc(strlen(buffer) + 1);
        strcpy(output[i],buffer);
        printf("%s", buffer);
        i++;
        printf("%s", output[1]);*/

        len = strlen(buffer);

        command = buffer[len-2];
        ind1 = atoi(buffer);

        if (ind1 >= 0)
        {
            ind2 = atoi(buffer + comma(buffer) +1);

        }
        //printf("%d,%d,%c\n", ind1,ind2, command);


        if(buffer[0] == '.') continue;
        if (buffer[0] == 'q' && command == 'q')
        {
            return 0;
        }

        if (command == 'r')
        {
            redo = redo + ind1;
        }

        if (command == 'u')
        {
            undo = undo + ind1;
            therewasanundo = 1;
            //if (lastaccess == 0 && redo != 0 ) undo = undo + ind1;
            //else if (lastaccess == 0 && redo == 0 ) oldundo = ind1;
            //else undo = undo + ind1;
        }

        if (command != 'u' && command!= 'r' && (undo!= 0 || redo != 0))
        {
            therewasanundo = 0;
            urFunction();
        }


        if (undo - redo + oldundo > memorylen)
        {
            undo = memorylen - oldundo;
            redo = 0;
        }
        if (undo != 0 && redo == 0 && lastaccess == 0 ) undo = 0;
        if (redo != 0 && undo == 0  && therewasanundo == 0 && oldundo == 0) redo = 0;
        if (redo != 0 && undo == 0  && therewasanundo == 1) therewasanundo = 0;
        if (oldundo + undo - redo < 0) redo = undo + oldundo;
        if (undo == redo)
        {
            undo = 0;
            redo = 0;
        }

        if (command == 'c')
        {

            cFunction(ind1,ind2);
        }

        if (command == 'p')
        {
            pFunction(ind1, ind2);
        }

        if (command == 'd')
        {

            dFunction(ind1, ind2);

        }



    }

}

