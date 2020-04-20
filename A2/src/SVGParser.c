/* 
	Name: Muhammad Danish Saeed
	Class: CIS*2750
	Date: January 28, 2019
	Task: Assignment 1
	Comments: This is the main souce file which has all the functions that we were suppose to do in order to create SVGImage
	This file also contains the addtional functions that I wrote. It's definitions are in the include "addtionalFunctions.h" file.
*/
#include "SVGParser_A2.h"
#include "LinkedListAPI.h"
#include  <stdio.h>
#include  <math.h>
#include "addtionalFunctions.h"

//Creats A New Attribute Node (Helper Function)
static Attribute * create_new_attribute(void)
{
	Attribute * att = calloc(1, sizeof(Attribute));
	att->name = malloc(1000 * sizeof(char));
	att->value = malloc(100000 * sizeof(char));
	
	return att;
}
//Creats A New Path Node (Helper Function)
static Path * create_new_path(void)
{
	Path * pth = calloc(1, sizeof(Path));
	pth->data = malloc(500000 * sizeof(char));
	strcpy(pth->data, "");
	//Function pointer for each one
	char* (*attrToStr)(void*) = &attributeToString;
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(const void*, const void*) = &compareAttributes;
	pth->otherAttributes = initializeList(attrToStr, delAttr, compAttr);
	
	return pth;
}
//Creats A New Circle Node (Helper Function)
static Circle * create_new_circle(void)
{
	Circle * cir = calloc(1, sizeof(Circle));
	cir->cx = 0.0;
	cir->cy = 0.0;
	cir->r = 0.0;
	strcpy(cir->units, "");

	//Function pointer for each one
	char* (*attrToStr)(void*) = &attributeToString;
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(const void*, const void*) = &compareAttributes;
	cir->otherAttributes = initializeList(attrToStr, delAttr, compAttr);
	
	return cir;
}
//Creats A New Rectangle Node (Helper Function)
static Rectangle * create_new_rectangle(void)
{
	Rectangle * rec = calloc(1, sizeof(Rectangle));
	rec->x = 0.0;
	rec->y = 0.0;
	rec->width = 0.0;
	rec->height = 0.0;
	strcpy(rec->units, "");

	//Function pointer for each one
	char* (*attrToStr)(void*) = &attributeToString;
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(const void*, const void*) = &compareAttributes;
	rec->otherAttributes = initializeList(attrToStr, delAttr, compAttr);
	
	return rec;
}
//Creats A New Group Node (Helper Function)
static Group * create_new_group(void)
{
	Group * grp = malloc(sizeof(Group));

	//Function pointer for each one
	//Creating Rectangle inside Group
    char* (*rectToStr)(void*) = &rectangleToString;
	void (*delRect)(void*) = &deleteRectangle;
	int (*compRect)(const void*, const void*) = &compareRectangles;
    
    grp->rectangles = initializeList(rectToStr, delRect, compRect);
    //Creating Circle inside Group
    char* (*circleToStr)(void*) = &circleToString;
	void (*delcircle)(void*) = &deleteCircle;
	int (*compCircle)(const void*, const void*) = &compareCircles;
    
    grp->circles = initializeList(circleToStr, delcircle, compCircle);
    //Creating Path inside Group
    char* (*pathsToStr)(void*) = &pathToString;
	void (*delPaths)(void*) = &deletePath;
	int (*compPaths)(const void*, const void*) = &comparePaths;
    
    grp->paths = initializeList(pathsToStr, delPaths, compPaths);
	//Creating Groups inside Group
	char* (*grpToStr)(void*) = &groupToString;
	void (*delGrp)(void*) = &deleteGroup;
	int (*compGrp)(const void*, const void*) = &compareGroups;
    
    grp->groups = initializeList(grpToStr, delGrp, compGrp);

	//Creating Attribute inside Group
	char* (*attrToStr)(void*) = &attributeToString;
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(const void*, const void*) = &compareAttributes;
	grp->otherAttributes = initializeList(attrToStr, delAttr, compAttr);
	
	return grp;
}
static void create_Groups_Inside_Group_from_tree(xmlNode *cur_node, Group * myGrp)
{
	xmlAttr *attr;
	Group * grp = create_new_group();
	if(cur_node->children != NULL)
	{
		//Going Through All Attributes
		xmlNode * grp_node = NULL;
		for(grp_node = cur_node->children->next; grp_node != NULL; grp_node = grp_node->next)
		{
			//Called If There is Rectangle inside group
			if(strcmp((char *)grp_node->name, "rect") == 0) 
			{
				//Calling Helper Function
				Rectangle * rec = create_new_rectangle();
				for (attr = grp_node->properties; attr != NULL; attr = attr->next)
				{
					//Getting each Attribute Name and Content
					xmlNode *value = attr->children;
					char *attrName = (char *)attr->name;
					char *cont = (char *)(value->content);
					//Check for Attributes 
					if(strcmp(attrName, "x") == 0)
					{
						int i = 0, j = 0;
						while(cont[i] != '\0')
						{
							if(((cont[i] < '0') || (cont[i] > '9')) && (cont[i] != '.'))
								rec->units[j++] = cont[i];
							i++;
						}
						rec->x = atof(cont);
					}
					else if(strcmp(attrName, "y") == 0)
						rec->y = atof(cont);
					else if(strcmp(attrName, "width") == 0)
						rec->width = atof(cont);
					else if(strcmp(attrName, "height") == 0)
						rec->height = atof(cont);
					else {
						//For any attribute, we add it in OtherAttributes List
						Attribute * grp_rec_attr = create_new_attribute();
						strcpy(grp_rec_attr->name, attrName);
						strcpy(grp_rec_attr->value, cont);
						insertBack(rec->otherAttributes, grp_rec_attr);
					}
				}
				insertBack(grp->rectangles, rec);
			}//Called If There is Circle inside group
			else if(strcmp((char *)grp_node->name, "circle") == 0) 
			{
				//Calling Helper Function
				Circle * cir = create_new_circle();
				for (attr = grp_node->properties; attr != NULL; attr = attr->next)
				{
					//Getting each Attribute Name and Content
					xmlNode *value = attr->children;
					char *attrName = (char *)attr->name;
					char *cont = (char *)(value->content);
					//Check for Attributes 
					if(strcmp(attrName, "cx") == 0)
					{
						int i = 0, j = 0;
						while(cont[i] != '\0')
						{
							if(((cont[i] < '0') || (cont[i] > '9')) && (cont[i] != '.'))
								cir->units[j++] = cont[i];
							i++;
						}
						cir->cx = atof(cont);
					}
					else if(strcmp(attrName, "cy") == 0)
						cir->cy = atof(cont);
					else if(strcmp(attrName, "r") == 0)
						cir->r = atof(cont);
					else {
						//For any attribute, we add it in OtherAttributes List
						Attribute * grp_cir_attr = create_new_attribute();
						strcpy(grp_cir_attr->name, attrName);
						strcpy(grp_cir_attr->value, cont);
						insertBack(cir->otherAttributes, grp_cir_attr);
					}
				}
				insertBack(grp->circles, cir);
			}//Called If There is Path inside group
			else if(strcmp((char *)grp_node->name, "path") == 0) 
			{
				//Calling Helper Function
				Path * pth = create_new_path();
				for (attr = grp_node->properties; attr != NULL; attr = attr->next)
				{
					//Getting each Attribute Name and Content
					xmlNode *value = attr->children;
					char *attrName = (char *)attr->name;
					char *cont = (char *)(value->content);
					//Check for Attributes Data
					if(strcmp(attrName, "d") == 0)
						strcpy(pth->data, cont);
					else {
						//For any attribute other than 'd' we add it in OtherAttributes List
						Attribute * grp_pth_attr = create_new_attribute();
						strcpy(grp_pth_attr->name,attrName);
						strcpy(grp_pth_attr->value, cont);
						insertBack(pth->otherAttributes, grp_pth_attr);
					}
				}
				insertBack(grp->paths, pth);
			}
			//Called If There is group inside group
			else if(strcmp((char *)grp_node->name, "g") == 0) 
			{
				create_Groups_Inside_Group_from_tree(grp_node, grp);
			}
			else{
				//For any other child of group do nothing
			}
		}
	}
	for (attr = cur_node->properties; attr != NULL; attr = attr->next)
	{
		//Getting each Attribute Name and Content
		xmlNode *value = attr->children;
		char *attrName = (char *)attr->name;
		char *cont = (char *)(value->content);
						
		//For any attribute, we add it in OtherAttributes List
		Attribute * grp_attr = create_new_attribute();
		strcpy(grp_attr->name, attrName);
		strcpy(grp_attr->value, cont);
		insertBack(grp->otherAttributes, grp_attr);
	}
	insertBack(myGrp->groups, grp);
}
static void create_svgimage_from_tree(xmlNode * root, SVGimage ** svgdoc)
{
	if(root == NULL)
		return;
	xmlAttr *attr;
	SVGimage * svg = *svgdoc;
    xmlNode *cur_node = NULL;
    for (cur_node = root; cur_node != NULL; cur_node = cur_node->next)
    {
		{
			if (cur_node->type == XML_ELEMENT_NODE) {
				//Check for the svg tag
				if(strcmp((char *)cur_node->name, "svg") == 0) 
				{
					if(cur_node->children != NULL)
					{
						//Checking for (Optional) elements title and desc in children of <svg> tag
						xmlNode * svg_node = NULL;
						for(svg_node = cur_node->children->next; svg_node != NULL; svg_node = svg_node->next->next)
						{
							char * content;
							if(strcmp((char *)svg_node->name, "title") == 0){
							    content = (char*)xmlNodeGetContent(svg_node);
								strcpy(svg->title,content );
								free(content);
							}
							else if(strcmp((char *)svg_node->name, "desc") == 0) {
								content = (char*)xmlNodeGetContent(svg_node);
								strcpy(svg->description, content);
								free(content);
							}
						}
					}
					strcpy(svg->namespace, (char *)root->ns->href);
					for (attr = cur_node->properties; attr != NULL; attr = attr->next)
					{
						//Getting each Attribute Name and Content
						xmlNode *value = attr->children;
						char *attrName = (char *)attr->name;
						char *cont = (char *)(value->content);
						//For any attribute other than 'xmlns' we add it in OtherAttributes List
						Attribute * svg_attr = create_new_attribute();
						strcpy(svg_attr->name,attrName);
						strcpy(svg_attr->value, cont);
						insertBack(svg->otherAttributes, svg_attr);	
					}
				}
				//Check for the Rectangle tag
				if(strcmp((char*)cur_node->name, "rect") == 0) 
				{
					//Calling Helper Function
					Rectangle * rec = create_new_rectangle();
					for (attr = cur_node->properties; attr != NULL; attr = attr->next)
					{
						//Getting each Attribute Name and Content
						xmlNode *value = attr->children;
						char *attrName = (char *)attr->name;
						char *cont = (char *)(value->content);
						//Check for Attributes 
						if(strcmp(attrName, "x") == 0)
						{
							int i = 0, j = 0;
							while(cont[i] != '\0')
							{
								if(((cont[i] < '0') || (cont[i] > '9')) && (cont[i] != '.'))
									rec->units[j++] = cont[i];
								i++;
							}
							rec->x = atof(cont);
						}
						else if(strcmp(attrName, "y") == 0)
							rec->y = atof(cont);
						else if(strcmp(attrName, "width") == 0)
							rec->width = atof(cont);
						else if(strcmp(attrName, "height") == 0)
							rec->height = atof(cont);
						else {
							//For any attribute, we add it in OtherAttributes List
							Attribute * rec_attr = create_new_attribute();
							strcpy(rec_attr->name, attrName);
							strcpy(rec_attr->value, cont);
							insertBack(rec->otherAttributes, rec_attr);
						}
					}
					insertBack(svg->rectangles, rec);
				}
				//Check for the Circle tag
				if(strcmp((char*)cur_node->name, "circle") == 0) 
				{
					//Calling Helper Function
					Circle * cir = create_new_circle();
					for (attr = cur_node->properties; attr != NULL; attr = attr->next)
					{
						//Getting each Attribute Name and Content
						xmlNode *value = attr->children;
						char *attrName = (char *)attr->name;
						char *cont = (char *)(value->content);
						//Check for Attributes 
						if(strcmp(attrName, "cx") == 0)
						{
							int i = 0, j = 0;
							while(cont[i] != '\0')
							{
								if(((cont[i] < '0') || (cont[i] > '9')) && (cont[i] != '.'))
									cir->units[j++] = cont[i];
								i++;
							}
							cir->cx = atof(cont);
						}
						else if(strcmp(attrName, "cy") == 0)
							cir->cy = atof(cont);
						else if(strcmp(attrName, "r") == 0)
							cir->r = atof(cont);
						else {
							//For any attribute, we add it in OtherAttributes List
							Attribute * cir_attr = create_new_attribute();
							strcpy(cir_attr->name, attrName);
							strcpy(cir_attr->value, cont);
							insertBack(cir->otherAttributes, cir_attr);
						}
					}
					insertBack(svg->circles, cir);
				}
				//Check for the Path tag
				if(strcmp((char *)cur_node->name, "path") == 0) 
				{
					//Calling Helper Function
					Path * pth = create_new_path();
					for (attr = cur_node->properties; attr != NULL; attr = attr->next)
					{
						//Getting each Attribute Name and Content
						xmlNode *value = attr->children;
						char *attrName = (char *)attr->name;
						char *cont = (char *)(value->content);
						//Check for Attributes Data
						if(strcmp(attrName, "d") == 0)
							strcpy(pth->data, cont);
						else {
							//For any attribute other than 'd' we add it in OtherAttributes List
							Attribute * pth_attr = create_new_attribute();
							strcpy(pth_attr->name,attrName);
							strcpy(pth_attr->value, cont);
							insertBack(pth->otherAttributes, pth_attr);
						}
					}
					insertBack(svg->paths, pth);
				}
				//Check for the Group tag
				if(strcmp((char*)cur_node->name, "g") == 0) 
				{
					Group * grp = create_new_group();
					if(cur_node->children != NULL)
					{
						//Going Through All Attributes
						xmlNode * grp_node = NULL;
						for(grp_node = cur_node->children->next; grp_node != NULL; grp_node = grp_node->next)
						{
							//Called If There is Rectangle inside group
							if(strcmp((char *)grp_node->name, "rect") == 0) 
							{
								//Calling Helper Function
								Rectangle * rec = create_new_rectangle();
								for (attr = grp_node->properties; attr != NULL; attr = attr->next)
								{
									//Getting each Attribute Name and Content
									xmlNode *value = attr->children;
									char *attrName = (char *)attr->name;
									char *cont = (char *)(value->content);
									//Check for Attributes 
									if(strcmp(attrName, "x") == 0)
									{
										int i = 0, j = 0;
										while(cont[i] != '\0')
										{
											if(((cont[i] < '0') || (cont[i] > '9')) && (cont[i] != '.'))
												rec->units[j++] = cont[i];
											i++;
										}
										rec->x = atof(cont);
									}
									else if(strcmp(attrName, "y") == 0)
										rec->y = atof(cont);
									else if(strcmp(attrName, "width") == 0)
										rec->width = atof(cont);
									else if(strcmp(attrName, "height") == 0)
										rec->height = atof(cont);
									else {
										//For any attribute, we add it in OtherAttributes List
										Attribute * grp_rec_attr = create_new_attribute();
										strcpy(grp_rec_attr->name, attrName);
										strcpy(grp_rec_attr->value, cont);
										insertBack(rec->otherAttributes, grp_rec_attr);
									}
								}
								insertBack(grp->rectangles, rec);
							}//Called If There is Circle inside group
							else if(strcmp((char *)grp_node->name, "circle") == 0) 
							{
								//Calling Helper Function
								Circle * cir = create_new_circle();
								for (attr = grp_node->properties; attr != NULL; attr = attr->next)
								{
									//Getting each Attribute Name and Content
									xmlNode *value = attr->children;
									char *attrName = (char *)attr->name;
									char *cont = (char *)(value->content);
									//Check for Attributes 
									if(strcmp(attrName, "cx") == 0)
									{
										int i = 0, j = 0;
										while(cont[i] != '\0')
										{
											if(((cont[i] < '0') || (cont[i] > '9')) && (cont[i] != '.'))
												cir->units[j++] = cont[i];
											i++;
										}
										cir->cx = atof(cont);
									}
									else if(strcmp(attrName, "cy") == 0)
										cir->cy = atof(cont);
									else if(strcmp(attrName, "r") == 0)
										cir->r = atof(cont);
									else {
										//For any attribute, we add it in OtherAttributes List
										Attribute * grp_cir_attr = create_new_attribute();
										strcpy(grp_cir_attr->name, attrName);
										strcpy(grp_cir_attr->value, cont);
										insertBack(cir->otherAttributes, grp_cir_attr);
									}
								}
								insertBack(grp->circles, cir);
							}//Called If There is Path inside group
							else if(strcmp((char *)grp_node->name, "path") == 0) 
							{
								//Calling Helper Function
								Path * pth = create_new_path();
								for (attr = grp_node->properties; attr != NULL; attr = attr->next)
								{
									//Getting each Attribute Name and Content
									xmlNode *value = attr->children;
									char *attrName = (char *)attr->name;
									char *cont = (char *)(value->content);
									//Check for Attributes Data
									if(strcmp(attrName, "d") == 0)
										strcpy(pth->data, cont);
									else {
										//For any attribute other than 'd' we add it in OtherAttributes List
										Attribute * grp_pth_attr = create_new_attribute();
										strcpy(grp_pth_attr->name,attrName);
										strcpy(grp_pth_attr->value, cont);
										insertBack(pth->otherAttributes, grp_pth_attr);
									}
								}
								insertBack(grp->paths, pth);
							}
							//Called If There is group inside group
							else if(strcmp((char *)grp_node->name, "g") == 0) 
							{
								create_Groups_Inside_Group_from_tree(grp_node, grp);
							}
							else{
								//For any other child of group do nothing
							}
						}
					}
					for (attr = cur_node->properties; attr != NULL; attr = attr->next)
					{
						//Getting each Attribute Name and Content
						xmlNode *value = attr->children;
						char *attrName = (char *)attr->name;
						char *cont = (char *)(value->content);
						
						//For any attribute, we add it in OtherAttributes List
						Attribute * grp_attr = create_new_attribute();
						strcpy(grp_attr->name, attrName);
						strcpy(grp_attr->value, cont);
						insertBack(grp->otherAttributes, grp_attr);
					}
					insertBack(svg->groups, grp);
				}
			} 
		}
        create_svgimage_from_tree(cur_node->children, svgdoc);
    }
    *svgdoc = svg;
}
//Create and Initialize the SVGimage Struct (Helper Function)
static SVGimage * create_svgdoc(void)
{
    SVGimage * svgdoc = malloc(sizeof(SVGimage));

	strcpy(svgdoc->namespace, "");
	strcpy(svgdoc->description, "");
	strcpy(svgdoc->title, "");
    //Creating Rectangles
    char* (*rectToStr)(void*) = &rectangleToString;
	void (*delRect)(void*) = &deleteRectangle;
	int (*compRect)(const void*, const void*) = &compareRectangles;
    
    svgdoc->rectangles = initializeList(rectToStr, delRect, compRect);
    //Creating Circles
    char* (*circleToStr)(void*) = &circleToString;
	void (*delcircle)(void*) = &deleteCircle;
	int (*compCircle)(const void*, const void*) = &compareCircles;
    
    svgdoc->circles = initializeList(circleToStr, delcircle, compCircle);
    //Creating Paths
    char* (*pathsToStr)(void*) = &pathToString;
	void (*delPaths)(void*) = &deletePath;
	int (*compPaths)(const void*, const void*) = &comparePaths;
    
    svgdoc->paths = initializeList(pathsToStr, delPaths, compPaths);
	//Creating Groups
	char* (*grpToStr)(void*) = &groupToString;
	void (*delGrp)(void*) = &deleteGroup;
	int (*compGrp)(const void*, const void*) = &compareGroups;
    
    svgdoc->groups = initializeList(grpToStr, delGrp, compGrp);

	//Creating Attributes
	char* (*attrToStr)(void*) = &attributeToString;
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(const void*, const void*) = &compareAttributes;
	svgdoc->otherAttributes = initializeList(attrToStr, delAttr, compAttr);
    
    return svgdoc;
}

