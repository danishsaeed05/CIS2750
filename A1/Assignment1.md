## CIS*2750

### Assignment 1

### Deadline: Tuesday, January 28, 9:00am

### Weight: 17%

### Module 1: Primary functions

This is Module 1 of Assignment 1. It will focus on creating a set of structs representing SVG files using
libxml2. Module 2 will describe a set of accessor and search functions that will accompany the parser and the
structs. It will be released after January 15. Module 2 may also contain additional submission details.

#### Description

In this assignment, you need to implement a library to parse the SVG files. SVG is one of the most common
vector graphics formats, and is supported by every Web browser.

The link to the format description is posted in the Assignment 1 description. Make sure you understand the
format before doing the assignment.

According to the specification, an SVG object contains:

- one or more svg elements
- 0 or more groups, which can contain other groups
- a large number of primitives, along with text, colour attributes, fill attributes, etc..

Our Assignment 1 parser will assume a somewhat simpler SVG file:
- one svg element
- 0 or more groups, which can contain other groups
- rectangles, circles, and paths - no other geometric primitives

Read the specification and pay attention to the specification details, e.g. what to do if the attributes x or y are
not specified in a circle or rectangle.

This structure is represented by the various types in SVGParser.h.

The SVG standard uses the common XML language. To help you build the parser, you will use the libxml
library, once of the most common C XML parsers. It will build an XML tree for you from an SVG file. You will
then use it to create an SVGimage struct with all of its components. The XML parser will do most of the heavy
lifting for you.

