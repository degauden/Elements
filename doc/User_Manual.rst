User Manual
===========

Introduction
------------

One challenge in the management of a C project is the need for a
building system. Unlike Java, for example, which has a powerful building
and distribution system (e.g. Maven), C projects require additional
software to perform the same task. Roughly speaking, the main
requirements for this system can be listed as

-  Build the C libraries and executables
-  Package the various C projects with management of their dependencies

The dependencies are in two forms: between projects as well as with
external software. It is also important to note Linux will be the main
platform considered. It is not limited to this platform but this is the
central one.

Overview and Requirements for A Build System
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Managing software for a large community requires providing
extra-flexibility that is not usually found by default on the installed
system.

Specifically, using a custom location for the software base and allowing
several versions of the same software to be installed side-by-side at
the same time on one system are features that have been borrowed from
the `Gaudi <http://cern.ch/gaudi>`__ philosophy. Unfortunately, these
useful features come with a price: more settings are needed in order to
handle these features. We will try to clarify and discuss these points
in the following sections.

Using A Custom Software Base
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The first feature that will guarantee some freedom and flexibility in
the software distribution and installation is the usage of a custom
location. For example, the ``/opt/<vendor>`` directory can be chosen to
host the different libraries, scripts and binaries of the collaboration.
The ``<vendor>`` keyword is the name of the software provider: a
company, or a collaboration or a mission name that makes the location
unique. For example, MacPorts is using the ``/opt/local`` directory for
its prefixed installation.

This is not exceptional and many software providers (Sun, Oracle,
Google, etc) are using a custom location for their software. But, this
implies that some additional settings are needed. For example, at run
time, the ``PATH`` and ``LD_LIBRARY_PATH`` environment variables need to
be updated to accommodate the extra directories:

===================== ===================== ===================
The Standard Location **Custom Location**   
===================== ===================== ===================
Path Entry            Path Entry            Environment
``/usr/lib``          ``/opt/<vendor>/lib`` ``LD_LIBRARY_PATH``
``/usr/bin``          ``/opt/<vendor>/bin`` ``PATH``
===================== ===================== ===================

The full path of the ``bin`` and ``lib`` subdirectories have to be
prepended (or appended) to their respective environment variables.

This is also true at build time. Some extra parameters (``-I`` and
``-L``) need to be passed to the compiler (and linker) to create the
software:

===================== =========================== ==========
The Standard Location **Custom Location**         
===================== =========================== ==========
Path Entry            Path Entry                  G Settings
``/usr/lib``          ``-L/opt/<vendor>/lib``     linker
``/usr/include``      ``-I/opt/<vendor>/include`` compiler
===================== =========================== ==========

The reason for a custom install location is not yet very clear.
Apparently, it only adds complexity to the deployment of the software.
The added value will appear with the scaling of the project. When many
implementers have to share the same space for an enormous amount of
code, structuring will become vital. Essentially, the subdivision of the
code into a hierarchy of inter-dependent projects will greatly simplify
the management of the various parts.

Using Several Software Projects
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The main reason for dividing the global software effort of a
collaboration into projects is that it allows the grouping of developers
around the same topic. From the ground frameworks through the utility
libraries up to the final and subsequent level final business
applications, different teams of developers join their effort into a
common workflow. Hence, the notion of a software project that each team
can develop, manage and release independently. This is especially
important when the development of projects occur at different rates. It
is not uncommon that the base libraries and frameworks have a longer
release cycle.