/* @Helper Functions (Saving Us From Memory Leak)*/
static void deletePointersList(void * data){	
	/*We Do Not do anything inside it because it is a list 
	of pointers to the actual structs and we do not want to delete Data from those structs.
	Just like the other deleteFunctions for structs. We just only free the memory allocated by each node*/
}

SVGimage* createSVGimage(char* fileName)
{
	/** Function to create a string representation of an SVG object.
	 *@pre SVGimgage exists, is not null, and is valid
	 *@post SVGimgage has not been modified in any way, and a string representing the SVG contents has been created
	 *@return a string contaning a humanly readable representation of an SVG object
	 *@param obj - a pointer to an SVG struct
	**/
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

 	if(fileName == NULL)
 	{
 		return NULL;
 	}

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /*parse the file and get the DOM */
    doc = xmlReadFile(fileName, NULL, 0);

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

	//Creating an empty SVGimage struct 
	SVGimage *SvgDoc = create_svgdoc();

    /* ********************************************************************************* */
 	/* ***DO I ALSO CHECK IF ROOTELEMENT IS NULL?????? */
 	/* ********************************************************************************* */
    if (doc == NULL) {
        return NULL;
    }

    /* ********************************************************************************* */
    /* Make helper function OR code it here and use SVGimage struct pointer, 
       save the data into appropriate variables & then return the struct pointer */
    /* ********************************************************************************* */
	//Calling The Helper Function
	create_svgimage_from_tree(root_element, &SvgDoc);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
    */
    xmlCleanupParser();

	/* ********************************************************************************* */
	/* Return the SVGImage Struct pointer*/
	/* ********************************************************************************* */
    return SvgDoc;
}

char* SVGimageToString(SVGimage* img)
{
	/** Function to delete image content and free all the memory.
	 *@pre SVGimgage  exists, is not null, and has not been freed
	 *@post SVSVGimgageG  had been freed
	 *@return none
	 *@param obj - a pointer to an SVG struct
	**/
	 if (img == NULL)
	{
 		return NULL;
	}

	//Allocating a string Array 
    char* string = (char *) malloc((5000000 * sizeof(img)) * sizeof(char));
	char * data1;
	//Placing that SVGImage struct data in String array
	strcpy(string, "Namespace: ");
	strcat(string, img->namespace);
	strcat(string, "\nTitle: ");
	strcat(string, img->title);
	strcat(string, "\nDescription: ");
	strcat(string, img->description);
	strcat(string, "\nRectangles: [");
	data1 = toString(img->rectangles);
	strcat(string, data1);
	free(data1);
	strcat(string, "]\n ,Circles: [");
	data1 = toString(img->circles);
	strcat(string, data1);
	free(data1);
	strcat(string, "]\n ,Paths[");
	data1 = toString(img->paths);
	strcat(string, data1);
	free(data1);
	strcat(string, "]");
	strcat(string, "\n ,Groups[");
	data1 = toString(img->groups);
	strcat(string, data1);
	strcat(string, "]");
	free(data1);
	strcat(string, "\n ,OtherAttributes[");
	data1 = toString(img->otherAttributes);
	strcat(string, data1);
	strcat(string, "]");
	free(data1);

	return string;
}
void deleteSVGimage(SVGimage* img)
{
    if (img != NULL)
    {
     /* Use linkedList API freeList() function to free */
        freeList(img->rectangles);
        freeList(img->circles);
        freeList(img->paths);
        freeList(img->groups);
        freeList(img->otherAttributes);

        /*free the img at the end */
        free(img);    
    }
}

/* For the four "get..." functions below, make sure you return a list of pointers to the existing structs 
 - do not allocate new structs.  They all share the same format, and only differ in the contents of the lists 
 they return.
 
 *@pre SVGimgage exists, is not null, and has not been freed
 *@post SVGimgage has not been modified in any way
 *@return a newly allocated List of components.  While the List struct itself is new, the components in it are just pointers
  to the ones in the image.

 The list must me empty if the element is not found - do not return NULL

 *@param obj - a pointer to an SVG struct
 */

