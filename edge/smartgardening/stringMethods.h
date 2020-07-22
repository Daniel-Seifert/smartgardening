#include <string.h>


typedef char Byte;
int getWord(WiFiSSLClient * api_client, Byte **word, unsigned * wordLength) {

   unsigned rf = 2;
   unsigned allocated_mem = 16;
   int c;
   *word = NULL;
   while((c = api_client->read()) != -1)
   {
      if (*wordLength == 0 || *wordLength >= allocated_mem/rf) 
         *word = (Byte * ) realloc(*word, sizeof(Byte)* (allocated_mem *= rf));


      if (!*word)
         return -1;


      (*word)[*wordLength] = (Byte)c;
      ++(*wordLength);
   }

   if (*wordLength > 0) {
      *word = (Byte*)realloc(*word, sizeof(Byte)*(*wordLength));
      (*word)[*wordLength] = 0;
   }
   return 0;
}
