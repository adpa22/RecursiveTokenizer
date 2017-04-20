#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "index.h"
Node * head=NULL;

/*This function takes in two chars and compares the first one to the second. This is how
we can make sure numbers come after letters in out implementation*/
int cmpChar(char s1,char s2){
        //Comparing two chars. (Subtracting ASCII values)
        if (isalpha(s1) && isalpha(s2)){
                return s1-s2;
        }
        //char 1 is a digit,char 2 is an alphabet.(1 means char 1 goes after char 2)
        else if (isdigit(s1) && (s2=='.'||isalpha(s2))){
                return 1;
        }
        //char 1 is an alphabet, char 2 is a digit. (-1 means char 1 goes before char 2)
        else if ((s1=='.'||isalpha(s1)) && isdigit(s2)){
                return -1;
        }
        //Comparing two digits.(Subtract them)
        else if (isdigit(s1) && isdigit(s2)){
                return s1-s2;
        }
        return 0;
}

//Takes in two strings and uses the cmpChar() to compare correspinding chars in the strings.
int cmpTo(char * s1,char *s2){ // < 0 if s1 is less than s2; > 0 if s2 is less than s1; == 0 if equal
        int size1 = strlen(s1);
        int size2 = strlen(s2);
        //Strings of equal length
        if (size1 == size2){ //go through everything one by one
                int i;
                for (i = 0; i < size1; i++){
                        if (s1[i] != s2[i]){ //not equal
                                return cmpChar(s1[i],s2[i]);
                        }
                }
                return 0; //equal
        }
        //If the length of string 1 is less than string 2
        else if (size1 < size2){
                //for loop size1 times
                int i;
                for (i = 0; i < size1; i++){
                        if (s1[i] != s2[i]){
                                return cmpChar(s1[i], s2[i]);
                        }
                }
                return -1;
        }
        else {
                //Length of string 2 is less than string 1
                //for loop size2 times
                int i;
                for (i = 0; i < size2; i++){
                        if (s1[i] != s2[i]){
                                return cmpChar(s1[i], s2[i]);
                        }
                }
                return 1;
        }
        return -1;
}
//This swaps the contents of two Nodes which is the word and the filesHead node.
void swap(Node * n1,Node * n2){ //
        fileNode * fileTemp1=n1->filesHead;
        char * word1=n1->word;

        fileNode * fileTemp2=n2->filesHead;
        char * word2=n2->word;

        n1->word=word2;
        n1->filesHead=fileTemp2;

        n2->word=word1;
        n2->filesHead=fileTemp1;


}



//This swaps the contents of two fileNodes which is the fileName and the occurrences.
void fileSwap(fileNode * n1, fileNode * n2){
        int occ1 = n1->occurrence;
        char * file1 = n1->fileName;

        int occ2 = n2->occurrence;
        char * file2 = n2->fileName;


        n1->occurrence = occ2;
        n1->fileName = file2;

        n2->occurrence = occ1;
        n2->fileName = file1;
}

//Sorts the names of the files that have the same occurrences.(a-z,0-9)Numbers after alphabet.
void sortFileName(){
        Node * start = head;//words Linked list
        int mostCmp = 0;


        while (start != NULL){
                mostCmp = 0;
                fileNode * first = start->filesHead;
                while (first!= NULL){
                        fileNode * second = first->nextFile;

                        fileNode * max = first;
                        if (second == NULL){
                                break;//One file node. Nothing to sort
                        } else {
                                //Check for common occurrence. If true, have pointer to the start(first) of occurrences and one for the end(second).
                                if (first->occurrence == second->occurrence){

                                        while (second->occurrence == first->occurrence && second->nextFile != NULL){

                                                second = second->nextFile;
                                        }
                                }
                                //At last node but same occurrence
                                if(first->occurrence==second->occurrence){
                                        second=second->nextFile;
                                }
                                //from first to second
                                fileNode * traverse = first;
								fileNode * traverse2=first;
                                //Similar to selection sort.
								
								while(traverse2!=second&&traverse2!=NULL){
									traverse=traverse2;
									max=traverse2;
									mostCmp=0;
	                                while (traverse != second && traverse!= NULL){ //maybe ->next
	                                        //Look for file name that should come first(greatest)
	                                        int temp = cmpTo(max->fileName, traverse->fileName);
	                                        if(temp > mostCmp){
	                                                max = traverse;
	                                                mostCmp = temp;
	                                        }
	                                        traverse = traverse->nextFile;
	                                }
								//Swap fileNode contents.
								fileSwap(traverse2,max);
								traverse2=traverse2->nextFile;
								
								}
                                
                                

                        }
                        first = first->nextFile;
                }

                start = start->nextWord;
        }
}

