#include "SVGParser.h"
#include "LinkedListAPI.h"
#include  <stdio.h>
#include  <math.h>


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