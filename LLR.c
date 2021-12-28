// Matthew C. Bascom
// Winter Break 2021-2022.
// Monday, December 28, 2021.
// Version: "LLR.c"
// Independent Study - Use it on clean data.

// "Revisiting LLR 1_2.c" includes basic background info.
// "Test Data.txt" is an intro corpus.

// Where is it useful? After piping the output of your favorite,
// cleaned Gutenberg text, run it through this program and 
// pipe the output to an empty file. Vim may be used to quickly 
// navigate to the LLR scores, copy them, and paste them into 
// their own empty file for easy copying in your favorite 
// spreadsheet program. Future editions may include refinements. 


// Compile & Link: gcc LLR.c -o LLR -lm
// Execute: ./LLR textfile.txt > file.output

// Version Notes:

	// Filename: "LLR.c"
	// Prepare for Git submission.

// Sample output: (Executing: ./LLR textfile.txt)
 // ngram: repeat of
 //	2 	5
 //	1 	90
 // ngram: of tokens
 //	2 	2
 //	6 	89
 // ngram: tokens i
 //	1 	5
 //	3 	90
 // . . . 
 // repeat of 3.931
 // of tokens 7.515
 // tokens i 5.283


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // tolower
#include <stdbool.h> // bool type.
#include <math.h> // log calculations.

#define MAX_WORD 30

typedef struct ngram {
	char wordOne[MAX_WORD];
	char wordTwo[MAX_WORD];
	struct ngram * pNextNgram;
	int  ngramCount;
	int eventACount;
	int eventBCount;
	int nonEventCount;
	float LLR;
} nGramNodeStruct;

char * TrimRight(char * str,  const char * trimChars);
typedef nGramNodeStruct * pNgNodeStruct; 
bool AddItem(nGramNodeStruct ngram, pNgNodeStruct * pngList); 

void InitializeList(pNgNodeStruct * plist);
void CopyToNode(nGramNodeStruct ngram, nGramNodeStruct * pnode);
void RemoveDuplicates(pNgNodeStruct * pngList);

void CalculateLLR(pNgNodeStruct pngList, 
		float rowH, float colH, float totalH); 

float ShannonEntropy(pNgNodeStruct pngList, 
		int rowsOnly, int colsOnly); 


