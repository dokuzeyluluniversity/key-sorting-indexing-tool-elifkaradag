#include <json/json.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <fcntl.h> //file control options
#include <sys/types.h>
#include <sys/stat.h>


struct _Index {
	void* key;
	int RRN; //relative record numbers;	
};
typedef struct _Index Index;

    void jsonread(FILE *fp);
    struct json_object *parsed_json;
	struct json_object *FileName;
	struct json_object *indexName;
	struct json_object *recordLength;
	struct json_object *keyEncoding;
	struct json_object *keyStart;
    struct json_object *keyEnd;
    struct json_object *order;

void jsonread(FILE *fp)
{
   
   // jf= fopen("ptr.json", "r");
	
	char buffer[1024];
   
	size_t i;	
	//fp = fopen(fp,"r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "dataFileName", &FileName);
	json_object_object_get_ex(parsed_json, "indexFileName", &indexName);
	json_object_object_get_ex(parsed_json, "recordLength", &recordLength);
    json_object_object_get_ex(parsed_json, "keyEncoding", &keyEncoding);
    json_object_object_get_ex(parsed_json, "recordLength", &recordLength);
    json_object_object_get_ex(parsed_json, "keyStart", &keyStart);
    json_object_object_get_ex(parsed_json, "keyEnd", &keyEnd);
    json_object_object_get_ex(parsed_json, "order", &order);

	//printf("Name: %s\n", json_object_get_string(FileName));
	//printf("Age: %d\n", json_object_get_int(keyStart));

}

int compareKey(const void* a, const void* b)//ASC with BIN
{
    const int *ia = (const int *)a; // casting pointer types 
    const int *ib = (const int *)b;
    return *ia  - *ib;
   // return ((int*)a > (int*)b) ? 1 : (((int*)a < (int*)b) ? -1 : 0);
	/* integer comparison: returns negative if b > a 
	and positive if a > b */ 
}
int compareKey1(const void* a, const void* b)//DSC with BIN
{
    const int *ia = (const int *)a; // casting pointer types 
    const int *ib = (const int *)b;
    return  *ib - *ia;
}
int cstring_cmp(const void *a, const void *b) //UTF - CHR ASC SORT
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
	/* strcmp functions works exactly as expected from
	comparison function */
}
int cmp(const void *p, const void *q) {//UTF-CHR DSC SORT
     char* const *pp = p;
     char* const *qq = q;
     return -strcmp(*pp, *qq);
}

