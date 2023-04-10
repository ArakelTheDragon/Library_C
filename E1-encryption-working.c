/*
The example is working. Implementing a simple encryption method. The professor told me that this is the encryption banks use with a longer public key (??? publicly known how long it is). In the example that he gave though, we were allocating memory with "malloc()" or "calloc()" (I thinkg it was "callock()". He was indexing both the file and the key with a counter (i,z), I think the same counter for the file and key (i for the the file and key) 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declare function before it's called
void Encrypt(char * FILENAME, char * NEW_FILENAME);

int main ()
{
    char f1[100];   // array big enough to hold expected input
    char f2[100];   // array big enough to hold expected input

    printf ("Please enter file for encryption\n");
    scanf  ("%99s", f1);    // set max characters to read to prevent buffer overflow
    printf ("Please enter the name of the file after encryption\n");
    scanf  ("%99s", f2);

    Encrypt(f1, f2);
    return 0;
}

void Encrypt(char * FILENAME, char * NEW_FILENAME)
{
         printf("Encryption started\n");
         FILE *inFile;
         FILE *outFile;
         char key[50];         // array big enough to hold expected input, dont give "char *" to scanf with "%s"
         char process[50];     // array big enough to hold expected input
         int keylen, keyidx;

         int Byte;       // fgetc returns int, so use an int for "Byte" and "newByte"
         int newByte;

         printf ("Please enter 'encryption or decryption'\n");
         scanf  ("%49s", process);
         printf ("Please enter the key\n");
         scanf  ("%49s", key);
         keylen = strlen(key);
         keyidx = 0;    // starting index into key
         printf("Opening files\n");

         inFile = fopen(FILENAME,"rb");
         outFile = fopen(NEW_FILENAME, "w");

         if(inFile == NULL)
         {
            printf("Error: Can't Open inFile\n");
         }
         else if(outFile == NULL)
         {
            printf("Error: Can't open outFile\n");
         }
         else
         {
                 printf("File Opened, Encrypting\n");

                 while((Byte = fgetc(inFile)) != EOF)    // read a byte, check if EOF, "EOF" is outside the range of "char most times"
                 {
                         if (!strcmp(process,"encryption")) /* ??? "strmcp()" compares (process) a string to(encryption) a string */
                         {
                                 newByte = Byte + key[keyidx];    // use key index
                                 if (newByte > 255) newByte -= 256;   // check for overflow
                         }
                         else if (!strcmp(process,"decryption"))
                         {
                                 newByte = Byte - key[keyidx];
                                 if (newByte < 0) newByte += 256;
                         }
                         else
                         {
                            newByte = Byte;
                         }
                         fputc(newByte, outFile);
                         keyidx++;
                         // loop to the start of the key if needed
                         if (keyidx >= keylen) keyidx = 0;
                 }
         }
         if (inFile != NULL) fclose(inFile);     // close your files when you're done
         if (outFile != NULL) fclose(outFile);
}