int main (int argc, char * argv[]) {

	int returnValue = 0;
 	
	pNgNodeStruct ngramList;
	pNgNodeStruct pngramListHead;
			// Assign a value to the head after ngramList
			// has been constructed through calls to 
			// the AddItem() function such that:
			// pngramListHead = ngramList;

	nGramNodeStruct Temp2;

	InitializeList(&ngramList);

  FILE *pInputFile;
  char word[MAX_WORD];

  if (argc != 2) {
   printf("Usage: %s filename\n", argv[0]);
   exit(EXIT_FAILURE);
  
  } 
  if ((pInputFile = fopen(argv[1], "r")) == NULL) {
   printf("Can't open %s\n", argv[1]);
   exit(EXIT_FAILURE);

  }
 

	while (fscanf(pInputFile, "%s", word) == 1) {
		TrimRight(word, "\t\n\v\f\r .,!?;:\"'");
		*word = tolower(*word);

		strncpy(Temp2.wordOne, word, sizeof(word));

		AddItem(Temp2, &ngramList);
	}

	fclose(pInputFile);

    printf(" ------ DONE READING FILE ------- \n");


    if(ngramList == NULL) {
        printf("NO DATA IN nGramwordList!\n");
        exit(EXIT_FAILURE); // This exit is new per 4_2.c
    }


// * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + // 

        // Keep track of the list head: it will be revisited...
        pngramListHead = ngramList;

        while(ngramList != NULL) {
        
	   if (ngramList->pNextNgram->wordOne != NULL){
	     strncpy(ngramList->wordTwo, 
		      ngramList->pNextNgram->wordOne, 
		       sizeof(ngramList->pNextNgram->wordOne));

	    }
	    // Copy the first word of the next bigram structure
	    // into the wordTwo of the current bigram strcture: 
            ngramList = ngramList->pNextNgram;
        }

// * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + // 

	RemoveDuplicates(&pngramListHead);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
	// Return the List pointer to the head of the linked list.
	
	ngramList = pngramListHead;
	
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
	
	// Initialize the ngramCount member variable in every node.
	
	ngramList = pngramListHead;

	while(ngramList->pNextNgram != 0) {
		ngramList->ngramCount += 1;
		ngramList = ngramList->pNextNgram;
	}


	// Version 3_3.c
	// Now, using the head of the list, traverse through 
	// the list again removing duplicates and incrementing 
	// the count member variable. Hold the first ngram constant, 
	// increment through all that follow.
	// ...
	// Advance the first ngram.

	// Reset the linked list to point to the head.
	ngramList = pngramListHead;
	// Initialize a structure to search the linked list.
	pNgNodeStruct ngramDupeSearchStruct;
	pNgNodeStruct previousNGDupeStruct;

	   
	if (ngramList->pNextNgram != NULL) {
           
        ngramDupeSearchStruct = ngramList->pNextNgram;
	previousNGDupeStruct = ngramList;

         while(ngramList->pNextNgram != NULL) {

	   while(ngramDupeSearchStruct->pNextNgram != NULL) {

	    // If a duplicate ngram is identified, update the list.
	    // 'strcmp' returns zero if the two tokens are equal. 
	    // '!strcmp' inverts the test. When the
	    // 'if' test returns TRUE, the ngrams are identical.
	    if (!strcmp(ngramList->wordOne, 
		        ngramDupeSearchStruct->wordOne) && 
	           !strcmp(ngramList->wordTwo, 
		           ngramDupeSearchStruct->wordTwo)) {
	   
	     ngramList->ngramCount += 1;
	     
	     ngramDupeSearchStruct = 
		     previousNGDupeStruct;
	     ngramDupeSearchStruct->pNextNgram = 
		    ngramDupeSearchStruct->pNextNgram->pNextNgram;
	     
	     //free(previousNGDupeStruct);
	    
	    }
	    // Else, if the ngram is not a duplicate, proceed to the 
	    // next node as usual.
	    else {

		 previousNGDupeStruct = 
			 ngramDupeSearchStruct;
		 
		 ngramDupeSearchStruct = 
		  ngramDupeSearchStruct->pNextNgram;
	    
	    } 
	    
	   }
	    // End: while (ngramDupeSearchStruct != NULL).

	   // Point the structures at the head of the list.
           ngramList = ngramList->pNextNgram;
	   ngramDupeSearchStruct = ngramList->pNextNgram;
	   
         }
	  // End while (ngramList->pNextNgram !=NULL).
	}
	 // End if (ngramList->pNextNgram->wordOne != NULL).	
	
	// Return the list pointer to the head of the linked list.	
	ngramList = pngramListHead;

// * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + // 
	// Iterate through each bigram, comparing 
	// it to every other in the bigram linked list.
	
	ngramList = pngramListHead;
        ngramDupeSearchStruct = ngramList->pNextNgram;
	// Equivalently, perhaps:
	 // ngramDupeSearchStruct = pngramListHead->pNextNgram;

	while (ngramList->pNextNgram != NULL) {
	   while(ngramDupeSearchStruct->pNextNgram != NULL) {
	 
	    // This could be a good place to write a case statement
	    // to forego the else-if speghetti sauce.
	    // Case: A or B has occurred.
	    if (!strcmp(ngramList->wordOne, 
		        ngramDupeSearchStruct->wordOne) || 
	           !strcmp(ngramList->wordTwo, 
		           ngramDupeSearchStruct->wordTwo) || 
		   !strcmp(ngramList->wordOne, 
		        ngramDupeSearchStruct->wordTwo) || 
	           !strcmp(ngramList->wordTwo, 
		           ngramDupeSearchStruct->wordOne)) {
	     // Since the bigrams being searched (should be)
	     // free of duplicate bigrams, either one case or
	     // the other holds, i.e., either Event A is a duplicate 
	     // or Event B is a duplicate. 
	    
	     // Nested If-Else handles the details of the subcase.
	     // Case: A and Not B has occurred.
	      if ((!strcmp(ngramList->wordOne, 
		   ngramDupeSearchStruct->wordOne) ||
                    !strcmp(ngramList->wordOne, 
			    ngramDupeSearchStruct->wordTwo)) && 
	          (strcmp(ngramList->wordTwo, 
		           ngramDupeSearchStruct->wordTwo) &&
		   strcmp(ngramList->wordTwo,
			   ngramDupeSearchStruct->wordOne))) {
		    
	             //printf("---- Event A Duplicate Event ----\n");
		     ngramList->eventACount += 
			     ngramDupeSearchStruct->ngramCount;
	      }
	     // Case: Not A and B has occurred.
	      else if ((strcmp(ngramList->wordOne, 
		       ngramDupeSearchStruct->wordOne) && 
			     strcmp(ngramList->wordOne,
			      ngramDupeSearchStruct->wordTwo)) &&
			     (!strcmp(ngramList->wordTwo,
				ngramDupeSearchStruct->wordOne) || 
	              !strcmp(ngramList->wordTwo, 
		       ngramDupeSearchStruct->wordTwo))) {
		      
	             //printf("---- Event B Duplicate Event ----\n");
		     ngramList->eventBCount += 
			     ngramDupeSearchStruct->ngramCount;
	     }
	      // End nested else-if.
	      //else {
		      //continue;
		      // Nothing to count here. The primary and 
		      // secondary node are equivalent.
		      // But it doesn't like this continue. 
	    }
	     // End if one of the events match.
	     // Else, neither event matches.
	    else {
	         //printf("---- Unique Event ----\n");
		 ngramList->nonEventCount += 
		    ngramDupeSearchStruct->ngramCount;
	    }
	     // Else, the events are disjoint.

	   // Proceed to the next secondary candidate for comparison.
	   ngramDupeSearchStruct = 
	     ngramDupeSearchStruct->pNextNgram;
	  }
	   // End while(ngramDupeSearchStruct->pNextNgram != NULL).
	 
	 // Proceed to the next lead candidate for comparison.  
         ngramList = ngramList->pNextNgram;
	 
       	   // Unlike the procedure of widdling down the linked list 
	   // to hold only unique bigrams, this list is doing a 
	   // slightly more subtle counting procedure that requires 
	   // the secondary search node to begin at the head of the 
	   // linked list after a full cycle of the linked list by 
	   // the primary search node. 
	   //
	 ngramDupeSearchStruct = pngramListHead;

	}
	 // End while (ngramList->pNextNgram != 0).

// * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + // 

	// Return the list pointer to the head of the linked list.
	ngramList = pngramListHead;

	while (ngramList->pNextNgram != 0) {
		printf("ngram: %s %s\n", ngramList->wordOne, 
				ngramList->wordTwo);
		// Print Occurrence of the ngram followed by 
		// the occurrence of B and Not A:
		printf("\t%i \t%i\n", ngramList->ngramCount,
				ngramList->eventBCount);
		// Print occurrence of A and Not B followed by
		// the occurrence of neither A or B.
		printf("\t%i \t%i\n", ngramList->eventACount,
				ngramList->nonEventCount);

         	ngramList = ngramList->pNextNgram;
	}

// * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + * + // 

	float totalEntropy;
	float rowEntropy;
	float colEntropy;

	int rowsOnly = 0;
	int colsOnly = 0;

	// Return the list pointer to the head of the linked list.
	
	ngramList = pngramListHead;

	while(ngramList->pNextNgram != 0) {

	rowEntropy = 
		ShannonEntropy(ngramList, rowsOnly = 1, colsOnly = 0);

	colEntropy = 
		ShannonEntropy(ngramList, rowsOnly = 0, colsOnly = 1);
	
	totalEntropy = ShannonEntropy(ngramList, rowsOnly = 0,
			colsOnly = 0);

	ngramList = ngramList->pNextNgram;

	CalculateLLR(ngramList, 
			rowEntropy, colEntropy, totalEntropy);
	}

	return returnValue;
}
 // End main.







