/* 
	Name: Muhammad Danish Saeed
	Class: CIS*2750
	Date: January 28, 2019
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
static SVGimage * create_svgdoc(void);
static void deletePointersList(void * data);

// For A2 
static void create_tree_from_image(xmlNodePtr * root, SVGimage * image);