void createIndexFileBinary(int startkey,int endkey,int recordLengt,const char* keyencoding,const char* order)
{
	FILE* fp;
    FILE* fp1;//index file

	fp = fopen("sample (1).dat", "rb");
	if(!fp)
		return;
	int i=0;
   
    fseek(fp,0,SEEK_END);
    int REC_COUNT=(ftell(fp)/recordLengt);//TOPLAM ELEMAN SAYISI!!!!!!!!!!!!!!!
    //printf("%d",REC_COUNT);
    Index indexes[REC_COUNT];
	//char ptr[REC_COUNT][REC_SIZE+1];//sample.dat dosyası burda satır satır
   // int data;
    char *data2;
    int data;
    int j;

	fseek(fp, 0, SEEK_SET);
    //fread(&ptr[k], REC_SIZE, 1, fp);
    for(j=0;j<REC_COUNT;j++)
    {
        fseek(fp, startkey+(recordLengt*j), SEEK_SET);
        int number=(endkey-startkey)+1;
       // fread(&data,number, 1, fp);
       // printf("%d \ndata:",data);

        if(strcmp(keyencoding,"BIN")==0){
            
            fread(&data,number, 1, fp);
            indexes[j].key=(malloc(sizeof(Index)));
            *(int*)(indexes[j].key)=data;
            indexes[j].RRN=(j+1)*recordLengt;
           // printf("%d-%d\n",*(int *)indexes[j].key,indexes[j].RRN);
        }
        else if(strcmp(keyencoding,"CHR")==0){
          // char data;
           data2 = malloc(sizeof(char)*number);
           fread(data2,number, 1, fp);
          // printf("%s",data2);

           indexes[j].key=(malloc(sizeof(char)*number));
           strcpy(indexes[j].key,data2);
           //(char*)(indexes[j].key)=data1;
           indexes[j].RRN=(j+1)*recordLengt;
          // printf("%s-%d\n",indexes[j].key,indexes[j].RRN);
        }
        else if(strcmp(keyencoding,"UTF")==0){
           data2 = malloc(sizeof(char)*number);
           fread(data2,number, 1, fp);
          // printf("%s",data2);

           indexes[j].key=(malloc(sizeof(char)*number));
           strcpy(indexes[j].key,data2);
           //(char*)(indexes[j].key)=data1;
           indexes[j].RRN=(j+1)*recordLengt;
          // printf("%s-%d\n",indexes[j].key,indexes[j].RRN);
        }
        // printf("%d-%d\n",*(int *)indexes[j].key,indexes[j].RRN);
    }
   // printf("%d-%d\n",*(int *)indexes[5].key,indexes[5].RRN);
    if(strcmp(keyencoding,"BIN")==0)//BIN ise ona göre sırala
    {
        if(strcmp(order,"ASC")==0){
           qsort(indexes, REC_COUNT, sizeof(Index), compareKey);//ASC sıralama
        }
        else{
            qsort(indexes, REC_COUNT, sizeof(Index), compareKey1);//DSC sıralama
           /* for(int k=0;k<REC_COUNT;k++){
                printf("%d-%d\n",*(int *)indexes[k].key,indexes[k].RRN);
                
            }*/
        }
    }
    else if(strcmp(keyencoding,"CHR")==0)//CHR ise ona göre sırala
    {
        if(strcmp(order,"ASC")==0){
             qsort(indexes, REC_COUNT, sizeof(Index), cstring_cmp);//ASC sıralama
           /* for(int k=0;k<REC_COUNT;k++){
                printf("%s-%d\n",indexes[k].key,indexes[k].RRN);
                
            }*/
        }
        else{
            qsort(indexes, REC_COUNT, sizeof(Index), cmp);//DSC sıralama
            /* for(int k=0;k<REC_COUNT;k++){
                printf("%s-%d\n",indexes[k].key,indexes[k].RRN);
                
            }*/
        }

    }
    else//UTF ise ona göre sırala
    {
        if(strcmp(order,"ASC")==0){
            qsort(indexes, REC_COUNT, sizeof(Index), cstring_cmp);//ASC sıralama
           //printf("%s-%d\n",(char*)indexes[1].key,indexes[1].RRN);
        }
        else{
            qsort(indexes, REC_COUNT, sizeof(Index), cmp);//DSC sıralama
        }

    }

	fp1 = fopen("indexes.ndx", "wb");
	if(!fp1)
		 return;
	fseek(fp1, 0, SEEK_SET);
	fwrite(indexes, sizeof(Index), REC_COUNT, fp1);
	fclose(fp1);  
    fclose(fp);

}
int binaryFileSearch(FILE* fp, void* name1, int firstIdx, int lastIdx)
{
	printf("\nRunning Binary Search %d %d\n", firstIdx, lastIdx);
	Index first, last, middle;
	int returnData;
	
	// Calculate the middle Index
	int middleIdx = (firstIdx+lastIdx)/2;

	// Read first record and return if it is the searched one.
	fseek(fp, firstIdx*(sizeof(Index)), SEEK_SET);
	fread(&first, sizeof(Index), 1, fp);
	//if(strcmp(first.key,name1) == 0)
    if(first.key==name1)
	{
		returnData=first.RRN;
		return returnData;
	}
	// Read last record and return if it is the searched one.
	fseek(fp, lastIdx*sizeof(Index), SEEK_SET);
	fread(&last, sizeof(Index), 1, fp);
	//if(strcmp(last.key,name1) == 0)
    if(last.key==name1)
	{
		returnData=last.RRN;
		return returnData;
	}
	// Recursion exit condition, If middle index is equal to first or last index
	// required comparisons are already made, so record is not found.
	// Create and return an empty person.
	if(middleIdx==firstIdx || middleIdx == lastIdx) {
		int d=-1;
		return d;
	}

	// Read the middle record and return if it is the searched one.
	fseek(fp, middleIdx*sizeof(Index), SEEK_SET);
	fread(&middle, sizeof(Index), 1, fp);
	//if(strcmp(middle.key,name1) == 0)
    if(middle.key==name1)
	{
		returnData=middle.RRN;
		return returnData;
	}
	// Determine the record position and recursively call with appropriate attributes.
	//if(strcmp(middle.key,name1)>0) {
    if(middle.key>name1){
		return binaryFileSearch(fp, name1, firstIdx+1, middleIdx-1);
	} 
	else {
		return binaryFileSearch(fp, name1, middleIdx+1, lastIdx-1);
	}
}
int findRecordByID(void* name1) {
    // Open the file
    FILE* inputFile;
    inputFile = fopen("indexes.ndx", "rb");
    fseek(inputFile, 0, SEEK_END);
    int REC_COUNT=(ftell(inputFile))/sizeof(Index);
    //printf("%d",REC_COUNT);

    // Calculate initial first and last indexes.
    int firstIdx = 0;
    fseek(inputFile, REC_COUNT*sizeof(Index), SEEK_SET);
    int lastIdx = (ftell(inputFile)/sizeof(Index))-1;
    printf("\n %d",lastIdx);

    // Initiate the search.
    int result = binaryFileSearch(inputFile, name1, firstIdx, lastIdx);
    fclose(inputFile);
    return result;
}