The documentation on libxml2 is available here: [http://www.xmlsoft.org/html/index.html.](http://www.xmlsoft.org/html/index.html.) You can use the
sample code to get started, but you **must** site it in your submission. You cannot use any other sample code.

**Your assignment will be graded using an automated test suite, so you must follow all requirements
exactly, or you will lose marks.**

**Required Functions**

Read the comments in SVGParser.h carefully. They provide additional implementation details. You **must**
implement **every** function in SVGParser.h; they are also listed below. If you do not complete any of the
functions, you **must** provide a stub for every one them.


Applications using the parser library will include SVGParser.h in their main program. The SVGParser.h
header has been provided for you. Do not change it in any way. SVGParser.h is the public “face” our our
vector image API. All the helper functions are internal implementation details, and should not be publicly/
globally visible. When we grade your code, we will use the standard SVGParser.h to compile and run it.

If you create additional header files, include them in the c. files that use them.

_SVG parser functions_

SVGimage* createSVGimage(char* fileName)

This function does the parsing and allocated a SVG image object. It accepts a filename If the file has been
parsed successfully, a pointer to the newly created SVGimage object is returned. If the parsing fails for any
reason, the function must return NULL.

Parsing can fail for a number of reasons, but libxml hides them from us. The xmlReadFile function will
simply return NULL instead of an XML doc if the file is invalid for any reason.

char* SVGimageToString(SVGimage* img)

This function returns a humanly readable string representation of the entire vector image object. It will be
used mostly by you, for debugging your parser. It must not modify the vector image object in any way. The
function must allocate the string dynamically.

void deleteSVGimage(SVGimage* img);

This function deallocates the object, including all of its subcomponents.

_Helper functions_

In addition the above functions, you must also write a number of helper functions. We will need to store the
types Group, Rectangle, Circle, Path, and Attribute in lists.

void deleteAttribute( void* data);
char* attributeToString( void* data);
int compareAttributes(const void *first, const void *second);

void deleteGroup(void* data);
char* groupToString( void* data);
int compareGroups(const void *first, const void *second);

void deleteRectangle(void* data);
char* rectangleToString(void* data);
int compareRectangles(const void *first, const void *second);

void deleteCircle(void* data);
char* circleToString(void* data);
int compareCircles(const void *first, const void *second);


void deletePath(void* data);
char* pathToString(void* data);
int comparePaths(const void *first, const void *second);

**Additional guidelines and requirements**

In addition, it is strongly recommended that you write additional helpers functions for parsing the file - e.g.
creating a Circle, Group, etc. from an xmlNode. You are free to create your additional "helper functions" in a
separate .c file, if you find some recurring processing steps that you would like to factor out into a single place.

Do **not** place headers for these additional helper function in SVGParser.h. They must be in a separate header
file, since they are internal to your implementation and not for public users of the parser package.

For your own test purposes, you will also want to code a main program in another .c file that calls your
functions with a variety of test cases, However, you **must not** submit that file with your assignment. Also, do
**not** put your main() function in SVGParser.h. Failure to do this may cause the test program will fail if you
incorrectly include main() in our shared library file; **you will lose marks for that** , and may get a 0 for the
assignment.

Your functions are supposed to be robust. They will be tested with various kinds of invalid data and must
detect problems without crashing. If your functions encounter a problem, they must free all memory and
return.

_Function naming_

You are welcome to name your helper functions as you see fit. However, **do not** put the underscore (_)
character at the start of your function names. That is reserved solely for the test harness functions. Failure to
do so may result in run-time errors due to name collisions - and a grade of zero (0) as a result.

_Linked list_

You are expected to use a linked list for storing various vector image components. You can use the list
implementation that I use in the class examples (and that you use for A0). You can also use your own.
However, your implementation **must** be compliant with the List API defined in LinkedListAPI.h. Failure to
do so may result in a grade deductions, up to and including a grade of zero.



 ## Module 2: accessor and search functions

The purpose of the parser we are building in A1 is to extract the major components from a vector image file,
and fill in our data structures, which will be used in later assignments. Make sure that, in addition to the SVG
specification, you also carefully read the comments in SVGParser.h - each data structure includes detailed
comments indicating the constraints for various components.

The functions below will allow us to examine the internal state of the SVGimage struct and its components,
and modify them in later assignments.

**Required Functions - accessors**

This set of four functions will act somewhat similar to accessors (get...) methods of a class. We will use them to
return a list of all specific primitives. Keep in mind that we do not want to replicate the primitives. So if we
have an image with two circles, we will return a list of pointers to the existing circles, rather than a list
containing newly allocated copies of the circles. This way we can also modify the image contents - for example,
we could scale all rectangles by iterating through the returned list and multiplying the width and height by 2.

List* getRects(SVGimage* img);

Function that returns a list of all rectangles in the image. If there are none, it returns an empty list (not a NULL
pointer).

List* getCircles(SVGimage* img);

Function that returns a list of all circles in the image. If there are none, it returns an empty list (not a NULL
pointer).

List* getGroups(SVGimage* img);

Function that returns a list of all groups in the image. If there are none, it returns an empty list (not a NULL
pointer).

List* getPaths(SVGimage* img);

Function that returns a list of all paths in the image. If there are none, it returns an empty list (not a NULL
pointer).

In all of the above functions, if img is NULL, return NULL.

**Required Functions - summaries**

This set of five functions will tell us something about the internal structure of the image. We will use them to
find if the image contains specific elements. Unfortunately elements in SVG are not uniquely named, so we


have to search by their properties. To make your life a little simpler, the API requires you to return the count of
elements with the search criterion (i.e. 0 or more), rather than a list containing a pointer to them.

This will be much easier to implement if you implement some sort of a generic search function, similar to the
findElement function in your List API. In fact, you will be able to use findElement as a template to help you
create your implementation.

All of the num... functions below must return 0 if their arguments are invalid - e.g. NULL pointers, empty
strings, negative area/length, etc..

int numRectsWithArea(SVGimage* img, float area);

Function that returns the number of all rectangles with the specified area. Return 0 if no such rectangles are
found, or if any of the arguments are invalid.

int numCirclesWithArea(SVGimage* img, float area);

Function that returns the number of all circles with the specified area. Return 0 if no such circles are found, or
if any of the arguments are invalid.

int numPathsWithdata(SVGimage* img, char* data);

Function that returns the number of all paths with the specified data. Return 0 if no such paths are found, or if
any of the arguments are invalid.

int numGroupsWithLen(SVGimage* img, int len);

Function that returns the number of all groups with the specified length. We will define group length as:
_length of the Group->rectangles list
 +
length of the Group->circles list
 +
length of the Group->paths list
 +
length of the Group->groups list_

So if a group has 0 circles, 2 rectangles, 1 subgroup, and 5 paths, its length is 8. Please keep in mind that we
don't care how many elements the subgroups contain - we simply count the number of subgroups. Return 0 if
no such circles are found, or if any of the arguments are invalid.

int numAttr(SVGimage* img);

Function that returns the number of Attributes in all structs of the SVGimage. For example, the SVGimage
created from Emoji_poo.svg should have the total of 9 Attribute structs in it. The total number of XML
element attributes is larger than that, but many of these attributes get their own fields in their respective
structs. For example, the values attribute d for path SVG elements becomes Path.data in our code, while all
the other attributes of the original path element are stored as Attribute structs in the
Path.otherAttributes list. Return 0 if no such circles are found, or if any of the arguments are invalid.


**NOTE** : keep in mind that some of the functions above will need to compute a float value, and then compare it
to the argument - another float - for equality. We don't want to mess around with float equality tests. To
simplify things, you will convert them to ints by applying the ceil() function (in math.h) to both values before
comparison.

For example, let's say you need to find all rectangles of width 20. Your image contains 2 rectangles:

- One with width=3.5, height = 4.
- One with width=4.3, height = 4.

someone calls the function: numRectsWithArea(someImage, 20);

Rectangle 1 has area 3.5 * 4.5 = 15.75. ceil(15.75) = 16, and ceil(20) = 20, so rectangle 1 does not match.
Rectangle 2 has area 4.3 * 4.5 = 19.35. ceil(19.35) = 20, and ceil(20) = 20, so rectangle 2 is a match, and
hasRectsWithArea returns 1.