//Sorts fileNodes by occurrence. Greatest to least. Selection sort implementation
void sortOccurence(){
        Node * start=head;
        fileNode * traversal=NULL;
        fileNode * max=NULL;

        while(start!=NULL){
                fileNode * fileStart=start->filesHead; //aka max
                while(fileStart->nextFile!=NULL){
                traversal=fileStart->nextFile;
                max = fileStart;
                while(traversal!=NULL){ //->nextFile was here before

                        if (traversal->occurrence > max->occurrence){
                                max = traversal;
                        }

                        traversal=traversal->nextFile;
                }
                fileSwap(fileStart, max);
                fileStart=fileStart->nextFile;
                }
                start = start->nextWord;
        }
}

//Sort Nodes by alphanumeric order(a-z,0-9) Selection sort implementation
void sort(){
        Node * start = head;
        Node * traversal=NULL;
        Node * min=NULL;
        int mostCmp=0; //swap with the node w/ highest number
        while (start != NULL){
                min=start;
                traversal=start->nextWord;

                while(traversal!=NULL){
                        int temp=cmpTo(min->word,traversal->word);
                        if(temp>mostCmp){
                                min=traversal;
                                mostCmp=temp;
                        }
                        traversal=traversal->nextWord;
                }
                //MIGHT NOT NEED IT
                if(mostCmp>0){
                swap(start,min);
        }

                mostCmp = 0;
                start = start->nextWord;
        }


}

//Converts a string into lowercase
void lowrider(char * in){
        //printf("in: %s\n", in);
        int i;
        int s=strlen(in);
        //printf("Length:%i\n",s);
        for (i = 0; i<s; i++){
                        //printf("letter:%c\n",in[i]);
                if (isalpha(in[i])){

                        in[i] = tolower(in[i]);

                } else if (isdigit(in[i])){
                        in[i] = in[i];
                }
        }
}

//Inserts a Node, or a fileNode into out linked list.
void insert(char * word, char * fileName){
        Node * curr=head;
        fileNode * toAdd=NULL;
        int found=0;//check if word already exists in list
        lowrider(word);
        //Adding the first thing to the list
        if (head == NULL){
                head = (Node *)malloc(sizeof(Node));
                toAdd=(fileNode *)malloc(sizeof(fileNode));
                toAdd->fileName=fileName;
                toAdd->nextFile=NULL;
                toAdd->occurrence=1;
                head->word = word;
                head->nextWord = NULL;
                head->filesHead=toAdd;
                //Adding to the end of the list
        }else{
                while(curr!=NULL){
                        //duplicate
                        if(strcmp(curr->word,word)==0){
                                found=1;
                                fileNode * ptr=curr->filesHead;
                                while(ptr!=NULL){
                                        if (strcmp(ptr->fileName, fileName) == 0){ //duplicate files for same word. just update occurrence
                                                ptr->occurrence++;
                                                break;
                                        }
                                        ptr = ptr-> nextFile;
                                }
                                //doesnt find a duplicate file. Create a fileNode for that file.
                                if(ptr==NULL){
                                        ptr=curr->filesHead;
                                        while(ptr->nextFile!=NULL){
                                                ptr=ptr->nextFile;
                                        }
                                        toAdd=(fileNode *)malloc(sizeof(fileNode));
                                        toAdd->fileName=fileName;
                                        toAdd->occurrence=1;
                                        toAdd->nextFile=NULL;
                                        ptr->nextFile=toAdd;

                                }
                        }
                        curr=curr->nextWord;
                }
                //word is not in our list. Create a Node and corresponding fileNode and insert it
                if(found==0){
                        curr=head;
                        while(curr->nextWord!=NULL){
                                curr=curr->nextWord;
                        }
                        Node * temp=(Node *)malloc(sizeof(Node));
                        toAdd=(fileNode *)malloc(sizeof(fileNode));
                        toAdd->fileName=fileName;
                        toAdd->nextFile=NULL;
                        toAdd->occurrence=1;
                        temp->word=word;
                        temp->nextWord=NULL;
                        temp->filesHead=toAdd;
                        curr->nextWord=temp;
                }
                found=0;

        }
		sortFileName();
		sort();
}