// Function that returns a list of all rectangles in the image.  
List* getRects(SVGimage* img)
{
	//Creating Rectangles List
    char* (*rectToStr)(void*) = &rectangleToString;
	//Helper Function used
	void (*delRect)(void*) = &deletePointersList;
	int (*compRect)(const void*, const void*) = &compareRectangles;
	List * list_of_pointers_of_rectangles = initializeList(rectToStr, delRect, compRect);
    if (img == NULL)
    {
        return list_of_pointers_of_rectangles;
    }
	//Creating Iterator For Rectangles List
	ListIterator itr_list = createIterator(img->rectangles);
	Rectangle * rec = nextElement(&itr_list);
	while (rec != NULL)
	{	
		insertBack(list_of_pointers_of_rectangles, rec);
		rec = nextElement(&itr_list);
	}
	return list_of_pointers_of_rectangles;
}

// Function that returns a list of all circles in the image.  
List* getCircles(SVGimage* img)
{
	//Creating Circles List
    char* (*circleToStr)(void*) = &circleToString;
	//Helper Function used
	void (*delcircle)(void*) = &deletePointersList;
	int (*compCircle)(const void*, const void*) = &compareCircles;
	List * list_of_pointers_of_circles = initializeList(circleToStr, delcircle, compCircle);
    if (img == NULL)
    {
        return list_of_pointers_of_circles;
    }
	//Creating Iterator For Circles List
	ListIterator itr_list = createIterator(img->circles);
	Circle * cir = nextElement(&itr_list);
	while (cir != NULL)
	{	
		insertBack(list_of_pointers_of_circles, cir);
		cir = nextElement(&itr_list);
	}	
	return list_of_pointers_of_circles;
}

// Function that returns a list of all groups in the image.  
List* getGroups(SVGimage* img)
{
    //Creating Groups List
    char* (*grpToStr)(void*) = &groupToString;
	//Helper Function used
	void (*delGrp)(void*) = &deletePointersList;
	int (*compGrp)(const void*, const void*) = &compareGroups;
	List * list_of_pointers_of_groups = initializeList(grpToStr, delGrp, compGrp);
    if (img == NULL)
    {
        return list_of_pointers_of_groups;
    }
	//Creating Iterator For Groups List
	ListIterator itr_list = createIterator(img->groups);
	Group * grp = nextElement(&itr_list);
	while (grp != NULL)
	{	
		insertBack(list_of_pointers_of_groups, grp);
		grp = nextElement(&itr_list);
	}	
	return list_of_pointers_of_groups;
}

// Function that returns a list of all paths in the image.  
List* getPaths(SVGimage* img)
{
    //Creating Groups List
    char* (*pathsToStr)(void*) = &pathToString;
	//Helper Function used
	void (*delPaths)(void*) = &deletePointersList;
	int (*compPaths)(const void*, const void*) = &comparePaths;
	List * list_of_pointers_of_paths = initializeList(pathsToStr, delPaths, compPaths);
    if (img == NULL)
    {
        return list_of_pointers_of_paths;
    }
	//Creating Iterator For Paths List
	ListIterator itr_list = createIterator(img->paths);
	Path * pth = nextElement(&itr_list);
	while (pth != NULL)
	{	
		insertBack(list_of_pointers_of_paths, pth);
		pth = nextElement(&itr_list);
	}	
	return list_of_pointers_of_paths;
}



/* For the four "num..." functions below, you need to search the SVG image for components  that match the search 
  criterion.  You may wish to write some sort of a generic searcher fucntion that accepts an image, a predicate function,
  and a dummy search record as arguments.  We will discuss such search functions in class

 NOTE: For consistency, use the ceil() function to round the floats up to the nearest integer once you have computed 
 the number you need.  See A1 Module 2 for details.

 *@pre SVGimgage exists, is not null, and has not been freed.  The search criterion is valid
 *@post SVGimgage has not been modified in any way
 *@return an int indicating how many objects matching the criterion are contained in the image
 *@param obj - a pointer to an SVG struct
 *@param 2nd - the second param depends on the function.  See details below
 */   

// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(SVGimage* img, float area)
{
	if(img == NULL)
		return -1;
	if(area < 0)
		return -1;
	int num_of_rectangles = 0;
	//Creating Iterator For Rectangles List
	ListIterator itr_list = createIterator(img->rectangles);
	Rectangle * rec = nextElement(&itr_list);
	while (rec != NULL)
	{	
		float actual_area = rec->width * rec->height;
		//Checking The Condition
		if(ceil(actual_area) == ceil(area))
			num_of_rectangles += 1;
		rec = nextElement(&itr_list);
	}
	return num_of_rectangles;
}
// Function that returns the number of all circles with the specified area
int numCirclesWithArea(SVGimage* img, float area)
{
	if(img == NULL)
		return -1;
	if(area < 0)
		return -1;
	int num_of_circles = 0;
	//Creating Iterator For Circles List
	ListIterator itr_list = createIterator(img->circles);
	Circle * cir = nextElement(&itr_list);
	while (cir != NULL)
	{	
		float actual_area = 3.14159 * cir->r * cir->r;
		//Checking The Condition
		if(ceil(actual_area) == ceil(area))
			num_of_circles += 1;	
		cir = nextElement(&itr_list);
	}	
	return num_of_circles;
}
// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(SVGimage* img, char* data)
{
	if(img == NULL)
		return -1;
	if(data == NULL)
		return -1;	
	int num_of_paths = 0;
	//Creating Iterator For Paths List
	ListIterator itr_list = createIterator(img->paths);
	Path * pth = nextElement(&itr_list);
	while (pth != NULL)
	{	
		//Checking The Condition
		if(strcmp(pth->data , data) == 0)
			num_of_paths += 1;	
		pth = nextElement(&itr_list);
	}	
	return num_of_paths;
}
// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(SVGimage* img, int len)
{
	if(img == NULL)
		return -1;
	if(len < 0)
		return -1;
	int num_of_groups = 0;
	//Creating Iterator For Groups List
	ListIterator itr_list = createIterator(img->groups);
	Group * grp = nextElement(&itr_list);
	while (grp != NULL)
	{	
		int grp_Length = 0;
		grp_Length += getLength(grp->rectangles);	
		grp_Length += getLength(grp->circles);	
		grp_Length += getLength(grp->paths);	
		grp_Length += getLength(grp->groups);	
		if(grp_Length == len)
			num_of_groups += 1;
		grp = nextElement(&itr_list);
	}	
	return num_of_groups;
}

/*  Function that returns the total number of Attribute structs in the SVGimage - i.e. the number of Attributes
    contained in all otherAttributes lists in the structs making up the SVGimage
    *@pre SVGimgage  exists, is not null, and has not been freed.  
    *@post SVGimage has not been modified in any way
    *@return the total length of all attribute structs in the SVGimage
    *@param obj - a pointer to an SVG struct
*/
int numAttr(SVGimage* img)
{
	if(img == NULL) 
		return 0; 
	
	int num_of_attributes = 0;

	//Creating Iterator For Rectangles List
	ListIterator itr_list = createIterator(img->rectangles);
	Rectangle * rec = nextElement(&itr_list);
	while (rec != NULL)
	{	
		num_of_attributes += getLength(rec->otherAttributes);
		rec = nextElement(&itr_list);
	}
	//Creating Iterator For Circles List
	itr_list = createIterator(img->circles);
	Circle * cir = nextElement(&itr_list);
	while (cir != NULL)
	{	
		num_of_attributes += getLength(cir->otherAttributes);	
		cir = nextElement(&itr_list);
	}	
	//Creating Iterator For Paths List
	itr_list = createIterator(img->paths);
	Path * pth = nextElement(&itr_list);
	while (pth != NULL)
	{	
		num_of_attributes += getLength(pth->otherAttributes);	
		pth = nextElement(&itr_list);
	}	

	//Creating Iterator For Groups List
	itr_list = createIterator(img->groups);
	Group * grp = nextElement(&itr_list);
	while (grp != NULL)
	{	
		num_of_attributes += getLength(grp->otherAttributes);
		grp = nextElement(&itr_list);
	}	
	//SVGimage Other attributes added
	num_of_attributes += getLength(img->otherAttributes);	
	return num_of_attributes;
}


/* ******************************* List helper functions  - MUST be implemented *************************** */
void deleteAttribute( void* data)
{
    /* Cast it to Attribute type and then free */
    Attribute *castAtt = (Attribute *) data;
	if(castAtt->name != NULL)
		free(castAtt->name);
	if(castAtt->value != NULL)
		free(castAtt->value);
    /* and lastly the Attribute itself */
    free(castAtt);
}

char* attributeToString( void* data)
{
    /* Cast it to Attribute type */
    Attribute *castAtt = (Attribute *) data;

     /* ********************************************************************************* */
    /* Either make helper function here or separate c file. ASK PROF*/
	/* ********************************************************************************* */
	/*BUT I did it here */

	char* string = malloc(5000000 * sizeof(char));

	strcpy(string, "name: ");
	strcat(string, castAtt->name);
	strcat(string, ", ");
	strcat(string, "value: ");
	strcat(string, castAtt->value);
    
	return string;
}


int compareAttributes(const void *first, const void *second)
{
    Attribute *castOne = (Attribute *) first;
    Attribute *castTwo = (Attribute *) second;
        
    return (strcmp(castOne->value, castTwo->value) && strcmp(castOne->name, castTwo->name));
}


void deleteGroup(void* data)
{
    Group *castGroup = (Group *) data;
    freeList(castGroup->rectangles);
    freeList(castGroup->circles);
    freeList(castGroup->paths);
    freeList(castGroup->groups);
    freeList(castGroup->otherAttributes);
    free(castGroup);
}


char* groupToString(void* data)
{
    Group *castGroup = (Group *)data;
	//Allocating a string array
    char * string = (char*) malloc(500000 * sizeof(char));
	char * data1;
	//Putting Group Data in String Array
	strcpy(string, "{ Rectangles [");
	data1 = toString(castGroup->rectangles);
    strcat(string, data1);
	free(data1);
	strcat(string, " ]");
	strcat(string, "\nCircles [");
	data1 = toString(castGroup->circles);
    strcat(string, data1);
	free(data1);
	strcat(string, " ]");
	strcat(string, "\nPaths [");
	data1 = toString(castGroup->paths);
    strcat(string, data1);
	free(data1);
	strcat(string, " ]");
	strcat(string, "\nGroups [");
	strcat(string, " ]");
	strcat(string, "\nOtherAttributes [");
	data1 = toString(castGroup->otherAttributes);
    strcat(string, data1);
	free(data1);
	strcat(string, " ] },");

    return string;

}


