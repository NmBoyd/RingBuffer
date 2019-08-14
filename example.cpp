#include "RingBuffer/RingBuffer.h"
#include <unistd.h>
int main()
{
    RingBuffer<int> buffer(10);
    int tmp =12;
    int count = 0;
    for(uint16_t i=0;i<20;i++) // create a list that overflows to 100
    {
        int tmp = count;
        // buffer.PrintDebug();
        buffer.Put(tmp);
        printf("added: %i\n", tmp);
        if(buffer.IsFull())
        {
            printf("Getting: %i \n", buffer.Pull());
        }
        count++;
    }
    printf("Emptying buffer");
    while (!buffer.Empty())
    {
        int pulled = buffer.Pull();
        printf("Got %d\n", pulled);
    }
    
    return 0;
}