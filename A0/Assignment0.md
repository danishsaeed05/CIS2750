### CIS*2750

#### Assignment 0

#### Deadline: Friday, January 17, 2020 9:00am

#### Weight: 1%

**Description**

For this assignment you must create a shared library for the doubly-linked list API that has been
provided for you. You have been provided with all necessary source code. The purpose of this
"assignment" (in scope, it's just a lab, and should take less than an hour to complete) is to get you
familiar with creating shared libraries, and using them with an automated test harness executable -
which is how your A1 and A2 will be graded.

Do not modify the code or the header in any way. Simply write a Makefile that will produce a
shared library file called liblist.so and place it in the bin/ directory. The target name will be
liblist, and it must the the first target. This way if you type make liblist - or just make - the
required target is created and placed in the bin/ directory.

To make sure that your library was created correctly, run the executable ListTest in the same
directory. You should see a long list of test cases. The executable was compiled on the SoCS Linux
server, so that's where you must run it. Needless to say, the library must be compiled there as well.

Make sure that your LD_LIBRARY_PATH variable is configured correctly, as discussed at the end of
Lecture 2a: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

**Submission structure**

The submission must have the following directory structure:

assign0/ contains the Makefile
assign0/bin contains the executable ListTest. This is where the Makefile will place
the shared library file
assign0/src contains all source files.
assign0/include contains all headers. Keep in mind that if it contains LinkedListAPI.h,
this file will be replaced with the standard LinkedListAPI.h provided to
the class.

NOTE: Your Makefile will need to reference bin/ and src/ subdirectories. Since the location of
the Makefile relative to these directories is always the same, this will be straightforward.

**Do not** hardcode any absolute paths or any account-specific details into your Makefile! This will
most likely break the compilation when we grade your code, and result in a grade of zero on the
assignment.

**Evaluation**

Your code must compile and run with the specified executable. We will grade it by compiling your
code to create the shared library, and running ListTest. Make sure your Makefile places
liblist.so into bin/

The grading scheme is simple - and binary:

- If your library runs with the executable, you get 100%.
- If your library fails to run with the executable, you get a 0.
- If your library fails to compile, you get a 0.

Make sure you compile and run your code in the lab or on the **linux.socs.uoguelph.ca** server.

**Submission**

Submit your source code in the specified structure

**Late submissions:** see course outline for late submission policies.

**This assignment is individual work and is subject to the University Academic Misconduct
Policy.** See course outline for details.