int compareGroups(const void *first, const void *second)
{
    const Group * a_first = (const Group *) first;
	const Group * b_second = (const Group *) second;

	if(getLength(a_first->rectangles) == getLength(b_second->rectangles))
	{
		if(getLength(a_first->circles) == getLength(b_second->circles))
		{
			if(getLength(a_first->paths) == getLength(b_second->paths))
			{
				if(getLength(a_first->groups) == getLength(b_second->groups))
				{
					if(getLength(a_first->otherAttributes) == getLength(b_second->otherAttributes))
					{
						ListIterator itr1;
						ListIterator itr2;
						//Comparing all the rectangles within the two groups
						if((getLength(a_first->rectangles) != 0) &&( getLength(b_second->rectangles) != 0))
						{
							itr1 = createIterator(a_first->rectangles);
							Rectangle * rect1  = nextElement(&itr1);
							itr2 = createIterator(b_second->rectangles);
							Rectangle * rect2  = nextElement(&itr2);
							while((rect1 != NULL) && (rect2 != NULL))
							{
								if(!compareRectangles(rect1, rect2))
									return 0;
								rect1 = nextElement(&itr1);
								rect2 = nextElement(&itr2);
							}
						}
						//Comparing all the circles within the two groups
						if((getLength(a_first->circles) != 0) &&( getLength(b_second->circles) != 0))
						{
							itr1 = createIterator(a_first->circles);
							Circle * cir1  = nextElement(&itr1);
							itr2 = createIterator(b_second->circles);
							Circle * cir2  = nextElement(&itr2);
							while((cir1 != NULL) && (cir2 != NULL))
							{
								if(!compareCircles(cir1, cir2))
									return 0;
								cir1 = nextElement(&itr1);
								cir2 = nextElement(&itr2);
							}
						}
						//Comparing all the paths within the two groups
						if((getLength(a_first->paths) != 0) &&( getLength(b_second->paths) != 0))
						{
							itr1 = createIterator(a_first->paths);
							Path * pth1  = nextElement(&itr1);
							itr2 = createIterator(b_second->paths);
							Path * pth2  = nextElement(&itr2);
							while((pth1 != NULL) && (pth2 != NULL))
							{
								if(!comparePaths(pth1, pth2))
									return 0;
								pth1 = nextElement(&itr1);
								pth2 = nextElement(&itr2);
							}
						}
						//Comparing all the groups within the two groups
						if((getLength(a_first->groups) != 0) &&( getLength(b_second->groups) != 0))
						{
							itr1 = createIterator(a_first->groups);
							Group * grp1  = nextElement(&itr1);
							itr2 = createIterator(b_second->groups);
							Group * grp2  = nextElement(&itr2);
							while((grp1 != NULL) && (grp2 != NULL))
							{
								if(!compareGroups(grp1, grp2))
									return 0;
								grp1 = nextElement(&itr1);
								grp2 = nextElement(&itr2);
							}
						}
						//If Both the groups are same than returning 1
						return 1;
					}
					else
						return 0;
				}
				else
					return 0;
			}
			else
				return 0;
		}
		else
			return 0;
	}
	else
		return 0;
}


void deleteRectangle(void* data)
{
    Rectangle *castRec = (Rectangle *) data;
	freeList(castRec->otherAttributes);
	free(castRec);
}


char* rectangleToString(void* data)
{
	char buffer[50];
    Rectangle *castRec = (Rectangle *) data;
	//Allocating a string Array 
    char* string = (char *) malloc(5000000 * sizeof(char));
	char* data1;
	//Putting Rectangle Data in String Array
	//sprintf used for storing float value in string
    strcpy(string, "XCoordinate: ");
	sprintf(buffer, "%f", castRec->x);
    strcat(string, buffer);
	strcat(string, ",YCoordinate: ");
    sprintf(buffer, "%f", castRec->y);
    strcat(string, buffer);
	strcat(string, ",Rectangle Width: ");
    sprintf(buffer, "%f", castRec->width);
    strcat(string, buffer);
	strcat(string, ",Rectangle Height:");
    sprintf(buffer, "%f", castRec->height);
    strcat(string, buffer);
	strcat(string, ",Units: ");
    strcat(string, castRec->units);
	strcat(string, ",\nOtherAttributes [");
	data1 = toString(castRec->otherAttributes);
    strcat(string, data1);
	strcat(string, " ]");
	free(data1);
	return string;
}


