# HW4
# Mustafa Abdullah Hakkoz - 150117509

#include <stdio.h>
#include <stdlib.h>

struct Firm{
    int firmID;
    char firmName[100];
    struct Firm *nextfirm;
};

typedef struct Firm firm;

struct Food{
	char prod_name[400];
	int exp_day;
	int exp_month;
	int exp_year;
};

typedef struct Food food;

struct FoodStock{
	struct Food ffood;
	struct FoodStock *nextfood;
};

typedef struct FoodStock foodstock;

firm *readFirms(char filename[]){
	unsigned int id;
	char name[ 100 ];
	FILE *newFile; // filename.txt file pointer
    // fopen opens the file; exits program if file cannot be opened
    if ( ( newFile = fopen( filename, "r" ) ) == NULL ) {
         puts( "File could not be opened" );
         return NULL;
    } // end if
    else {
    	firm *startPtr = NULL;
    	fscanf( newFile, "%d %s", &id, name);
    	while ( !feof( newFile ) ) {
            firm *newFirm = malloc(sizeof(firm)); //create node
            if (newFirm != NULL) { // is space available
		      newFirm->firmID = id; // place value in node
		      sprintf(newFirm->firmName, "%s", name);
		      newFirm->nextfirm = NULL; // node does not link to another node
		
		      firm *previousPtr = NULL;
		      firm *currentPtr = startPtr;
		
		      // loop to find the correct location in the list       
		      while (currentPtr != NULL && id < currentPtr->firmID) {
		         previousPtr = currentPtr; // walk to ...               
		         currentPtr = currentPtr->nextfirm; // ... next node 
		      }                                          
		
		      // insert new node at beginning of list
		      if (previousPtr == NULL) { 
		         newFirm->nextfirm = startPtr;
		         startPtr = newFirm;
		      } 
		      else { // insert new node between previousPtr and currentPtr
		         previousPtr->nextfirm = newFirm;
		         newFirm->nextfirm = currentPtr;
		      } 
		   } 
		   else {
		      printf("%s not inserted. No memory available.\n", name);
		   } 
        fscanf( newFile, "%d %s", &id, name);
        } // end while
        return startPtr;
	}	
}

void printFirms(firm *firmptr){
	// if list is empty
   if (firmptr==NULL) {
      puts("List is empty.\n");
   } 
   else { 
      // while not the end of the list
      while (firmptr != NULL) { 
         printf("%d %s\n", firmptr->firmID, firmptr->firmName);
         firmptr = firmptr->nextfirm;   
      } 
   } 
}

foodstock *readFoods(char filename[]){
	int i;
	unsigned int amount;
	char firm[ 100 ];
	char foodName[ 100 ];
	char foodType[ 100 ];
	
	FILE *newFile; // filename.txt file pointer
    // fopen opens the file; exits program if file cannot be opened
    if ( ( newFile = fopen( filename, "r" ) ) == NULL ) {
         puts( "File could not be opened" );
         return NULL;
    } // end if
    else {
    	foodstock *startPtr = NULL;
    	fscanf( newFile, "%s %s %s %d", firm, foodName, foodType, &amount);
    	while ( !feof( newFile ) ) {  
			for(i=0; i<amount; i++)  {			
	            foodstock *newStock = malloc(sizeof(foodstock)); //create node
	            if (newStock != NULL) { // is space available
			      sprintf(newStock->ffood.prod_name, "%s %s %s", firm,foodName,foodType);
			      newStock->ffood.exp_day= (rand() % 14)+1;
			      newStock->ffood.exp_month= (rand() % 4)+1;
			      newStock->ffood.exp_year= (rand() % 2)+2018;
			      newStock->nextfood = NULL; // node does not link to another node
			
			      foodstock *currentPtr = startPtr;
			
			      // loop to find the end of the list       
			      while (currentPtr != NULL) {          
			         currentPtr = currentPtr->nextfood; // ... next node 
			      }                                          
			
			      // insert new node at beginning of list
			      if (currentPtr == NULL) { 
			         newStock->nextfood = startPtr;
			         startPtr = newStock;
			      } 
			      else { // insert new node to end of the list
			         currentPtr->nextfood = newStock;
			         newStock->nextfood = NULL;
			      } 
			   } 
			   else {
			      printf("new food not inserted to stock list. No memory available.\n");
			   } 
			}
        	fscanf( newFile, "%s %s %s %d", firm, foodName, foodType, &amount);
        } // end while
        return startPtr;
	}	
}

void printFood(foodstock *foodptr, char filename[]){
	// if list is empty
   if (foodptr==NULL) {
      puts("List is empty.\n");
   } 
   else {
		FILE *outputFile = fopen(filename,"w"); 
      // while not the end of the list
		while (foodptr != NULL) { 
	        fprintf(outputFile, "%s EXP : %d/%d/%d", foodptr->ffood.prod_name, foodptr->ffood.exp_day, foodptr->ffood.exp_month, foodptr->ffood.exp_year);
	        foodptr = foodptr->nextfood;   
      } 
   } 
}

foodstock *stockOut(foodstock *foodptr, int day, int month, int year){
	// delete first node if a match is found
   if (isExpired(foodptr, day, month, year)) { 
      foodstock *tempPtr = foodptr; // hold onto node being removed
      foodptr = foodptr->nextfood; // de-thread the node
      free(tempPtr); // free the de-threaded node
      return foodptr;
   } 
   else { 
      foodstock *previousPtr = foodptr;
      foodstock *currentPtr = foodptr->nextfood;

      // loop to find the correct location in the list
      while (currentPtr != NULL && !isExpired(currentPtr, day, month, year)) { 
         previousPtr = currentPtr; // walk to ...  
         currentPtr = currentPtr->nextfood; // ... next node  
      } 

      // delete node at currentPtr
      if (currentPtr != NULL) { 
         foodstock *tempPtr = currentPtr;
         previousPtr->nextfood = currentPtr->nextfood;
         free(tempPtr);
         return foodptr;
      } 
   } 

   return foodptr;
}

int isExpired(foodstock *foodptr, int day, int month, int year){		//function to find if product has expired or not
	if(foodptr->ffood.exp_year > year)
		return 1;
	if(foodptr->ffood.exp_year == year && foodptr->ffood.exp_month > month)
		return 1;
	if(foodptr->ffood.exp_year == year && foodptr->ffood.exp_month == month && foodptr->ffood.exp_day > day)
		return 1;
	else 
		return 0;		
}

int main(  int argc, char *argv[] )
{
	unsigned int day;
	unsigned int month;
	unsigned int year;
	
	firm *firms = NULL;	
	firms=readFirms("firms.txt");
	printFirms(firms);
	
	foodstock *stocks = NULL;
	stocks=readFoods("products.txt");
	printFood(stocks, "initial_stock.txt");
	
	printf("Please enter the day, month and year to be checked\n");
	scanf("%d %d %d", &day, &month, &year);
	
	stocks= stockOut(stocks, day, month, year);
	
	printFood(stocks, "final_stock.txt");
}
