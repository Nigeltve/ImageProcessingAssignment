// Example : bit level access to a byte (char)

// Author: Toby Breckon, toby.breckon@cranfield.ac.uk
// License : GPL, http://www.gnu.org/copyleft/gpl.html

#include <stdlib.h>
#include <stdio.h>

unsigned char getBitN(unsigned char val, int n) {
   
     unsigned char b;
     b = 1 & (val >> n);
     return b;
 }

int mfain(int argc, char *argv[])
{

    // useful test values: 10101010 = 170, 01010101 = 85, 11111111 = 255
    // 11111111 = 255, 11110000 = 240, 00001111 = 15

    unsigned char testvalue = 170;
    int bit;

    printf("decimal value %i in binary is = ", testvalue);

    for(bit = 7; bit >= 0; bit--)
    {

      // get Nth 'bit' of integer 'testvalue'. 
      // (bits indexed as = 7->0 for left-to-right output)
    
      printf("%i", getBitN(testvalue, bit));
    }
    printf("\n");
}