float ShannonEntropy(pNgNodeStruct pngList, 
		int rowsOnly, int colsOnly) {

 float LLR;
 int rowSums;
 int colSums;
 float matrixElementsSum;
 
 int nonEventZero = 0;
 int eventAZero = 0;
 int eventBZero = 0;
 int ngramZero = 0;

 float entropy = 0;

 int rowOneSum = 0;
 int rowTwoSum = 0;
 int colOneSum = 0;
 int colTwoSum = 0;

 float rowOneEntropy = 0;
 float rowTwoEntropy = 0;
 float colOneEntropy = 0;
 float colTwoEntropy = 0;
 
 
 matrixElementsSum = pngList->nonEventCount + 
	 	      pngList->eventACount + 
                       pngList->eventBCount + 
		        pngList->ngramCount;

 if(rowsOnly && colsOnly) {

	printf("There was misuse of the ShannonEntropy() function.\n");
	printf("The parameters rowsOnly and colsOnly cannot both be one.\n");
        exit(EXIT_FAILURE);

 }

 else if  ((rowsOnly == 1) && (colsOnly == 0)) {

	rowOneSum = pngList->ngramCount + pngList->eventBCount;
	rowTwoSum = pngList->eventACount + pngList->nonEventCount;
	
	rowOneEntropy = (rowOneSum/matrixElementsSum) * 
		log(rowOneSum/matrixElementsSum);
        rowTwoEntropy = (rowTwoSum/matrixElementsSum) *
	        log(rowTwoSum/matrixElementsSum);	

	entropy = rowOneEntropy + rowTwoEntropy;
	
	return entropy;

 }
 
 else if ((colsOnly == 1) && (rowsOnly == 0)) {

	colOneSum = pngList->ngramCount + pngList->eventACount;
	colTwoSum = pngList->eventBCount + pngList->nonEventCount;

	colOneEntropy = (colOneSum/matrixElementsSum) *
		log(colOneSum/matrixElementsSum);
	colTwoEntropy = (colTwoSum/matrixElementsSum) * 
		log(colTwoSum/matrixElementsSum);

	entropy = colOneEntropy + colTwoEntropy;
	
	return entropy;

 }

 else {
	 // Else we want the total entropy.

 if (pngList->nonEventCount == 0) {
	nonEventZero = 1;
 }
    entropy += (pngList->nonEventCount / matrixElementsSum) * 
	    log(pngList->nonEventCount / 
			    matrixElementsSum + nonEventZero); 

 if (pngList->eventACount == 0) {
	eventAZero = 1;
 }
    entropy += (pngList->eventACount / matrixElementsSum) * 
	    log(pngList->eventACount / 
			    matrixElementsSum + eventAZero); 
			
 if (pngList->eventBCount == 0) {
	eventBZero = 1;
 }
    entropy += (pngList->eventBCount / matrixElementsSum) * 
	    log(pngList->eventBCount / 
			    matrixElementsSum + eventBZero); 
			
 if (pngList->ngramCount == 0) {
	ngramZero = 1;
 }
    entropy += (pngList->ngramCount / matrixElementsSum) * 
	    log(pngList->ngramCount / matrixElementsSum + ngramZero); 
 
 }

 return entropy;

}

