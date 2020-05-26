/*
	Name: Muhammad Danish Saeed
	Class: CIS*2750
	Date: February, 2019
	Task: Assignment 1
	Comments: This is header file which has the function definitions for any addtional functions I created
*/
#include <stdio.h>
#include "LinkedListAPI.h"

static Attribute * create_new_attribute(void);
static Path * create_new_path(void);
static Circle * create_new_circle(void);
static Rectangle * create_new_rectangle(void);
static Group * create_new_group(void);
static void create_svgimage_from_tree(xmlNode * root, SVGimage ** svgdoc);
static void create_Groups_Inside_Group_from_tree(xmlNode *cur_node, Group * myGrp);
static SVGimage * create_svgdoc(void);
static void deletePointersList(void * data);

/* For A2. Helper/additional functions */
static void create_tree_from_image(xmlNodePtr * root, SVGimage * image);
static bool create_Groups_Inside_Group_from_image(List * myGroups, List * groupsCheckingList, xmlNode * root_node);

/*A3 : HTML Functions */
/*file names function, repeatitive */
// static char* fileNameRepeatition(char *file);
// static SVGimage* viewPanelHTML (char * filename );
char* lists (char *filename);
char* test (char *filename);
char * requestCreateSVG(char * filename, char * title, char * description);
char* fileNameRepeatition(char *file);
void addRectangle(char* filename, char* shapeType, float xCor, float yCor, float rwCor, float hCor ,char* units);
void scaleShape(char* filename, char* shapeType, char* scaleFactor);
void TempscaleShape(char* filename, char* shapeType, char* scaleFactor);
void editAttribute(char* filename, char* title, char* description);