int compareRectangles(const void *first, const void *second)
{
	const Rectangle * a_first = (const Rectangle *) first;
	const Rectangle * b_second = (const Rectangle *) second;
	
	if(a_first->x == b_second->x)
	{
		if(a_first->y == b_second->y)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}


void deleteCircle(void* data)
{
    Circle *castCircle = (Circle *) data;
    freeList(castCircle->otherAttributes);
	free(castCircle);
}


char* circleToString(void* data)
{
	char buffer[50];
    Circle *castCircle = (Circle *) data;
	//Allocating a string Array 
    char* string = (char *) malloc(5000000 * sizeof(char));
	char * data1;
	//Putting Circle Data in String Array
	//sprintf used for storing float value in string
    strcpy(string, "XCoordinate: ");
	sprintf(buffer, "%f", castCircle->cx);
    strcat(string, buffer);
	strcat(string, ",YCoordinate: ");
    sprintf(buffer, "%f", castCircle->cy);
    strcat(string, buffer);
	strcat(string, ",Radius: ");
    sprintf(buffer, "%f", castCircle->r);
    strcat(string, buffer);
	strcat(string, ",Units: ");
    strcat(string, castCircle->units);
	strcat(string, ",\nOtherAttributes [");
	data1 = toString(castCircle->otherAttributes);
    strcat(string, data1);
	strcat(string, " ]");
	free(data1);

	return string;
}


int compareCircles(const void *first, const void *second)
{
	const Circle * a_first = (const Circle *) first;
	const Circle * b_second = (const Circle *) second;
	
	if(a_first->cx == b_second->cx)
	{
		if(a_first->cy == b_second->cy)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void deletePath(void* data)
{
    Path *castPath = (Path *) data;
	if(castPath->data != NULL)
		free(castPath->data);
    freeList(castPath->otherAttributes);
	free(castPath);
}


char* pathToString(void* data)
{
    Path *castPath = (Path *) data;
	//Allocating a string Array 
    char* string = (char *) malloc(5000000 * sizeof(char));
	char* data1;
	//Putting Path Data in String Array
    strcpy(string, "Data: ");
    strcat(string, castPath->data);
	strcat(string, "\nOtherAttributes [");
	data1 = toString(castPath->otherAttributes);
    strcat(string, data1);
	strcat(string, " ]");
    free(data1);

    return string;
}


int comparePaths(const void *first, const void *second)
{
	const Path * a_first = (const Path *) first;
	const Path * b_second = (const Path *) second;
	
	if(strcmp(a_first->data , b_second->data) == 0)
		return 1;
	else
		return 0;
}

/* ******************************* A2 stuff *************************** */
//Used for checking groups inside Group and than creating tree accordingly 
static bool create_Groups_Inside_Group_from_image(List * myGroups, List * groupsCheckingList, xmlNodePtr root_node)
{
	int count = 0;
	char float_str[20];
	xmlNodePtr node = NULL;
	xmlNodePtr child = NULL;
	ListIterator itr = createIterator(myGroups);
	Group* grp = nextElement(&itr);
	while(grp != NULL)
	{
		if(grp->rectangles == NULL|| grp->circles == NULL || grp->paths == NULL || grp->groups == NULL || grp->otherAttributes == NULL)
			return false;
		count = 0;
		//Checking Whether the current selected Group exists in a group or seperate a group
		if(getLength(groupsCheckingList) != 0)
		{
			ListIterator itr = createIterator(groupsCheckingList);
			Group * grp_group  = nextElement(&itr);
			while(grp_group != NULL)
			{
				//If Both the Groups are same, count incremented, loop breaked
				if(compareGroups(grp, grp_group))
				{
					count++;
					break;
				}
				grp_group = nextElement(&itr);
			}
		}
		//When this Group does not exists in any of the grp->groups, Create a child node for that Group in xml Tree
		if(!count)
		{
			node = xmlNewChild(root_node, NULL, BAD_CAST "g", NULL);
			//creating the Rectangle child node, its attributes and its children
			if(getLength(grp->rectangles) != 0)
			{
				ListIterator itr = createIterator(grp->rectangles);
				Rectangle * rect  = nextElement(&itr);
				while(rect != NULL)
				{
					//Checking if every Rectangle Struct field is Within the Contraints
					if(rect->width < 0.0 || rect->height < 0.0 || rect->units == NULL || rect->otherAttributes == NULL)
						return false;
					child = xmlNewChild(node, NULL, BAD_CAST "rect", NULL);
					snprintf(float_str, sizeof(float_str), "%.2f", rect->x);
					strcat(float_str, rect->units);
					xmlNewProp(child, BAD_CAST "x", BAD_CAST float_str);
			
					snprintf(float_str, sizeof(float_str), "%.2f", rect->y);
					strcat(float_str, rect->units);
					xmlNewProp(child, BAD_CAST "y", BAD_CAST float_str);

					snprintf(float_str, sizeof(float_str), "%.2f", rect->width);
					strcat(float_str, rect->units);
					xmlNewProp(child, BAD_CAST "width", BAD_CAST float_str);

					snprintf(float_str, sizeof(float_str), "%.2f", rect->height);
					strcat(float_str, rect->units);
					xmlNewProp(child, BAD_CAST "height", BAD_CAST float_str);

					if(getLength(rect->otherAttributes) != 0)
					{	
						ListIterator inner_itr = createIterator(rect->otherAttributes);
				
						Attribute * attr = nextElement(&inner_itr);
						while(attr != NULL)
						{
							//Checking whether the attribute name or value is NULL
							if(attr->name == NULL || attr->value == NULL)
								return false;
							xmlNewProp(child, BAD_CAST attr->name, BAD_CAST attr->value);	
							attr = nextElement(&inner_itr);
						}
					}
					rect = nextElement(&itr);
				}
			}
			//creating the Circle child node, its attributes and its children
			if(getLength(grp->circles) != 0)
			{
				ListIterator itr = createIterator(grp->circles);
				Circle* cir = nextElement(&itr);
		
				while(cir != NULL)
				{
					//Checking if every Circle Struct field is Within the Contraints
					if(cir->r < 0 || cir->units == NULL || cir->otherAttributes == NULL)
						return false;
					child = xmlNewChild(node, NULL, BAD_CAST "circle", NULL);
					snprintf(float_str, sizeof(float_str), "%.2f", cir->cx);
					strcat(float_str, cir->units);
					xmlNewProp(child, BAD_CAST "cx", BAD_CAST float_str);
			
					snprintf(float_str, sizeof(float_str), "%.2f", cir->cy);
					strcat(float_str, cir->units);
					xmlNewProp(child, BAD_CAST "cy", BAD_CAST float_str);

					snprintf(float_str, sizeof(float_str), "%.2f", cir->r);
					strcat(float_str, cir->units);
					xmlNewProp(child, BAD_CAST "r", BAD_CAST float_str);
		
					if(getLength(cir->otherAttributes) != 0)
					{
						ListIterator inner_itr = createIterator(cir->otherAttributes);
						Attribute * attr = nextElement(&inner_itr);
						while(attr != NULL)
						{
							//Checking whether the attribute name or value is NULL
							if(attr->name == NULL || attr->value == NULL)
								return false;
							xmlNewProp(child, BAD_CAST attr->name, BAD_CAST attr->value);
							attr = nextElement(&inner_itr);
						}
					}
					cir = nextElement(&itr);
				}
			}
			//creating the Path child node, its attributes and its children
			if(getLength(grp->paths) != 0)
			{
				ListIterator itr = createIterator(grp->paths);
				Path* pth = nextElement(&itr);
		
				while(pth != NULL)
				{
					//Checking if every Path Struct field is Within the Contraints
					if(pth->data == NULL || pth->otherAttributes == NULL)
						return false;
					child = xmlNewChild(node, NULL, BAD_CAST "path", NULL);
					xmlNewProp(child, BAD_CAST "d", BAD_CAST pth->data);
		
					if(getLength(pth->otherAttributes) != 0)
					{
						ListIterator inner_itr = createIterator(pth->otherAttributes);
						Attribute * attr = nextElement(&inner_itr);
						while(attr != NULL)
						{
							//Checking whether the attribute name or value is NULL
							if(attr->name == NULL || attr->value == NULL)
								return false;
							xmlNewProp(child, BAD_CAST attr->name, BAD_CAST attr->value);
							attr = nextElement(&inner_itr);
						}
					}
					pth = nextElement(&itr);
				}
			}
			//creating the Group child node, its attributes and its children
			if(getLength(grp->groups) != 0)
			{
				if(!create_Groups_Inside_Group_from_image(grp->groups, groupsCheckingList, node))
					return false;
			}
			//creating Group Other properties
			if(getLength(grp->otherAttributes) != 0)
			{
				ListIterator inner_itr = createIterator(grp->otherAttributes);
				Attribute * attr = nextElement(&inner_itr);
				while(attr != NULL)
				{
					//Checking whether the attribute name or value is NULL
					if(attr->name == NULL || attr->value == NULL)
						return false;
					xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
					attr = nextElement(&inner_itr);
				}
			}
		}
		//Here Adding the groups present inside a group in the list
		insertBack(groupsCheckingList, grp);
		grp = nextElement(&itr);
	}
	return true;
}
/** Function to validating an existing a SVGimage object against a SVG schema file
 *@pre 
    SVGimage object exists and is not NULL
    schema file name is not NULL/empty, and represents a valid schema file
 *@post SVGimage has not been modified in any way
 *@return the boolean aud indicating whether the SVGimage is valid
 *@param obj - a pointer to a GPXSVGimagedoc struct
 *@param obj - the name iof a schema file
 **/
 static void create_tree_from_image(xmlNodePtr * root, SVGimage * image)
{
	int count = 0;
	xmlNodePtr root_node = *root;
	xmlNodePtr node = NULL;
	xmlNodePtr child = NULL;

	//Checking if any of the SVGimage Struct field is Null
	if(image->namespace == NULL || image->title == NULL || image->description == NULL || image->rectangles == NULL|| image->circles == NULL || image->paths == NULL || image->groups == NULL|| image->otherAttributes == NULL)
	{
		*root = NULL;
		return;
	}

	//Creating Rectangles List
    char* (*rectToStr)(void*) = &rectangleToString;
	void (*delRect)(void*) = &deletePointersList;
	int (*compRect)(const void*, const void*) = &compareRectangles;
	List* rectangles = initializeList(rectToStr, delRect, compRect);

	//Creating Circles List
    char* (*circleToStr)(void*) = &circleToString;
	void (*delcircle)(void*) = &deletePointersList;
	int (*compCircle)(const void*, const void*) = &compareCircles;
    
    List* circles = initializeList(circleToStr, delcircle, compCircle);
    //Creating Paths List
    char* (*pathsToStr)(void*) = &pathToString;
	void (*delPaths)(void*) = &deletePointersList;
	int (*compPaths)(const void*, const void*) = &comparePaths;
    
    List* paths = initializeList(pathsToStr, delPaths, compPaths);

	//Creating Groups inside Group
	char* (*grpToStr)(void*) = &groupToString;
	void (*delGrp)(void*) = &deletePointersList;
	int (*compGrp)(const void*, const void*) = &compareGroups;
    
    List* groups = initializeList(grpToStr, delGrp, compGrp);

	//Creating Namespace, Title and description attributes in xml Tree
	xmlNsPtr ns = xmlNewNs(root_node, BAD_CAST image->namespace, NULL);
	xmlSetNs(root_node, ns);
	if(strcmp(image->title, "") != 0) 
		xmlNewChild(root_node, NULL, BAD_CAST "title", BAD_CAST image->title);
	if(strcmp(image->description, "") != 0) 
		xmlNewChild(root_node, NULL, BAD_CAST "desc", BAD_CAST image->description);
	char float_str[20];
	
	//Checking all the Groups in img->groups, 
	//Putting the Rectangles, Circles and Paths in Their appropriate Lists  of paths, rectangles, circles
	if(getLength(image->groups) != 0)
	{
		ListIterator itr = createIterator(image->groups);
		Group* grp = nextElement(&itr);
		
		while(grp != NULL)
		{
			if(grp->rectangles == NULL|| grp->circles == NULL || grp->paths == NULL || grp->groups == NULL || grp->otherAttributes == NULL)
			{
				//Free the List Resources
				freeList(rectangles);
				freeList(circles);
				freeList(paths);
				freeList(groups);
				*root = NULL;
				return;
			}
			//Checking all the Rectangles, Putting it in the list rectangles
			if(getLength(grp->rectangles) != 0)
			{
				ListIterator itr = createIterator(grp->rectangles);
				Rectangle * rect  = nextElement(&itr);
				while(rect != NULL)
				{
					//Checking if every Rectangle Struct field is Within the Contraints
					if(rect->width < 0.0 || rect->height < 0.0 || rect->units == NULL || rect->otherAttributes == NULL)
					{
						//Free the List Resources
						freeList(rectangles);
						freeList(circles);
						freeList(paths);
						freeList(groups);
						*root = NULL;
						return;
					}
					if(getLength(rect->otherAttributes) != 0)
					{	
						ListIterator inner_itr = createIterator(rect->otherAttributes);
						Attribute * attr = nextElement(&inner_itr);
						while(attr != NULL)
						{
							//Checking whether the attribute name or value is NULL
							if(attr->name == NULL || attr->value == NULL)
							{
								//Free the List Resources
								freeList(rectangles);
								freeList(circles);
								freeList(paths);
								freeList(groups);
								*root = NULL;
								return;
							}
							attr = nextElement(&inner_itr);
						}
					}
					//Adding group rectangle in rectangles List
					insertBack(rectangles, rect);
					rect = nextElement(&itr);
				}
			}
			//Checking all the Circles, Putting it in the list circles
			if(getLength(grp->circles) != 0)
			{
				ListIterator itr = createIterator(grp->circles);
				Circle* cir = nextElement(&itr);
				while(cir != NULL)
				{
					//Checking if every Circle Struct field is Within the Contraints
					if(cir->r < 0 || cir->units == NULL || cir->otherAttributes == NULL)
					{
						//Free the List Resources
						freeList(rectangles);
						freeList(circles);
						freeList(paths);
						freeList(groups);
						*root = NULL;
						return;
					}
					if(getLength(cir->otherAttributes) != 0)
					{
						ListIterator inner_itr = createIterator(cir->otherAttributes);
						Attribute * attr = nextElement(&inner_itr);
						while(attr != NULL)
						{
							//Checking whether the attribute name or value is NULL
							if(attr->name == NULL || attr->value == NULL)
							{
								//Free the List Resources
								freeList(rectangles);
								freeList(circles);
								freeList(paths);
								freeList(groups);
								*root = NULL;
								return;
							}
							attr = nextElement(&inner_itr);
						}
					}
					//Adding Group Circle in circles List
					insertBack(circles, cir);
					cir = nextElement(&itr);
				}
			}
			//Checking all the Paths, Putting it in the list paths
			if(getLength(grp->paths) != 0)
			{
				ListIterator itr = createIterator(grp->paths);
				Path* pth = nextElement(&itr);
				while(pth != NULL)
				{
					//Checking if every Path Struct field is Within the Contraints
					if(pth->data == NULL || pth->otherAttributes == NULL)
					{
						//Free the List Resources
						freeList(rectangles);
						freeList(circles);
						freeList(paths);
						freeList(groups);
						*root = NULL;
						return;
					}
					if(getLength(pth->otherAttributes) != 0)
					{
						ListIterator inner_itr = createIterator(pth->otherAttributes);
						Attribute * attr = nextElement(&inner_itr);
						while(attr != NULL)
						{
							//Checking whether the attribute name or value is NULL
							if(attr->name == NULL || attr->value == NULL)
							{
								//Free the List Resources
								freeList(rectangles);
								freeList(circles);
								freeList(paths);
								freeList(groups);
								*root = NULL;
								return;
							}
							attr = nextElement(&inner_itr);
						}
					}
					//Adding Group Path in paths List
					insertBack(paths, pth);
					pth = nextElement(&itr);
				}
			}
			grp = nextElement(&itr);
		}
	}

	//Creating the Rectangle child node, its attributes and its children
	if(getLength(image->rectangles) != 0)
	{
		ListIterator itr = createIterator(image->rectangles);
		Rectangle * rect  = nextElement(&itr);
		while(rect != NULL)
		{
			//Checking if every Rectangle Struct field is Within the Contraints
			if(rect->width < 0.0 || rect->height < 0.0 || rect->units == NULL || rect->otherAttributes == NULL)
			{
				//Free the List Resources
				freeList(rectangles);
				freeList(circles);
				freeList(paths);
				freeList(groups);
				*root = NULL;
				return;
			}
			count = 0;
			//Checking Whether the current selected rectangle exists in a group or seperate from group
			if(getLength(rectangles) != 0)
			{
				ListIterator itr = createIterator(rectangles);
				Rectangle * grp_rect  = nextElement(&itr);
				while(grp_rect != NULL)
				{
					//If Both the rectangles are same, count incremented, loop breaked
					if(compareRectangles(rect, grp_rect))
					{
						count++;
						break;
					}
					grp_rect = nextElement(&itr);
				}
			}
			//When this rectangle does not exists in any of the group->rectangles, Create a child node for that rectangle in xml Tree
			if(!count)
			{
				node = xmlNewChild(root_node, NULL, BAD_CAST "rect", NULL);
				snprintf(float_str, sizeof(float_str), "%.2f", rect->x);
				strcat(float_str, rect->units);
				xmlNewProp(node, BAD_CAST "x", BAD_CAST float_str);
			
				snprintf(float_str, sizeof(float_str), "%.2f", rect->y);
				strcat(float_str, rect->units);
				xmlNewProp(node, BAD_CAST "y", BAD_CAST float_str);

				snprintf(float_str, sizeof(float_str), "%.2f", rect->width);
				strcat(float_str, rect->units);
				xmlNewProp(node, BAD_CAST "width", BAD_CAST float_str);

				snprintf(float_str, sizeof(float_str), "%.2f", rect->height);
				strcat(float_str, rect->units);
				xmlNewProp(node, BAD_CAST "height", BAD_CAST float_str);
				if(getLength(rect->otherAttributes) != 0)
				{	
					ListIterator inner_itr = createIterator(rect->otherAttributes);
					Attribute * attr = nextElement(&inner_itr);
					while(attr != NULL)
					{
						//Checking whether the attribute name or value is NULL
						if(attr->name == NULL || attr->value == NULL)
						{
							//Free the List Resources
							freeList(rectangles);
							freeList(circles);
							freeList(paths);
							freeList(groups);
							*root = NULL;
							return;
						}
						xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);	
						attr = nextElement(&inner_itr);
					}
				}
			}
			rect = nextElement(&itr);
		}
	}
	//Creating the Circle child node, its attributes and its children
	if(getLength(image->circles) != 0)
	{
		ListIterator itr = createIterator(image->circles);
		Circle* cir = nextElement(&itr);
		while(cir != NULL)
		{
			//Checking if every Circle Struct field is Within the Contraints
			if(cir->r < 0 || cir->units == NULL || cir->otherAttributes == NULL)
			{
				//Free the List Resources
				freeList(rectangles);
				freeList(circles);
				freeList(paths);
				freeList(groups);
				*root = NULL;
				return;
			}
			count = 0;
			//Checking Whether the current selected Circle exists in a group or seperate from group
			if(getLength(circles) != 0)
			{
				ListIterator itr = createIterator(circles);
				Circle * grp_circ  = nextElement(&itr);	
				while(grp_circ != NULL)
				{
					//If Both the circles are same, count incremented, loop breaks
					if(compareCircles(cir, grp_circ))
					{
						count++;
						break;
					}
					grp_circ = nextElement(&itr);
				}
			}
			//When this circle does not exists in any of the group->circles, Create a child node for that circle in xml Tree
			if(!count)
			{
				node = xmlNewChild(root_node, NULL, BAD_CAST "circle", NULL);
				snprintf(float_str, sizeof(float_str), "%.2f", cir->cx);
				strcat(float_str, cir->units);
				xmlNewProp(node, BAD_CAST "cx", BAD_CAST float_str);
			
				snprintf(float_str, sizeof(float_str), "%.2f", cir->cy);
				strcat(float_str, cir->units);
				xmlNewProp(node, BAD_CAST "cy", BAD_CAST float_str);

				snprintf(float_str, sizeof(float_str), "%.2f", cir->r);
				strcat(float_str, cir->units);
				xmlNewProp(node, BAD_CAST "r", BAD_CAST float_str);
		
				if(getLength(cir->otherAttributes) != 0)
				{
					ListIterator inner_itr = createIterator(cir->otherAttributes);
					Attribute * attr = nextElement(&inner_itr);
					while(attr != NULL)
					{
						//Checking whether the attribute name or value is NULL
						if(attr->name == NULL || attr->value == NULL)
						{
							//Free the List Resources
							freeList(rectangles);
							freeList(circles);
							freeList(paths);
							freeList(groups);
							*root = NULL;
							return;
						}
						xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
						attr = nextElement(&inner_itr);
					}
				}
			}
			cir = nextElement(&itr);
		}
	}
	//Creating the Path child node, its attributes and its children
	if(getLength(image->paths) != 0)
	{
		ListIterator itr = createIterator(image->paths);
		Path* pth = nextElement(&itr);
		
		while(pth != NULL)
		{
			//Checking if every Path Struct field is Within the Contraints
			if(pth->data == NULL || pth->otherAttributes == NULL)
			{
				//Free the List Resources
				freeList(rectangles);
				freeList(circles);
				freeList(paths);
				freeList(groups);
				*root = NULL;
				return;
			}
			count = 0;
			//Checking Whether the current selected Path exists in a group or seperate from group
			if(getLength(paths) != 0)
			{
				ListIterator itr = createIterator(paths);
				Path * grp_pth  = nextElement(&itr);
				while(grp_pth != NULL)
				{
					//If Both the paths are same, count incremented, breaks loop
					if(comparePaths(pth, grp_pth))
					{
						count++;
						break;
					}
					grp_pth = nextElement(&itr);
				}
			}
			//When this path does not exists in any of the group->paths, Create a child node for that path in xml Tree
			if(!count)
			{
				node = xmlNewChild(root_node, NULL, BAD_CAST "path", NULL);
				xmlNewProp(node, BAD_CAST "d", BAD_CAST pth->data);
				if(getLength(pth->otherAttributes) != 0)
				{
					ListIterator inner_itr = createIterator(pth->otherAttributes);
					Attribute * attr = nextElement(&inner_itr);
					while(attr != NULL)
					{
						//Checking whether the attribute name or value is NULL
						if(attr->name == NULL || attr->value == NULL)
						{
							//Free the List Resources
							freeList(rectangles);
							freeList(circles);
							freeList(paths);
							freeList(groups);
							*root = NULL;
							return;
						}
						xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
						attr = nextElement(&inner_itr);
					}
				}
			}
			pth = nextElement(&itr);
		}
	}
	//Creating the Group child node, its attributes and its children
	if(getLength(image->groups) != 0)
	{
		ListIterator itr = createIterator(image->groups);
		Group* grp = nextElement(&itr);
		
		while(grp != NULL)
		{
			if(grp->rectangles == NULL|| grp->circles == NULL || grp->paths == NULL || grp->groups == NULL || grp->otherAttributes == NULL)
			{
				//Free the List Resources
				freeList(rectangles);
				freeList(circles);
				freeList(paths);
				freeList(groups);
				*root = NULL;
				return;
			}
			count = 0;
			//Checking Whether the current selected Group exists in a group or seperate a group
			if(getLength(groups) != 0)
			{
				ListIterator itr = createIterator(groups);
				Group * grp_group  = nextElement(&itr);
				while(grp_group != NULL)
				{
					//If Both the groups are same, count incremented, loop breaked
					if(compareGroups(grp, grp_group))
					{
						count++;
						break;
					}
					grp_group = nextElement(&itr);
				}
			}
			//When this Group does not exists in any of the grp->groups, Create a child node for that Group in xml Tree
			if(!count)
			{
				node = xmlNewChild(root_node, NULL, BAD_CAST "g", NULL);
				//creating the Rectangle child node, its attributes and its children
				if(getLength(grp->rectangles) != 0)
				{
					ListIterator itr = createIterator(grp->rectangles);
					Rectangle * rect  = nextElement(&itr);
					while(rect != NULL)
					{
						//Checking if every Rectangle Struct field is Within the Contraints
						if(rect->width < 0.0 || rect->height < 0.0 || rect->units == NULL || rect->otherAttributes == NULL)
						{
							//Free the List Resources
							freeList(rectangles);
							freeList(circles);
							freeList(paths);
							freeList(groups);
							*root = NULL;
							return;
						}
						child = xmlNewChild(node, NULL, BAD_CAST "rect", NULL);
						snprintf(float_str, sizeof(float_str), "%.2f", rect->x);
						strcat(float_str, rect->units);
						xmlNewProp(child, BAD_CAST "x", BAD_CAST float_str);
			
						snprintf(float_str, sizeof(float_str), "%.2f", rect->y);
						strcat(float_str, rect->units);
						xmlNewProp(child, BAD_CAST "y", BAD_CAST float_str);

						snprintf(float_str, sizeof(float_str), "%.2f", rect->width);
						strcat(float_str, rect->units);
						xmlNewProp(child, BAD_CAST "width", BAD_CAST float_str);

						snprintf(float_str, sizeof(float_str), "%.2f", rect->height);
						strcat(float_str, rect->units);
						xmlNewProp(child, BAD_CAST "height", BAD_CAST float_str);

						if(getLength(rect->otherAttributes) != 0)
						{	
							ListIterator inner_itr = createIterator(rect->otherAttributes);
							Attribute * attr = nextElement(&inner_itr);
							while(attr != NULL)
							{
								//Checking whether the attribute name or value is NULL
								if(attr->name == NULL || attr->value == NULL)
								{
									//Free the List Resources
									freeList(rectangles);
									freeList(circles);
									freeList(paths);
									freeList(groups);
									*root = NULL;
									return;
								}
								xmlNewProp(child, BAD_CAST attr->name, BAD_CAST attr->value);	
								attr = nextElement(&inner_itr);
							}
						}
						rect = nextElement(&itr);
					}
				}
				//creating the Circle child node, its attributes and its children
				if(getLength(grp->circles) != 0)
				{
					ListIterator itr = createIterator(grp->circles);
					Circle* cir = nextElement(&itr);
		
					while(cir != NULL)
					{
						//Checking if every Circle Struct field is Within the Contraints
						if(cir->r < 0 || cir->units == NULL || cir->otherAttributes == NULL)
						{
							//Free the List Resources
							freeList(rectangles);
							freeList(circles);
							freeList(paths);
							freeList(groups);
							*root = NULL;
							return;
						}
						child = xmlNewChild(node, NULL, BAD_CAST "circle", NULL);
						snprintf(float_str, sizeof(float_str), "%.2f", cir->cx);
						strcat(float_str, cir->units);
						xmlNewProp(child, BAD_CAST "cx", BAD_CAST float_str);
			
						snprintf(float_str, sizeof(float_str), "%.2f", cir->cy);
						strcat(float_str, cir->units);
						xmlNewProp(child, BAD_CAST "cy", BAD_CAST float_str);

						snprintf(float_str, sizeof(float_str), "%.2f", cir->r);
						strcat(float_str, cir->units);
						xmlNewProp(child, BAD_CAST "r", BAD_CAST float_str);
		
						if(getLength(cir->otherAttributes) != 0)
						{
							ListIterator inner_itr = createIterator(cir->otherAttributes);
							Attribute * attr = nextElement(&inner_itr);
							while(attr != NULL)
							{
								//Checking whether the attribute name or value is NULL
								if(attr->name == NULL || attr->value == NULL)
								{
									//Free the List Resources
									freeList(rectangles);
									freeList(circles);
									freeList(paths);
									freeList(groups);
									*root = NULL;
									return;
								}
								xmlNewProp(child, BAD_CAST attr->name, BAD_CAST attr->value);
								attr = nextElement(&inner_itr);
							}
						}
						cir = nextElement(&itr);
					}
				}
				//creating the Path child node, its attributes and its children
				if(getLength(grp->paths) != 0)
				{
					ListIterator itr = createIterator(grp->paths);
					Path* pth = nextElement(&itr);
		
					while(pth != NULL)
					{
						//Checking if every Path Struct field is Within the Contraints
						if(pth->data == NULL || pth->otherAttributes == NULL)
						{
							//Free the List Resources
							freeList(rectangles);
							freeList(circles);
							freeList(paths);
							freeList(groups);
							*root = NULL;
							return;
						}
						child = xmlNewChild(node, NULL, BAD_CAST "path", NULL);
						xmlNewProp(child, BAD_CAST "d", BAD_CAST pth->data);
		
						if(getLength(pth->otherAttributes) != 0)
						{
							ListIterator inner_itr = createIterator(pth->otherAttributes);
							Attribute * attr = nextElement(&inner_itr);
							while(attr != NULL)
							{
								//Checking whether the attribute name or value is NULL
								if(attr->name == NULL || attr->value == NULL)
								{
									//Free the List Resources
									freeList(rectangles);
									freeList(circles);
									freeList(paths);
									freeList(groups);
									*root = NULL;
									return;
								}
								xmlNewProp(child, BAD_CAST attr->name, BAD_CAST attr->value);
								attr = nextElement(&inner_itr);
							}
						}
						pth = nextElement(&itr);
					}
				}
				//creating the Group child node, its attributes and its children
				if(getLength(grp->groups) != 0)
				{
					if(!create_Groups_Inside_Group_from_image(grp->groups, groups, node))
					{
						//Free the List Resources
						freeList(rectangles);
						freeList(circles);
						freeList(paths);
						freeList(groups);
						*root = NULL;
						return;
					}
				}
				//creating Group Other properties
				if(getLength(grp->otherAttributes) != 0)
				{
					ListIterator inner_itr = createIterator(grp->otherAttributes);
					Attribute * attr = nextElement(&inner_itr);
					while(attr != NULL)
					{
						//Checking whether the attribute name or value is NULL
						if(attr->name == NULL || attr->value == NULL)
						{
							//Free the List Resources
							freeList(rectangles);
							freeList(circles);
							freeList(paths);
							freeList(groups);
							*root = NULL;
							return;
						}
						xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
						attr = nextElement(&inner_itr);
					}
				}
			}
			grp = nextElement(&itr);
		}
	}
	//Creating SVGimage Other Attributes in xml Tree svg Root Node
	if(getLength(image->otherAttributes) != 0)
	{
		ListIterator inner_itr = createIterator(image->otherAttributes);
		Attribute * attr = nextElement(&inner_itr);
		while(attr != NULL)
		{
			//Checking whether the attribute name or value is NULL
			if(attr->name == NULL || attr->value == NULL)
			{
				//Free the List Resources
				freeList(rectangles);
				freeList(circles);
				freeList(paths);
				freeList(groups);
				*root = NULL;
				return;
			}
			xmlNewProp(root_node, BAD_CAST attr->name, BAD_CAST attr->value);
			attr = nextElement(&inner_itr);
		}
	}
	//Free the List Resources
	freeList(rectangles);
	freeList(circles);
	freeList(paths);
	freeList(groups);
	*root = root_node;
}
bool validateSVGimage(SVGimage* image, char* schemaFile)
{
	bool valid = false;
	
	if(schemaFile == NULL) 
		return NULL;
	if(image == NULL) 
		return NULL;
	
	xmlDocPtr xml_doc = NULL;
	xmlNodePtr root = NULL;
	
	LIBXML_TEST_VERSION;
	
	xml_doc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "svg");
	xmlDocSetRootElement(xml_doc, root); 
	
	//Calling Helper Function for creating xml tree from image structure
	create_tree_from_image(&root, image);
	
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;
	xmlLineNumbersDefault(1);

	ctxt = xmlSchemaNewParserCtxt(schemaFile);

	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);

	if (xml_doc == NULL)
	{ 
		fprintf(stderr, "Not abale to parse the given image file");
	} //If The SVGimage Struct is Not Valid according to given constraints, then root NULL returned
	else if(root == NULL)
	{
		xmlFreeDoc(xml_doc);
	}
	else {
		xmlSchemaValidCtxtPtr ctxt;
		int ret;

		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, xml_doc);
		if (ret == 0)
		{
			valid = true;
		}
		else if (ret > 0) {
			valid = false;
		} 
		else {
			return NULL;
		}
		xmlSchemaFreeValidCtxt(ctxt);
		xmlFreeDoc(xml_doc);
	}

	//Free the resource
	if(schema != NULL)
		xmlSchemaFree(schema);

	xmlSchemaCleanupTypes();
	xmlCleanupParser();
	xmlMemoryDump();
	
	return valid;
}