In this view, the Elements project serves as a ground framework
providing the structuring tools for all other projects. The software of
the collaboration consists of a dependency graph
(`DAG <https://en.wikipedia.org/wiki/Directed_acyclic_graph)>`__ of
projects which has Elements at its root.

Structuring the software into several projects doesn’t necessarily imply
separating the location of their respective products. For example, every
executable and every library of each project could all be located in the
same target directories: ``/opt/<vendor>/bin`` and
``/opt/<vendor>/lib``. This is certainly an advantage for the runtime
setup where only a unique entry for the ``PATH`` and ``LD_LIBRARY_PATH``
is needed to reach those binary files.

There are, however, some advantages when using different deployment
areas for different projects. For the run time part:

+-----------------------------------------------+---------------------+
| Path Entry                                    | Environment         |
+===============================================+=====================+
| ``/opt/<vendor>/                              | ``LD_LIBRARY_PATH`` |
| <project2>/lib:/opt/<vendor>/<project1>/lib`` |                     |
+-----------------------------------------------+---------------------+
| ``/opt/<vendor>/                              | ``PATH``            |
| <project2>/bin:/opt/<vendor>/<project1>/bin`` |                     |
+-----------------------------------------------+---------------------+

and for the build time:

+--------------------------------------------------------+------------+
| Path Entry                                             | G Settings |
+========================================================+============+
| ``-L/opt/<v                                            | linker     |
| endor>/<project2>/lib -L/opt/<vendor>/<project1>/lib`` |            |
+--------------------------------------------------------+------------+
| ``-I/opt/<v                                            | compiler   |
| endor>/<project2>/bin -I/opt/<vendor>/<project1>/bin`` |            |
+--------------------------------------------------------+------------+

The main advantage is to be able to **override some specific
functionality implemented in the binary executables or libraries of a
dependency project**. This is especially useful when base projects, like
the Elements framework or some other fundamental libraries, have a slow
release cycle. The top projects might need some urgent fixes in these
base projects, which may not happen before their next release if
scheduled months later from that point.

In this situation, the top project just does a copy of the buggy library
sources, fixes it, and builds it. Since that library will be found first
at run time in the ``LD_LIBRARY_PATH``, it will override the one from
the base project.

However, this handy feature has some constraints:

-  The functions of the copied library must respect the same public
   interface. The local changes can only be internal and the public
   header files have to stay the same. Otherwise, the binary
   substitution will not work.
-  As a matter of good collaboration, the fixes brought to the
   overridden library should be fed back to the original library. This
   will make that enhanced version available for a broader audience of
   dependent projects.
-  The run time configuration is now a bit more complicated. Each
   project has to have its own ``bin`` and ``lib`` directory entries in
   the ``PATH`` and ``LD_LIBRARY_PATH`` environment variables. This will
   be handled by the EuclidEnv standalone package.

Using Multiple Versions
^^^^^^^^^^^^^^^^^^^^^^^

There is one last feature which comes very handy for the software
development by a large collaboration of developers: the freedom to
choose the version of the project for development.

At times during the development phase (but often after that), it can
occur that the various teams of developers working on different projects
cannot afford to be dependent on the same version of a given base
project. Let’s say an analysis project is depending on Elements 2.0
whereas a Monte-Carlo simulation project is depending on Elements 1.7.
Certainly, it is foreseeable in the future that the latter one will
catch up and move to Elements 2.0. But, it is not at the right moment to
do so.

If the development and run are performed on a separate machine, like a
laptop, for example, the user can choose to install whichever version
she wants: a unique version.

There are in fact situations when the presence of multiple versions of a
project are useful such as:

-  When working on a server or on a shared file system: as the projects
   become large and long to build, a shared location containing the base
   projects with several versions will allow many people to use them.
-  This is also the case for a cluster batch system: Top-level projects
   which have to run don’t depend necessarily on the same project
   versions.
-  For the development: if a programmer wishes to compare his results
   against the old and new version of a set of libraries, he will need
   to have two versions of the same project installed side-by-side.

This is the reason why the support for the dependency on a **specific
version of a project** has been implemented in the Elements building
structure. The run time environment will then look like:

+-----------------------------------------------+---------------------+
| Path Entry                                    | Environment         |
+===============================================+=====================+
| ``/opt/<vendor>/<project2>/<ve                | ``LD_LIBRARY_PATH`` |
| r2>/lib:/opt/<vendor>/<project1>/<ver1>/lib`` |                     |
+-----------------------------------------------+---------------------+
| ``/opt/<vendor>/                              | ``PATH``            |
| <project2>/bin:/opt/<vendor>/<project1>/bin`` |                     |
+-----------------------------------------------+---------------------+

and for the build time:

+--------------------------------------------------------+------------+
| Path Entry                                             | G Settings |
+========================================================+============+
| ``-L/opt/<vendor>/<projec                              | linker     |
| t2>/<ver2>/lib -L/opt/<vendor>/<project1>/<ver1>/lib`` |            |
+--------------------------------------------------------+------------+
| ``-I/opt/<vendor>/<projec                              | compiler   |
| t2>/<ver2>/bin -I/opt/<vendor>/<project1>/<ver1>/bin`` |            |
+--------------------------------------------------------+------------+

where ``<ver1>`` is the version of the ``<project1>`` and ``<ver2>`` is
the version of the ``<project2>``.

Being Modular
^^^^^^^^^^^^^

One of the nice feature that can help greatly the work within the same
project is the possibility to split it into several *modules* (or
*packages* according to an old naming convention). The modules are top
level directories in the source tree of the project that contains the
configuration and sources of various built items. They can contains the
files to construct several executables and libraries. For example,
``ElementsKernel`` and ``ElementsExamples`` are both modules of the
Elements project.

For convenience, it is recommended that the override of some feature,
like described above, is done at the level of a full module. This is
easier to spot later on. In details, if one wants to override locally a
library (e.g. libMyExample.so), it recommended to copy locally the
MyExample module in order to make modifications. Not only the library
sources.

Review of Build Tools
~~~~~~~~~~~~~~~~~~~~~

Several build tools are available. From a rather low level (like gmake
for example) or with very extended features (autotools, CMake, etc). The
main difference resides in the possibility of doing a kind of meta
configuration by adapting to the system with the more sophisticated
tools.

While there exist quite a lot of these high level tools, we will only
consider three that could satisfy our requirements:

-  CMT
-  the GNU Autotools
-  CMake

The CMT Build Tool
^^^^^^^^^^^^^^^^^^

| CMT is the original build tool that was develop especially to match
  our requirements. CMT has been written for the management of large
  physics projects
| mainly at `CERN <http://cern.ch>`__. It is supported by the
  `LAL <http://http://www.lal.in2p3.fr/>`__. It was also used by the
  `Gaudi <http://cern.ch/gaudi>`__ project from which Elements is
  derived.

This is a software manager that generates Makefiles and setup scripts.
It relies on conventions and tools.

-  Pros

   -  Powerful and simple to handle even for beginners
   -  Fulfill most of the requirements for collaborative development of
      a largecommunity
   -  Provides tools to inspect the configuration and the dependencies
   -  Calculates automatically the dependencies between the packages.
   -  Extensible

-  Cons

   -  Poor documentation
   -  Not widely Used. Only in particule physics.
   -  Doesn’t provide out-of-sources build
   -  Doesn’t make any distinction between the build environment and the
      run environment
   -  Provides only dependencies between packages, does not provide fine
      grained dependencies between the files inside the packages.
   -  No install step provided for the packaging procedure
   -  It starts to mark its age.
   -  It can be impossibly slow for a big hierarchy of software projects
      containing many packages.

The GNU Autotools
^^^^^^^^^^^^^^^^^

| The GNU autotools chain looks like:
| |image1|

-  Pros

   -  The Autotools system is an old set of well tested GNU tools to
      build any software.
   -  It is present on any Linux platform without anyextra installation.

-  Cons

   -  It is almost a Linux (Unix) only tool. Difficult if not impossible
      to use on other platforms.
   -  It has many steps and many configuration files. It is difficult to
      maintain and support.
   -  It is also difficult to extend and painful to provide a generic
      easy to use build framework for any package.

CMake
^^^^^

Finally, the CMake build tool will be our tool of choice. It has many
features ideal features:

-  CMake is one of the only open-source full-fledged build manager.
-  It is supported by a large community. There exist Wiki, FAQ, etc.
-  CMake is an extensible, open-source system that manages the build
   process in an operating system and in a compiler-independent manner.
-  Unlike many cross-platform systems, CMake is designed to be used in
   conjunction with the native build environment. Simple configuration
   files placed in each source directory (called CMakeLists.txt files)
   are used to generate standard build files (e.g., makefiles on Unix
   and projects/workspaces in Windows MSVC) which are used in the usual
   way.
-  It relies on short standard instructions to build libraries,
   executables. It has a complete language that allows to extend it for
   any need.
-  It can do native clean out-of-source builds.
-  It supports an install procedure.

As one can have guess, **CMake is our build tool** and it is used to
construct the CMake stucturing library that Elements is providing.

Management of Extra Software Sources
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On top of classical binary build, generally from C sources, Elements has
been extended to support various other types of file:

-  The python packages, modules and scripts,
-  Some extra configuration files
-  and some auxiliary files

which are included in the source tree. We will explain later where these
kind of files are meant to be located within the package structure. But
anyway, they imply that the run time environment has to be extended. The
following table explains the setup:

+----------------------+---------------------+----------------------+
| Variable             | Type of files       | File Format          |
+======================+=====================+======================+
| ``PATH``             | binary executables, | generally without    |
|                      | shell scripts,      | extension            |
|                      | python scripts      |                      |
+----------------------+---------------------+----------------------+
| ``LD_LIBRARY_PATH``  | binary libraries    | ``lib<name>.so`` for |
|                      |                     | C libraries,         |
|                      |                     | ``_<name>.so`` for   |
|                      |                     | python binary        |
|                      |                     | modules              |
+----------------------+---------------------+----------------------+
| ``PYTHONPATH``       | python packages     | python packages or   |
|                      |                     | python modules       |
+----------------------+---------------------+----------------------+
| ``                   | default             | with a ``.conf``     |
| ELEMENTS_CONF_PATH`` | configuration files | extension            |
+----------------------+---------------------+----------------------+
| `                    | auxiliary files     | common files with an |
| `ELEMENTS_AUX_PATH`` |                     | extension            |
+----------------------+---------------------+----------------------+

The setup of the run time environment with all the mentioned environment
variables, for the whole chain of the target project is generated by a
standalone executable called ``E-Run``. This command is provided by the
EuclidEnv project and it goal is to close the gap between the standard
layout of a Unix-like system and the custom structure of our projects
deployment with:

-  a custom location: ``/opt/euclid``
-  several projects: ``/opt/euclid/Elements``,
   ``/opt/euclid/Alexandria``, etc
-  several versions for each project: ``/opt/euclid/Elements/1.0``,
   ``/opt/euclid/Elements/1.1``, ``/opt/euclid/Elements/2.0``
-  several type of binaries for each build:
   ``/opt/euclid/Elements/1.0/InstallArea/x86_64-co7-gcc48-opt``,
   ``/opt/euclid/Elements/1.0/InstallArea/x86_64-co7-gcc48-dbg``,
   ``/opt/euclid/Elements/1.0/InstallArea/x86_64-co7-gcc48-pro``, etc

Building
--------

The system of build implemented in the Elements framework is written on
top of the CMake libraries. It can be seen as a CMake library or as a
CMake extension. A top wrapper Makefile is provided to ease the build
process for the developer. The detailed build instruction can be access
at that [[NewUserManual#Build-Instructions|link]]

Behavior of the Elements Build Library and Management of Project Dependencies
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Roughly, the search for parent projects for a given Alpha project is
provided by 2 ingredients: the name of the parent project(s) and
its(their) version(s). This search is done in the collection of paths
hold by the CMAKE_PROJECT_PATH environment variable. This variables is
made of filesystem paths separated by a “:”. *The dependencies of each
project is defined in the ``CMakeLists.txt`` file that is located at the
root of each project.*

First lets suppose that we have 4 projects, Alpha, Beta, Gamma and
Delta. Delta is the base project which is usually held by the framework.
In a concrete case, this would be the Elements project itself. Beta and
Gamma are intermediate projects which are providing useful libraries and
tools. Finally Alpha is the top project. This is one of the project
(they can be many) from which we are running the final applications
(executables) of our collaboration.

In details, we have the project Alpha that depends on Beta and Gamma.
And Beta and Gamma that depend on Delta:

-  Alpha 4.1 depends on Beta 1.0 and on Gamma 2.1
   ::

      elements_project(Alpha 4.1 USE Beta 1.0 Gamma 2.1)

-  Beta 1.0 depends on Delta 2.2
   ::

      elements_project(Beta 1.0 USE Delta 2.2)

-  Gamma 2.1 depends on Delta 2.2
   ::

      elements_project(Gamma 2.1 USE Delta 2.2)

-  Delta 2.2 depends on nothing
   ::

      elements_project(Delta 2.2)

Interleaved in the previous list is written the entry for the definition
of the project in the top ``CMakeLists.txt`` file. It shows the
dependencies in terms of CMake functions.

Now that we have the names and the versions, we can use the
``CMAKE_PROJECT_PATH`` environment variable for the searching. This
variable is built with a list of filesystem paths separated with a “:”:
``path1:path2:path3``. Each of the path can be structured in a different
way. It can contain:

-  projects without version directories
-  projects **with** version directories

Generally these are not mixed. These top paths contain only the first or
the second type. Typically the ``CMAKE_PROJECT_PATH`` has the form

::

   CMAKE_PROJECT_PATH=$User_area:/opt/euclid

The first part (``User_area``) is often a directory holding only
projects without version directories. This is generally the branch
checkout of the developer. It is worth noting that this part is
automatically set up by the EuclidEnv package if it is installed on the
system. The location of the personal user workspace can be overridden if
needed, but by default we have ``User_area=${HOME}/Work/Projects``.

The second part (``/opt/euclid``) contains the various installed
versions of the projects. These are, by construction, projects with
version directories.

The search for parent projects is incremental. It is done project by
project, in the order of the list of paths of the ``CMAKE_PROJECT_PATH``
environment variable. For example, the
``$User_area/Alpha/CMakeLists.txt`` file that contains:

::

   elements_project(Alpha 4.1 USE Beta 1.0 Gamma 2.1)

The lookup will first try:

-  ``$User_area/Beta/1.0/CMakeLists.txt``
-  or ``$User_area/Beta/CMakeLists.txt`` with the 1.0 version (ie
   ``elements_project(Beta 1.0 ...)``)

This will then be repeated in the ``/opt/euclid`` directory if nothing
has been found yet. The second project (Gamma 2.1) is then searched for.
The search is recursive and the parent projects of the newly found
projects will be search in turn.

The keywords that can be used as versions are voluntarily restricted. In
order to avoid to mismatch source subdirectories for versions, the only
allowed version are in the form *X.Y[.Z]* with each of the letter being
replaced by an integer. The only exception is the keyword *HEAD* that
can be used for moving versions. This typically happens in the
continuous build systems, where the direct checkout of branches is used
in the build. Without these rather stringent restrictions on the version
naming scheming the system will become rather difficult to manage.

Build Configuration Options
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Elements framework comes with a set of options that can be passed at
configure time to the CMake command. They can be passed on the command
line itself if one is using directly the call to CMake. But they can
also be passed by changing the ``CMAKEFLAGS`` value if one is using the
top level Makefile wrapper. This Makefile is calling CMake for
configuration with the “``make configure``” command. For example, the
following (bash) shell command:

::

   export CMAKEFLAGS="${CMAKEFLAGS} -DOPT_DEBUG=OFF"

| 
| removes the default optimization used for the debug build. It removes
  the ``-Og`` option from the ``g++`` command line in order to ease the
  debugging. All of these options starts with a ``-D`` and are either
  set to ``ON`` or ``OFF``. Several options, separated by a space can be
  used in the ``CMAKEFLAGS`` environment variable.

The list of common switches are

-  **``OPT_DEBUG``** (default value: ``ON``): option to activate the
   ``-Og`` for the debug build. In order to perform a detailed debugging
   (with gdb for example), one should set the value of this option to
   ``OFF``. Please note that the code is quite debuggable, even with
   that option set to ``ON``.
-  **``ELEMENTS_CPP11``** (default value: ``ON``): this switch activates
   the C++11 features of the compiler. It adds ``-std=c++11`` to the
   ``g++`` compiler and ``-std=c11`` to the ``gcc`` compiler.
-  **``ELEMENTS_HIDE_SYMBOLS``** (default value: ``OFF``): this switch
   activates the explicit symbol hiding. It makes the created libraries
   only expose their public interface and hide all other internal
   symbols. If it is not active, the Linux system exposes all symbols by
   default.
-  **``ELEMENTS_PARALLEL``** (default value: ``OFF``): This activates
   the OpenMP parallel support of the compiler.
-  **``ELEMENTS_FORTIFY``** (default value: ``ON``): This enables the
   ``FORTIFY`` standard C library option that caries out further checks
   on the code.

Documentation Configuration Options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There are a few flags that control the generation of the automatic
documentation of an Elements-based project. They change the behavior of
the “``make doc``” command:

-  **``USE_SPHINX``** (default value: ``ON``): Activate or deactivate
   the whole sphinx documentation generation.
-  **``USE_DOXYGEN``** (default value: ``ON``): Activate or deactivate
   the whole doxygen generation. Please note that the sphinx breathe
   plugin that allows to import the Doxygen XML output into sphinx is
   also disabled.
-  **``USE_SPHINX_APIDOC``** (default value: ``ON``): This switch remove
   from sphinx the generation of the API documentation both native (for
   python) and for the breathe plugin (for C). What remains is the copy
   of the static \*.rst files into the documentation tree.

In principles, these flags can also be passed to the CMake commands with
a ``-D`` option in the ``CMAKEFLAGS`` environment variable. But since
they depend on how the project is exactly documented, a better solution
is to define the value of these options in the main ``CMakeLists.txt``
file of the project. For example, in the Elements project itself the
documentation is purely done with Doxygen. But we still need the top
Sphinx entry for the general Euclid documentation site. We then only
disable the Sphinx API documentation generation:

::

   <code>set(USE_SPHINX_APIDOC OFF
       CACHE STRING "Generates the sphinx API documentation"
       FORCE)
   </code>

And this has to be placed before the call to the ``elements_project``
function. Please have a look at `the main ``CMakeLists.txt`` file of
Elements <http://euclid.esac.esa.int/svn/EC/SGS/SDC/CH/Projects/Elements/trunk/CMakeLists.txt>`__
for an explicit example.

The full list of CMake build options is available
[[GlobalSwitches|here]]

Elements provide default configurations for both doxygen and sphinx but
it is also possible to provide a custom configuration locally which
superseded the standard one:

-  For doxygen: put the custom configuration in
   ``$User_area/MyProject/cmake/doc/Doxyfile.in``
-  For sphinx: put the custom configuration in
   ``$User_area/MyProject/cmake/doc/Sphinx_conf.py.in``

Build Instructions
~~~~~~~~~~~~~~~~~~

The Build Environment
^^^^^^^^^^^^^^^^^^^^^

Before being able to build an Elements-based project, the right build
environment has to be provided. This environment consists of 3
environment variables: ``BINARY_TAG``, ``CMAKE_PROJECT_PATH`` and
``CMAKE_PREFIX_PATH``. The first one describes the target to build on,
the second one defines the locations for the dependents projects lookup
and finally, the last one provides the PATH to the CMake bootstrap
library.

While these variables could easily be defined by hand, the separate
EuclidEnv python package provides an handy way to setup the base
environment. It does many things

-  it guesses the platform and defines the ``BINARY_TAG``,
-  it contains the boostrap CMake library and defines the
   ``CMAKE_PREFIX_PATH`` that points to it,
-  and finally it provides a default value for the
   ``CMAKE_PROJECT_PATH``.

Some information about EuclidEnv can be found at [[EuclidEnv|this page]]

It the EuclidEnv package has been preinstalled on your system, you
should see that banner

::

   ********************************************************************************
   *                          ---- Euclid Login 2.0 ----                          *
   *       Building with gcc48 on co7 x86_64 system (x86_64-co7-gcc48-o2g)        *
   ********************************************************************************
    --- User_area is set to /home/isdc/degauden/Work/Projects
    --- EUCLIDPROJECTPATH is set to:
       /opt/euclid
   --------------------------------------------------------------------------------

| 
| at login time. This means that everything has been setup for you.

If it is not the case and that you have installed the EuclidEnv package
by hand, you might have to call (for bash):

::

   [hubert@degauden:~] . ELogin.sh

| 
| or (for csh)

::

   [hubert@degauden:~] source `which ELogin.csh`

| 
| After that the 3 variables should be defined.

The Build Commands
^^^^^^^^^^^^^^^^^^

In order to build a custom project, say ``MyProject``, it first has to
be located in one of the ``CMAKE_PROJECT_PATH`` directory list. This is
not important for itself, but it is crucial if another project depends
on that very guy. By convention, it is recommended to create the user
project in the ``$User_area`` location which happens to also be the
first entry in the ``CMAKE_PROJECT_PATH`` environment variable.

The MyProject must have its top CMakeLists.txt file and a top wrapper
Makefile (which is identical in all the projects). And then

::

   [hubert@degauden] cd $User_area/MyProject # or "cd $User_area/MyProject/1.0" if it has a version directory
   [hubert@degauden] make purge
   [hubert@degauden] make configure
   [hubert@degauden] make -j 4 -l 6 # parallel build
   [hubert@degauden] make test # run the tests
   [hubert@degauden] make install # creates the InstallArea, visible from the dependent projects

To summarize, here are the list of the targets of the top wrapper
Makefile:

+-----------+------------------------------------------------------+
| Target    | Action                                               |
+===========+======================================================+
| configure | Run CMake to generate the build directory            |
+-----------+------------------------------------------------------+
| all       | build everything (implies configure). This is the    |
|           | default target                                       |
+-----------+------------------------------------------------------+
| test      | run the tests, note that it does not imply the build |
|           | and does not require installation                    |
+-----------+------------------------------------------------------+
| tests     | same as above but implies all                        |
+-----------+------------------------------------------------------+
| install   | populate the InstallArea directory                   |
+-----------+------------------------------------------------------+
| clean     | clean-up of the built objects                        |
+-----------+------------------------------------------------------+
| purge     | total removal of the built directory                 |
+-----------+------------------------------------------------------+
| doc       | generation of the documentation                      |
+-----------+------------------------------------------------------+

Please note that when running “``make test``”, the actual command that
is run is ``ctest``, one of the companion tool of the CMake build suite.
It is also worth noting that some extra options can be passed to the
underlying executable through the make call with the ``ARGS`` parameter:

::

   make ARGS="-R ElementsKernel" test
   make ARGS="-L PyTest" test

| 
| Here the ``-R`` option is using a regular expression to filter the
  tests according to their name and the ``-L`` option is using a regular
  expression to filter the tests according to one of their label. One
  can find more informations about these about by running:

::

   ctest -h

| 
| For example, in order to increase the verbosity of the tests, one can
  run

::

   make ARGS="-VV" test

Parallel Build
^^^^^^^^^^^^^^

While the general build command is using the top wrapper Makefile (and
thus gmake), it then runs the cmake configuration step (aka
“``make configure``”) and then dispatches the build to a sub-builder.
Elements can use 2 different sub-builders: gmake (the default one) and
ninja (also called ninja-build).

The behavior of the parallel build to speed up the software construction
depends on the chosen sub-builder (gmake or ninja-build).

Using the gmake sub-builder
'''''''''''''''''''''''''''

For this sub-builder, there is no special setting to be done in order to
activate it. This is the default one.

#. two options are especially usefull for the the parallel build:

   -  ``-j N``: for the maximal number of parallel threads ``N``
   -  ``-l N``: for the max load ``N`` above which a new thread cannot
      be spawn. If the system has a higher load the build hangs until
      better conditions are met. This is quite useful on a desktop
      machine.

#. These are options can be passed to gmake in 2 ways:

   -  on the command line like ``make -j 4 -l 6``
   -  or with an environment variable:
      ``export MAKEFLAGS="-j 4 -l 6"; make``

For both of these possibilities, gmake sets up internally the MAKEFLAGS
variable and passes to its subcalls.

That’s the reason why, when using the gmake sub-builder, the parallel
options are passed directly to the other calls to gmake. Please note
that the gmake system has implemented *an internal jobserver that
controls the overall number of threads* that is created regardless of
the number of simutaneous calls that is made to gmake.

Using the ninja-build sub-builder
'''''''''''''''''''''''''''''''''

In order to enable this sub-builder, the following environment has to be
set:

::

   export USE_NINJA=1

Please note that any value will enable the ninja sub-builder. Even 0. To
re-instate gmake as the sub-builder, the ``USE_NINJA`` environment
variable has to be unset.

While the ninja sub-builder does accept the ``-j`` and ``-l`` options,
the MAKEFLAGS variable doesn’t mean anything for it. Moreover, gmake
when used in the Elements top wrapper Makefile strips of the ``-j`` and
``-l`` options from the internal MAKEFLAGS variable in order to use it
with its jobserver feature. It makes it unusable to extract the options
that would be needed to pass to the ninja executable.

The workable solution was to create a specific NINJAFLAGS environment
variable that would be read by the top level Makefile. This is what has
been implemented a long time ago and thus to build in parallel, the
command looks like:

::

   export NINJAFLAGS="-j 4 -l 6"
   make

This also means that passing directly the ``-j`` and ``-l`` option to
the top make call won’t work either. The NINJAFLAGS environment variable
has to be set and exported.

The other difference of the ninja-build tool with regards to gmake, is
that it doesn’t implement a global job server feature to control the
overall number of spawn build commands. And the number of threads could
go well above the number specified by the ``-j`` option. This statement
might not be true anymore with latest version of the tool though.

The advantage of the ninja-build tool lies in the quicker dependency
calculation that it provides, compared to gmake. It improves the build
speed quite nicely.

Run Instructions
~~~~~~~~~~~~~~~~

There are actually 2 ways for running the built software of an
Elements-based project. One from the built directory, this is the
preferred way for the developer and from the ``InstallArea``. The later
way involves an external command provided by the EuclidEnv project:
``E-Run``. The former is using a provided ``run`` script from the build
directory.

Run from the build directory
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If we have, say, a built executable in the current project (i.e.
``build.${BINARY_TAG}/bin/MyExecutable``), it can be run be the local
``build.${BINARY_TAG}/run`` generated script:

::

   [hubert@degauden] cd $User_area/MyProject # or "cd $User_area/MyProject/1.0" if it has a version directory
   [hubert@degauden] ./build.${BINARY_TAG}/run MyExecutable 

| 
| The ``run`` script wrapper will provide the needed environment for the
  MyExecutable execution (``PATH``, ``LD_LIBRARY_PATH``, ``PYTHONPATH``,
  ``ELEMENTS_CONF_PATH``, ``ELEMENTS_AUX_PATH``)

Global Run for the Installed Software
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The global ``E-Run`` command should be available if the build-time
environment has been setup. The is done typically by the ``ELogin``
procedure.

::

   ********************************************************************************
   *                          ---- Euclid Login 2.0 ----                          *
   *       Building with gcc48 on co7 x86_64 system (x86_64-co7-gcc48-o2g)        *
   ********************************************************************************
    --- User_area is set to /home/isdc/degauden/Work/Space/Euclid
    --- EUCLIDPROJECTPATH is set to:
       /opt/euclid
   --------------------------------------------------------------------------------
   [degauden@piecld00] which E-Run
   /usr/bin/E-Run

| 
| The ``E-Run`` command will only use the contents of the chained
  InstallArea of the involved project. Thus, the “``make install``” has
  to have been run and the ``build.${BINARY_TAG}`` directories will be
  ignored. A typical call would look like:

::

   [hubert@degauden] E-Run MyProject 1.0 MyExecutable

| 
| or

::

   [hubert@degauden] E-Run MyProject MyExecutable

The ``E-Run`` command will look for the selected project (MyProject),
setup the environment (``PATH``, ``LD_LIBRARY_PATH``, ``PYTHONPATH``,
``ELEMENTS_AUX_PATH``, ``ELEMENTS_CONF_PATH``) and run the command
(``MyExecutable``) found in the ``PATH`` variable.

Some importants remarks:

-  The lookup of projects is done incrementally in the
   ``CMAKE_PROJECT_PATH`` entries
-  The version of the project (1.0) is optional.

   -  if it is present, a perfect match has to be found with the right
      version. It can be explicit (with a version directory) or implicit
      by looking inside the definition of the project in the
      ``CMakeLists.txt`` file.
   -  If there is no version provided, the first project (without
      version directory) with the right name will be used. If only
      projects with version directories can be found, the highest
      version is always selected.

-  It is important to **call ``E-Run`` with the right ``BINARY_TAG``**.
   If that environment variable is not set to the same value used for
   the building (and installing), no executable will be found.
-  One can always check which executable is found be calling something
   like:
   ::

      [hubert@degauden] E-Run MyProject 1.0 which MyExecutable

   This should return the full path to the selected executable.

Alternative Instructions
~~~~~~~~~~~~~~~~~~~~~~~~

As mentioned above, the top wrapper Makefile is simplifying the calls
for the building. The underlying instructions can however also be called
directly. They are a bit more complicated but rather instructive:

::

   [hubert@degauden] cd $User_area/MyProject
   [hubert@degauden] mkdir build.${BINARY_TAG}
   [hubert@degauden] cd build.${BINARY_TAG}
   [hubert@degauden] cmake -DCMAKE_TOOLCHAIN_FILE=/usr/share/EuclidEnv/cmake/ElementsToolChain.cmake -DUSE_LOCAL_INSTALLAREA=ON ../
   [hubert@degauden] cmake --build .
   [hubert@degauden] cmake --build . --target test
   [hubert@degauden] cmake --build . --target install

Extending the Build
~~~~~~~~~~~~~~~~~~~

Starting from the 5.12 version, the top wrapper Makefile of Elements
(through its make library) allows to extend the build process with
custom make targets.

The Elements.mk library will automatically look for a makefile fragment
file pointed by the CUSTOM_MAKEFILE environment variable. This fragment
will be appended to the general top wrapper Makefile.

Installing
----------

By default, if the top wrapper Makefile is used, the ``make install``
command is creating an installation directory (``InstallArea``) at the
top of the project source tree. While this is fine for the development
phase, **another approach has to be taken for the global installation on
a system for production**. This is also the technique used to create RPM
distribution kits.

The set of command that calls directly the CMake system looks like:

::

    [hubert@degauden] cd $User_area/MyProject
    [hubert@degauden] mkdir build.${BINARY_TAG}
    [hubert@degauden] cd build.${BINARY_TAG}
    [hubert@degauden] cmake -DCMAKE_TOOLCHAIN_FILE=/usr/share/EuclidEnv/cmake/ElementsToolChain.cmake ../
    [hubert@degauden] cmake --build .
    [hubert@degauden] cmake --build . --target test
    [hubert@degauden] cmake --build . --target install

Here one can notice that the ``-DUSE_LOCAL_INSTALLAREA=ON`` option has
disappeared. The installation is then performed by using the
``EUCLID_BASE`` environment variable value as the base directory. If
that variable doesn’t exist, the ``/opt/euclid`` location is used as a
fallback.

The build time settings would look like

+--------------------------------------------------------+------------+
| Path Entry                                             | G Settings |
+========================================================+============+
| ``-L${EUCLID_BAS                                       | linker     |
| E}/<project>/<version>/InstallArea/${BINARY_TAG}/lib`` |            |
+--------------------------------------------------------+------------+
| ``-I${EUCLID_BASE}/<                                   | compiler   |
| project>/<version>/InstallArea/${BINARY_TAG}/include`` |            |
+--------------------------------------------------------+------------+

And the run time settings:

+------------------------+--------------------------------------------+
| Variable               | location                                   |
+========================+============================================+
| ``PATH``               | ``${EUCLID_                                |
|                        | BASE}/<project>/<version>/InstallArea/${BI |
|                        | NARY_TAG}/bin:${EUCLID_BASE}/<project>/<ve |
|                        | rsion>/InstallArea/${BINARY_TAG}/scripts`` |
+------------------------+--------------------------------------------+
| ``LD_LIBRARY_PATH``    | ``${EUCLID_BASE}/<project>                 |
|                        | /<version>/InstallArea/${BINARY_TAG}/lib`` |
+------------------------+--------------------------------------------+
| ``PYTHONPATH``         | ``${EUCLID_BASE}/<project>/<v              |
|                        | ersion>/InstallArea/${BINARY_TAG}/python`` |
+------------------------+--------------------------------------------+
| ``ELEMENTS_CONF_PATH`` | ``${EUCLID_BASE}/<project>/                |
|                        | <version>/InstallArea/${BINARY_TAG}/conf`` |
+------------------------+--------------------------------------------+
| ``ELEMENTS_AUX_PATH``  | ``${EUCLID_BASE}/<project>/<v              |
|                        | ersion>/InstallArea/${BINARY_TAG}/auxdir`` |
+------------------------+--------------------------------------------+

Please note that both CMake and ERun use the same variable to locate the
build time and run time parameter: **the ``CMAKE_PROJECT_PATH``
environment variable**. That variable must contains the top deployment
directory (``${EUCLID_BASE}``) for

-  CMake to locate the ``-I`` and ``-L`` for the build of a dependent
   project
-  and for ERun to setup the run time environment of the project.

The key point of the system being that all these settings are chained
using the project dependency tree. Both CMake and E-Run will produce the
settings for the whole dependency list of a given project.

Naming Conventions
------------------

For the system to work well, meaning that we have a nice incremental
lookup of resources from the top projects down to the base ones, a
[[codeen-users:Naming_Rules|naming scheme]] that prevent clashes has to
be adopted. The base of this naming scheme is uniqueness of the name of
the project modules:

**Each module of each project has to be unique throughout the whole
Euclid software.**

The module name will then be used to make other resources (header files,
python packages, configuration files, auxiliary files) unique. This will
allow us to be able to structure a large amount of code.

Please refer to the **Euclid Naming Rules section:
[[codeen-users:Naming_Rules]]**

Binary Objects
~~~~~~~~~~~~~~

For binary built objects (executables and libraries), their names have
to be unique.

In details, the E-Run command is finding the executable in the PATH that
look like:

::

   ${EUCLID_BASE}/<project2>/<version2>/InstallArea/${BINARY_TAG}/bin:${EUCLID_BASE}/<project1>/<version1>/InstallArea/${BINARY_TAG}/bin

| 
| In this example, we are looking for executable in the ``<project2>``
  environment setup. Since that project depends on the project, a second
  component is added to the ``PATH``. If we don’t want to shadow the
  executables defined in the , we have to carefully choose the name of
  the ``<project2>`` project. This is also true for the python
  executable scripts.

The same is true for binary libraries. Except that the variable involved
is the LD_LIBRARY_PATH that looks like

::

   ${EUCLID_BASE}/<project2>/<version2>/InstallArea/${BINARY_TAG}/lib:${EUCLID_BASE}/<project1>/<version1>/InstallArea/${BINARY_TAG}/lib

For the same reason, to avoid shadowing the resources of the base
projects, the names of the libraries have to be unique.

It is worth noting that, this shadowing could also happen at build time
for libraries. Effectively the link instructions look like:

::

   -L${EUCLID_BASE}/<project2>/<version2>/InstallArea/${BINARY_TAG}/lib -L${EUCLID_BASE}/<project1>/<version1>/InstallArea/${BINARY_TAG}/lib

Header Files
~~~~~~~~~~~~

Also at build time, one wants also to be able to include unique header
files — using a directory to create a kind of namespace for the C
preprocessor:

::

   <code class="cpp">
   #include "MyModule/ThatHeader.h"
   </code>

| 
| Here the ``MyModule`` is the namespace. Since the build include
  options look like:

::

   -I${EUCLID_BASE}/<project2>/<version2>/InstallArea/${BINARY_TAG}/include -I${EUCLID_BASE}/<project1>/<version1>/InstallArea/${BINARY_TAG}/include

| 
| it is recommended to create another ``MyModule`` directory in the
  project source tree

::

   <project>
      ├── CMakeLists.txt
      ├── MyModule
      │   ├── CMakeLists.txt
      │   ├── MyModule
      │   │   └── ThatHeader.h
      │   └── src
      └── Makefile

Because the whole ``<project>/MyModule/MyModule`` directory will copied
to
``${EUCLID_BASE}/<project>/<version>/InstallArea/${BINARY_TAG}/include``
when running the ``make install`` command.

Python Files
~~~~~~~~~~~~

We can also use the same facility of the unique project module name in
order to make the python modules unique for the whole Euclid software.
If we want to write a python import like

::

   <code class="Python">
   from MyModule.ThatFeature import ThatFunc
   </code>

and we know that the ``PYTHONPATH`` will look like:

::

   ${EUCLID_BASE}/<project2>/<version2>/InstallArea/${BINARY_TAG}/python:${EUCLID_BASE}/<project1>/<version1>/InstallArea/${BINARY_TAG}/python

, it is recommended to create a MyModule subdirectory in the python
directory:

::

   <project>
      ├── CMakeLists.txt
      ├── MyModule
      │   ├── CMakeLists.txt
      │   ├── python
      │   │   └── MyModule
      │   │       ├── __init__.py
      │   │       └── ThatFeature.py
      │   └── scripts
      └── Makefile

Because the *content* of the ``<project>/MyModule/python`` directory
will copied to
``${EUCLID_BASE}/<project>/<version>/InstallArea/${BINARY_TAG}/python``
when running the ``make install`` command.

Please note that **this namespacing feature doesn’t exist for the moment
(as of version 4.0 of Elements) for the generated python modules
(through SWIG)**. If we have the following settings in a module
CMakeLists.txt file:

::

   elements_add_swig_binding(SwigExample ElementsExamples/SwigExample.i
                             LINK_LIBRARIES ElementsExamples
                             INCLUDE_DIRS ElementsExamples
                             PUBLIC_HEADERS ElementsExamples)

, the ``SwigExample.py`` will be generated in the local
``build.${BINARY_TAG}/python`` directory and then copied to
``InstallArea/${BINARY_TAG}/python`` when running “make install”. And
thus, it has to be used like:

::

   <code class="Python">
   from SwigExample import bla
   </code>

That’s the reason why the name of the SWIG module has to be carefully
chosen in order to be unique.

Configuration and Auxiliary Files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We can also apply the same procedure for the pure custom Elements path
``ELEMENTS_CONF_PATH`` and ``ELEMENTS_AUX_PATH``. The whole point being
(as for the other type of files): avoiding clashes between modules when
installing and avoiding shadowing the resources of base projects.

For the auxiliary path the ``ELEMENTS_AUX_PATH`` at run time will look
like:

::

   ${EUCLID_BASE}/<project2>/<version2>/InstallArea/${BINARY_TAG}/auxdir:${EUCLID_BASE}/<project1>/<version1>/InstallArea/${BINARY_TAG}/auxdir

and it is recommended to create a MyModule subdirectory in the auxdir
directory:

::

   <project>
      ├── CMakeLists.txt
      ├── MyModule
      │   ├── CMakeLists.txt
      │   └── auxdir
      │       └── MyModule
      │           └── my_file.txt
      └── Makefile

Because the *content* of the ``<project>/MyModule/auxdir`` directory
will copied to
``${EUCLID_BASE}/<project>/<version>/InstallArea/${BINARY_TAG}/auxdir``
when running the ``make install`` command. Please note that the name of
the subdirectory is *auxdir* (and not ``aux``) because of the limitation
of some filesystems.

It is almost the same procedure for the configuration files. At run time
the configuration files are located through the ``ELEMENTS_CONF_PATH``
environment variable that looks like:

::

   ${EUCLID_BASE}/<project2>/<version2>/InstallArea/${BINARY_TAG}/conf:${EUCLID_BASE}/<project1>/<version1>/InstallArea/${BINARY_TAG}/conf

and it is recommended to create a MyModule subdirectory in the conf
directory:

::

   <project>
      ├── CMakeLists.txt
      ├── MyModule
      │   ├── CMakeLists.txt
      │   └── conf
      │       └── MyModule
      │           └── my_prog.conf
      └── Makefile

Because the *content* of the ``<project>/MyModule/conf`` directory will
copied to
``${EUCLID_BASE}/<project>/<version>/InstallArea/${BINARY_TAG}/conf``
when running the ``make install`` command.

For the configuration files, there is however an alternative. The
configuration files are bound to an executable. And since the executable
names are unique (by constraints of the ``PATH`` settings), we can also
have the possible tree:

::

   <project>
      ├── CMakeLists.txt
      ├── MyModule
      │   ├── CMakeLists.txt
      │   └── conf
      │       └── MyExecutable_main.conf
      └── Makefile

. The uniqueness of the configuration file is ensured by the one of the
``MyExecutable`` executable.

The Euclid Naming Site
~~~~~~~~~~~~~~~~~~~~~~

A site is provide to check the name of the existing projects, modules,
executables and libraries:

https://pieclddj00/euclidnaming/NameCheck/

This site is not accessible for updates for the moment. But the goal is
to make it available to the Euclid developers

Helper Scripts - Creating New Source Files
------------------------------------------

| In Elements we made several python scripts in order to ease the
  developer experience.
| The scripts available are the following:

::

   1- CreateElementsProject 
   2- AddElementsModule
   3- AddCppClass
   4- AddCppProgram 
   5- AddPythonModule
   6- AddPythonProgram
   7- RemoveCppClass, RemoveCppProgram
   8- RemovePythonProgram, RemovePythonModule

Use the < Script_name —help (or -h) > option to know more how to use
them.

**Create a Elements Project**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

| Let’s make the assumption you have installed the Elements version
  **3.9**.
| For creating an Element Project use the following command line in a
  shell terminal:

::

    > ERun Elements 3.9 CreateElementsProject <Project_Name> <Project_Version>
    where
    <Project_Name> : your project name
    <Project_Version> : your project version

The project is created either at your **home** directory or at the
directory defined by the **$User_area** environment variable

Option also available:

::

     --dependency(or -d) project_name version --> Dependency project name and its version number" e.g "-d Elements x.x.x"
     --no-version-directory(or -novd) -->  Does not create the <project-version> directory, it just creates the <project-name> directory.

*Project Example* :

::

   > ERun Elements 3.9 CreateElementsProject TestProject 1.0
   The directory structure should look like as follows :

   .
   └── TestProject
       └── 1.0
           ├── CMakeLists.txt
           └── Makefile

   or
   > ERun Elements 3.9 CreateElementsProject TestProject 1.0 -novd
   The directory structure should look like as follows :

   TestProject
   ├── CMakeLists.txt
   └── Makefile

**Add an Elements Module**
~~~~~~~~~~~~~~~~~~~~~~~~~~

| For creating a module you must be inside an Elements project. So move
  to your project first.
| Use the following command for creating your module:

::

   > ERun Elements 3.9 AddElementsModule <Module_Name>
   where
   <Module_Name> : your module name

The module is created where you are.

Options available:

::

     --module-dependency (or -md) module_name --> Dependency module name e.g "-md ElementsKernel"

*Module Example*:

::

   Following the Project example we move to the project.
   > cd TestProject/1.0
   > ERun Elements 3.9 AddElementsModule TestModule
   The directory structure should look like as follows:
   .
   └── TestProject
       └── 1.0
           ├── CMakeLists.txt
           ├── Makefile
           └── TestModule
               ├── CMakeLists.txt
               ├── TestModule
               ├── conf
               ├── doc
               └── tests
                   └── src

**Add a C Class**
~~~~~~~~~~~~~~~~~

| For creating a C class you must be inside an Elements module.
| For adding a C class use the following command:

::

   > ERun Elements 3.9 AddCppClass <Class_Name>
   where 
   <Class_Name> : your class name

Options available:

::

     --elements-dependency(or -ed) module_name --> Dependency module name e.g. "-ed ElementsKernel"
     --external-dependency(or -extd) library_name --> External dependency library name e.g. "-extd ElementsKernel"

*Class Example*:

::

   Following our module example we move to this module directory.
   > cd TestProject/1.0/TestModule/
   > ERun Elements 3.9 AddCppClass TestClass
   The directory structure should look like as follows:
   .
   └── TestProject
       └── 1.0
           ├── CMakeLists.txt
           ├── Makefile
           └── TestModule
               ├── CMakeLists.txt
               ├── TestModule
               │   └── TestClass.h
               ├── conf
               ├── doc
               ├── src
               │   └── lib
               │       └── TestClass.cpp
               └── tests
                   └── src
                       └── TestClass_test.cpp

**Add a C Program**
~~~~~~~~~~~~~~~~~~~

| For creating a C program you must be inside an Elements module.
| For adding a C program use the following command:

::

   > ERun Elements 3.9 AddCppProgram <Program_Name>
   where 
   <Program_Name> : your program name

Options available:

::

     --module-dependency(or -md) module_name --> Dependency module name e.g."-md ElementsKernel"
     --library-dependency(or -ld) library_name --> Dependency library name e.g."-ld ElementsKernel"

*Program Example*:

::

   Following our module example we move to this module directory.
   > cd TestProject/1.0/TestModule/
   > ERun Elements 3.9 AddCppProgram TestProgram
   The directory structure should look like as follows:
   .
   └── TestProject
       └── 1.0
           ├── CMakeLists.txt
           ├── Makefile
           └── TestModule
               ├── CMakeLists.txt
               ├── TestModule
               │   └── TestClass.h
               ├── conf
               │   └── TestProgram.conf
               ├── doc
               ├── src
               │   ├── lib
               │   │   └── TestClass.cpp
               │   └── program
               │       └── TestProgram.cpp
               └── tests
                   └── src
                       └── TestClass_test.cpp

**Add a Python Module**
~~~~~~~~~~~~~~~~~~~~~~~

| For creating a python module you must be inside an Elements module.
| For adding a python module use the following command:

::

   > ERun Elements 3.9 AddPythonModule <Py_Module_Name>
   where 
   <Py_Module_Name> : your python module name

*Python Module Example*:

::

   Following our module example we move to this module directory.
   > cd TestProject/1.0/TestModule/
   > ERun Elements 3.9 AddPythonModule TestPythonModule
   The directory structure should look like as follows:
   .
   └── TestProject
       └── 1.0
           ├── CMakeLists.txt
           ├── Makefile
           └── TestModule
               ├── CMakeLists.txt
               ├── TestModule
               │   └── TestClass.h
               ├── conf
               │   └── TestProgram.conf
               ├── doc
               ├── python
               │   └── TestModule
               │       ├── TestPythonModule.py
               │       └── __init__.py
               ├── src
               │   ├── lib
               │   │   └── TestClass.cpp
               │   └── program
               │       └── TestProgram.cpp
               └── tests
                   ├── python
                   │   └── TestPythonModule_test.py
                   └── src
                       └── TestClass_test.cpp

**Add a Python Program**
~~~~~~~~~~~~~~~~~~~~~~~~

| For creating a python program you must be inside an Elements module.
| For adding a python program use the following command:

::

   > ERun Elements 3.9 AddPythonProgram <Py_Program_Name>
   where 
   <Py_Program_Name> : your python program name

*Python Program Example*:

::

   Following our module example we move to this module directory.
   > cd TestProject/1.0/TestModule/
   > ERun Elements 3.9 AddPythonProgram TestPythonProgram
   The directory structure should look like as follows:
   .
   └── TestProject
       └── 1.0
           ├── CMakeLists.txt
           ├── Makefile
           └── TestModule
               ├── CMakeLists.txt
               ├── TestModule
               │   └── TestClass.h
               ├── conf
               │   ├── TestProgram.conf
               │   └── TestPythonProgram.conf
               ├── doc
               ├── python
               │   └── TestModule
               │       ├── TestPythonModule.py
               │       ├── TestPythonProgram.py
               │       └── __init__.py
               ├── scripts
               ├── src
               │   ├── lib
               │   │   └── TestClass.cpp
               │   └── program
               │       └── TestProgram.cpp
               └── tests
                   ├── python
                   │   └── TestPythonModule_test.py
                   └── src
                       └── TestClass_test.cpp

**Add a Script** (since version 5.0)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

| For creating a plain script you must be inside an Elements module.
| For adding a script, use the following command:

::

   > ERun Elements 5.0 AddScript <Script_Name>
   where 
   <Script_Name> : your script name

*Script Example*:

::

   Following our module example we move to this module directory.
   > cd TestProject/1.0/TestModule/
   > ERun Elements 5.0 AddScript TestScript
   The directory structure should look like as follows:
   .
   └── TestProject
       └── 1.0
           ├── CMakeLists.txt
           ├── Makefile
           └── TestModule
               ├── CMakeLists.txt
               ├── TestModule
               │   └── TestClass.h
               ├── conf
               │   ├── TestProgram.conf
               │   └── TestPythonProgram.conf
               ├── doc
               ├── python
               │   └── TestModule
               │       ├── TestPythonModule.py
               │       ├── TestPythonProgram.py
               │       └── __init__.py
               ├── scripts
               │   └── TestScript
               ├── src
               │   ├── lib
               │   │   └── TestClass.cpp
               │   └── program
               │       └── TestProgram.cpp
               └── tests
                   ├── python
                   │   └── TestPythonModule_test.py
                   └── src
                       └── TestClass_test.cpp

It is not recommended to post-fixed script name with an extension (.py,
.sh…) as it can raises some
`issues <https://euclid.roe.ac.uk/issues/12648#note-3>`__ at the
deployment stage. `Shebang
line <https://en.wikipedia.org/wiki/Shebang_%28Unix%29>`__ should be
preferred. It consists in adding interpreter directive at the beginning
of the file which removes the need prefixed scripts with their
interpreter on the command line.

**RemoveCppClass**
~~~~~~~~~~~~~~~~~~

This script allows you to remove all files on disk related to a cpp
class name.

| > *WARNING*: The script can not remove all dependencies related to the
  class in
| > the <CMakeLists.txt> file. You maybe need to edit it and remove all
| > stuff related to this class. Check at least the
  elements_add_library,
| > elements_add_library, find_package and elements_depends_on_subdirs
| > macros

*Remove Cpp class Example*:

::

   Imagine you have the following class named: my_class in a module: my_module
   with the following directory structure:
   .
   ├── CMakeLists.txt
   ├── conf
   ├── doc
   ├── MyModule
   │   └── MyClass.h
   ├── src
   │   └── lib
   │       └── MyClass.cpp
   └── tests
       └── src
           └── MyClass_test.cpp

   Call the script as follows under the <my_module> directory:
   > ERun Elements 3.9 RemoveCppClass MyClass

   And the directory structure should look like this:

   .
   ├── CMakeLists.txt
   ├── CMakeLists.txt~
   ├── conf
   ├── doc
   ├── my_module
   ├── src
   │   └── lib
   └── tests
       └── src

   Please note that the <CMakeLists.txt~> file is a backup of the original file made by the script.

The RemoveCppProgram script acts the same way as the RemoveCppClass
script.

**RemovePythonProgram**
~~~~~~~~~~~~~~~~~~~~~~~

| This script allows you to remove all files on disk related to a python
  program. Usually
| you use this script when you made a typo in the program name when
  calling the
|  script. The <CMakeLists.txt> file is updated accordingly.

*Remove python program Example*:

::

   Imagine you have the following python program named: my_python_program in a module: my_module
   with the following directory structure:

   .
   ├── CMakeLists.txt
   ├── conf
   │   └── MyPythonProgram.conf
   ├── doc
   ├── MyModule
   ├── python
   │   └── MyModule
   │       ├── __init__.py
   │       └── MyPythonProgram.py
   ├── scripts
   └── tests
       └── src

   Call the script as follows under the <my_module> directory:
   > ERun Elements 3.9 RemovePythonProgram MyPythonProgram

   And the directory structure should look like this:

   .
   ├── CMakeLists.txt
   ├── CMakeLists.txt~
   ├── conf
   ├── doc
   ├── MyModule
   ├── python
   │   └── MyModule
   │       └── __init__.py
   ├── scripts
   └── tests
       └── src

The RemovePythonModule script acts the same way.

Build Configuration - Writing ``CMakeLists.txt``
------------------------------------------------

In an Elements-based project, there are 2 different kind of
CMakeLists.txt configuration files. There is the top level
CMakeLists.txt that is unique to the project. It contains general
information about the project.

The second kind of CMakeLists.txt file is locate at the base of each
project module. It goal is to provide the configuration for the build of
the various software components.

Project Level Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The CMake instructions that are absolutely needed for the definition of
a project are

#. The minimal version of the CMake software that is required for the
   build (CMake is adamant about that),
   ::

       CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

#. the line to locate the Elements CMake library
   ::

       find_package(ElementsProject)

#. and the project definition
   ::

       elements_project(MyProject 0.3 USE OtherProject 1.2)

And then the *whole* file would read:

::

   CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)


   #---------------------------------------------------------------
   # Load macros and functions for Elements-based projects
   find_package(ElementsProject)
   #---------------------------------------------------------------

   # Declare project name and version
   elements_project(Alexandria 2.5 USE Elements 4.1)

Module Level Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~

After the reading of the top level CMakeLists.txt file of the project,
the CMake executable will make a recursion and read all the other
CMakeLists.txt files in all the subdirectories. It will then create a
single global configuration for the building of the project.

One of the nice feature of the framework is to be able to create
dependencies between modules. These dependencies will give access, at
build time and run time to the object created in the other libraries.
For example, here is the head lines of the CMakeLists.txt file of a
module:

::

   # Declare MyModule as an Elements module
   elements_subdir(MyModule)

   # Declare Elements module dependencies
   #    This module is using of the MyOtherModule module
   elements_depends_on_subdirs(MyOtherModule)

The dependency relation can also involve several dependency modules:

::

   elements_depends_on_subdirs(MyOtherModule YetAnotherModule)

or

::

   elements_depends_on_subdirs(MyOtherModule)
   elements_depends_on_subdirs(YetAnotherModule)

C Objects - Libraries and Executables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Libraries
'''''''''

The general syntax to add an Elements library to a module CMakeLists.txt
file is :

::

    elements_add_library(<name>
                         source1 source2 ...
                         LINK_LIBRARIES library1 library2 ...
                         INCLUDE_DIRS dir1 module2 ...
                         [LINKER_LANGUAGE C|CXX]
                         [NO_PUBLIC_HEADERS | PUBLIC_HEADERS dir1 dir2 ...])

where

-  the first argument (``<name>``) is the *name of the library*\ (e.g:
   ElementsKernel). This is not the name of the built binary file but
   rather the core name.

-  the next unnamed arguments (``source1 source2 ...``) are the source
   files of the library.

   -  These are relative paths (from the base of the module) to the
      source files. By conventions they are locate in a ``src``
      sub-directory.
   -  Wildcard entries are supported (e.g.: ``src/lib/*.cpp``)
   -  If some private header files are also present, it is recommended
      to add them (e.g.: ``src/lib/*.cpp src/lib/*.h``). CMake will then
      be able to do a better dependency resolution.

-  The named ``LINK_LIBRARIES`` argument contains the name of the
   libraries that will be linked to the created library.

   -  The libraries that can be used are the local ones (from the same
      module) or the ones from the dependency modules (brought by
      ``elements_depends_on_subdirs``).
   -  The pure CMake package names (brought by the ``find_package``
      statement) can also be added to that argument.
   -  The public header location of the link libraries (if they exist)
      are automatically added to the include directory list used for the
      build.

-  The named ``INCLUDE_DIRS`` argument contains the list of extra header
   file directories that are needed for the build.

   -  It can be relative path (to the current module) to local
      directories
   -  It can be names of dependency modules (brought by
      ``elements_depends_on_subdirs``).
   -  The pure CMake package names (brought by the ``find_package``
      statement) can also be added to that argument.
   -  Generally they are not needed for the libraries that are linked.
      Their public header file directories are automatically added.
   -  It is mandatory for the header files-only libraries (or external
      package) which have nothing to link (like Eigen3):
      ::

         find_package(Eigen3)
         elements_add_library(EigenExample src/lib/EigenExample.cpp
                             LINK_LIBRARIES ElementsExamples
                             INCLUDE_DIRS Eigen3)

-  The named ``LINKER_LANGUAGE`` argument specifies the language of the
   source files. This is usually unneeded, except if the library has no
   source file and is only made of public header files. In that case,
   this argument has to be explicitly set to ``C`` or ``CXX``.

-  The ``PUBLIC_HEADERS`` argument lists the relative path of the
   directories containing the public interface of the libraries.

-  If the built library has no public header files (which can happen for
   a plugin system for example), the ``NO_PUBLIC_HEADERS`` argument has
   to be explicitly present to show that this is intended.

Note: The Elements building framework is doing a full resolution of the
first level of symbol used in a library. That’s the reason why the
-Wl,—no-undefined option is used for the library creation. It forces
that the dependency libraries are linked onto the created one. Please
note that this process is *not* recursive.

The build system is also discarding libraries that are not needed. If
the -Wl,—as-needed dectects that no symbol is used from a linked
library, it removes it from the list of linked libraries.

Executables
'''''''''''

The general syntax to add an Elements executable to a module
CMakeLists.txt file is close to the one for a library :

::

   elements_add_executable(<name>
                           source1 source2 ...
                           LINK_LIBRARIES library1 library2 ...
                           INCLUDE_DIRS dir1 module2 ...)

| 
| Except for the ``LINKER_LANGUAGE``, ``PUBLIC_HEADERS`` and
  ``NO_PUBLIC_HEADERS`` options that make no sense for an executable.

In details:

-  the first argument (``<name>``) is the *name of the executable*. This
   is also the name of the built binary file.

-  the next unnamed arguments (``source1 source2 ...``) are the source
   files of the executable.

   -  These are relative paths (from the base of the module) to the
      source files. By conventions they are locate in a ``src``
      sub-directory.
   -  Wildcard entries are supported (e.g.: ``src/prog/*.cpp``)
   -  If some private header files are also present, it is recommended
      to add them (e.g.: ``src/prog/*.cpp src/prog/*.h``). CMake will
      then be able to do a better dependency resolution.

-  The named ``LINK_LIBRARIES`` argument contains the name of the
   libraries that will be linked to the created executable.

   -  The libraries that can be used are the local ones (from the same
      module) or the ones from the dependency modules (brought by
      ``elements_depends_on_subdirs``).
   -  The pure CMake package names (brought by the ``find_package``
      statement) can also be added to that argument.
   -  The public header location of the link libraries (if they exist)
      are automatically added to the include directory list used for the
      build.

-  The named ``INCLUDE_DIRS`` argument contains the list of extra header
   file directories that are needed for the build.

   -  It can be relative path (to the current module) to local
      directories
   -  It can be names of dependency modules (brought by
      ``elements_depends_on_subdirs``).
   -  The pure CMake package names (brought by the ``find_package``
      statement) can also be added to that argument.
   -  Generally they are not needed for the libraries that are linked.
      Their public header file directories are automatically added.
   -  It is mandatory for the header files-only libraries (or external
      package) which have nothing to link (like Eigen3):
      ::

         find_package(Eigen3)
         elements_add_executable(EigenExample src/prog/EigenExample.cpp
                                 LINK_LIBRARIES ElementsExamples
                                 INCLUDE_DIRS Eigen3)

Unit Tests
''''''''''

The general syntax to add an Elements CppUnit or Boost unit test to a
module CMakeLists.txt file is :

::

   elements_add_unit_test(<name>
                          source1 source2 ...
                          [EXECUTABLE exename]
                          LINK_LIBRARIES library1 library2 ...
                          INCLUDE_DIRS dir1 package2 ...
                          [WORKING_DIRECTORY dir]
                          [ENVIRONMENT variable[+]=value ...]
                          [TIMEOUT seconds]
                          [TYPE Boost|CppUnit])

| 
| It is the same syntax as the one to create an executable with some
  extra options.

In details:

-  the first argument (``<name>``) is the *name of the test*. This is
   also the name of the built binary file if the ``EXECUTABLE`` argument
   is not passed.

-  the next unnamed arguments (``source1 source2 ...``) are the source
   files of the executable.

   -  These are relative paths (from the base of the module) to the
      source files. By conventions they are locate in a ``tests``
      sub-directory.
   -  Wildcard entries are supported (e.g.: ``tests/src/*.cpp``)
   -  If some private header files are also present, it is recommended
      to add them (e.g.: ``tests/src/*.cpp tests/src/*.h``). CMake will
      then be able to do a better dependency resolution.

-  The ``EXECUTABLE`` argument allows to give a different name from the
   name of the test.

-  The named ``LINK_LIBRARIES`` argument contains the name of the
   libraries that will be linked to the created executable.

   -  The libraries that can be used are the local ones (from the same
      module) or the ones from the dependency modules (brought by
      ``elements_depends_on_subdirs``).
   -  The pure CMake package names (brought by the ``find_package``
      statement) can also be added to that argument.
   -  The public header location of the link libraries (if they exist)
      are automatically added to the include directory list used for the
      build.

-  The named ``INCLUDE_DIRS`` argument contains the list of extra header
   file directories that are needed for the build.

   -  It can be relative path (to the current module) to local
      directories
   -  It can be names of dependency modules (brought by
      ``elements_depends_on_subdirs``).
   -  The pure CMake package names (brought by the ``find_package``
      statement) can also be added to that argument.
   -  Generally they are not needed for the libraries that are linked.
      Their public header file directories are automatically added.

-  The ``WORKKING_DIRECTORY`` argument specifies the location (absolute
   or relative) where the test will be run. It is usually not needed.

-  The ``TIMEOUT`` argument specifies the maximum amount of time in
   seconds that a test can last. Beyond that, it is considered as a
   failure.

-  The ``TYPE`` argument specifies which kind of unit test framework to
   use. By default (without this argument), it is using the Boost
   framework. But the CppUnit framework is also supported.

   -  It is mandatory for the header files-only libraries (or external
      package) which have nothing to link (like Eigen3).

Python Objects
^^^^^^^^^^^^^^

The python files are split into 2 categories: the **executable** ones,
located in the scripts sub-directory of an Elements module and the
**library** ones, located in the python sub-directory. The have been
split since they are installed in different directories. The first ones
are installed in a directory found in an entry of the ``PATH``
environment variable and the later ones are found in an entry of the
``PYTHONPATH`` environment variable.

Python Scripts
''''''''''''''

For the installation and setup in the ``PATH`` of the script files
located in the scripts sub-directory of an Elements module, the
following CMake instruction is needed:

::

   elements_install_scripts()

Python Libraries
''''''''''''''''

For the installation and setup in the ``PYTHONPATH`` of the library
files located in the python sub-directory of and Elements module, the
following CMake instruction is needed:

::

   elements_install_python_modules([TEST_TIMEOUT n])

The hierarchy of directories and files in the python directory will be
installed. If the first level of directories do not contain *init*.py, a
warning is issued and an empty one will be installed.

Some remarks:

-  It is recommended to ensure the uniqueness of the python package by
   prefixing it with the name of the Elements module:
   ``<module>/python/<moduel>``. Please have a look at the naming
   conventions above.
-  Most of the python functionalities should be coded in the python
   libraries. The python scripts, located in a different directory,
   should only be the shallow interfaces to the libraries. This ensure a
   maximal code reuse possibility.
-  The use of this CMake function automatically trigger the creation of
   python unit test instruction if the ``tests/python`` sub-directory
   exists. If it is the case, an extra ``TEST_TIMEOUT`` parameter can be
   passed to the function to set the maximal amount of time of the test
   (in seconds).

SWIG Python Bindings
''''''''''''''''''''

::

    elements_add_swig_binding(<name>
                              [interface] source1 source2 ...
                              LINK_LIBRARIES library1 library2 ...
                              INCLUDE_DIRS dir1 package2 ...
                              [NO_PUBLIC_HEADERS | PUBLIC_HEADERS dir1 dir2 ...])

Create a SWIG binary python module from the specified sources (glob
patterns are allowed), linking it with the libraries specified and
adding the include directories to the search path. The sources can be
either **.i or**.cpp files. Their location is relative to the base of
the Elements package (module).

The arguments and options are close to the ones of a plain compiled
library:

-  the first argument (``<name>``) is the \_name of the SWIG module.
-  the next unnamed arguments (``source1 source2 ...``) are the source
   files of the module. Include the \*.i files that can be either
   private (in the src/lib directory for example) or public (in the
   PUBLIC_HEADERS directory)

   -  These are relative paths (from the base of the module) to the
      source files. By conventions they are locate in a ``src``
      sub-directory.
   -  Wildcard entries are supported (e.g.: ``src/lib/*.cpp``)
   -  If some private header and interface files are also present, it is
      recommended to add them (e.g.:
      ``src/lib/*.cpp src/lib/*.h src/lib/*.i``). CMake will then be
      able to do a better dependency resolution.

-  The named ``LINK_LIBRARIES`` argument contains the name of the
   libraries that will be linked to the created library.

   -  The libraries that can be used are the local ones (from the same
      module) or the ones from the dependency modules (brought by
      ``elements_depends_on_subdirs``).
   -  The pure CMake package names (brought by the ``find_package``
      statement) can also be added to that argument.
   -  The public header location of the link libraries (if they exist)
      are automatically added to the include directory list used for the
      build.

-  The named ``INCLUDE_DIRS`` argument contains the list of extra header
   file directories that are needed for the build.

   -  It can be relative path (to the current module) to local
      directories
   -  It can be names of dependency modules (brought by
      ``elements_depends_on_subdirs``).
   -  The pure CMake package names (brought by the ``find_package``
      statement) can also be added to that argument.
   -  Generally they are not needed for the libraries that are linked.
      Their public header file directories are automatically added.
   -  It is mandatory for the header files-only libraries (or external
      package) which have nothing to link (like Eigen3):
      ::

         find_package(Eigen3)
         elements_add_library(EigenExample src/lib/EigenExample.cpp
                             LINK_LIBRARIES ElementsExamples
                             INCLUDE_DIRS Eigen3)

-  The named ``LINKER_LANGUAGE`` argument specifies the language of the
   source files. This is usually unneeded, except if the library has no
   source file and is only made of public header files. In that case,
   this argument has to be explicitly set to ``C`` or ``CXX``.

-  The ``PUBLIC_HEADERS`` argument lists the relative path of the
   directories containing the public interface of the libraries.

-  If the built library has no public header files (which can happen for
   a plugin system for example), the ``NO_PUBLIC_HEADERS`` argument has
   to be explicitly present to show that this is intended.

-  Since the python module is generated, its name has to be unique
   because it can’t be produced in as sub-directory that serves as a
   namespace. Please note that this could be changed in the future.

Other Utilities
^^^^^^^^^^^^^^^

Generic Test
''''''''''''

In order to add a generic test using a custom executable, the lines to
be added are:

::

    elements_add_test(<name>
                      [COMMAND cmd args ...]
                      [WORKING_DIRECTORY dir]
                      [ENVIRONMENT variable[+]=value ...]
                      [DEPENDS other_test ...]
                      [FAILS] [PASSREGEX regex] [FAILREGEX regex]
                      [LABELS label1 label2]
                      [TIMEOUT seconds])

This declares a test that, by definition, succeeds if the command
succeeds. By default, in its simplest form:

::

   elements_add_test(ScriptThatChecksFile COMMAND ScriptThatChecksFile_test)

, the test succeeds if the command return 0. And it fails, it the
command returns a different value. This function is only a wrap around
the plain CMake add_test function that add the right run time
environment setup produced by the chain of Elements-base projects.

The detailed arguments and options are:

-  The first argument ``<name>`` is the name of the test.
-  the ``COMMAND`` option contains the command line to be run.
-  the ``WORKING_DIRECTORY`` is specifying the location where the test
   will be run.
-  the ENVIRONMENT option allows to update the environment in which the
   test will be run.
-  the FAILS, PASSREGEX, and FAILREGEX are options for the test
   behaviour:

   -  With the FAILS option, the command has to fail for the test to
      succeed. The command has then to return something different from
      0.
   -  With PASSREGEX, the test succeeds if the command standard output
      matches the regular expression.
   -  With FAILREGEX, the test succeeds if the command standard error
      matches the regular expression.

-  LABELS for giving tag names to the test. Very useful for running
   specific tests.
-  TIMEOUT in seconds that fails the test if the time is exceeded.

Default Configuration Files
'''''''''''''''''''''''''''

For the installation and setup in the ``ELEMENTS_CONF_PATH`` of the
library files located in the conf sub-directory of and Elements module,
the following CMake instruction is needed:

::

   elements_install_conf_files()

Auxiliary Software Files
''''''''''''''''''''''''

For the installation and setup in the ``ELEMENTS_AUX_PATH`` of the
library files located in the *auxdir* sub-directory of and Elements
module, the following CMake instruction is needed:

::

   elements_install_aux_files()

Please note that the name of the sub-directory is “auxdir” and not “aux”
because of the limitation of some filesystems.

Other CMake Facilities
~~~~~~~~~~~~~~~~~~~~~~

Adding Support for External Software
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Most of the support for the build of the software using external
libraries is provided by the ``find_package`` CMake function. This
function is looking for a package definition contained in a CMake file
and use it. If one is looking for the ``XYZ`` feature, there is a
corresponding ``FindXYZ.cmake`` file that will contain the compile and
link instructions in order to use that ``XYZ`` feature at build time.

Many of these ``FindXYZ.cmake`` files are directly provided by the CMake
software itself in the ``/usr/share/cmake/Modules`` directory (on
Linux). But it is not uncommon that the very feature that one wants to
use is not directly supported by CMake. In that case, one has to provide
its own CMake support for the feature.

In Elements, in the source:cmake/modules sub-directory, there are quite
a few examples of FindXYZ.cmake files. And the good thing is that one
can add its own files to **the local ``cmake/modules`` directory of the
current project in use**. The Elements framework will automatically add
it to the list of CMake source files.

As an example, lets have a look at the ``FindLog4CPP.cmake`` file
(please mind the casing):

::

   if(NOT LOG4CPP_FOUND)

     FIND_PATH(LOG4CPP_INCLUDE_DIR log4cpp/Category.hh
               HINTS ENV LOG4CPP_INSTALL_DIR
               PATH_SUFFIXES include
             )

     FIND_LIBRARY(LOG4CPP_LIBRARY log4cpp
                  HINTS ENV LOG4CPP_INSTALL_DIR
                  PATH_SUFFIXES lib
               )

     set(LOG4CPP_INCLUDE_DIRS ${LOG4CPP_INCLUDE_DIR})
     set(LOG4CPP_LIBRARIES ${LOG4CPP_LIBRARY})

     INCLUDE(FindPackageHandleStandardArgs)
     FIND_PACKAGE_HANDLE_STANDARD_ARGS(Log4CPP DEFAULT_MSG LOG4CPP_INCLUDE_DIRS LOG4CPP_LIBRARIES)

     mark_as_advanced(LOG4CPP_FOUND LOG4CPP_INCLUDE_DIRS LOG4CPP_LIBRARIES)

   endif()

Writing and Running Tests
-------------------------

The framework allows the developer to write various kind of tests. The
main advantage of the Elements test environment is the possibility to
define the tests in the CMakeLists.txt files and to run them with the
CTest tool. Everything is hidden in the top wrapper Makefile which
allows to simply run the

::

   make test

command.

One can divide the nature of the tests in 3 categories:

-  C tests constructed with the `Boost Test (the
   default) <https://www.boost.org/doc/libs/1_67_0/libs/test/doc/html/index.html>`__
   or `CppUnit utility
   framework <http://cppunit.sourceforge.net/doc/cvs/cppunit_cookbook.html>`__,
-  Python tests run by py.test (the default), nosetests or the python
   internal test runner,
-  and finally the plain tests which are just executables that return
   either 0 (Success) or another number (failure).

Please note that the tests have to be built, obviously, in order to be
able to run them. A ``make all`` or ``make install`` has to be issued
before the ``make test`` command. For the impatient, the command
``make tests`` (mark the final “s”) will chain the ``make all`` and
``make test`` commands.

The information about the run of the tests is located under the
``./build.${BINARY_TAG}/Testing`` directory. Specifically, the last log
can be found at
``./build.${BINARY_TAG}/Testing/Temporary/LastTest_*.log``.

Remarks:

-  There are a few examples of tests in both ElementsKernel and
   ElementsExamples modules that demonstrate the various categories of
   tests.
-  The tests that are described are generic, they could be unit,
   integration, regression, smoke, etc tests.
-  It is clear nevertheless that some languages like C are less suited
   for the high test levels like integration or regression tests that
   could require calls to several executables.

A quite complete list of possible tests is implemented in the
`ElementsExamples <https://gitlab.euclid-sgs.uk/ST-TOOLS/Elements/blob/develop/ElementsExamples/CMakeLists.txt>`__
module

C Tests
~~~~~~~

This is the fundamental type that is used for the C unit testing.
Currently 2 C unit testing framework are supported: `Boost
Test <https://www.boost.org/doc/libs/1_67_0/libs/test/doc/html/index.html>`__
and
`CppUnit <http://cppunit.sourceforge.net/doc/cvs/cppunit_cookbook.html>`__.
Since the *Boost Test framework is the recommended one for Euclid*, we
will concentrate on the particular one.

As for many other test framework, the Boost Unit Test gist is to write
test suite that will contain test cases. Ultimately each test case will
contains one or several commands that will assert the truth of an
expression. Failing these assertions will fail the test.

Boost is providing macros for:

-  starting a suite (``BOOST_AUTO_TEST_SUITE``)
-  ending a suite (``BOOST_AUTO_TEST_SUITE_END``)
-  declaring a case (``BOOST_AUTO_TEST_CASE``)
-  declaring a case with a fixture (``BOOST_FIXTURE_TEST_CASE``)
-  doing assertions (``BOOST_CHECK``, ``BOOST_CHECK_EQUAL``,
   ``BOOST_CHECK_EXCEPTION``, …)

The Elements framework is automatically generating the part of the code
that contain the ``main`` function and create directly an executable
from the cpp file. For example, one can declare the test as

::

   elements_add_unit_test(BoostClassExample tests/src/Boost/ClassExample_test.cpp
                          EXECUTABLE BoostClassExample_test
                          INCLUDE_DIRS ElementsExamples
                          LINK_LIBRARIES ElementsExamples TYPE Boost)

in the CMakeLists.txt file of the module (``elements_add_unit_test`` is
described in more details in
[[NewUserManual#Module-Level-Configuration|that section]]). And then,
only the suite has to be written:

::

   <code class="cpp">
   #include "ElementsExamples/ClassExample.h"

   #include <cstdint>                          // for int64_t
   #include <string>                           // for string

   #include <boost/test/unit_test.hpp>
   #include <boost/test/test_tools.hpp>

   #include "ElementsKernel/Exception.h"


   using std::string;
   using std::int64_t;

   using Elements::Examples::ClassExample;

   constexpr double tolerance = 1e-12;

   struct ClassExampleFixture {

     string static_string { "This is a static field example" };
     int64_t source_id { 123456789 };
     double ra { 266.40506655 };
     double input_variable { 1.273645899 };
     double expected_result { 1.273645899 };

     ClassExample example_class = ClassExample::factoryMethod(source_id, ra);

     ClassExampleFixture() {
     }

     ~ClassExampleFixture() {
     }
   };

   BOOST_AUTO_TEST_SUITE(ClassExampleTestSuite)

   BOOST_AUTO_TEST_CASE(WithoutFixture) {
     BOOST_CHECK(true);
   }

   BOOST_FIXTURE_TEST_CASE(fundamentalTypeMethod_test, ClassExampleFixture) {
     BOOST_CHECK_CLOSE(expected_result,
         example_class.fundamentalTypeMethod(input_variable), tolerance);
   }

   BOOST_FIXTURE_TEST_CASE(Getter_test, ClassExampleFixture) {
     BOOST_CHECK_EQUAL(source_id, example_class.getSourceId());
   }

   BOOST_FIXTURE_TEST_CASE(exception_in_divideNumbers_test, ClassExampleFixture ) {

     BOOST_CHECK_EXCEPTION(example_class.divideNumbers(1.0, 0.0), Elements::Exception,
         [](const Elements::Exception& e){
               string exception_str = e.what();
               return exception_str.find("exception in ClassExample::divideNumbers") != string::npos;
         });
   }

   BOOST_AUTO_TEST_SUITE_END()
   </code>

| 
| Remarks:

-  The test source files are located by convention in the tests/src
   sub-directory, with other sub-levels if necessary.
-  Each of the ``elements_add_unit_test`` call creates one test to be
   run with “make test”.
-  The name of the test is always prefix by the name of the Elements
   module that contains it.
-  When using [[NewUserManual#Add-a-C-Class|the AddCppClass creation
   script]], an empty test cpp source file is automatically added to the
   module structure.
-  Each declaration of ``elements_add_unit_test`` generates an
   executable in the ./build.${BINARY_TAG}/bin sub-directory and it can
   be run from the command line (for the example above) like
   ::

      ./build.${BINARY_TAG}/run BoostClassExample_test

Python Tests
~~~~~~~~~~~~

For the python unit testing the `py.test
framework <https://docs.pytest.org/en/latest/contents.html>`__ is the
recommended one. If the framework is not able to locate that executable,
it will fall back on “nosetests” or even to the “python -m unittest”
internal test runner. If the test are defined while only using the
unittest python module, all 3 runners will be suitable. It will not be
the case if the unit test python files are using explicit calls to the
“pytest” python modules.

For the demonstration, if using only the unittest framework:

::

   <code class="python">
   import unittest
   from ElementsExamples.PythonModuleExample import ClassExample


   class ClassExampleTestCase(unittest.TestCase):

       def setUp(self):
           unittest.TestCase.setUp(self)
           self.tol = 1e-6
           self.first = 23.4857
           self.second = 3.4756
           self.my_list = [6, 7, 8, 9]
           self.expected_result = 6 + 7 + 8 + 9
           self.example_object = ClassExample(self.my_list)

       def tearDown(self):
           unittest.TestCase.tearDown(self)

       def testProduct(self):
           result = ClassExample.product(self.first, self.second)
           assert abs(result - (self.first * self.second)) < self.tol

       def testDestruction(self):
           assert abs(self.expected_result - self.example_object.sumListValues()) < self.tol

   if __name__ == '__main__':
       unittest.main()

   </code>

By convention, the python test files are located in the tests/python
sub-directory of a module. They are automatically added if there are
already some python files to be tested in the python sub-directory and
that the ``elements_install_python_modules()`` call has been place in
the CMakeLists.txt file of the module.

On the contrary, if there is only test python files and no genuine
python modules (declared with ``elements_install_python_modules``) to be
tested, a direct call to ``add_python_test_dir(tests/python)`` is
necessary in the CMakeLists.txt file.

Remarks:

-  the name of the methods containing the tests have to start with
   ``test``.
-  ``setUp`` and ``tearDown`` are special methods called automatically
   to prepare and destroy the needed resources before and after the
   calls to the test methods
-  the ``unittest.TestCase`` based class is equipped with several test
   assert methods (``assert_``, ``assertEqual``, ``assertGreater``, …)
-  These tests/python/*.py files are as a *single test* for the global
   CTest test runner that is launched by the “make test” command. The
   name of the test is always “PyTest” prefixed by the name of the
   Elements module that contains it.

Plain Tests
~~~~~~~~~~~

The last type of test is the simplest one. It just relies on executables
(scripts or binaries) that return 0 (success) or something else
(failure). They are declared like:

::

   elements_add_test(PythonTestProgram COMMAND PythonTestProgramExample)

where ``PythonTestProgram`` is the name of the test and
``PythonTestProgramExample`` is the actual test executable. A more
detailed description of that macro is available
[[NewUserManual#Other-Utilities|here]].

An executable can be created in an Elements module in several ways:

-  a C binary declared with ``elements_add_executable``,
-  a script (bash, python, perl etc) placed in the scripts sub-directory
   and declared with ``elements_install_scripts()``,
-  a script generated from a python module declared with
   ``elements_add_python_program``. It is using the
   ``ElementsKernel.Program`` wrapper module.

This type of test is especially suited for integration or validation
testing. This means when no specific unit (class, method, function)
needs to be tested in the module and that the test is aimed at a the
global behavior of an application. There is then no need to do unit
testing. There are a few examples in ElementsKernel where a dozen bash
scripts have been written to test the
[[NewUserManual#Helper-Scripts-Creating-New-Source-Files|creation
scripts]]. In this situation where many call to external extecutables
are needed, it often easier to create bash test script rather than a
full python test application.

This is also the typical kind of test that one would do if one creates a
project to validate others. If this project has the right dependency
(through the “USE” statement of the ``elements_project`` macro), it can
access all the executables, libraries, python of its dependency
projects. It can then have plain tests described above that call these
features and test them.

Helper Tools
~~~~~~~~~~~~

For the run of tests, it is often needed to create
[[NewUserManual#Temporary-Directories-and-Files|temporary files,
temporary directories]] and/or
[[NewUserManual#Temporary-Environment|temporary environments]]. Elements
provides a few helper classes for the creation of these objects for both
C and Python.

They provides the following features:

-  They ensure that the created resource (temporary file, directory or
   environment) is destroyed when the classes go out of scope and that
   the initial state is restored.
-  They ensure that the created resource is unique and doesn’t collide
   which an existing entity.

It is also recommended to use a similar feature for the tests which are
written in a pure bash script. The ``mktemp`` shell utility can
generated a temporary directory that is unique. The clean up, however,
has to be done manually:

::

   <code class="bash">
   #!/bin/sh

   home_dir=${PWD}

   # Create unique directory
   tmploc=$(mktemp -d -t temp1.XXXXXX)

   # clean up function
   clean_and_exit() {
     cd ${home_dir}
     if [ -z "${KEEPTEMPDIR}" ]; then
       rm -rf ${tmploc}
     fi
     exit $1
   }

   cd ${tmploc}

   # do test stuff
   ExecExample

   # if error stop and clean up
   if [ $? -ne 0 ]; then
      echo "Error: <ExecExample> command failed!" 1>&2
      clean_and_exit 1
   fi


   clean_and_exit 0
   </code>

Extra Test Run Options
~~~~~~~~~~~~~~~~~~~~~~

As described in the [[NewUserManual#The-Build-Commands|above section]],
several options can be passed to the ``make test`` build command. Since
this only a wrapper that calls ``ctest``, the options have to be passed
with the ``ARGS=`` command line option. The common filter options are

-  Filter according to a regular expression on the name of the test:
   ::

      make ARGS="-R ElementsKernel" test
-  Filter according to a regular expression on the label list of the
   test:
   ::

      make ARGS="-L PyTest" test

The ``ARGS`` content is passed directly to ctest and a call to

::

   ctest -h

will provide more information about the possible ctest options.

Another feature that is very useful is the detailed crash if a test is
not passing. This is very useful when a crash appears in a python test
that is run together with other tests within the same Py.Test call. For
example:

::

   [hubert@degauden:~/Work/Space/Euclid/Elements(git)-[develop]] make test
   rm -f -r /home/hubert/Work/Space/Euclid/Elements/build.x86_64-fc27-gcc73-dbg/Testing /home/hubert/Work/Space/Euclid/Elements/build.x86_64-fc27-gcc73-dbg/html
   cd /home/hubert/Work/Space/Euclid/Elements/build.x86_64-fc27-gcc73-dbg && ctest -T test 
      Site: degauden.isdc.unige.ch
      Build name: Linux-c++
   Create new tag: 20180524-1301 - Experimental
   Test project /home/hubert/Work/Space/Euclid/Elements/build.x86_64-fc27-gcc73-dbg
         Start  1: Elements.cmake.EnvConfig.PyTest
    1/74 Test  #1: Elements.cmake.EnvConfig.PyTest ...........................   Passed    0.84 sec
    ...
    ...
   22/74 Test #22: ElementsKernel.Sleep ......................................   Passed    1.08 sec
         Start 23: ElementsKernel.Exception
   23/74 Test #23: ElementsKernel.Exception ..................................   Passed    0.07 sec
         Start 24: ElementsKernel.PyTest
   24/74 Test #24: ElementsKernel.PyTest .....................................***Failed    1.54 sec
         Start 25: ElementsKernel.CreateElementsProject
   25/74 Test #25: ElementsKernel.CreateElementsProject ......................   Passed    1.39 sec
         Start 26: ElementsKernel.AddElementsModule
   ...

. This is impossible to tell which python test has failed. But by
setting the ``CTEST_OUPUT_ON_FAILURE`` environment variable:

::

   hubert@degauden:~/Work/Space/Euclid/Elements(git)-[develop]] export CTEST_OUTPUT_ON_FAILURE=1
   [hubert@degauden:~/Work/Space/Euclid/Elements(git)-[develop]] make ARGS="-R ElementsKernel.PyTest" test 
   rm -f -r /home/hubert/Work/Space/Euclid/Elements/build.x86_64-fc27-gcc73-dbg/Testing /home/hubert/Work/Space/Euclid/Elements/build.x86_64-fc27-gcc73-dbg/html
   cd /home/hubert/Work/Space/Euclid/Elements/build.x86_64-fc27-gcc73-dbg && ctest -T test -R ElementsKernel.PyTest
      Site: degauden.isdc.unige.ch
      Build name: Linux-c++
   Create new tag: 20180524-1307 - Experimental
   Test project /home/hubert/Work/Space/Euclid/Elements/build.x86_64-fc27-gcc73-dbg
       Start 24: ElementsKernel.PyTest
   1/1 Test #24: ElementsKernel.PyTest ............***Failed    1.52 sec
   ============================= test session starts ==============================
   platform linux2 -- Python 2.7.15, pytest-3.2.3, py-1.4.34, pluggy-0.4.0
   rootdir: /home/hubert/Work/Space/Euclid/Elements, inifile:
   plugins: cov-2.5.1
   collected 18 items

   ../ElementsKernel/tests/python/BoostrapTest.py .
   ../ElementsKernel/tests/python/EnvironmentTest.py .
   ../ElementsKernel/tests/python/NameCheckTest.py ....
   ../ElementsKernel/tests/python/PathTest.py .F..
   ../ElementsKernel/tests/python/ProgramTest.py .
   ../ElementsKernel/tests/python/TempTest.py .......

    generated xml file: /home/hubert/Work/Space/Euclid/Elements/build.x86_64-fc27-gcc73-dbg/Testing/Temporary/ElementsKernel.PyTest.xml 
   =================================== FAILURES ===================================
   ____________________________ PathTest.testJoinPath _____________________________

   self = <PathTest.PathTest testMethod=testJoinPath>

       def testJoinPath(self):

           path_list = ["/toto", "titi", "./tutu"]
   >       self.assert_(joinPath(path_list) != "/toto:titi:./tutu")
   E       AssertionError: False is not true

   ../ElementsKernel/tests/python/PathTest.py:46: AssertionError
   ===================== 1 failed, 17 passed in 0.68 seconds ======================


   0% tests passed, 1 tests failed out of 1

   Label Time Summary:
   ElementsKernel    =   1.52 sec*proc (1 test)
   PyTest            =   1.52 sec*proc (1 test)
   Python            =   1.52 sec*proc (1 test)
   UnitTest          =   1.52 sec*proc (1 test)

   Total Test time (real) =   1.52 sec

   The following tests FAILED:
        24 - ElementsKernel.PyTest (Failed)
   Errors while running CTest
   make: [/home/hubert/Work/Space/Euclid/Elements/make/Elements.mk:145: test] Error 8 (ignored)

C and Python Utilities
----------------------

{{>toc}}

C Utilities
~~~~~~~~~~~

As mentioned earlier, the ElementsKernel module has also as function to
iron out the differences between various platforms and compilers. All
the nasty pieces of code using specific preprocessor macros should all
be included in the ElementsKernel library. This will allow to keep the
rest of the code from other projects nice and clean.

One specific class of instructions which are compilers dependent are the
specific “attributes” that one could riddle his code. These attributes
give special instructions to the compiler for, say, optimize, debug etc.
Until C+\ *14, there was no standard syntax for writing attributes.
Since we are using the C*\ +11 standard, we can only use compiler
dependent attributes. For example gcc uses the ``__attribute__`` syntax
and microsoft ``__declspec``.

This is the reason why a set of header files located in ElementsKernel
are exposing a useful set of attributes. It provides the same keyword
for an attribute present for various compilers. Eventually this header
files will disappear when all the compilers will support the same syntax
for the same set of attributes that Elements is actively using.

Helpful Header Files
^^^^^^^^^^^^^^^^^^^^

*``ElementsKernel/Unused.h``*

As first example, this header file defines a single macro called
``ELEMENTS_UNUSED``. This macro is made to force the compiler to ignore
unused variables. This looks weird but actually it is not rare. It
happens typically when one has to implement a function that respect a
typical interface but doesn’t use the argument. Another way of telling
the same thing is that the function only depends the *types* that are
passed.

For example, if one has the following function:

::

   <code class="cpp">

   int myFunc(int a, int b) {
     return 1+a;
   }

   </code>

Typically the gcc compiler will issue a warning:

::

    myFunc.cpp:2:23: warning: unused parameter ‘b’ [-Wunused-parameter]
    int myFunc(int a, int b) {
                          ^

| 
| Because the ``b`` argument is not used.

In order to silence the compiler (and because we *must* have this
signature for the ``myFunc`` function), one can comment out the unused
argument:

::

   <code class="cpp">

   int myFunc(int a, int /*b*/) {
     return 1+a;
   }

   </code>

| 
| This might not however be perfect. Typically depending on the editor
  you are using, the comments might appears very pale and the
  information that the name of the parameter could provide is pretty
  much unnoticeable.

The recommended way is then

::

   <code class="cpp">

   #include "ElementsKernel/Unused.h"

   int myFunc(int a, ELEMENTS_UNUSED int b) {
     return 1+a;
   }

   </code>

This macro can also be used for local variable that are unused. This is
typically happening in the unit test case:

::

   <code class="cpp">

   #include "ElementsKernel/Unused.h"

   int myFunc(int a, ELEMENTS_UNUSED int b) {
     ELEMENTS_UNUSED int c = 7;
     return 1+a;
   }

   </code>

Please note that this ``ELEMENTS_UNUSED`` macro is only used in the
definition, in the compiled translation unit (*.cpp)

*``ElementsKernel/Deprecated.h``*

This is another important header file that also present a uniform macro
built on a compiler attribute. This one is especially interesting
because it allows to flag the deprecated function in an interface. These
functions will then issue a warning at compilation time. If we have the
function ``myFunc`` declared in the myFunc.h file:

::

   <code class="cpp">
   #ifndef EXAMPLE_MYFUNC_H_
   #define EXAMPLE_MYFUNC_H_

   #include "ElementsKernel/Deprecated.h"

   ELEMENTS_DEPRECATED int myFunc(int a, int b);

   #endif  // EXAMPLE_MYFUNC_H_
   </code>

If then, one calls the ``myFunc`` function in another file (Test.cpp):

::

   <code class="cpp">
   #include <iostream>
   #include "myFunc.h"

   using namespace std;

   void myTest() {

     cout << myFunc(1, 2) << endl;

   }
   </code>

one will get the following warning at compilation time:

::

   Test.cpp: In function ‘void myTest()’:
   Test.cpp:8:11: warning: ‘int myFunc(int, int)’ is deprecated (declared at myFunc.h:7) [-Wdeprecated-declarations]
      cout << myFunc(1, 2) << endl;
              ^
   Test.cpp:8:22: warning: ‘int myFunc(int, int)’ is deprecated (declared at myFunc.h:7) [-Wdeprecated-declarations]
      cout << myFunc(1, 2) << endl;
                         ^

One can also be a bit nicer to the client software that uses our
function by adding a custom message. This can be done with the
``ELEMENTS_DEPRECATED_MSG`` macro, provided in the same header file:

::

   <code class="cpp">
   #ifndef EXAMPLE_MYFUNC_H_
   #define EXAMPLE_MYFUNC_H_

   #include "ElementsKernel/Deprecated.h"

   ELEMENTS_DEPRECATED_MSG("Please use myOther function instead") int myFunc(int a, int b);

   #endif // EXAMPLE_MYFUNC_H_
   </code>

| 
| This will then produce the following warning when compiling:

::

   Test.cpp: In function ‘void myTest()’:
   Test.cpp:8:11: warning: ‘int myFunc(int, int)’ is deprecated (declared at myFunc.h:7): Please use myOther function instead [-Wdeprecated-declarations]
      cout << myFunc(1, 2) << endl;
              ^
   Test.cpp:8:22: warning: ‘int myFunc(int, int)’ is deprecated (declared at myFunc.h:7): Please use myOther function instead [-Wdeprecated-declarations]
      cout << myFunc(1, 2) << endl;
                         ^

*``ElementsKernel/Likely.h``*

This utility header file is a bit special and has to used with great
care. It provides a way to tell the compiler which part of a branch is
executed the most. The compiler will try then to rearrange the code in
order to make is faster. As for any optimization procedure, this should
only be done with *extreme caution* and with thorough testing.

One typical usage would look like:

::

   <code class="cpp">
   #include "ElementsKernel/Likely.h"
   #include "ElementsExamples/myFunc.h"

   int myFunc(int a) {

     int result = 0;

     if(LIKELY(a>0)) {
       result += 1;
     } else {
       result += 2;
     }

     return result;
   }
   </code>

One can also use the opposite macro ``UNLIKELY`` to get the same result:

::

   <code class="cpp">
   #include "ElementsKernel/Likely.h"
   #include "ElementsExamples/myFunc.h"

   int myFunc(int a) {

     int result = 0;

     if(UNLIKELY(a<=0)) {
       result += 1;
     } else {
       result += 2;
     }

     return result;
   }
   </code>

The number of cases where these macros (``LIKELY`` and ``UNLIKELY``)
actually make a time-wise difference is probably small. Perhaps in the
very critical part of a pure calculation intensive algorithm. I would
strongly advise *not* to use them if the timing difference is
negligible. It makes the code much uglier.

*``ElementsKernel/Export.h``*

This utility header file is quite important. It allows to expose or hide
the symbols of a library.

Some more explanation is somehow needed. By default, on the linux-like
platforms, the libraries (shared objects) are exposing all the symbols
(classes, functions, constants, etc) they contain. It means that they
allow any other external client to “link” against these symbols and
further use them. So far so fine.

The problem with this feature is coming from the excess of symbols
exposed. All of the symbols exposed by a library are loaded into a table
in memory when the shared object is loaded. The symbols used by other
clients are then looked for in that table. And when the software is
becoming large it could cost some sizeable amount of time. Not a lot but
some time.

For example, on the windows-like platform, the symbols of a shared
library (dll) have to be *explicitly* exposed. On that platform, by
default no symbol is exposed. The net result of such policy is that one
exposes only the symbols that are meant to be viewed and not the
internal ones. As a result, it is said that this feature is one of the
reason why the graphical applications (GUIs) on windows are much more
snappy and react better than the equivalent application on Linux. The
list of symbols in memory is shorter and there is much less searching to
do. The libraries are also lighter. There is some more information about
symbol visibility at this
`link <https://gcc.gnu.org/wiki/Visibility>`__.

For Elements-based projects, we can easily identify the symbols that
need to be exposed. They are carried by the public header files that are
located in the separate folder. The one that has the same name as the
one of the module. All other header files, the ones with are located
together with the implementation files (.cpp) in the ``src`` directory
(and also the implementation files) contain declarations of symbols that
should *not* be exposed, ie that should not be visible in a dynamic
library.

Logging
~~~~~~~

See the `Elements
examples <https://gitlab.euclid-sgs.uk/ST-TOOLS/Elements/tree/master/ElementsExamples>`__
to find an example usage of the Elements logger in
`C <https://gitlab.euclid-sgs.uk/ST-TOOLS/Elements/blob/master/ElementsExamples/src/program/ProgramExample.cpp>`__
and
`Python <https://gitlab.euclid-sgs.uk/ST-TOOLS/Elements/blob/master/ElementsExamples/python/ElementsExamples/PythonProgramExample.py>`__.

For example, for C:

::

   <code class="Cpp">
   #include "ElementsKernel/Logging.h"

   using Elements::Logging;

   void myLocalLogTestFunc() {

     Logging logger = Logging::getLogger();
     logger.info("Test of Message");

     Logging logger2 = Logging::getLogger(__func__);
     logger2.info("Test2 of Message");

     Logging logger3 = Logging::getLogger(BOOST_CURRENT_FUNCTION);
     logger3.info("Test3 of Message");

   }
   </code>

which gives:

::

   ...
   2018-05-03T16:27:21CEST   INFO : Test of Message
   2018-05-03T16:27:21CEST myLocalLogTestFunc  INFO : Test2 of Message
   2018-05-03T16:27:21CEST void Elements::Examples::myLocalLogTestFunc()  INFO : Test3 of Message
   ...

where 3 different loggers are created. For these loggers the output is
always redirected to the standard error stream (not the standard output
stream).

The logger are implemented as singleton and thus, only one copy is
created for a given ``Logging::getLogger`` argument. Any subsequent call
to ``Logging::getLogger`` will just get the reference to the already
existing instance of the logger.

Redirecting to a File
^^^^^^^^^^^^^^^^^^^^^

Optionally a ``--log-file`` option can be given on the command line
(with a file name) to redirect the logged message to a file. All the
executables created with ``ElementsKernel/Program.h`` (as the
CppProgramExample) are equipped with this functionality.

The Log Levels
^^^^^^^^^^^^^^

There is 5 different levels of messages:

-  debug, for debugging
-  info, for general informations
-  warn, warnings
-  error, non-fatal errors, the program will try to recover and continue
-  fatal, fatal error, the program cannot continue

The default log level is set to show info and above messages. The level
can be changed by passing the keyword FATAL, ERROR, WARN, INFO or DEBUG
to the ``--log-level`` command line option.

and for Python
^^^^^^^^^^^^^^

::

   <code class="Python">
   from ElementsKernel import Logging

   def myLocalLogTestFunc() :

       logger = Logging.getLogger()
       logger.info("Test of Message");

       logger2 = Logging.getLogger("TestLogger");
       logger2.info("Test2 of Message");

       logger3 = Logging.getLogger(__name__);
       logger3.info("Test3 of Message");

   </code>

which gives the result:

::

   ...
   2018-05-04T17:37:11CEST root  INFO : Test of Message
   2018-05-04T17:37:11CEST TestLogger  INFO : Test2 of Message
   2018-05-04T17:37:11CEST ElementsExamples.PythonProgramExample  INFO : Test3 of Message
   ...

The log level and command line options are similar for Python and C.

Temporary Directories and Files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

C Temporary Directories and Files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to be able to use temporary resources like directories and
files during the implementation features like the
``boost::filesystem::unique_path()`` functions come in very handy for
this kind of jobs. It creates a unique temporary path name that then can
be used further to create a directory or a file.

This is however a so common task that a little wrapping could help even
more the programmers especially in the region of the unit testing. The
main idea is to have 2 classes, ``TempDir`` and ``TempFile``, that will
be created automatically in a secure (and unique) way. When they will go
out of scope (when their destructors will be called), the resources (the
actual directories and files on the filesystem) will be destroyed.

These features are provided by the ``ElementsKernel`` package. And the
public API is defined in the ``ElementsKernel/Temporary.h`` header file.

The Location of the Temporary Resources
'''''''''''''''''''''''''''''''''''''''

The top directory to create the temporary resources is provided
internally by the ``boost::filesystem::temp_directory_path()`` function.
On a Unix-like system it first tries to use the ``TMPDIR`` variable
value if it exists and falls back on ``/tmp``. On large shared clusters,
it is not uncommon to have the default value of the ``TMPDIR`` variable
shown like ``/tmp/$USER``

The Unique Naming Scheme
''''''''''''''''''''''''

On top of the location, described above, holding the resources, the
second ingredient is the unique second part of the path that will be
generated by the ``boost::filesystem::unique_path`` function. This
function is taking a pattern or no pattern and generate a unique name.

The combination of the location and of the unique name provide the final
path that is needed. But it doesn’t create the resource itself. It just
provide the path string to the resources. The creation and automatic
destruction of the resources is provided by the ``TempDir`` and
``TempFile`` classes from the ``ElementsKernel/Temporary.h`` header
file.

Temporary Directories
'''''''''''''''''''''

The ``TempDir`` is providing the automatic construction and destruction
of a guaranteed unique directory. It is created at construction and
deleted from the filesystem at destruction.

::

   <code class="cpp">
   #include <boost/filesystem.hpp>
   #include <boost/filesystem/fstream.hpp>

   #include "ElementsKernel/Temporary.h"

   using boost::filesystem::path;


     // handle on created path names
     path test_path;

     {

        using boost::filesystem::ofstream;
        using Elements::TempDir;

        // block creation for local variables
        TempDir one;
        test_path = one.path();

        path test_file_path = test_path / "toto.txt" ;
        ofstream ofs(test_file_path);
        ofs << "test text" << endl;
        ofs.close();

     }

     // the items must have been destroyed and removed from the disk after the
     // closing of the block

   </code>

After the closing of the inner block, the ``one TempDir`` is destroyed
as well as the directory it points to. One can also choose to have a
different name scheme with the usage of a template pattern:

::

   <code class="cpp">
   #include "ElementsKernel/Temporary.h"

   using Elements::TempDir;

   TempDir two{"This-new-scheme-%%%%%%"};
   </code>

| 
| The ``%`` are placeholders that will be replaced by a random sequence
  of letters and digits.

Temporary Files
'''''''''''''''

The temporary files are behaving exactly as their directory
counterparts. They just create files instead of directories.

::

   <code class="cpp">
   #include <boost/filesystem.hpp>
   #include <boost/filesystem/fstream.hpp>

   #include "ElementsKernel/Temporary.h"

   using boost::filesystem::path;

     // handle on created path names
     path test_path;

     {

        using boost::filesystem::ofstream;
        using Elements::TempFile;

        // block creation for local variables
        TempFile three;
        test_path = three.path();
        ofstream ofs(test_path);
        ofs << "test text" << endl;
        ofs.close();

     }

     // the items must have been destroyed and removed from the disk after the
     // closing of the block

   </code>

Python Temporary Directories and Files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As for their C counterpart, the ``ElementsKernel.Temporary`` python
module contains utilities for the usage of temporary directories and
temporary files. These entities has the property to delete the resources
when their instance is going out of scope. It is quite handy to use the
**``with``** python keyword when possible (This feature is available
since Elements 4.1).

.. _temporary-directories-1:

Temporary Directories
'''''''''''''''''''''

As for its C equivalent

::

   <code class="Python">
   import os
   from ElementsKernel.Temporary import TempDir

   test_path = ""

   with TempDir() as one:
       test_path = one.path()
       test_file_path = os.path.join(test_path, "toto.txt")
       ofs = open(test_file_path, "w")
       ofs.write("test text\n")
       ofs.close()

   # the items must have been destroyed and removed from the disk after the
   # closing of the "with" context
   </code>

.. _temporary-files-1:

Temporary Files
'''''''''''''''

::

   <code class="Python">
   import os
   from ElementsKernel.Temporary import TempFile

   test_path = ""

   with TempFile() as three:
       test_path = three.path()
       ofs = open(test_path, "w")
       ofs.write("test text\n")
       ofs.close()

   # the items must have been destroyed and removed from the disk after the
   # closing of the "with" context
   </code>

Temporary Environment
~~~~~~~~~~~~~~~~~~~~~

Elements also provides 2 classes, one for python and one for C, that are
able to deal with a temporary environment. When one of these instance is
created, environment variables can be set, unset or changed. Whenever
the instance gets out of the scope, its destructor is called and the
original environment is restored.

This is a useful feature for writing tests.

For C
^^^^^

The ``TempEnv`` is providing the automatic construction and destruction
of an overlaid environment. It can be changed at will and the original
environment will be restored when the destructor is called.

::

   <code class="cpp">
   #include "ElementsKernel/System.h"
   #include "ElementsKernel/Temporary.h"

   using Elements::System::setEnv;


     // sets the MYVAR  variable to "alpha"
     setEnv("MYVAR", "alpha");

     {
        using Elements::TempEnv;

        // block creation for local variables
        TempEnv local;

        // sets the MYVAR  variable to "beta"
        local["MYVAR"] = "beta";
        local["OTHERVAR"] = "omega"

     }

     // MYVAR is now set back to "alpha" and OTHERVAR is unset

   </code>

After the closing of the inner block, the ``local TempEnv`` is destroyed
and the original environment is restored.

For Python
^^^^^^^^^^

As for C, the ``TempEnv`` is providing the automatic construction and
destruction of an overlaid environment.

::

   <code class="python">
   import os
   from ElementsKernel.Temporary import TempEnv

   # sets the MYVAR  variable to "alpha"
   os.environ["MYVAR"] = "alpha"


   with TempEnv() as local:
       local["MYVAR"] = "beta"
       local["OTHERVAR"] = "omega"



   # MYVAR is now set back to "alpha" and OTHERVAR is unset

   </code>

After the closing of the inner block, the ``local TempEnv`` is destroyed
and the original environment is restored.

Accessing Auxiliary and Configuration Files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The framework also provides ways to access 2 other types of files:
auxiliary and configuration files.

In principle, the configuration files don’t need to be accessed manually
because they are handles automatically by the generated C and python
executables.

In contrast, the so-called auxiliary files are custom auxiliary files
(small) that are kept with the sources. They need to be accessed
manually.

For this task 2 functions, ``getConfigurationPath`` and
``getAuxiliaryPath`` are provided both for C and Python. They rely on
the ``ELEMENTS_CONF_PATH`` and ``ELEMENTS_AUX_PATH`` environment
variables and they also rely on some hardcoded default locations.

.. _for-c-1:

For C
^^^^^

The following example shows how to get a template file from the
ElementsKernel auxiliary files. The real code that uses this feature and
generates a project skeleton is done in python. But this is just for the
sake of the example.

::

   <code class="cpp">
   #include <boost/filesystem.hpp>
   #include "ElementsKernel/Auxiliary.h"

   using boost::filesystem::path;

   path make_template = Elements::getAuxiliaryPath("ElementsKernel/templates/Makefile.in");

   </code>

Some remarks:

-  This functions is templated and it can accept both
   ``boost::filesystem::path`` stem and string arguments.
-  A second argument (``raise_exception``) is set to ``true`` by
   default. It can explicitly be set to ``false`` if one wants a
   graceful failure.
-  The return value is of ``boost::filesystem::path`` type.
-  Please note the relative location (from the auxdir directory) to
   ensure the uniqueness of this template file through the whole Euclid
   software.

.. _for-python-1:

For Python
^^^^^^^^^^

::

   <code class="python">
   from ElementsKernel.Auxiliary import getAuxiliaryPath

   make_template = getAuxiliaryPath("ElementsKernel/templates/Makefile.in")

   </code>

Ad Hoc Support for Data Files
-----------------------------

There are 2 kind of data files which get a minimal support from the
Elements framework. The main difference is if they are either maintained
under a VCS (git most of the time) or managed in different way. Here are
the 2 possible managements of data files that the Elements framework is
proposing:

-  Auxiliary files: there are stored together with the Elements module
   sources under the ``auxdir`` directory. This directory typically
   contains small files which are maintained together with the code that
   use them. For example, in the ElementsKernel module, the template
   files that are used to create various software components
   (project/module/class) are stored in the ElementsKernel module
   source, under the ``auxdir/ElementsKernel/templates`` directory.
-  Elements Data Modules (since version 5.4):

Auxiliary Files
~~~~~~~~~~~~~~~

These files are located together with the other source files (as
described [[NewUserManual#Configuration-and-Auxiliary-Files|here]]) and
it recommended to place them under a sub-directory of the module that
looks like ``auxdir/<module>/``. This will ensure that the files in
there will be uniquely located since the ``<module>`` name has to be
unique.

Typically the files which are stored in the ``auxdir`` sub-directory are
entities which are used together with the code, like template files or
GUI icons for example. They should be small in order to be handled
easily by the version control system.

In order to be able to use these auxiliary files, the module has to
declare that it provide some ``auxdir`` directory. This is described
[[NewUserManual#Other-Utilities|here]] where the CMake snippet is shown.
This CMake line will both do the installation into the ``InstallArea``
of the project and setup the right path entries in the ELEMENTS_AUX_PATH
environment variable for the run-time execution.

Data Modules (since version 5.4)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Another way to support data files is the so-called data modules. This
other possibility is targeting data files which are larger than the
auxiliary files and that cannot be kept together with the sources.
Therefore, a natural location for the data modules is a dedicated
location on the filesystem outside of the software deployment. **By
nature these data modules are not kept under version control**.

The Data Module Declaration
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The declaration of the data module has to be done at the level of the
project ``CMakeLists.txt``:

::

   elements_project(<project_name> <project_version>
                    DESCRIPTION <project_description>
                    DATA <data_mod1_name> <data_mod1_version> [<data_mod2_name> <data_mod2_version> [...]])

For example:

::

   elements_project(MyProject 1.0
                    DESCRIPTION "This is a standalone project without Elements dependency"
                    DATA ParamFiles 1.0)

The Data Module Structure
^^^^^^^^^^^^^^^^^^^^^^^^^

The structure of the data module is a bit special

-  it has to have a version directory. And generally several version are
   provided under the same directory.
-  it doesn’t contain any CMakeLists.txt file.
-  it contains a special XML file that described its environment. The
   file is always named ``<data_mod_name>Environment.xml``

For example, the ParamFiles data module would look like:

::

   ParamFiles
   ├── 1.0
   │   ├── data
   │   │   ├── MyDb.txt
   |   │   └── Calib.fits
   │   └── ParamFilesEnvironment.xml
   └── 1.1
       ├── data
       │   ├── MyDb.txt
       │   └── Calib.fits
       └── ParamFilesEnvironment.xml

And the ParamFilesEnvironment.xml would look like:

::

   <code class="xml">
   <?xml version="1.0" encoding="UTF-8"?>
   <env:config xmlns:env="EnvSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="EnvSchema EnvSchema.xsd ">
     <env:set variable="PARAMFILESROOT">${.}</env:set>
   </env:config>
   </code>

In the above example file, the PARAMFILESROOT environment is set to
point to the very location of the XML file. Other variable can be set,
but it is highly recommended to set at least the ``<MODULE_NAME>ROOT``
variable as done here.

The rest of the module content can have a free form: here, for example,
a data sub-directory has been created to contain a couple of files
(MyDb.txt and Calib.fits).

The Data Module Lookup
^^^^^^^^^^^^^^^^^^^^^^

Please remember that the *data module lookup is always done at build
time*. There are 2 components which are used for the lookup:

-  a list of base path entries
-  and a list of custom suffixes
   the final absolute path to the data module root would look like:
   ::

      <path>/<suffix>/<data_mod_name>/<data_mod_version>

The list of path entries is build in the following order:

#. The CMAKE_PROJECT_PATH environment variable
#. The CMAKE_PREFIX_PATH environment variable
#. The DATA_MODULE_PATH CMake cache variable

The last one, the DATA_MODULE_PATH CMake cache variable can be
overridden in the main CMakeLists.txt file of the project. By default,
it contains the content of the XDG_DATA_HOME and XDG_DATA_DIRS
environment variables if they are available at build time, and
/usr/share.

The second ingredient for the lookup, the suffix, is the collation of

#. , the empty string
#. The ELEMENTS_DATA_SUFFIXES CMake cache variable

The ELEMENTS_DATA_SUFFIXES CMake cache variable can be overridden in the
main CMakeLists.txt file of the project. By default, it contains the
words ``DBASE``, ``PARAM``, ``EXTRAPACKAGES``.

Accessing the Data Module
^^^^^^^^^^^^^^^^^^^^^^^^^

The access to the root of the data module is always done through the
ROOT environment variable. This variable is set up by either the local
``build.${BINARY_TAG}/run`` development script or the ``E-Run`` command.

.. _for-c-2:

For C
'''''

::

   <code class="cpp">
   #include <string>
   #include <boost/filesystem/path.hpp>
   #include "ElementsKernel/Environment.h"

   using boost::filesystem::path;

     {
        Elements::Environment current;

        std::string paramfiles_root = current["PARAMFILESROOT"];
        auto calib_path = path(paramfiles_root) / "data" / "Calib.fits";

     }

   </code>

.. _for-python-2:

For Python
''''''''''

::

   <code class="python">
   import os
   from ElementsKernel.Environment import Environment

   with Environment() as current:
       paramfiles_root = current["PARAMFILESROOT"]
       calib_path = os.path.join(paramfiles_root, "data", "Calib.fits")


   </code>

The DataSync Service (since 5.8 — integrated from project:testdata)
-------------------------------------------------------------------

The tool is designed to be triggered by the unit test suite of an
Elements project when one of the following commands is executed at the
root directory of the project:

-  ``make test``
-  ``checkQuality``

Getting started
~~~~~~~~~~~~~~~

Prerequisites
^^^^^^^^^^^^^

The test files you need must be accessible in a compatible hosting
solution (preferably WebDAV).

If working with iRODS, you must have the `client
installed <https://euclid.roe.ac.uk/projects/ousdcd/wiki/TestDataHosting>`__.
In this case, the data is not retrieved by CODEEN, which might block
continuous integration and deployment.

Installation and setup
^^^^^^^^^^^^^^^^^^^^^^

The tool benefits from continuous integration and deployment, so **no
installation** is required: it is available to any Elements project.

All you have to do is to **declare the dependency** of your project to
DataSync, by editing your CMakeLists’ and adding the required
include/import.

Root CMakeLists (beta-version of the tool only)
'''''''''''''''''''''''''''''''''''''''''''''''

Declare the dependency of your project to the beta project
``CT_TestDataManager``:

::

   <code class="xml">
   elements_project(<ProjectName> <ProjectVersion> USE Elements <ElementsVersion> CT_TestDataManager <CT_TestDataManagerVersion>)
   </code>

Module CMakeLists (C only)
''''''''''''''''''''''''''

Declare the dependency of each test suite to the tool module
``ElementsServices``:

::

   <code class="xml">
   elements_add_unit_test(<TestSuiteName> tests/src/<TestSuiteName>_test.cpp 
                        EXECUTABLE <ModuleName>_<TestSuiteName>_test
                        LINK_LIBRARIES <ModuleName> ElementsServices
                        TYPE Boost)
   </code>

C test suite source
'''''''''''''''''''

Include the end-user header and optionally use the tool namespace at
test suite level:

::

   <code class="cpp">
   #include "ElementsServices/DataSync.h"
   using namespace ElementsServices::DataSync;
   </code>

Python test suite source
''''''''''''''''''''''''

Import the end-user functions:

::

   <code class="python">
   from ElementsServices.DataSync import DataSync
   </code>

Quick start
^^^^^^^^^^^

See the [[DataSync quick start|dedicated page]].

Configuration Files
~~~~~~~~~~~~~~~~~~~

See the [[configuration files|dedicated page]].

C usage
~~~~~~~

Download command
^^^^^^^^^^^^^^^^

Whatever the use case, the download command is always the same:

::

   <code class="cpp">
   DataSync sync(connectionFile, dependencyFile);
   sync.download();
   </code>

where the paths to the configuration files are relative to the ``conf``
directory of the module of your tests.

Optionally, you can specify a fallback host via another connection
configuration file this way:

::

   <code class="cpp">
   DataSync sync(connectionFile, dependencyFile);
   sync.downloadWithFallback(fallbackConnectionFile);
   </code>

Downloading a set of files for one test suite
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to download a set of files for one test suite, you have to
create a so-called **fixture class** in which the download command will
be called. The fixture class will be instantiated **at the suite
level**.

Let us create the fixture class (preferably choose a fixture name which
describes the set of data you want to download) and use the download
command in its default constructor:

::

   <code class="cpp">
   struct SomeFilesForMySuite {
     DataSync sync;
     SomeFilesForMySuite () :
         sync("connectionFile", "dependencyFile") {
       sync.download()
     }
   };
   </code>

You can also download several sets of files as follows:

::

   <code class="cpp">
   struct SomeFilesForMySuite {
     DataSync sync1, sync2, syncN;
     SomeFilesForMySuite () :
         sync1("connectionFile1", "dependencyFile1");
         sync2("connectionFile2", "dependencyFile2");
         syncN("connectionFileN", "dependencyFileN");
       sync1.download();
       sync2.download();
       syncN.download();
     }
   };
   </code>

Now, we will tell the test suite that it has to rely on our fixture, by
changing the classical ``BOOST_AUTO_TEST_SUITE`` macro for
``BOOST_FIXTURE_TEST_SUITE``:

::

   <code class="cpp">
   BOOST_FIXTURE_TEST_SUITE (MyTestSuite, SomeFilesForMySuite)

   BOOST_AUTO_TEST_CASE (MyFirstTest) {
     BOOST_CHECK(...) // Use your files here...
   }

   BOOST_AUTO_TEST_CASE (MySecondTest) {
     BOOST_CHECK(...) // ... and here
   }

   BOOST_AUTO_TEST_SUITE_END ()
   </code>

And that’s all!

Downloading a set of files for one test
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The process is nearly the same if you want to download a set of files
for a single test: you still have to create the **fixture class** but it
will be instantiated **at the test level**.

So, create a fixture ``SomeFilesForMyTest`` as explained in the previous
section, keep the classical ``BOOST_AUTO_TEST_SUITE`` macro and use
``BOOST_FIXTURE_TEST_CASE`` instead of ``BOOST_AUTO_TEST_CASE``:

::

   <code class="cpp">
   BOOST_FIXTURE_TEST_CASE (MyTestCase, SomeFilesForMyTest) {
     BOOST_CHECK(...) // Use your files here
   }
   </code>

Using downloaded files
^^^^^^^^^^^^^^^^^^^^^^

The files are downloaded to a path that you contribute to specify but
may be prefixed in some case with an environment-specific root (e.g., in
CODEEN). Therefore, you **cannot** just use a path like:

::

   /theLocalWorkspaceIDefined/theTestFileIWant

Instead, you **have to** build an absolute path like:

::

   /someSecretPrefix/theLocalWorkspaceIDefined/theTestFileIWant

By chance, you don’t have to guess, because this is handled by the
following function:

::

   <code class="cpp">
   sync.absolutePath(theTestFileIWant)
   </code>

Finally, your test will look like:

::

   <code class="cpp">
   BOOST_AUTO_TEST_CASE (MyTest) {
     ...
     path myTestFile = sync.absolutePath("theTestFileIWant");
     ... // use myTestFile
   }
   </code>

Now you may want have a look at a **real example** explained in details
[[DataSync example usage|here]].

Python usage
~~~~~~~~~~~~

.. _download-command-1:

Download command
^^^^^^^^^^^^^^^^

Whatever the use case, the download command is always the same:

::

   <code class="python">
   sync = DataSync(connectionFile, dependencyFile)
   sync.download()
   </code>

where the paths to the configuration files are relative to the ``conf``
directory of the module of your tests.

Optionally, you can specify a fallback host via another connection
configuration file this way:

::

   <code class="python">
   sync = DataSync(connectionFile, dependencyFile)
   sync.downloadWithFallback(fallbackConnectionFile)
   </code>

.. _downloading-a-set-of-files-for-one-test-suite-1:

Downloading a set of files for one test suite
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to download a set of files for one test suite, we will rely on
a ``py.test`` functionality: the **test suite setup**, which is a class
method.

::

   <code class="python">
   class TestSuite(object):

     @classmethod
     def setup_class(cls):
       cls.sync = DataSync("connectionFile", "dependencyFile")
       cls.sync.download()
   </code>

You can also download several sets of files as follows:

::

   <code class="python">
   class TestSuite(object):

     @classmethod
     def setup_class(cls):
       cls.sync1 = DataSync("connectionFile1", "dependencyFile1")
       cls.sync2 = DataSync("connectionFile2", "dependencyFile2")
       cls.syncN = DataSync("connectionFileN", "dependencyFileN")
       cls.sync1.download()
       cls.sync2.download()
       cls.syncN.download()

     def testSomething(self):
       assert ... # Use the test files here...

     def testSomethingElse(self):
       assert ... # ... and here
   </code>

And that’s all!

.. _downloading-a-set-of-files-for-one-test-1:

Downloading a set of files for one test
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you want to download a set of files for a single test, you can
directly call the download command at the beginning of the test:

::

   <code class="python">
   def testWhichNeedsData(self):
     sync = DataSync("connectionFile", "dependencyFile")
     ... # Use the test files here
   </code>

.. _using-downloaded-files-1:

Using downloaded files
^^^^^^^^^^^^^^^^^^^^^^

The files are downloaded to a path that you contribute to specify but
may be prefixed in some case with an environment-specific root (e.g., in
CODEEN). Therefore, you **cannot** just use a path like:

::

   /theLocalWorkspaceIDefined/theTestFileIWant

Instead, you **have to** build an absolute path like:

::

   /someSecretPrefix/theLocalWorkspaceIDefined/theTestFileIWant

By chance, you don’t have to guess, because this is handled by the
following function:

::

   <code class="python">
   sync.absolutePath(theTestFileIWant)
   </code>

Finally, your test will look like:

::

   <code class="python">
   def testSomething(self):
     ...
     myTestFile = self.sync.absolutePath("theTestFileIWant");
     ... # use myTestFile
   </code>

Now you may want have a look at a **real example** explained in details
[[DataSync example usage|here]].

Going further
~~~~~~~~~~~~~

Setting the local workspace prefix
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The local workspace prefix was implemented to allow using the tool in
CODEEN, where the file system constraints are not known a priori. For
example, you might not be allowed to write to ``/tmp`` while you would
want to do that in LODEEN. Therefore, the local test workspace is
prefixed by the CODEEN job workspace, which is pointed by the
environment variable ``$WORKSPACE``.

This means that you can yourself set the variable in your environment to
set the prefix. For example, running:

::

   <code class="text">
   make WORKSPACE=/home/user/testdata test
   </code>

or

::

   <code class="text">
   export WORKSPACE=/home/user/testdata
   make test
   </code>

will prefix the local workspace with ``/home/user/testdata``.

.. |image1| image:: autotool_map.png
   :width: 30.0%
