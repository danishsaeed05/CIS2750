## CIS*

### Assignment 2

### Deadline: Saturday, February 15, 9am

### Weight: 17%

Assignment 2 will consist of three modules. To simplify regression testing of our Assignment 1 code, we will not be
updating any Assignment 1 functionality.

Assignment 2 modules:

**1.** Functions for creating valid SVGimages, validating SVGimages, and writing SVGimages to a file.
**2.** Functions for adding components to SVGimages.
**3.** A set of "glue" functions to convert an SVGimage and its components into JSON strings - and vice versa. Some of
    these functional will be optional, and form a bonus mark. However, all of these functions will be useful in later
    assignments: they will help us integrate the C library created in in A1/A2 with the server-side JavaScript code in
    A3/A4.

You are provided with a temporary header file for A2 Module 1 (SVGParser_A2temp.h). This header will be
updated when Modules 2 and 3 come out. Once Module 3 is released, I will post the final official header file for
Assignment 2, which will be used in the A2 test harness for grading.

Keep in mind that functions in Modules 2 and 3 are shorter than those in Module 1, and Modules 2 and 3 may be
released at the same time.

#### Module 1 functionality

SVGimage* createValidSVGimage(char* fileName, char* schemaFile);

An updated version of createSVGimage. The only difference is that this function validates the xmlDoc returned by
the libxml2 function xmlReadFile against an XSD file that represents the SVG standard before doing any further
parsing and building the SVGimage.

This function does the parsing and allocates a SVG image object. It accepts the name of the SVG file, and the name of
the XSD file. If the file contains a valid SVG image and has been parsed successfully, a pointer to the to the newly
created SVGimage object is returned. If the parsing fails for any reason - invalid XML format, invalid SVG format,
etc. - the function must return NULL.

bool writeSVGimage(SVGimage* doc, char* fileName);

This function takes an SVGimage struct and saves it to a file in SVG format. Its arguments are an SVGimage and the
name of a new file. It must return true if the write was successful, and false is the write failed for any reason -
invalid output file name, etc.. This function can assume that the doc argument has already been validated with
validateSVGimage, which is discussed below. It must still make sure the arguments are not NULL.

bool validateSVGimage(SVGimage* doc, char* schemaFile);

This function takes an SVGimage and the name of a valid SVG schema file, and validates the contents of the
SVGimage against an XSD file that represents the SVG standard. It also validated the contents against the
constraints specified in SVGParser.h. It returns true if the SVGimage contains valid data, and false otherwise.


There are two aspects to SVGimage validity. First, its contents must represent a valid SVG image once converted to
XML. This can be validated using a method similar to what do in createValidSVGimage.

The second aspect is whether the SVGimage violates any of the constraints specified in the SVGparser.h. Some of
these constraints reflect the SVG specification. For example, the SVG documentation states that a circle radius cannot
be negative. However, validating a libxml tree against the SVG schema file will not catch this violation - as long as the
radius is a valid number, libxml will consider the underlying XML document to be valid and fully compliant with the
schema.

In addition, there are constraints that enforce the internal consistency of the data structures in the SVGimage - for
example, all pointers in an SVGimage must be initialized and must not be NULL.

This means that validateSVGimage must manually check the constraints of the struct against the specifications
listed in SVGParser.h - ensure that the numbers are within valid ranges, lists are not NULL, etc.

###****Read PDF for the rest of the instructions*****###