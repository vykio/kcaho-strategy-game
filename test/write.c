#include <stdio.h>

struct threeNum
{
   int n1, n2, n3;
   char name[15];
};

int main()
{
   int n;
   struct threeNum num;
   FILE *fptr;

   if ((fptr = fopen("test.bin","wb")) == NULL){
       printf("Error! opening file");

       // Program exits if the file pointer returns NULL.
       exit(1);
   }


   num.n1 = 1;
   num.n2 = 2;
   num.n3 = 2;
   num.name = "CHAT";
   fwrite(&num, sizeof(struct threeNum), 1, fptr);

   fclose(fptr);

   return 0;
}