/** Function to create an SVG object based on the contents of an SVG file.
 * This function must validate the XML tree generated by libxml against a SVG schema file
 * before attempting to traverse the tree and create an SVGimage struct
 *@pre File name cannot be an empty string or NULL.
       File represented by this name must exist and must be readable.
       Schema file name is not NULL/empty, and represents a valid schema file
 *@post Either:
        A valid SVGimage has been created and its address was returned
		or 
		An error occurred, or SVG file was invalid, and NULL was returned
 *@return the pointer to the new struct or NULL
 *@param fileName - a string containing the name of the SVG file
**/
SVGimage* createValidSVGimage(char* fileName, char* schemaFile)
{
	xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
	bool valid = false;
 	if(fileName == NULL)
 		return NULL;
	if(schemaFile == NULL)
 		return NULL;
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /*parse the file and get the DOM */
    doc = xmlReadFile(fileName, NULL, 0);

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

	//Validating the xml generated tree with the schema file
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;
	xmlLineNumbersDefault(1);

	ctxt = xmlSchemaNewParserCtxt(schemaFile);

	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);

	if (doc == NULL)
	{ 
		fprintf(stderr, "Not abale to parse the given image file");
	} //If The SVGimage Struct is Not Valid according to given constraints, then root NULL returned
	else if(root_element == NULL)
	{
		xmlFreeDoc(doc);
	}
	else {
		xmlSchemaValidCtxtPtr ctxt;
		int ret;

		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, doc);
		if (ret == 0)
		{
			valid = true;
		}
		else if (ret > 0) {
			valid = false;
		} 
		else {
			return NULL;
		}
		xmlSchemaFreeValidCtxt(ctxt);
	}
	//Free the resource for schema
	if(schema != NULL)
		xmlSchemaFree(schema);
	xmlSchemaCleanupTypes();
	//If Valid with schema than creating SVGimage
	if(valid)
	{
		//Creating an empty SVGimage struct 
		SVGimage *SvgDoc = create_svgdoc();
		//Calling The Helper Function
		create_svgimage_from_tree(root_element, &SvgDoc);

		/*free the document */
		xmlFreeDoc(doc);

		/*
		 *Free the global variables that may
		 *have been allocated by the parser.
		*/
		xmlCleanupParser();
		xmlMemoryDump();
		/* ********************************************************************************* */
		/* Return the SVGImage Struct pointer*/
		/* ********************************************************************************* */
		return SvgDoc;
	}
	/*Free the document */
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();
	return NULL;
}

