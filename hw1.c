#include <stdio.h>

void FIFO(int string[], int num_frames, int pages)
{
    //Creating array for block storage
    int frames[num_frames];

    //Initializing each block with -1
    for (int i=0; i<num_frames; i++)
    {
        frames[i] = -1;
    }

    //Initializing dirty frame
    int dirty_frames[600];
    for(int i=0; i<600; i++)
    {
        if((i+1) % 2 == 0)  //if string value is even, set dirty = 1
            dirty_frames[i] = 1;
        else
            dirty_frames[i] = 0;
    }

    int index = -1; //Index to insert element
    int page_faults = 0;    //Counters for page_faults
    int disk_writes = 0;    //Counters for disk_writes
    int interrupts = 0;     //Counters for interrupts

    //Traversing each symbol in fifo
    for (int i=0; i<pages; i++)
    {
        int symbol = string[i];
        int flag = 0;

        for(int j=0; j<num_frames; j++)
        {
            if (symbol == frames[j])
            {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
        {
            index = (index+1) % num_frames;
            frames[index] = symbol;
            page_faults++;

            //Check value is dirty or not
            if(dirty_frames[frames[index]-1] == 1)
            {
                disk_writes++;
                dirty_frames[frames[index]-1] = 0;

                //Clean dirty bit trigger interrupt
                interrupts++;
                dirty_frames[frames[index]-1] = 1;
            }
        }
    }

    interrupts += page_faults;

    //Check the value in final frames is dirty or not
    for(int i=0; i<num_frames; i++)
    {
        if(dirty_frames[frames[i] -1] == 1)
            disk_writes++;
    }

    printf("\tPage faults: %d", page_faults);
    printf("\tDisk writes: %d", disk_writes);
    printf("\tInterrupts: %d \n", interrupts);
}

void Optimal(int string[], int num_frames, int pages)
{
    int index, max;
    int frames[num_frames];
    int temp[num_frames];

    int disk_writes = 0;    //Counters for disk_writes
    int page_faults = 0;    //Counters for page_faults
    int interrupts = 0;     //Counters for interrupts

    for(int i = 0; i < num_frames; i++)
    {
        frames[i] = -1;
    }

    //Initializing dirty frame
    int dirty_frames[600];
    for(int i=0; i<600; i++)
    {
        if((i+1) % 2 == 0)  //if string value is even, set dirty = 1
            dirty_frames[i] = 1;
        else
            dirty_frames[i] = 0;
    }

    for(int i = 0; i < pages; i++)
    {
        int flag1 = 0;
        int flag2 = 0;

        for(int j = 0; j < num_frames; j++)
        {
            if(frames[j] == string[i])  //string is already in the frames
            {
                flag1 = flag2 = 1;
                break;
            }
        }

        if(flag1 == 0)
        {
            for(int j = 0; j < num_frames; j++)
            {
                if(frames[j] == -1)
                {
                    frames[j] = string[i];
                    flag2 = 1;
                    page_faults++;
                    break;
                }
            }
        }

        if(flag2 == 0)
        {
            int flag3 = 0;
            for(int j = 0; j < num_frames; j++)
            {
                temp[j] = -1;
                for(int k = i + 1; k < pages; k++)
                {
                    if(frames[j] == string[k])  //if string is already in frames, find the position and set into temp
                    {
                        temp[j] = k;
                        break;
                    }
                }
            }

            for(int j = 0; j < num_frames; j++)
            {
                if(temp[j] == -1)   //if frames are not full
                {
                    index = j;
                    flag3 = 1;
                    break;
                }
            }

            if(flag3 == 0)  //if frames are full, decide which string should be replaced
            {
                max = temp[0];
                index = 0;

                for(int j = 1; j < num_frames; j++) //find maximum position in temp
                {
                    if(temp[j] > max)
                    {
                        max = temp[j];
                        index = j;
                    }
                }
            }

            frames[index] = string[i];
            page_faults++;

            //Check value is dirty or not
            if(dirty_frames[frames[index]-1] == 1)
            {
                //printf("\nreference string : %d , and dirty bit value : %d\n",frames[index],dirty_frames[frames[index] -1]);
                //scanf("%*c");
                disk_writes++;
                dirty_frames[frames[index]-1] = 0;

                //Clean dirty bit trigger interrupt
                interrupts++;
                dirty_frames[frames[index]-1] = 1;
            }

        }
    }

    interrupts += page_faults;

    //Check the value in final frames is dirty or not
    for(int i=0; i<num_frames; i++)
    {
        if(dirty_frames[frames[i] -1] == 1)
            disk_writes++;
    }
    printf("\tPage faults: %d", page_faults);
    printf("\tDisk writes: %d", disk_writes);
    printf("\tInterrupts: %d \n", interrupts);
}


void My_algorithm(int string[], int num_frames, int pages)
{
    //Creating array for block storage
    int frames[num_frames];

    //Initializing each block with -1
    for (int i=0; i<num_frames; i++)
    {
        frames[i] = -1;
    }

    //Initializing dirty frame
    int dirty_frames[600];
    for(int i=0; i<600; i++)
    {
        if((i+1) % 2 == 0)  //if string value is even, set dirty = 1
            dirty_frames[i] = 1;
        else
            dirty_frames[i] = 0;
    }

    //Create an array to how many times the value is hit
    int hits_frames[600];
    for(int i=0; i<600; i++)
        hits_frames[i] = 0;

    int index = -1; //Index to insert element
    int page_faults = 0; //Counters for page_faults
    int disk_writes = 0; //Counters for disk_writes
    int interrupts = 0;

    //Traversing each symbol in fifo
    for (int i=0; i<pages; i++)
    {
        int symbol = string[i];
        int flag = 0;

        for(int j=0; j<num_frames; j++)
        {
            if (symbol == frames[j])
            {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
        {
            /*
            printf("symbol : %d\n",symbol);
            for (int j = 0 ; j < num_frames ; j ++)
            {
                printf("%d ",frames[j]);

            }
            scanf("%*c");
            index = (index+1) % num_frames;

            printf("Now change frame member : %d\n",frames[index]);
            */

            index = (index+1) % num_frames;

            //Check value is dirty or not
            if(dirty_frames[frames[index]-1] == 1)
            {
                //printf("\nreference string : %d , and dirty bit value : %d\n",frames[index],dirty_frames[frames[index] -1]);
                //scanf("%*c");
                if(hits_frames[frames[index]-1] == 0 || hits_frames[frames[index]-1] == 1)
                {
                    hits_frames[frames[index]-1]++;
                }
                else
                {
                    hits_frames[frames[index]-1] = 0;

                    disk_writes++;
                    dirty_frames[frames[index]-1] = 0;

                    //Clean dirty bit trigger interrupt
                    interrupts++;
                    dirty_frames[frames[index]-1] = 1;
                }

            }
            frames[index] = symbol;
            page_faults++;

        }
    }

    interrupts += page_faults;

    //Check the value in final frames is dirty or not
    for(int i=0; i<num_frames; i++)
    {
        if(dirty_frames[frames[i] -1] == 1)
            disk_writes++;
    }

    printf("\tPage faults: %d", page_faults);
    printf("\tDisk writes: %d", disk_writes);
    printf("\tInterrupts: %d \n", interrupts);
}


int summ = 0;
int rfstring[180000/30*40];

int* locality()
{
    int number = 180000;
    int subset_lower = number/40;
    int subset_upper = number/30;
    int count = 0;
    int sum = 0; //string length

    srand( time(NULL) );

    while(count != 40)  //generate 40 times
    {
        int subset_range = rand() % (subset_upper - subset_lower + 1) + subset_lower; //subset length range
        int lower = rand() % 100 + 1;
        int upper = 600;

        for(int i=0; i<(sum + subset_range); i++)
        {
            rfstring[i] = rand() % (upper - lower + 1) + lower;
        }

        sum += subset_range;
        count++;
    }

    summ = sum;
    return rfstring;
}

int* my_reference_string(int string[], int pages)
{
    int tmp;
    int my_rfstring[180000];

    for(int i=0; i<(pages/2); i++)
    {
        tmp = string[i];
        string[i] = string[pages-i-1];
        string[pages-i-1] = tmp;
    }

    return string;
}

int main()
{
    int random_min = 1, random_max = 600, Number_of_memory_references = 180000;
    int Random_string[Number_of_memory_references];

    srand( time(NULL) );

    for(int i=0; i<Number_of_memory_references; i++)
    {
        Random_string[i] = rand() % random_max + 1; //%600+1
    }

    printf("\n");

    int Pages = sizeof(Random_string)/sizeof(Random_string[0]);   //reference string length

    //Random
    printf("Random:\n");

    printf("FIFO\n");
    for(int Frames=10; Frames<=100; Frames+=10)
    {
        printf("Frames = %d ", Frames);
        FIFO(Random_string, Frames, Pages);
    }

    printf("\nOptimal\n");
    for(int Frames=10; Frames<=100; Frames+=10)
    {
        printf("Frames = %d ", Frames);
        Optimal(Random_string, Frames, Pages);
    }

    printf("\nMy algorithm\n");
    for(int Frames=10; Frames<=100; Frames+=10)
    {
        printf("Frames = %d ", Frames);
        My_algorithm(Random_string, Frames, Pages);
    }

    //Locality
    int* Locality_string = locality();

    printf("\nLocality:\n");

    printf("FIFO\n");
    for(int Frames=10; Frames<=100; Frames+=10)
    {
        printf("Frames = %d ", Frames);
        FIFO(Locality_string, Frames, summ);
    }

    printf("\nOptimal\n");
    for(int Frames=10; Frames<=100; Frames+=10)
    {
        printf("Frames = %d ", Frames);
        Optimal(Locality_string, Frames, summ);
    }

    printf("\nMy algorithm\n");
    for(int Frames=10; Frames<=100; Frames+=10)
    {
        printf("Frames = %d ", Frames);
        My_algorithm(Locality_string, Frames, Pages);
    }

    //My reference string
    printf("\nMy reference string:\n");
    int* mystring = my_reference_string(Random_string, Pages);

    printf("FIFO\n");
    for(int Frames=10; Frames<=100; Frames+=10)
    {
        printf("Frames = %d ", Frames);
        FIFO(mystring, Frames, Pages);
    }

    printf("\nOptimal\n");
    for(int Frames=10; Frames<=100; Frames+=10)
    {
        printf("Frames = %d ", Frames);
        Optimal(mystring, Frames, Pages);
    }

    printf("\nMy algorithm\n");
    for(int Frames=10; Frames<=100; Frames+=10)
    {
        printf("Frames = %d ", Frames);
        My_algorithm(mystring, Frames, Pages);
    }

    return 0;
}
