FAQ
===

{{>toc}}

What is Elements?
-----------------

Elements is a **development**, **compilation** and **execution**
framework tailored to Euclid needs.

It provides a folder and file structure to organize the code in
Projects, and some functions and classes to interface with the
infrastructure and other Processing Functions.

Projects are composed of Modules, which can be seen as libraries, and
Programs which can be called in command line.

Compilation tools are provided to correctly install and link your
Programs, as well as setting up the execution environment.

In particular, it ensures compatibility with the IAL-DRM aka. **Pipeline
Runner**.

Elements is based on **CMake**.

Where to start?
---------------

| A **quick start tutorial** has just been released
  `here <http://euclid.roe.ac.uk/projects/elements/wiki/Tutorial>`__
| You will learn how to create a Project, Modules and Programs, and how
  to compile and run them.

Also, you will find a very good companion in the comments of the
`automatically
generated <http://euclid.roe.ac.uk/projects/elements/wiki/newusermanual#Helper-Scripts-Creating-New-Source-Files>`__
``CMakeLists.txt`` files.

Why is the Project structure so complex?
----------------------------------------

To have one structure for both C, Python and hybrid Projects generates
some complexity.

Moreover, Elements provides many features (e.g., unit tests,
documentation generation, Pipeline Runner compatibility) which come with
their own constraints.

In the end, such a structure is very common in large sofware
developments such as Euclid’s.

However, Elements is provided with a handful set of
`scripts <http://euclid.roe.ac.uk/projects/elements/wiki/newusermanual#Helper-Scripts-Creating-New-Source-Files>`__
for **automatically generating** the whole structure and template
``CMakeLists.txt`` files.

How to use a Module in another Module?
--------------------------------------

| Assume ``ModuleB`` of project ``ProjectA 1.0`` depends on ``ModuleA``
  of the same project.
| (If the modules are in different projects, go to the next how-to.)

| First, you need to declare the dependency in the ``CMakeLists.txt`` of
  ``ModuleB``, which is located in ``ProjectA/1.0/ModuleB/`` or
  ``ProjectA/ModuleB/``.
| In this ``CMakeLists.txt``, add the following declaration:

elements_depends_on_subdir(ModuleA)

| elements_add_library(ModuleB src/lib/*.cpp
| LINK_LIBRARIES ModuleA
| INCLUDE_DIRS ModuleA
| PUBLIC_HEADERS ModuleB)

Then, it depends on your language.

C case
~~~~~~

Let’s call ``ModuleA/ClassA.h`` the header file to be included in
``ModuleB/ClassB.h``\ or ``ModuleB/ClassB.cpp``.

Just add:

#include “ModuleA/ClassA.h”

to the ``ClassB.h`` or ``ClassB.cpp`` file.

Python case
~~~~~~~~~~~

Let’s call ``ModuleA/ClassA.py`` the Python file to be used by
``ModuleB/ClassB.py``.

Just add:

import ModuleA.ClassA

to the ``ModuleB/ClassB.py`` file.

How to use a Project in another Project?
----------------------------------------

Let’s call ``A 1.0`` the project to be used by project ``B 0.1``.

-  Compile and install project ``A 1.0``
-  In the root ``CMakeLists.txt`` file of project ``B 0.1`` (in folder
   ``B/0.1/`` or ``B/``), write:

elements_project(B 0.1 USE A 1.0)

-  Handle the Module dependency by following the previous how-to.

How to use an external library?
-------------------------------

There are two cases: either the library is in EDEN (`check
here <http://euclid.roe.ac.uk/projects/codeen-users/wiki/EDEN#EDEN-Production-Environment-content)>`__,
or you have to install it.

EDEN library case
~~~~~~~~~~~~~~~~~

Using an EDEN library ``LibA 2.3`` is similar to using another Project:
just add ``USE LibA 2.3`` to the root ``CMakeLists.txt`` (see previous
how-to).

Non-EDEN library case
~~~~~~~~~~~~~~~~~~~~~

If you think this library should be added to EDEN, please have a look at
the `Change
Process <http://euclid.roe.ac.uk/projects/ct_change_process/wiki/Change_Process>`__.

| First you will have to install the library (let’s call it
  ``LibA 2.3``), and then give Elements the linking information (for
  more details, look for CMake packages on the Web)
| To this end:

-  Provide the ``FindLibA.cmake`` file, which is generally bundled with
   the library installation.
-  Add the following line to the root ``CMakeLists.txt`` file:

find_package(LibA)

-  Follow the EDEN library case how-to

How to specify cross dependencies?
----------------------------------

This is **not** possible.

| The graph of dependencies between projects and between modules should
  be a tree.
| If you want that ``A`` depend on ``B`` which depends on ``A``, you
  have a design issue (which is not related to Elements).

This generally means a common code ``C`` should be extracted so that
both ``A`` and ``B`` depend on ``C`` but ``C`` is standalone.

How to run my Program?
----------------------

Let’s ``ProgramA`` be the Program of project ``ProjectA 1.0`` to be run
with option ``iter`` set to 1000.

-  Compile and install ``ProjectA 1.0``
-  Call

E-Run ProjectA 1.0 ProgramA —iter 1000

By the way, note that ``ERun`` is an alias for ``E-Run`` so that the
latter is generally safer.

How to increase the informations of the the test run?
-----------------------------------------------------

Usually the tests are run by calling:

::

   make test

| 
| from the main directory of the project. If one would like to see more
  details, for a failing test for example, the verbosity can be
  increased by doing

::

   make ARGS="-VV" test

instead.

How to chain Programs?
----------------------

Head to the `Pipeline Runner
documentation <http://euclid.roe.ac.uk/projects/codeen-users/wiki/Pipeline_Framework>`__
;-)

I have another question!
------------------------

Please contact the `CT support mailing list:
commontools.support@lists.euclid-ec.org <mailto:commontools.support@lists.euclid-ec.org>`__

If general enough, the question (and answer) will be added to the FAQ.

I think I found a bug!
----------------------

| Bugs are tracked through the `issue
  tracker <http://euclid.roe.ac.uk/projects/codeen-users/issues>`__
| Please check your bug is not already known.

If not, please contact the `CT support mailing list:
commontools.support@lists.euclid-ec.org <mailto:commontools.support@lists.euclid-ec.org>`__
