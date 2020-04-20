/*
For your own test purposes, you will also want to code a main program in another .c file that calls your
functions with a variety of test cases, However, you must not submit that file with your assignment. Also, do
not put your main() function in SVGParser.h. Failure to do this may cause the test program will fail if you
incorrectly include main() in our shared library file; you will lose marks for that, and may get a 0 for the
assignment.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "LinkedListAPI.h"
#include "SVGParser.h"

void printList(List* list, char* shape){
    void* elem;
	
	//Create an iterator - again, the iterator is allocated on the stack
	ListIterator iter = createIterator(list);

	/*Traverse the list using an iterator.  
	nextElement() returns NULL ones we reach the end of the list */
	while ((elem = nextElement(&iter)) != NULL){
		Attribute* tmpAttr;
		Rectangle* tempRect;
		Circle* tempCircle;
		Path* tempPath;
		Group* tempGroup;

		char* str = NULL;

		if(strcmp((char*)shape, "Attribute") == 0)
		{
			tmpAttr = (Attribute*)elem;
			str = list->printData(tmpAttr);
		}
		if(strcmp((char*)shape, "Rectangle") == 0) 
		{
			tempRect = (Rectangle*)elem;
			str = list->printData(tempRect);
		}
		if(strcmp((char*)shape, "Circle") == 0) 
		{
			tempCircle = (Circle*)elem;
			str = list->printData(tempCircle);
		}
		if(strcmp((char*)shape, "Path") == 0)
		{
			tempPath = (Path*)elem;
			str = list->printData(tempPath);
		}
		if(strcmp((char*)shape, "Group") == 0)
		{
			tempGroup = (Group*)elem;
			str = list->printData(tempGroup);
		}
		/*
		We use the printData function that we created to return a string representation 
		of the data associated with the current node
		*/
		printf("%s\n", str);
		
		//Since list.printData dynamically allocates the string, we must free it
		free(str);
	}
}


int main(int argc, char **argv) {
    SVGimage* mySVGimage = NULL;
    char *filename = NULL;
    char *myString_representation = NULL;
    printf("Helloo world\n\n");

    if (argc != 2)  
		return 1;

    filename = (char*)malloc(sizeof(char)*strlen((char*)argv[1]) + 1 );
    strcpy(filename, argv[1]);
    mySVGimage = createSVGimage(filename);

    myString_representation = (char *)SVGimageToString(mySVGimage);
    printf("String representation of SVG image: \n%s\n", myString_representation);
	free(myString_representation);
    /* *********************** Print Lists ************************************* */
	printf("\n\n SVGImage representation of Fileds in order !!!!!!!!!!!!@@@@@@@ \n\n");
    printList(mySVGimage->otherAttributes, "Attribute");
	printf("\n");
	printList(mySVGimage->rectangles, "Rectangle");
	printf("\n");
	printList(mySVGimage->circles, "Circle");
	printf("\n");
	printList(mySVGimage->paths, "Path");
	printf("\n");
    
	
	List * Rectangles = getRects(mySVGimage);
	printf("\n All Rectangles @##########@ \n\n");
	printList(Rectangles, "Rectangle");
	printf("\n");

	//Must be called to free each node of List of pointers (Save from memory leakage)
	freeList(Rectangles);
	
	List * Circles = getCircles(mySVGimage);
	printf("\n All Circles @##########@ \n\n");
	printList(Circles, "Circle");
	printf("\n");

	//Must be called to free each node of List of pointers (Save from memory leakage)
	freeList(Circles);

	List * Groups = getGroups(mySVGimage);
	printf("\n All Groups @##########@ \n\n");
	printList(Groups, "Group");
	printf("\n");

	//Must be called to free each node of List of pointers (Save from memory leakage)
	freeList(Groups);

	List * Paths = getPaths(mySVGimage);
	printf("\n All Paths @##########@ \n\n");
	printList(Paths, "Path");
	printf("\n");

	//Must be called to free each node of List of pointers (Save from memory leakage)
	freeList(Paths);

	int numberOfAttributes = numAttr(mySVGimage); 
	printf("\n Number Of Attributes In Image : %d \n", numberOfAttributes);

	int result1 = numRectsWithArea(mySVGimage, 716404); 
	printf("\n Rectangles With Given Area in Image  : %d \n", result1);

	int result2 = numCirclesWithArea(mySVGimage, 40.69); 
	printf("\n Circles With Given Area in Image  : %d \n", result2);

	int result3 = numPathsWithdata(mySVGimage, "m49 16.7c-4.6-10.3-26.4.3-22.6-12.2.6-1.9.5-2.7-.9-2.4-7.8 1.6-13.5 9.6-11.5 16.3 6.4 21.4 41.3 12.3 35-1.7"); 
	printf("\n Paths With Given Data in Image  : %d \n", result3);

	int result4 = numGroupsWithLen(mySVGimage, 3); 
	printf("\n Groups With Given Length in Image  : %d \n", result4);

    free(filename);
    deleteSVGimage(mySVGimage);                               
	
    return 0;
}