//This returns an int that lets us know how many valid tokens there are in the file.
int countTokens(char * input){
        int counter = 1;

        //to count the number of "tokens"
        int j =0;
        while(!isalpha(input[j]) && j<strlen(input)){
                                j++;
        }
        int i;
        for ( i = j ; i < strlen(input); i++){
                if(isalpha(input[i])){
                        counter++;
                        while((isalpha(input[i])||isdigit(input[i]))&&i<strlen(input)){
                                i++;
                        }
                }

        }

        //if the last char in the array is not an alpha, decrement the num of tokens
        if (!isalpha(input[i-1])){
                counter--;
        }
        //printf("counter: %i\n", counter);
        return counter;

}

//Parses the file token by token and calls insert to try and put it in the list.
void parser(char * input , char * fileName){
        //printf("INPUT: %s\n", input);
        int numToks = countTokens(input);
        //printf("NUMTOKS: %i\n", numToks);

        int j =0;
        int i;

        //Iterates through the input, finds token, and inserts it.
        for ( i=0; i < numToks; i++){ //iterate words array
                while(!isalpha(input[j]) && j<strlen(input)){ //skip delimators in the beginning
                        j++;
                }

                int wordStart = j; // start of token
                int index = wordStart;
                while ((isalpha(input[index]) || isdigit(input[index])) && index < strlen(input)){
                        index++;
                }

                int wordEnd = index -1;//end of token
                //Allocate memory for one word
                char * tok = (char *)malloc (wordEnd- wordStart+1);

                strncpy(tok, input + wordStart, wordEnd - wordStart+1);
                tok[wordEnd-wordStart + 1 ] = '\0';
                //Puts the word into the array.
                insert(tok, fileName);
                j = index;


        }
}

//Prints words in list in XML format.
void printList(char * output, char * initial){
       // printf("Initial: %s\n", initial);
        FILE * fp = fopen(output, "w");
        Node * temp=head;
        fileNode * fileTemp=NULL;
        fprintf(fp, "<?xml version=\"%i.%i\" encoding=\"UTF-%i\"?>\n<fileIndex>\n",1,0,8);
        while(temp!=NULL){
                fprintf(fp, "\t<word text=\"%s\">\n",temp->word);
                fileTemp=temp->filesHead;

                while(fileTemp!=NULL){
                        fprintf(fp, "\t\t <file name=\"%s\">%i</file>\n",fileTemp->fileName,fileTemp->occurrence);
                        fileTemp=fileTemp->nextFile;
                }
                fprintf(fp, "\t</word>");
                fprintf(fp, "\n");
                temp=temp->nextWord;
        }
        fprintf(fp, "</fileIndex>");
}