/** Function to writing a SVGimage into a file in SVG format.
 *@pre
    SVGimage object exists, is valid, and and is not NULL.
    fileName is not NULL, has the correct extension
 *@post SVGimage has not been modified in any way, and a file representing the
    SVGimage contents in SVG format has been created
 *@return a boolean value indicating success or failure of the write
 *@param
    doc - a pointer to a SVGimage struct
 	fileName - the name of the output file
 **/
bool writeSVGimage(SVGimage* image, char* fileName)
{
	bool ret_val;
	if(fileName == NULL)
		return NULL;
	if(image == NULL)
		return NULL;

	
	xmlDocPtr xml_doc = NULL;
	xmlNodePtr root = NULL;
		
	LIBXML_TEST_VERSION;
		
	xml_doc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "svg");
	xmlDocSetRootElement(xml_doc, root); 
	
	create_tree_from_image(&root, image);
		
	ret_val = xmlSaveFormatFileEnc(fileName , xml_doc, "UTF-8", 1);

	xmlFreeDoc(xml_doc);
	xmlCleanupParser();
	xmlMemoryDump();
	
	return ret_val;
}

/** Function to setting an attribute in an SVGimage or component
 *@pre
    SVGimage object exists, is valid, and and is not NULL.
    newAttribute is not NULL
 *@post The appropriate attribute was set corectly
 *@return N/A
 *@param
    image - a pointer to an SVGimage struct
    elemType - enum value indicating elemtn to modify
    elemIndex - index of thje lement to modify
    newAttribute - struct containing name and value of the updated attribute
 **/
void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttribute)
{
	if(image == NULL) 
		return;
	if(elemType < 0)
		return;
	if(newAttribute == NULL)
		return;
	bool flagCheck = false;
	int count = 0;
	if(elemType == 0)
	{
		if(getLength(image->otherAttributes) != 0)
		{
			ListIterator inner_itr = createIterator(image->otherAttributes);
			Attribute * attr = nextElement(&inner_itr);
			while(attr != NULL)
			{
				//Checking whether the attribute name exists in the Attributes list
				//If Yes than change attribute value, delete Newattribute and break loop
				if(strcmp(attr->name, newAttribute->name) == 0)
				{
					strcpy(attr->value, newAttribute->value);
					deleteAttribute(newAttribute);
					flagCheck = true;
					break;
				}
				attr = nextElement(&inner_itr);
			}
			//If the attribute is not in otherAttributes List, adding it at the end
			if(!flagCheck)
				insertBack(image->otherAttributes, newAttribute);
		}
		else
			insertBack(image->otherAttributes, newAttribute);
	}
	else if(elemType == 1)
	{
		if((getLength(image->circles) != 0) && elemIndex >= 0)
		{
			ListIterator itr = createIterator(image->circles);
			Circle* cir = nextElement(&itr);
			while(cir != NULL)
			{
				//If this is the circle which we are looking in circles list 
				if(count == elemIndex)
				{
					//If the attribute Name is cx, than change attribute value, true the flagCheck and deleteAttribute
					if(strcmp(newAttribute->name, "cx") == 0)
					{
						cir->cx = atof(newAttribute->value);
						flagCheck = true;
						deleteAttribute(newAttribute);
						return;
					}
					//If the attribute Name is cy, than change attribute value, true the flagCheck and deleteAttribute
					else if(strcmp(newAttribute->name, "cy") == 0)
					{
						cir->cy = atof(newAttribute->value);
						flagCheck = true;
						deleteAttribute(newAttribute);
						return;
					}
					//If the attribute Name is r, than change attribute value, true the flagCheck and deleteAttribute
					else if(strcmp(newAttribute->name, "r") == 0)
					{
						cir->r = atof(newAttribute->value);
						flagCheck = true;
						deleteAttribute(newAttribute);
						return;
					}
					if(getLength(cir->otherAttributes) != 0)
					{
						ListIterator inner_itr = createIterator(cir->otherAttributes);
						Attribute * attr = nextElement(&inner_itr);
						while(attr != NULL)
						{
							//Checking whether the attribute name exists in the Attributes list
							//If Yes than change attribute value, delete Newattribute and break loop
							if(strcmp(attr->name, newAttribute->name) == 0)
							{
								strcpy(attr->value, newAttribute->value);
								deleteAttribute(newAttribute);
								flagCheck = true;
								break;
							}
							attr = nextElement(&inner_itr);
						}
						//If the attribute is not in otherAttributes List, adding it at the end
						if(!flagCheck)
							insertBack(cir->otherAttributes, newAttribute);
					}
					else
						insertBack(cir->otherAttributes, newAttribute);
					return;
				}
				count++;
				cir = nextElement(&itr);
			}
		}
	}
	else if(elemType == 2)
	{
		if((getLength(image->rectangles) != 0) && elemIndex >= 0)
		{
			ListIterator itr = createIterator(image->rectangles);
			Rectangle * rect  = nextElement(&itr);
			while(rect != NULL)
			{
				//If this is the rectangle which we are looking in rectangles list 
				if(count == elemIndex)
				{
					//If the attribute Name is x, than change attribute value, true the flagCheck and deleteAttribute
					if(strcmp(newAttribute->name, "x") == 0)
					{
						rect->x = atof(newAttribute->value);
						flagCheck = true;
						deleteAttribute(newAttribute);
						return;
					}
					//If the attribute Name is y, than change attribute value, true the flagCheck and deleteAttribute
					else if(strcmp(newAttribute->name, "y") == 0)
					{
						rect->y = atof(newAttribute->value);
						flagCheck = true;
						deleteAttribute(newAttribute);
						return;
					}
					//If the attribute Name is width, than change attribute value, true the flagCheck and deleteAttribute
					else if(strcmp(newAttribute->name, "width") == 0)
					{
						rect->width = atof(newAttribute->value);
						flagCheck = true;
						deleteAttribute(newAttribute);
						return;
					}
					//If the attribute Name is height, than change attribute value, true the flagCheck and deleteAttribute
					else if(strcmp(newAttribute->name, "height") == 0)
					{
						rect->height = atof(newAttribute->value);
						flagCheck = true;
						deleteAttribute(newAttribute);
						return;
					}
					if(getLength(rect->otherAttributes) != 0)
					{	
						ListIterator inner_itr = createIterator(rect->otherAttributes);
						Attribute * attr = nextElement(&inner_itr);
						while(attr != NULL)
						{
							//Checking whether the attribute name exists in the Attributes list
							//If Yes than change attribute value, delete Newattribute and break loop
							if(strcmp(attr->name, newAttribute->name) == 0)
							{
								strcpy(attr->value, newAttribute->value);
								deleteAttribute(newAttribute);
								flagCheck = true;
								break;
							}
							attr = nextElement(&inner_itr);
						}
						//If the attribute is not in otherAttributes List, adding it at the end
						if(!flagCheck)
							insertBack(rect->otherAttributes, newAttribute);
					}
					else
						insertBack(rect->otherAttributes, newAttribute);
					return;
				}
				count++;
				rect = nextElement(&itr);
			}
		}
	}
	else if(elemType == 3)
	{
		if((getLength(image->paths) != 0) && elemIndex >= 0)
		{
			ListIterator itr = createIterator(image->paths);
			Path * pth  = nextElement(&itr);
			while(pth != NULL)
			{
				//If this is the path which we are looking in paths list 
				if(count == elemIndex)
				{
					//If the attribute Name is d, than change attribute value, true the flagCheck and deleteAttribute
					if(strcmp(newAttribute->name, "d") == 0)
					{
						strcpy(pth->data, newAttribute->value);
						flagCheck = true;
						deleteAttribute(newAttribute);
						return;
					}
					if(getLength(pth->otherAttributes) != 0)
					{	
						ListIterator inner_itr = createIterator(pth->otherAttributes);
						Attribute * attr = nextElement(&inner_itr);
						while(attr != NULL)
						{
							//Checking whether the attribute name exists in the Attributes list
							//If Yes than change attribute value, delete Newattribute and break loop
							if(strcmp(attr->name, newAttribute->name) == 0)
							{
								strcpy(attr->value, newAttribute->value);
								deleteAttribute(newAttribute);
								flagCheck = true;
								break;
							}
							attr = nextElement(&inner_itr);
						}
						//If the attribute is not in otherAttributes List, adding it at the end
						if(!flagCheck)
							insertBack(pth->otherAttributes, newAttribute);
					}
					else
						insertBack(pth->otherAttributes, newAttribute);
					return;
				}
				count++;
				pth = nextElement(&itr);
			}
		}
	}
	if(elemType == 4)
	{
		if((getLength(image->groups) != 0) && elemIndex >= 0)
		{
			ListIterator itr = createIterator(image->groups);
			Group * grp  = nextElement(&itr);
			while(grp != NULL)
			{
				//If this is the group which we are looking in groups list 
				if(count == elemIndex)
				{
					if(getLength(grp->otherAttributes) != 0)
					{	
						ListIterator inner_itr = createIterator(grp->otherAttributes);
						Attribute * attr = nextElement(&inner_itr);
						while(attr != NULL)
						{
							//Checking whether the attribute name exists in the Attributes list
							//If Yes than change attribute value, delete Newattribute and break loop
							if(strcmp(attr->name, newAttribute->name) == 0)
							{
								strcpy(attr->value, newAttribute->value);
								deleteAttribute(newAttribute);
								flagCheck = true;
								break;
							}
							attr = nextElement(&inner_itr);
						}
						//If the attribute is not in otherAttributes List, adding it at the end
						if(!flagCheck)
							insertBack(grp->otherAttributes, newAttribute);
					}
					else
						insertBack(grp->otherAttributes, newAttribute);
					return;
				}
				count++;
				grp = nextElement(&itr);
			}
		}
	}
}

