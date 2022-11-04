Tutorial
========

This tutorial has been tested with Elements version 4.0

1. Starting a new Elements project using command-line helper scripts
--------------------------------------------------------------------

Create or select a root working directory where all your Elements
project source code will be placed

For example

::

   mkdir $HOME/Work/Projects

Execute the following commands in a terminal

::

   cd $HOME/Work/Projects
   ERun Elements 4.0 CreateElementsProject ExampleProject 1.0
   cd $HOME/Work/Projects/ExampleProject/1.0
   ERun Elements 4.0 AddElementsModule ExampleModule
   cd ExampleModule
   ERun Elements 4.0 AddCppClass ExampleCppClass
   ERun Elements 4.0 AddCppProgram ExampleCppProgram
   ERun Elements 4.0 AddPythonModule ExamplePyModule
   ERun Elements 4.0 AddPythonProgram ExamplePyProgram

Script names are rather self-explanatory. The above commands create a
new **Elements project**, populate it with an **Elements module**,
create a **C class** and a **C program** as well as a **Python module**
and a **Python program**. All required source files are created and
place at specific appropriate location. The directory structure of an
Elements project must respect the standard represented in the following
[[codeen-users:ElementsProjectStructureAndBuildInstruction|sketch]].

All scripts have a **—help** option which provide a short description of
their actions and of all the command line options. For example

::

   ERun Elements 4.0 CreateElementsProject --help

You can freely experiment with the helper scripts listed above, but
please make sure you have a clean and complete project before continuing
with the second point of this tutorial.

You can start everything from scratch again after executing a simple,
deep cleaning with

::

   cd /home/"MyUserName"/Work/Projects
   rm -rf ExampleProject

2. Eclipse start
----------------

2.1 Opening the new project
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Start Eclipse and create a project using the source code you have just
created:

::

   File : New : Makefile Project with Existing Code

   Existing Code Location: → Browse "/home/"MyUserName"/Work/Projects/ExampleProject/1.0"
   Project Name: change "1.0" to "ExampleProject"
   Toolchain for Indexer Settings: → Linux GCC
   → Finish

2.2 Eclipse configuration: Fixing Eclipse Build Output Parser
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This step is required to avoid false Eclipse error detection, i.e., red
crosses in editor.

::

   Right click on “ExampleProject” → Properties
   C/C++ General : Preprocessor Include Paths, Macros, etc.
   Providers
   CDT GCC Build Output Parser
   Select: Use global provider shared between projects → Apply

Check on your screen that the “compiler command pattern” changed from

“(gcc)|([gc]\\\ *\\*)|(clang)” to
“(.\ **gcc)|(.**\ [gc]\\\ *\\*)|(.*clang)”

2.3 Eclipse configuration: Fixing Python auto completion
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is no-longer an issue, Python auto completion seems to work fine
without extra configuration with more recent versions of Eclipse.

3 Compiling and executing the tests
-----------------------------------

At the early stage, the project should compile but the tests
ExampleModule.ExampleCppClass_test and ExampleModule_test.py should FAIL
(see below). Do not worry, this is expected as these tests are not yet
implemented.

3.1 Command lines
~~~~~~~~~~~~~~~~~

Run a make target in the top level directory

::

   cd /home/"MyUserName"/Work/Projects/ExampleProject/1.0
   make configure
   make all
   make test

   make purge (if you want to start from scratch again)

Note that”make all” and “make test” also calls the “configure” target.

3.2 Eclipse
~~~~~~~~~~~

::

   Go to the Make Target window
   (“Window : Show View : Make Target” if it is not visible)
   Click on "ExampleProject"
   Click on the "New Make Target" symbol (leftmost in the upper menu)
   Enter the target name "all"
   (also “configure”, “install”, “clean”, “purge”, “test” if you like)
   Double click the “all” target
   Check the “Console” for build errors
   Check that the built products appeared in the new directory build_x86_64-sl7-gcc48-o2g/
   Double click the “test” target to execute the tests.

Please keep the compilation and execution instructions on the back of
your mind (or come back to this section if needed), as you will need to
compile and execute several times during this tutorial.

4. Completing the ExampleCppClass C class
-----------------------------------------

4.1 Header file
~~~~~~~~~~~~~~~

In the following header file,