void printMenu(int* answer){

    //print the user menu
    printf("You can perform the following tasks: \n");
    printf("(1) Open \n");
    printf("(2) Create index \n");
    printf("(3) Search \n");
    printf("(4) close \n");
    printf("-------------------------------------\n");
    printf("(5) Quit \n");
    printf("Please Select one... \n");
    scanf("%d",answer);
}

void main()
{
    //createIndexFileBinary(4,14,64,"CHR","DSC");
    //findRecordByName(9739436911H);


    int answer;
    int who;
    char ptr[50];
    char *word;
    void * key1;
    //personNumber = RECORD_COUNT;
    
    ab:
    //print the user menu and read user answer
    printMenu(&answer);

    while(answer>5 || answer<1)
    {
        printf("\nEnter a valid choice by pressing ENTER key again");
        printMenu(&answer);
    }

    switch(answer)
    {
        case 1: 
            printf("Enter json file name :");
            scanf("%s",ptr);
            strcat(ptr,".json");
            FILE *
            jf= fopen(ptr, "r");
            jsonread(jf);
            goto ab;
            break;
        case 2: 
            createIndexFileBinary(json_object_get_int(keyStart),json_object_get_int(keyEnd),json_object_get_int(recordLength),json_object_get_string(keyEncoding),json_object_get_string(order));//json dosyasından 
            goto ab;
            break;
           
        case 3: printf("Enter the data that you want to search:");

            scanf("%s", key1);
            who = findRecordByID(key1);   
            printf("Was the index found?");
            printf(" %d",who);
			FILE* file1;
    	    file1 = fopen(json_object_get_string(FileName),"rb");
            if(who >-1){
				fseek(file1, who*json_object_get_int(recordLength), SEEK_SET);
				char data[100];
				fread(data, json_object_get_int(recordLength), 1,file1);
                printf("%s",data);
				
            } 
            else 
				printf("Not found...");        
            goto ab;
            break;
        case 4: printf("closed\n");
            goto ab;
            break;    
        case 5: printf("Program is terminating \n");
            break;
    }
}