/** Function to adding an element - Circle, Rectngle, or Path - to an SVGimage
 *@pre
    SVGimage object exists, is valid, and is not NULL.
    newElement is not NULL
 *@post The appropriate element was added correctly
 *@return N/A
 *@param
    image - a pointer to an SVGimage struct
    elemType - enum value indicating elemtn to modify
    newElement - pointer to the element sgtruct (Circle, Rectngle, or Path)
 **/
void addComponent(SVGimage* image, elementType type, void* newElement)
{
	if(image == NULL) 
		return;
	if(type < 0)
		return;
	if(newElement == NULL)
		return;
	if(type == 1)
		insertBack(image->circles, newElement);
	else if(type == 2)
		insertBack(image->rectangles, newElement);
	else if(type == 3)
		insertBack(image->paths, newElement);
	return;
}

char* attrToJSON(const Attribute* a)
{
	char * string =  calloc(100000, sizeof(char));
	if(a == NULL) 
	{
		strcpy(string, "{}");
		return string;
	}

	strcpy(string, "{\"name\":\"");
	strcat(string, a->name);
	
	strcat(string, "\",\"value\":\"");	
	strcat(string, a->value);
	
	strcat(string, "\"}");
	return string;
}
char* circleToJSON(const Circle* c)
{
	char * string =  calloc(100000, sizeof(char));
	if(c == NULL) 
	{
		strcpy(string, "{}");
		return string;
	}
	
	char buffer[50];
	strcpy(string, "{\"cx\":");
	snprintf(buffer, sizeof(buffer), "%.2f", c->cx);
	strcat(string, buffer);
	strcat(string, ",\"cy\":");
	snprintf(buffer, sizeof(buffer), "%.2f", c->cy);
	strcat(string, buffer);
	strcat(string, ",\"r\":");
	snprintf(buffer, sizeof(buffer), "%.2f", c->r);
	strcat(string, buffer);
	strcat(string, ",\"numAttr\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(c->otherAttributes));
	strcat(string, buffer);
	strcat(string, ",\"units\":\"");	
	strcat(string, c->units);

	strcat(string, "\"}");
	return string;
}
char* rectToJSON(const Rectangle* r)
{	
	char * string =  calloc(100000, sizeof(char));
	if(r == NULL) 
	{
		strcpy(string, "{}");
		return string;
	}
	char buffer[50];
	strcpy(string, "{\"x\":");
	snprintf(buffer, sizeof(buffer), "%.2f", r->x);
	strcat(string, buffer);
	strcat(string, ",\"y\":");
	snprintf(buffer, sizeof(buffer), "%.2f", r->y);
	strcat(string, buffer);
	strcat(string, ",\"w\":");
	snprintf(buffer, sizeof(buffer), "%.2f", r->width);
	strcat(string, buffer);
	strcat(string, ",\"h\":");
	snprintf(buffer, sizeof(buffer), "%.2f", r->height);
	strcat(string, buffer);
	strcat(string, ",\"numAttr\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(r->otherAttributes));
	strcat(string, buffer);
	strcat(string, ",\"units\":\"");	
	strcat(string, r->units);

	strcat(string, "\"}");
	return string;
}
char* pathToJSON(const Path* p)
{
	char * string =  calloc(100000, sizeof(char));
	if(p == NULL) 
	{
		strcpy(string, "{}");
		return string;
	}
	char buffer[50];
	strcpy(string, "{\"d\":\"");
	strcat(string, p->data);
	strcat(string, "\",\"numAttr\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(p->otherAttributes));
	strcat(string, buffer);

	strcat(string, "}");
	return string;
}
char* groupToJSON(const Group* g)
{
	char * string =  calloc(100000, sizeof(char));
	if(g == NULL) 
	{
		strcpy(string, "{}");
		return string;
	}
	char buffer[50];
	strcpy(string, "{\"children\":");
	snprintf(buffer, sizeof(buffer), "%d", (getLength(g->rectangles) + getLength(g->circles) + getLength(g->paths) + getLength(g->groups)));
	strcat(string, buffer);
	strcat(string, ",\"numAttr\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(g->otherAttributes));
	strcat(string, buffer);

	strcat(string, "}");
	return string;
}

char* attrListToJSON(const List* list)
{
	char * string =  calloc(1000000, sizeof(char));
	if(list == NULL) 
	{
		strcpy(string, "[]");
		return string;
	}
	if(list->length == 0) 
	{
		strcpy(string, "[]");
		return string;
	}
	strcat(string, "[");
	ListIterator itr = createIterator((List *)list);
	Attribute * attr = nextElement(&itr);
	while(attr != NULL)
	{
		char * attrJSON =  attrToJSON(attr);
		strcat(string, attrJSON);
		free(attrJSON);
		if((attr = nextElement(&itr)) != NULL)
			strcat(string, ",");
	}
	strcat(string, "]");
	return string;
}
char* circListToJSON(const List* list)
{
	char * string =  calloc(1000000, sizeof(char));
	if(list == NULL) 
	{
		strcpy(string, "[]");
		return string;
	}
	if(list->length == 0) 
	{
		strcpy(string, "[]");
		return string;
	}
	
	strcat(string, "[");
	ListIterator itr = createIterator((List *)list);
	Circle * cir = nextElement(&itr);
	while(cir != NULL)
	{
		char * cirJSON =  circleToJSON(cir);
		strcat(string, cirJSON);
		free(cirJSON);
		if((cir = nextElement(&itr)) != NULL)
			strcat(string, ",");
	}
	strcat(string, "]");
	return string;
}
char* rectListToJSON(const List* list)
{
	char * string =  calloc(1000000, sizeof(char));
	if(list == NULL) 
	{
		strcpy(string, "[]");
		return string;
	}
	if(list->length == 0) 
	{
		strcpy(string, "[]");
		return string;
	}

	strcat(string, "[");
	ListIterator itr = createIterator((List *)list);
	Rectangle * rec = nextElement(&itr);
	while(rec != NULL)
	{
		char * rectJSON =  rectToJSON(rec);
		strcat(string, rectJSON);
		free(rectJSON);
		if((rec = nextElement(&itr)) != NULL)
			strcat(string, ",");
	}
	strcat(string, "]");
	return string;
}
char* pathListToJSON(const List* list)
{
	char * string =  calloc(1000000, sizeof(char));
	if(list == NULL) 
	{
		strcpy(string, "[]");
		return string;
	}
	if(list->length == 0) 
	{
		strcpy(string, "[]");
		return string;
	}
	strcat(string, "[");
	ListIterator itr = createIterator((List *)list);
	Path * pth = nextElement(&itr);
	while(pth != NULL)
	{
		char * pthJSON =  pathToJSON(pth);
		strcat(string, pthJSON);
		free(pthJSON);
		if((pth = nextElement(&itr)) != NULL)
			strcat(string, ",");
	}
	strcat(string, "]");
	return string;
}
char* groupListToJSON(const List* list)
{
	char * string =  calloc(1000000, sizeof(char));
	if(list == NULL) 
	{
		strcpy(string, "[]");
		return string;
	}
	if(list->length == 0) 
	{
		strcpy(string, "[]");
		return string;
	}
	strcat(string, "[");
	ListIterator itr = createIterator((List *)list);
	Group * grp = nextElement(&itr);
	while(grp != NULL)
	{
		char * grpJSON = groupToJSON(grp);
		strcat(string, grpJSON);
		free(grpJSON);
		if((grp = nextElement(&itr)) != NULL)
			strcat(string, ",");
	}
	strcat(string, "]");
	return string;
}

char* SVGtoJSON(const SVGimage* imge)
{
	if(imge == NULL) 
		return "{}";
	
	char * string = calloc((10000 * sizeof(imge)), sizeof(char));
	char buffer[50];
	strcpy(string, "{\"numRect\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(imge->rectangles));
	strcat(string, buffer);
	strcat(string, ",\"numCirc\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(imge->circles));
	strcat(string, buffer);
	strcat(string, ",\"numPaths\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(imge->paths));
	strcat(string, buffer);
	strcat(string, ",\"numGroups\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(imge->groups));
	strcat(string, buffer);

	strcat(string, "}");
	return string;
}

/* ******************************* Bonus A2 functions - optional for A2 *************************** */
SVGimage* JSONtoGPX(const char* svgString)
{
	if(svgString == NULL) 
		return NULL;
	char * temp = calloc(5000, sizeof(char));
	SVGimage * svgdoc = create_svgdoc();
	strcpy(temp, (char *)svgString);
	temp[strlen(temp) + 1] = '\0';
	
	int c = 0, i = 0, j = 0;
	char buffer[50];
	while(temp[i] != '\0')
	{
		if(temp[i] == '{')
		{
			i++;
			continue;
		}
		if(temp[i] == '\"')
		{
			i++;
			while(temp[i] != '\"')
			{
				i++;
				continue;
			}
		}
		if(temp[i] == ':')
		{
		  if(temp[i + 1] != '\"')
		  {
			while(temp[i + 1] != ',')
			{
				buffer[j] = temp[i + 1];
				j++;
				i++;
			}
			if(c == 0)
				svgdoc->rectangles->length = atoi(buffer);
			if(c == 1)
				svgdoc->circles->length = atoi(buffer);
			if(c == 2)
				svgdoc->paths->length = atoi(buffer);
			if(c == 3)
				svgdoc->groups->length = atoi(buffer);
			c++;
		  }
		  i++;
		  continue;
		}
		i++;
	}
	
	strcpy(svgdoc->namespace,  "http://www.w3.org/2000/svg");
	return svgdoc;
}
Rectangle* JSONtoRect(const char* svgString)
{
	if(svgString == NULL) 
		return NULL;
	char * temp = calloc(5000, sizeof(char));
	Rectangle * rect = create_new_rectangle();
	strcpy(temp, (char *)svgString);
	temp[strlen(temp) + 1] = '\0';
	
	int c = 0, i = 0, j = 0;
	char buffer[50];
	while(temp[i] != '\0')
	{
		if(temp[i] == '{')
		{
			i++;
			continue;
		}
		if(temp[i] == '\"')
		{
			i++;
			while(temp[i] != '\"')
			{
				i++;
				continue;
			}
		}
		if(temp[i] == ':')
		{
		  if(temp[i + 1] != '\"')
		  {
			while(temp[i + 1] != ',')
			{
				buffer[j] = temp[i + 1];
				j++;
				i++;
			}
			if(c == 0)
				rect->x = atof(buffer);
			if(c == 1)
				rect->y = atof(buffer);
			if(c == 2)
				rect->width = atof(buffer);
			if(c == 3)
				rect->height = atof(buffer);
			if(c == 4)
				rect->otherAttributes->length = atoi(buffer);
			c++;
		  }
		  else
		  {
			while(temp[i + 1] != '\"')
			{
				buffer[j] = temp[i + 1];
				j++;
				i++;
			}
			strcpy(rect->units, buffer);
		  }
		  i++;
		  continue;
		}
		i++;
	}
	return rect;
}
Circle* JSONtoCircle(const char* svgString)
{
	if(svgString == NULL) 
		return NULL;
	char * temp = calloc(5000, sizeof(char));
	Circle * cir = create_new_circle();
	strcpy(temp, (char *)svgString);
	temp[strlen(temp) + 1] = '\0';
	
	int c = 0, i = 0, j = 0;
	char buffer[50];
	while(temp[i] != '\0')
	{
		if(temp[i] == '{')
		{
			i++;
			continue;
		}
		if(temp[i] == '\"')
		{
			i++;
			while(temp[i] != '\"')
			{
				i++;
				continue;
			}
		}
		if(temp[i] == ':')
		{
		  if(temp[i + 1] != '\"')
		  {
			while(temp[i + 1] != ',')
			{
				buffer[j] = temp[i + 1];
				j++;
				i++;
			}
			if(c == 0)
				cir->cx = atof(buffer);
			if(c == 1)
				cir->cy = atof(buffer);
			if(c == 2)
				cir->r = atof(buffer);
			if(c == 3)
				cir->otherAttributes->length = atoi(buffer);
			c++;
		  }
		  else
		  {
			while(temp[i + 1] != '\"')
			{
				buffer[j] = temp[i + 1];
				j++;
				i++;
			}
			strcpy(cir->units, buffer);
		  }
		  i++;
		  continue;
		}
		i++;
	}
	return cir;
}