/home/”MyUserName”/Work/Projects/ExampleProject/1.0/ExampleModule/ExampleModule/ExampleCppClass.h

include the below declaration in the public part of the ExampleCppClass
class

::

   /**
   * @brief
   * Returns a particular version of the "average" of the vector values
   *
   * @details
   * This method computes the "average" as the median of the given values if there are more
   * than 5 values. Otherwise, it returns the mean value instead of the median.
   *
   * If an even number of numbers is given, the returned value is the mean of the
   * two middle values.
   *
   * @param input_vector
   * The values to compute the "average" for
   * @return
   * The "average" of the given values
   * @throws Elements::Exception
   * If the given vector is empty
   */
   double average (const std::vector<int>& input_vector);

and add the following statement at the beginning of the same file, just
after the “#define \_EXAMPLEMODULE_EXAMPLECPPCLASS_H” statement.

::

   #include <vector>

4.2 Implementation
~~~~~~~~~~~~~~~~~~

In the file

/home/”MyUserName”/Work/Projects/ExampleProject/1.0/ExampleModule/src/lib/ExampleCppClass.cpp

Copy the below method implementation within the “namespace ExampleModule
{” body.

::

   double ExampleCppClass::average (const std::vector<int>& v) {

   double result = 0;
   auto size = v.size();
   // Throw an exception if the number of vector elements is null!
   if (size == 0) {
   throw Elements::Exception() << "Input vector has no element!";
   }

   // We check if we have enough numbers to compute the median
   if (size > 5) {
   //
   std::vector<int> ordered { v.begin(), v.end() };
   std::sort(ordered.begin(), ordered.end());
   if (size % 2 == 0) {
   result = (ordered[size / 2 - 1] + ordered[size / 2]) / 2.;
   } else {
   result = ordered[size / 2];
   }

   } else {
   // If we have less than 5 numbers we compute the mean
   auto sum = std::accumulate(v.begin(), v.end(), 0.);

   result = sum / size;
   }

   return result;
   }

Make sure to include the appropriate statements at the beginning of the
same file, just before the other include statement.

::

   #include <algorithm>
   #include <numeric>
   #include "ElementsKernel/Exception.h"

4.3 Unit testing
~~~~~~~~~~~~~~~~

In the unit test file

/home/”MyUserName”/Work/Projects/ExampleProject/1.0/ExampleModule/tests/src/ExampleCppClass_test.cpp

add the following include statement at the appropriate location

::

   #include "ElementsKernel/Exception.h"

and replace the existing test case below:

::

   BOOST_AUTO_TEST_CASE( example_test ) {

     BOOST_FAIL("!!!! Please implement your tests !!!!");

   }

by the following ones:

::

   /*
   * 1. Check the median is returned when more than 5 values are provided and that
   * for even number the returned value is the mean of the to middle values.
   */
   BOOST_AUTO_TEST_CASE(median_test) {
   ExampleCppClass tested_object{};
   std::vector<std::vector<int>> input_vec
   {{1, 2, 3, 10, 101, 102},
   {1, 2, 3, 11, 101, 102, 103}
   };

   std::vector<double> expected_result {6.5, 11};

   for(int test_id=0; test_id<2;++test_id){
   BOOST_CHECK_EQUAL(expected_result[test_id],
   tested_object.average(input_vec[test_id]));
   }
   }

   /*
   * 2. Check the initial order of the input values has no impact on the returned
   * value for the median case.
   */
   BOOST_AUTO_TEST_CASE(order_test) {
   ExampleCppClass tested_object{};

   std::vector<int> input_vec {103, 101, 2 ,1 , 3, 11, 102};
   double expected_result {11};

   BOOST_CHECK_EQUAL(expected_result, tested_object.average(input_vec));
   }

   /*
   * 3. Check the mean is returned when 5 or less values are provided.
   */
   BOOST_AUTO_TEST_CASE(mean_test) {
   ExampleCppClass tested_object{};
   std::vector<std::vector<int>> input_vec {
   {1},
   {1,2},
   {1,2,4},
   {1,2,4,8},
   {1,2,4,8,16}};
   std::vector<double> expected_result
   {1,1.5,2.33333333333333333,3.75,6.2};
   double tolerence {1e-10};

   for(int test_id=0; test_id<5;++test_id){
   BOOST_CHECK_CLOSE(expected_result[test_id],
   tested_object.average(input_vec[test_id]),
   tolerence);
   }
   }

   /*
   * 4. Check that the method throws an exception when called with an empty vector.
   */
   BOOST_AUTO_TEST_CASE(exception_test) {
   ExampleCppClass tested_object{};
   std::vector<int> input_vec {};

   BOOST_CHECK_THROW(tested_object.average(input_vec),
   Elements::Exception);
   }

Now, you should be able to compile again and run the tests.

5. Completing the ExampleCppProgram executable C program
--------------------------------------------------------

5.1 Checking log messages
~~~~~~~~~~~~~~~~~~~~~~~~~

Replace the following lines of code within the main method(…) { … } of
the ExampleModule/src/program/ExampleCppProgram.cpp source file. at :

::

       //
       // !!! Implement you program here !!!
       //

by:

::

   logger.info() << "#";
   logger.info() << "This is the entry point to your program implementation";
   logger.info() << "#";

Save and compile your project (see above section 4) and execute the
ExampleCppProgram with the “—help” options or without any option, in
command line

::

   cd /home/"MyUserName"/Work/Projects/ExampleProject/1.0
   ./build.x86_64-co7-gcc48-o2g/run ExampleCppProgram --help
   ./build.x86_64-co7-gcc48-o2g/run ExampleCppProgram

or within Eclipse:

::

   Right click on “ExampleProject” → Run As : Run Configurations
   Main : C/C++ Application
   New launch configuration (upper, left-most icon)
   ﻿ Name: ExampleProject ExampleCppProgram
   C/C++ Application
   Enter: build.x86_64-co7-gcc48-o2g/run
   Arguments : Program arguments﻿ :
   Enter: ExampleCppProgram
   Apply : Run … and check the Console

You can also use the upper menu bar to run the programs and click on
“Organize Favorites…” if you want to see our favorite launch
configuration in the launch menu.

Check that the above messages appear in the log on the console.

5.2 Adding program options
~~~~~~~~~~~~~~~~~~~~~~~~~~

In defineSpecificProgramOptions() of the ExampleCppProgram.cpp, at the
“// !!! Implement the program options here !!!” location, add the
definition of two program options (note that “po” is short of
“boost::program_options”) as follow

::

   // Add specific program options
   options.add_options()
   ("double-option", po::value<double>()->default_value(double { }), "An example double option")
   ("int-vector-option", po::value<std::vector<int>>()->multitoken()->default_value(std::vector<int> { }, "Empty"),
   "An example vector option");

Further in the mainMethod(…) of the same file (between the log statement
included in the last section), add

::

   /*
   * Get and log one of the program arguments (or options)
   *
   */
   double double_option { args["double-option"].as<double>() };
   logger.info() << "Double option value: " << double_option;

and add the following statement at the beginning of the same file next
to the other includes.

::

   #include <vector>

Now, you can compile and execute the program again to check the log.

Try to enter option values as command line arguments

::

   cd /home/"MyUserName"/Work/Projects/ExampleProject/1.0
   ./build.x86_64-co7-gcc48-o2g/run ExampleCppProgram --double-option 3.54 --int-vector-option 1 2 3

or through the configuration file, adding

::

   double-option = 3.14
   int-vector-option = 17
   int-vector-option = 41
   int-vector-option = 77

into

::

   /home/"MyUserName"/Work/Projects/ExampleProject/1.0/ExampleModule/conf/ExampleModule/ExampleCppProgram.conf

Also try to put default values in the sources code. Which of the above
three ways to pass option values has the precedence?

6. Completing the ExamplePyModule.py Python class
-------------------------------------------------

In the file

/home/”MyUserName”/Work/Projects/ExampleProject/1.0/ExampleModule/python/ExampleModule/ExamplePyModule.py

Copy the following code

::

   class ExampleClass(object):
       """ 
       @class ExampleClass
       @brief This is an example of the most basic Python class
       """ 
       def __init__(self, list):
           """ 
           @brief Constructor
           """ 
           self._m_i = 5
           self._m_list = list

       def sumListValues(self):
           """ 
           @brief Method summing the elements of the list
           """ 
           sum = 0
           for element in self._m_list:
               sum += element
           return sum

       @staticmethod
       def product(first, second):
           """ 
           @brief Procuct
           @details This is an example static method
           @param first
           The first number
           @param second
           The second number
           @returns
           The product of the two numbers
           """ 
           return first * second

Move to the Python test directory, i.e.,

::

   /home/"MyUserName"/Work/Projects/ExampleProject/ExampleModule/tests/python

open the ExamplePyModule_test.py file and replace

::

   def testFailure(self):
   assert False, "!!!! Please implement your tests !!!!"

with

::

   import pytest
   from ExampleModule.ExamplePyModule import ExampleClass

   class TestExamplePyModule(object):
       """
       @class TestExamplePyModule
       @brief Unit Test class
       !!! Test class example for python             !!!
       !!! Please remove it and add your tests there !!!
       """
       tol = 1e-6
       first = 23.4857
       second = 3.4756
       my_list = [6, 7, 8, 9]
       expected_result = 6 + 7 + 8 + 9
       example_object = ExampleClass(my_list)

       def testProduct(self):
           result = ExampleClass.product(self.first, self.second)
           assert abs(result - (self.first * self.second)) < self.tol

       def testSumListValues(self):
           assert abs(self.expected_result - self.example_object.sumListValues()) < self.tol

Build and run the test target to make sure the unit test are OK.

::

   make purge
   make all
   make test

More on unit test during the session devoted to unit testing

7. Completing the ExamplePyProgram.py Python program
----------------------------------------------------

In the file

/home/”MyUserName”/Work/Projects/ExampleProject/ExampleModule/python/ExampleModule/ExamplePyProgram.py

add in the defineSpecificProgramOptions() method, the following code

::

       parser.add_argument('--float-option', type=float, help='A float option')
       parser.add_argument('--int-list-option', nargs='+', type=int, default=[], 
                           help='A integer list option')

in the mainMethod(args) add

::

       #
       # Log some of the arguments
       #
       float_from_configuration = args.float_option
       logger.info('Example float : ' + str(float_from_configuration))
       if len(args.int_list_option) > 1:
           second_element = args.int_list_option[1]
           logger.info('Second elements from the list : ' + str(second_element) )

       #
       # Calling a static method
       #
       result = ExampleClass.product(3, 4)
       logger.info('Static method result : ' + str(result) )

       #
       # Calling the constructor and a method member
       #
       example_object = ExampleClass(args.int_list_option)
       logger.info('Sum of the list elements : ' + str(example_object.sumListValues()))

and add the following import statement at the begining of the file, just
after the other import statement (Eclipse may get confused about the
“import ElementsKernel…” but this should have no consequences).

::

   from ExampleModule.ExamplePyModule import ExampleClass # for ExampleClass

Now you can execute the ExamplePyProgram program with different options,
on the command line

::

   cd /home/"MyUserName"/Work/Projects/ExampleProject/1.0
   ./build.x86_64-co7-gcc48-o2g/run ExamplePyProgram --help
   ./build.x86_64-co7-gcc48-o2g/run ExamplePyProgram
   ./build.x86_64-co7-gcc48-o2g/run ExamplePyProgram --float-option=3.14 --int-list-option 1 2 3

or within Eclipse:

::

   Right click on "ExampleProject" → Run As : Run Configurations
   Main : C/C++ Application
   New launch configuration (upper, left-most icon)
   ﻿ Name: ExampleProject ExamplePyProgram
   C/C++ Application
   Enter: build.x86_64-co7-gcc48-o2g/run
   Arguments : Program arguments﻿ :
   Enter: "ExamplePyProgram"
   Apply : Run … and check the Console

You can also use the upper menu bar to run the programs and click on
“Organize Favorites…” if you want to see our favorite launch
configuration in the launch menu.

Try to pass different options within Eclipse or through the
configuration file, adding

::

   float-option = 3.14
   int-list-option = 1 2 3

into

::

   /home/"MyUserName"/Work/Projects/ExampleProject/1.0/ExampleModule/conf/ExampleModule/ExamplePyProgram.conf

8. Structure of an Elements project and CMakeLists.txt build instructions
-------------------------------------------------------------------------

If you would like to better understand the structure of an
Elements-based project and how the build instruction are encoded in the
CMakeLists.txt files, please follow this
[[codeen-users:ElementsProjectStructureAndBuildInstruction|link]].

Next steps
----------

Congratulations you have now completed this tutorial. You can continue
with your own software implementation and consult the main Elements page
for additional user resources.