void CalculateLLR(pNgNodeStruct pngList, 
		float rowH, float colH, float totalH) {

float LLR;
int rowSums;
int colSums;
int matrixElementsSum;

 matrixElementsSum = pngList->nonEventCount + 
	 	      pngList->eventACount + 
                       pngList->eventBCount + 
		        pngList->ngramCount;
	LLR = 2 * matrixElementsSum * (totalH - rowH - colH);
	pngList->LLR = LLR;
	printf("%s %s ", pngList->wordOne, pngList->wordTwo);
        printf("%.3f\n", LLR);


	return;
}

char * TrimRight(char * str,  const char * trimChars) {
	int i;
	if (trimChars == NULL) {
		trimChars = "\t\n\v\f\r ";
	}
	i = strlen(str) - 1;
	while (i >= 0 && strchr(trimChars, str[i]) != NULL) {
		str[i] = '\0';
		i--;
	}
	return str;
}


void InitializeList(pNgNodeStruct * plist) {
	*plist = NULL;
}



bool AddItem(nGramNodeStruct ngram, pNgNodeStruct * pngList) {

	nGramNodeStruct * pnew;
	nGramNodeStruct * curNode = *pngList;
	nGramNodeStruct * pPrev = *pngList;
	int attached = 0;

	pnew = (nGramNodeStruct *)malloc(sizeof(nGramNodeStruct));
	if (pnew == NULL) {
		return false;	//quit function on failure.
	}
		CopyToNode(ngram, pnew);
		pnew->pNextNgram = NULL;
	    // ^^^^^^   Setting up New Node   ^^^^^^
		if (curNode == NULL) {
			*pngList = pnew;
		}

		else {
		
		while(attached != 1)
		{
			if(curNode->pNextNgram != NULL)
			{
			    pPrev = curNode;
			    curNode = curNode->pNextNgram;
			}
			else
			{
			    curNode->pNextNgram = pnew;
			    attached = 1;
			}
		    }
		}
		attached = 0;
		//free(pnew);
	}
	// End bool AddItem() Definition.


void CopyToNode(nGramNodeStruct ngram, nGramNodeStruct * pnode){

	strncpy(pnode->wordOne, ngram.wordOne, sizeof(ngram.wordOne)); 
}

void RemoveDuplicates(pNgNodeStruct * pngList) {
// Start by accessing the elements of the linked list.
// Then create a basic comparison and print the result of the test.
// Lastly, negotiate the linked list to increment the counter and
// remove duplicate entries from the linked list.

    nGramNodeStruct * curNode = *pngList;
    nGramNodeStruct * pPrev = *pngList;

	return;
}
