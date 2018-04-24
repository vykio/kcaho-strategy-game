#include <stdio.h>

struct threeNum
{
   int n1, n2, n3;
   char name[15];
};

int main()
{
   //int n;
   struct threeNum num;
   FILE *fptr;

   if ((fptr = fopen("test.bin","rb")) == NULL){
       printf("Error! opening file");

       // Program exits if the file pointer returns NULL.
       exit(1);
   }

   fread(&num, sizeof(struct threeNum), 1, fptr);
   printf("n1: %d\tn2: %d\tn3: %d\tname: %s\n", num.n1, num.n2, num.n3, num.name);

   fclose(fptr);

   return 0;
}