int getFiles(){
        int counter = 0;
        Node * ptr = head;
        fileNode * ptr2 = NULL;
        while (ptr != NULL){
                ptr2 = ptr->filesHead;
                while (ptr2 !=NULL){
                        counter++;
                        ptr2 = ptr2->nextFile;
                }
                ptr = ptr->nextWord;
        }
        return counter;
}
//Tries to open file and calls parser if successful. Error otherwise.
void fileOpen(char * file){
        int filedesc = open(file, O_RDWR);
        int size=0;
        int bytes_read=0;
        char * buffer=NULL;//Holds all the data in the file
        if (filedesc <0 ){
                perror("fileOpen error");
        } else {
                //start parsing,add to linked list
                //take all content put it in buffer *
                size=lseek(filedesc,0,SEEK_END);
                lseek(filedesc, 0, SEEK_SET);
                buffer=(char *)malloc(sizeof(char)*size+1);
                bytes_read=read(filedesc,buffer,size);
                if(bytes_read==0){
                        printf("Nothing in this file: %s\n",file);
                }else if(bytes_read<0){
                        perror("error");
                }else{
                        buffer[strlen(buffer)]='\0';

                        parser(buffer, file);
                }
        }
        close(filedesc);
}

//# of words in our list
int getNodes(){
        int counter = 0;
        Node * ptr = head;
        while (ptr != NULL){
                counter++;
                ptr = ptr->nextWord;

        }
        return counter;
}

//open dir: Open a directory recusrively getting everything, and use openfile to add all tokens to the head
void dirOpen(DIR * dir){
        //Error or done looking through directory
        if(dir==NULL){
                perror("Error");
                //printf("hey\n");
                closedir(dir);
                return;
        }
        errno = 0;
        //first try to open file, if error, then open dir
        struct dirent * ent;
        while ((ent = readdir(dir)) != NULL){
                if (ent->d_name[0] == '.'){
                        //do nothing for '.' and '..'
                } else{
                        DIR * dirp = opendir(ent->d_name);
                        int err = errno;
                        if(err == ENOTDIR){ //file
                                errno = 0;
                                fileOpen(ent->d_name);
                        } else{ //dir
                                //errno = 0;
                                chdir(ent->d_name);
                                dirOpen(dirp);
                                chdir("..");
                        }
                }
        }

}

void dirExists(char * out){
	if(out[strlen(out)-1]=='/'){
	fprintf(stderr,"Invalid output file path exiting\n");
	exit(0);
}
	DIR * dir=opendir(out);
	if(dir!=NULL){
	fprintf(stderr,"output file name is a directory exiting\n");
	exit(0);
}
	





errno=0;
}

int main(int argc,char** argv){
        //Check for correct amount of args
        if (argc != 3 ){
                fprintf(stderr,"Need 2 parameters\n");
                exit(0);
        }


        int exists=0;
        char * output = argv[1];//output file name
        char * initial = argv[2]; // folder or file to start indexing
        char * answer=(char *)malloc(sizeof(char)*3);
        //Check if output file exits.
	
        errno=0;


        int err; //if it's not a directory
        DIR * dirp=opendir(initial);
        err = errno;

        if (err == ENOENT){ //no such file or directory
                perror("error exiting");
		exit(0);
        }
        else if (err == ENOTDIR){ // not a directory, could be file or nothing
                //try to open and arrange as a file (have method for this), and if not file error and break program
                printf("file\n");
		dirExists(output);
                chdir(initial);
                fileOpen(initial);
                //chdir(initial);
        }else{
        //directory
		dirExists(output);//check if output location exists.
                chdir(initial);
                dirOpen(dirp);
                chdir("..");

        }

        int numNodes = getNodes(); //was getNodes();
        int p;

        //make sure words are sorted.
        for (p = 0; p < numNodes; p++){
                sort();
        }	

        sortOccurence();
        sortFileName();
	if(access(output,F_OK)!=-1){
                printf("File exists\n Would you like to overwrite Yes or No?\n");
                exists=1;
                scanf("%s",answer);
        }else{
                printf("Output file does not exist,creating it\n");
        }

        if(exists==1){
                printf("Answer:%s\n",answer);
                while(strcmp("Yes",answer)!=0&&strcmp("No",answer)!=0){
                        printf("I said Yes or No!!\n");
                        scanf("%s",answer);
                }
                if(strcmp("No",answer)==0){
                        printf("Ok exiting program\n");
                        exit(0);
                }
        }
       

        printList(output, initial);


        return 0;
}
