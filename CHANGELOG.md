# Changelog
All notable changes to this project will be documented in this file.

Project Coordinators: Hubert Degaudenzi @hdegaude

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]


## [6.1.1] - 2022-10-19

### Changed
- forgot to update this very file


## [6.1.0] - 2022-10-19

### Added
- Add the FindXsData.cmake module in order to find the xsdata executable. xsData will
  replace at some point the usage of the pyxbgen tool
- Add trivial PlantUML example in Doxygen
- update sphinx config to auto retrieve current year and add display build date (Manuel Grizonnet)
- Initial implementation of the pytest coverage report
- Add 2 CMake variables to be passed to the pytest and nose test runners
    - PYTEST_EXTRA_OPTIONS for pytest
    - NOSE_EXTRA_OPTIONS for nosetests
- Add a CMake configure test to disable -D_GLIBCXX_PARALLEL
    - there are some cases where it misbehaves.
    - it only happens when the ELEMENTS_PARALLEL CMake flags is enabled
- Use Read The Docs HTML Sphinx theme if available (Manuel Grizonnet)
- Add the extra `--output-junit` option to the ctest call
    - the default call is now `-T test --output-junit
    $(BUILDDIR)/Testing/JUnitTestReport.xml`
    - add a CTEST_ARGS environment variable to recover the previous behavior
    (`export CTEST_ARGS="-T test"`)
    - add a CTEST_EXTRA_ARGS environment variable to extend the already
    present options.
- Add the Sphinx support for the cmake directory
    - use the sphinxcontrib-moderncmakedomain
    - the documentation is provided by either
        - a local cmake/index.rst static file
        - a generated list from cmake/*.cmake if the former file doesn't exist
    - the documentation only appear if there are some rst inserted in the
    cmake files.
- Import the documentation from the Euclid Redmine site
    - the site https://euclid.roe.ac.uk/projects/elements/wiki
    - convert the textile source with pandoc:
      `pandoc -s FAQ.textile -f textile -t rst -o FAQ.rst`
- Add support for Markdown files in Sphinx
- Add the CMake EXTRA_SPHINX_FILES variable for files to be copied into the sphinx doc dir
- Implementation of the HTML tree merge
    - the merge is triggered by the MERGE_HTML_DOC_TREES=ON CMake option. By
    default it is set to OFF
    - the tree is generated at build/doc/html
    - the root of the tree is the doxygen one
    - the sphinx tree is located at build/doc/html/sphinx


### Changed
- Move to the [Keep a Changelog](http://keepachangelog.com/en/1.0.0/) format
- Update the jenkins configuration for EDEN-3
- Change the obsolete failUnlessRaises methods that were deprecated in Python 3.1.
    - it is changed to assertRaises
    - it will be removed in Python 3.11
- replace the obsolete imp python module with the importlib one
- Improve the python coding style
    - more consistency with the import statements
    - use the `__name__` for the global logger when applicable
    - define the global in capitals
- Move the pytest coverage results into build.*/cov/PyTest. Now both XML 
  and HTML reports are generated.
- Use new CMake find_package layout
    - create a specific target
    - use caching for some variables only
- Make C++17 the default standard
- Restrict the -Wshadow warning to local variables only
- Change the sanitize style into a list
    - several sanitizers can be used
    - the sanitizers are passed with a CMake option string that uses spaces
    as separator (e.g: -DSANITIZE_STYLE="undefined thread")

### Fixed
- Fix build of Logging.cpp in centos7 (Alejandro Álvarez Ayllón)
- Remove of the obsolete distutils python package. Use the sysconfig module
  to retrieve the default PYTHONPATH.
- Readd the needed `-g`compile option for the profiling build type
- Fix the python multiprocessing spawning by adding the `__main__` guard


## [6.0.1] - 2021-11-18
This is a patched version of 6.0.0 to clean up deployment issues.

### Added
- Enable debugging of CMake includes with USE_DEBUG_PRINT=ON
- Add fake HTMLSummary target to be compatible with older make libraries

### Changed
- Use the env var CC if present to have priority over the compiler found in 
  the PATH environment variable. This is needed because conda defines a new name 
  for the compiler (x86_64-conda_cos6-linux-gnu-cc). This change allows the 
  correct parsing of the compiler version and its use for the compiler flags switching.

### Fixed
- Cure ill-formed grep tests that were causing a test timeout.


##  [6.0.0] - 2021-10-19
This new release is targeting the EDEN 3 development environment. The major changes are:
* A new companion software called ElementsEnv that supersedes the original EuclidEnv python project.
* The support for a `BINARY_TAG` with only the major version of the compiler. For example, `x86_64-fc34-gcc112-dbg` becomes `x86_64-fc34-gcc11-dbg` for gcc 11.2. It relaxes the binary dependency between the projects. Internally, however, the branching on the compiler can still be done on the major, minor and patch numbers of the full version.

### Added
- Add the build, test and install tests for a squeezed Elements  
    - Obviously this test file has to be avoided in the second level tests
    in order to avoid an infinite recusion.
    - This doubles roughly the global testing time
    - This kind of tests get the label "Meta"
- Add checkQuality config files and output dir to gitignore template 
  file (Manuel Grizonnet). Update Elements gitignore accordingly.
- Add checkQuality config file and output directory to gitignore template file.
- Add the Elements::Auxiliary namespace  
    - add the alias Auxiliary::getVariableName = getAuxiliaryVariableName
    - and Auxiliary::getPath = getAuxiliaryPath
    - and Auxiliary::getLocations = getAuxiliaryLocation
- Add the Elements::Configuration namespace with its aliases
- Create the Elements::Services inline namespace. Add 
  an ElementsServices = Elements::Services namespace alias for backward compatibility.
- Added the support for the templated class creation task #21291 (Nicolas Morisset)
- Add default docstring by default in __init__.py to silent sonar analyzer (Manuel Grizonnet)

### Changed
- Use the same directory for binary python module as the regular ones. They
  are not placed in a separate PYTHONPATH entry anymore.
- Revert the guard logic for the template implementation files. This could 
  hopefully force sonar to analyze them
- Generate the doxygen graphs as PNG instead of SVG. There is a bug in the SVG 
  generation software (https://github.com/doxygen/doxygen/issues/5724) and then
  the graphs are not displayed correctly in the web browser. This will be
  reverted back when the bug will be fixed, probably in doxygen 1.9.
- Convert the .icpp extension in the .tpp one. This is related to 
  the issue https://euclid.roe.ac.uk/issues/15512 where Sonar is not 
  parsing these *.icpp files. This conversion should solve this.
- Use only the major compiler version in the BINARY_TAG  
    - Add detection of the major and minor version of the target compiler
    - Add a conditional use of the custom include_guard macro depending
    on the CMake version.
    - Remove all the branching for gcc older than version 4.8
    - Add the optional inclusion of local flag from LocalBuildFlags.cmake
    - Use the global keyword for the include_guard macro
    - Fix the CMake include_guard macro

### Fixed
- Fixing the double indirection for the squeezed installation directory
    - Testing if the SOFTWARE_BASE_VAR is defined and not empty.
    - Testing if the ${SOFTWARE_BASE_VAR} is defined and not empty
    - in that case the ${${SOFTWARE_BASE_VAR}} value is used as the base
    installation directory.
    - otherwise /opt is chosen
  Please note that, by convention, SOFTWARE_BASE_VAR=EUCLID_BASE and
  EUCLID_BASE=/opt/euclid have to be found for the canonical Euclid
  installation
- Fixes for the EuclidEnv -> ElementsEnv change
- Attempt to fix the sphinx errors    
    - fixing the issue https://euclid.roe.ac.uk/issues/17076
    - don't link the "modules.rst" file if it has not been generated
- Force the link of the stdc++ library to cure a conda problem. This is done on
  the only C program of the project.


##  [5.14.0] - 2021-03-23
Fixes and improvements for EDEN 2.1

### Added
- Add the BOOST_TEST_TOOLS_UNDER_DEBUGGER compile definition. This allows
  a more detailed (hopefully) debug experience for the boost tests.
- Add the DEBUG_FORMAT and DEBUG_LEVEL options
    - They are appended to the -g compile option like -g${DEBUG_FORMAT}${DEBUG_LEVEL}
    - They both default to the empty string
    - a typical usage is -DDEBUG_FORMAT=gdb -DDEBUG_LEVEL=3
- Add support for keyword (named) variable in Python swig binding (Manuel Grizonnet)
- Add the NO_EXIST_CHECK option to the elements_add_library function
    - it prevents the checking of the existence of the header files beforehands.
    - this is needed for the DataModelBindings project
- Add clang formatting (and format the source)
    - a file for Elements in its root: .clang-format
    - a file generated by the creation script
- Extend the library directory lookup for the Debian specific directory
- Add the gmake mrproper target
    - it removes the whole InstallArea directory
    - it removes the build and build.* directories
    - it removes the python byte-compiled files
- Add attributes for the icpp files to be displayed in gitlab. Add the .gitattributes file
  for a new project creation

### Changed
- Improve the cmake options passed to gcovr
    - the GCOVR_EXCLUDE_UNREACHABLE option is set to ON by default and
    passes the "--exclude-unreachable-branches" option to gcovr.
    - the GCOVR_EXCLUDE_THROW option is set to ON by default and passes the
    "--exclude-throw-branches" option to gcovr.
    - the GCOVR_EXTRA_OPTIONS options is set to "" by default and allows
    extra options to be appended to the gcovr command line.
- Improve the build dependency calculations
    - Remove the unneeded options for the scripts without a config file
    - Replace the -MD compile option with -MMD

### Removed
- Remove the broken HTMLSummary gmake target
- Remove the unrelated lines in the coverage html report. There were some extra line caused by
    - the custom compiler location (on CVMFS for example)
    - the use of conda

### Fixed
-  Override the pybind11Tools.cmake file
    - remove the unconditional -flto for the optimized building. It can
     fails if the external linked library LTOs have been built with a
     slightly different compiler.
    - Use the the ELEMENTS_LINKOPT cmake option to control this part.
- Quick fix for the numpy/sphinx docstring formatting. Related to the
  https://euclid.roe.ac.uk/issues/13241#note-4 note
- Quick fix for FindBoost.cmake with Boost>=1.70. Don't use the BoostConfig.cmake file
  introduced that version. This will a need a deeper review to adapt to this new scheme
- Fix the mangling of the python version in the shebang line. Fedora 32 and later
  require to have an explicit version in any case. A line like "/usr/bin/env python" breaks
  the rpm building.
- Fix the behavior of the JUnitSummary target with python lxml is not found. A fake target
  was not created and crash was happening.
- Fix the local prefix build
    - use the GnuInstallDirs cmake library for the prefix lookup
    - fix the default Projects lookup by added the internal prefix path. it was relying solely on the CMAKE_PREFIX_PATH before.



## [5.12.0] - 2020-07-21

### Added
specific tag for the Euclid release


## [5.12] - 2020-07-10
First version that has the C++14 standard as default

### Added
- Factor out Elements own build default values into a separate file 
  cmake/ElementsDefaults.cmake
- Add an the TEST_XML_REPORT cmake option for the boost tests It will 
  generated XML files (instead of HRF) when set to ON. It is set to OFF 
  by default.
- Add alias for the boost::filesystem::path type called Elements::Path::Item.
- Add an example of a second library inside the ElementsExamples module
- Create a local <PROJECT>_EXPORT.h header for symbol hiding This can replace
  the default ElementsKernel/Export.h library
- Add static functions to extract the current project informations. They are located 
  in the Elements::Project structure.
- Add static functions to extract the current module informations. They are located
  in the Elements::Module structure.
- Add the support for a custom Makefile. The file has to be pointed by the 
  CUSTOM_MAKEFILE environment variable. It can be absolute or can be searched 
  in the usual directories otherwise. The custom Makefile is appended to the regular 
  one.
- Add a utlity make library to be used in the CUSTOM_MAKEFILE.
- Add the CMake USER_SPHINX_NUMPYDOC option
    - it is ON by default
    - add the the numpydoc_show_class_memebers=False in the conf.py to
    prevent the extra warnings generated by numpydoc
- Add include guards around the icpp implementations. Thus they can't be included by
  mistake
- Add a warning for the explicit replacement of the empty rdm motif in the TempPath class.
- Add the -std=c++17 and -std=c++20 flags
- Add a check on the version format in the USE statement of elements_project

### Changed
- Make the Logging and ExitCode local types of the Program class in order to do 
  less typing when creating a program using them.
- Activate the Sphinx API doc generation by default
- Set the default C++ standard to C++14 if it is available The option passing the C++ 
  standard will be explicit set in Elements, regardless of the internal compiler 
  standard. The default command line option will be:
    - -std=c++14 for gcc >= 5.0 or clang >= 3.5 or icc >= 17.0
    - -std=c++11 for gcc >= 4.7 or clang >= 3.0 or icc >= 15.0

### Fixed
- Fix the python files to follow the sphinx docstring style


## [5.10.1] - 2020-04-08
Fix for the Debian (Ubuntu) deployment

### Added
- Add a test and an example for the OpenMP usage
- Add the numpydoc extension to sphinx
- add a pybind11 python module binding example

### Changed
- Tune the debinfo detachment for the rpm build only
- Use the GNU Install directories cmake library for the library install. This
  is needed for the Debian architecture.

### Fixed
- Fix the INSTALL_RPATH for the python binary modules
- Fix the library linkage. It is PRIVATE by default


## [5.10] - 2020-02-06
Main adaptation to the EDEN 2.1 environment and improvements of the compilation warnings

### Added
- Add the GnuAstro CMake find package with an example
- Add example how to override the log for string and char[]
- Travis build and packaging (Alejandro Alvarez Ayllon)
- Add the possibility to generate C file for Cython
- Add option to install the binary tests. By default, it is set to OFF.
- Add option to use the so version in the library file names.
- Add new gcc warnings
    - -Wformat-security
    - -Wduplicated-cond
    - -Wshadow
    - -Wconversion
    - -Wjump-misses-init
    - -Wlogical-not-parentheses
    - -Wnull-dereference
-  Add the cmake CONVERSION_WARNING option
    - it enables the gcc -Wconversion option
    - Since it is very verbose, it is OFF by default
    - it is enabled explicitly for Elements
- Add Visual Studio Code in the gitignore template for Elements
  project (Manuel Grizonnet)
- Add the doc subdirectory for the doxygen mainpage lookup
    - the <project>/doc subdir will be looked first,
    - then the <project> dir
    - and then the <project>/build.*/doc
- Add the generation of a doc/mainpage.dox file at the project creation
- Add the lookup of the xsd executable if the xsdcxx is not found
- Add the detection of the sub OS wrapped by Conda
- Add alternative name for the healpix cxx support library on Conda
- Add the -Wno-dev CMake option by default. It prevents the display of
  the "project" instruction not being present in the main CMakeLists.txt file
- Add the generation of the git informations in the manifest

### Changed
- Enable the original CMake symbol hiding mechanism
- Use MathJax in doxygen
- Use the python Auxiliary functions for the creation scripts (Nicolas Morisset)
- Enhance the detection of the sphinx tool with a fallback without version
- Make AddScript create the file with the execution flag
- Remove unneeded lines (for compatibility py2 to py3) in the python
  template files (Nicolas Morisset)

### Removed
- Remove the FORCE for CMAKE_*_INSTALL_SUFFIX. Elements only uses lib64 for x86_64,
  but in Fedora we also have aarch64, ppc64, and s390x. For packaging there, we need
  to be able to override the destination (Alejandro Alvarez Ayllon)
- remove the __linux macro that is deprecated (Alejandro Alvarez Ayllon).

### Fixed
- Fix the packaging of byte-compiled python files for FC30. The byte compilation
  is note done for files outside of the /usr prefix
- CTestXML2HTML.py compatible with Python 2 (Alejandro Alvarez Ayllon)
- Sonar and doxygen warning fixes
- https://euclid.roe.ac.uk/issues/12302. On rpm generation, Elements searches
  the CMAKE_PREFIX_PATH for ElementsToolChain.cmake (Frederic Leroux)
- Fix the initial copy of the files in the main doc sub-directory It was done
  at configure time which prevented the proper update detection by make. Now
  it's done within the sphinx target with a genuinedependency.
- Replace the imp module by the importlib module https://euclid.roe.ac.uk/issues/12377
  (Tristan Grégoire).
- Fix issue https://euclid.roe.ac.uk/issues/12390. INCLUDE_DIRS not
  written by generation scripts (Nicolas Morisset)


## [5.8] - 2019-07-08
Integration of the DataSync service (Antoine Basset). A new ElementsServices module.

### Added
- Add the New DataSync service. (Antoine Basset)
- Add function to copy and configure an aux file 
  (ElementsKernel.Auxiliary.configure)
- Install a .editorconfig file when creating a new project.
- Add new common exit codes for python (ElementsKernel.Exit)
- Add the USE_ENV_FLAGS cmake switch. It allows the environment variables CXXFLAGS
  and CFLAGS to be used by the builder. The default is OFF and these are 
  ignored.
- Add the USE_RPM_CMAKE_MACRO cmake switch. It forces the RPM spec file to use the 
  %__cmake RPM macro for the build. 
    - By default it is OFF.
    - If is ON, it implies that USE_ENV_FLAGS=ON for consistency with some platform
    RPM build scripts.
- Add a CMake option (ELEMENTS_DEFAULT_LOGLEVEL) to set the default log level for 
  Elements proper messages at compile time. The possible values are
    - INFO: this keeps the same behavior and this is the default for the unsqueeded 
    (Euclid-like) installation.
    - DEBUG: this hides the internal Elements messages when running the built executables.
    This is the default for the squeezed installation and the original message can be
    recovered by using the --log-level=DEBUG option of the executable.
- Add an example that opens explicitly a FITS file

### Changed
- Make the `--config-file` option crash if the file is not present
  It was already the case for the python executables. Now a non-existing
  configuration file passed explicitly on the command line will crash and
  burn.
- Implement an EXCLUDE option for the Automatic Python Testing.
- Move all the test data from the source tree to the auxdir.

### Fixed
- Fix warnings from the compilation of SWIG and Cython generated sources with
  the clang++ compiler 


## [5.6] - 2019-04-12

### Added
- LICENSE.md: add LGPL license file
- cmake/modules/FindRPM.cmake: add the CMake module for the RPM executable
- Add support for the KEEPTEMPDIR env variable for C++. Like for python, the
  presence of this environment variable will avoid the self-destruction
  of the temporary files and directories.
- ElementsExamples/src/program/FloatPrecisionExample.cpp: Add example to exhibit
  the floating points characteristics. The float, double and long double properties
  are printed.
- Add the elements_include_directories CMake macro.
  Dependending on the HIDE_SYSINC_WARNINGS cmake option (OFF by default), the
  include directories which are not starting with one of the CMAKE_PROJECT_PATH
  entries are added with the SYSTEM option. In that case, the "-isystem" gcc command
  line is used instead of the traditional "-I". The warnings coming from these
  directories are thenignored.
- Add the HIDE_OTHERINC_WARNINGS CMake option. It prevent any compilation warnings
  coming not only from the system (or third party) libraries, but also from other
  Elements-based project as well.
- Add fix for the version of rpmbuild >= 4.14. Fix the handling of the byte-compiled
  files in the rpm creation process.
- ElementsKernel/ElementsKernel/Off64Type.h: Add header file to define off64_t on
  both Linux and MacOSX.
- Add Travis build configuration file.
- Add the extern declaration for the instantiated templates.
- cmake/modules/FindLibM.cmake: Add CMake module to locate the "m" library. Especially
  usefully for C code
- Add support for bool on logging of options (Alejandro Álvarez Ayllón).
- Add the VCS version of the project. This is the number that is produced
  by the "--version" of the executables. It boils down to the git tag if it exists.
  Otherwise use a timestamp like 20190329. If git is not present, it falls back
  to the CMake version of the project.
- Add info make target. It is generating a cmake script called cmake_info.cmake
  that has to be called like "cmake -P cmake_info.cmake". for the moment the following
  CMake informations are provided:
    - the project name
    - the project version
    - the source directory
    - the module list
    - the include directory list
    - the project dependencies
    - the C++ test list with their respective executables.
- ElementsKernel/ElementsKernel/Main.h: Add new macros that allows the passing of
  arguments to the constructor of the program manager.

### Changed
- Generalize the usage of the KEEPTEMPDIR variable. It can also be used for
  temporary files and the name of the variable can be chosen in the constructor
- improve the messages in the CMake configure step (Alejandro Álvarez Ayllón).
- ElementsKernel/ElementsKernel/Main.h: Factor out the creation of the program
  manager into a separate macro.
- Test the existence of sphinxcontrib.napoleon before enabling it.
- Inhibit the python byte-compilation for CVMFS in squeezed install mode. The script
  that is called by rpmbuild cannot handle a python executable which is not installed
  in /usr/bin.
- Factor out the creation of the compiled test executable. It allows to create
  a unit executable of CppUnit or Boost type without adding it to the list of
  tests. This is useful to delay the usage of the executable in an other project
  that runs some integration tests.
- make/Elements.mk: Append the CMAKEFLAGS instead of prepending it. It allows
  to override some of the hardcoded CMake flags from the make library.
- Make the finding of Sphinx REQUIRED if USE_SPHINX is set to ON.
- Use SVG instead of PNG for the dot graphs in doxygen.

### Removed
- Remove the DB name check.


## [5.4] - 2018-06-04


### Added 
- Add support for the numpy include dirs. This is located in the cmake/modules/FindNumPy.cmake
  file.
- Add example for the SWIG interface to numpy.
- Add a new script called GetElementsFile to located any runtime Elements
  resource. Run like:
  ```
  E-Run <project_name> <project_version> GetElementsFile -t executable  
  ```
  in order to get the executable on the path. Please have a look at the
  "--help" option.
- Feature #5886: Add a gitignore template for the creation script that generates an
  Elements-based project
- Feature #5887: Add a --yes option to bypass the interactive part in the creation
  scripts.
- add a new "cov" make target. This generates a local coverage for the C++ executables.
  Notes:
    - the commands to run it are: make; make test; make cov
    - the BINARY_TAG has to be the cov one (e.g. x86_64-co7-gcc48-cov)
    - the gcov, genhtml, gcovr, lcov tools have to be installed.
- Implementation of the installation and packaging of the documentation
  Notes:
    - A separate RPM file is generated
    - everything is controlled by the INSTALL_DOC CMake option. It is set to OFF by
      default.
    - If INSTALL_DOC ist set to ON, the documentation is automatically built
      together with the default target (all)
- Add the CONCEPT_CHECKS CMake option. This add the -D_GLIBCXX_CONCEPT_CHECKS
  compile macro. This activate some internal check from the std c++ library. See
  https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_macros.html. It is OFF by
  default.

### Changed
- Reduce the top level wrapper Makefile to a minimum. The main library
  is located in the make/Elements.mk file of the framework. The lookup of
  that file is done through the CMAKE_PREFIX_PATH environment variable and
  thus the EuclidEnv package has to be installed on the machine.
- Use python3 executable explicitly when calling python scripts.
- Adapt the CMake code for the new layout of the HealpixCpp distribution
- Reenable the Elements data modules ad hoc support. Please have a look at the user
  manual.

### Fixed 
- Fix for the version detection of the GCC 7 series and above. The "-dumpversion"
  option is not providing the full version anymore.
- Bug #6131 fixed: Now the configuration file is created under the <conf>
  directory and not under <conf/module_name>
- Bug #7881 (Nikos/Mher/Hugo): the source hardcoded default values were overriding the
  ones provides in the configuration files.
  

## [5.2.2] - 2017-08-09
This release is a collection of emergency fixes done on top of the 5.2.1 release. Since the develop branch has diverged since that version, the 5.2.2 tag has been placed on a dedicated support branch based on 5.2.1.

### Added
- Add an example for the SWIG interface to numpy
- Add support for the NumPy include directories
- Add the lookup of the Xsd include directories

### Fixed
- Fix the explicit version of the python interpreter for NumPy on CVMFS.
- Fix the import of future_builtins to work with python 3


## [5.2.1] - 2017-08-09
This version is mainly designed to integrate the EDEEN 2.0 environment. Most of the modifications are done to integrate the _combination_ of 
* python 3
* the usage of python 3 and all the other overridden external software from the /cvmfs/euclid.in2p3.fr/EDEN*/.../usr location
* the RPM packaging for Elements-based software rooted at the /cvmfs/euclid.in2p3.fr/EDEN*/.../opt/euclid location
* the limited usage of the CVMFS-located RPM database for the projects.
Together with this Elements release, the EuclidEnv 3.3 helper package is released to interface the original Linux system with the CVMFS-installed software. It takes into account the various changes needed by the modifications mentioned above.

### Added
- Add the new CPACK_REMOVE_SYSTEM_DEPS CMake option to avoid dragging system dependencies into the CVMFS RPM database.
- Pass a new (`--no-warn-unused-cli`) option to CMake in order to avoid the "unused" CMake options/variable warnings. This is passed through the top wrapper Makefile
- Add options to use distcc and ccache for the build of the rpm. These options are CPACK_USE_CCACHE and CPACK_USE_DISTCC. They are @OFF@ by default.
- Add options to use the sanitize feature of gcc. 
    - `-DSANITIZE_OPTIONS=ON` activates the feature. The default is `OFF`. 
    - `-DSANITIZE_STYLE=leak` choose the sanitizer. The default is the `undefined `. The undefined checker. Please note that the 
      corresponding sanitizer library must be installed for this feature to work.
- Add experimental support for Cython
- Add option to forward the CMAKE_PREFIX_PATH to the RPM creation. The option is called RPM_FORWARD_PREFIX_PATH and it is ON by default.
* Add the getConfigurationLocations and getAuxiliaryLocations functions. They take into account the /usr/share locations and they are the recommended way to access configuration and auxiliary files. 

### Changed
- Use the Elements module name as search possibitlity for conf. The first default tried is the name of the executable with a ".conf" extension. If not found,  `<module>/<executable>.conf` is tried as a fallback.

### Fixed
- Fix the test on the version of sphinx.
- Generate the correct new layout for the compiled py files for python 3 in the RPM
- Fix the usage of the `HEAD` keyword for the project version. This is a usual substitute for sliding development versions.
- Fix the lookup order in the Environment XML files. There was a bug where the system directories where looked up first.
- Fix issue with incompatible `-pg` and `-pie` gcc options for executables. It is somehow related  with the glibc 2.17. The fix will only be used if the build is "Profile" and the gcc version is less that 5.0. The problem appears on CentOS 7 (and not on Fedora) when linking executables.
- Bug fixed: #2305 (Nicolas Morisset)
- Fix a bug which was giving priority to the options from the configuration file over the ones from the command line (Nikos Apostolakos)


## [5.2] - 2017-08-08

### Fixed
- fix C++ include directories ordering.


## [5.1] - 2017-07-28

### Added
- Add the generation of the current Elements module header file
- Add the getConfigurationLocations and getAuxiliaryLocations functions. They take
- Add the --elements-module-name and --elements-module-version options for the python
  script creation.
- Add option to forward the CMAKE_PREFIX_PATH to the RPM creation. The option is called
  RPM_FORWARD_PREFIX_PATH and it is ON by default.
- Add option to forward the CMAKE_PREFIX_PATH to the RPM creation
- Add experimental support for Cython
- Add options to use the sanitize feature of gcc. "-DSANITIZE_OPTIONS=ON" activate
  the feature. The default is OFF. "-DSANITIZE_STYLE=leak" choose the sanitizer.
  The default is the "undefined". The undefined checker. Please note that the
  corresponding sanitizer library must be installed for this feature to work.
- Add options to use distcc and ccache for the build of the rpm. These options are
  CPACK_USE_CCACHE and CPACK_USE_DISTCC. They are OFF by default.
- Add the -s option for the project and module creation. it allows to remove the
  implicit dependencies onto Elements and ElementsKernel.
- Pass option to avoid warnings about unused CMake options/variables.
- add new cmake option CPACK_REMOVE_SYSTEM_DEPS to remove system dependencies. Very
  useful when building agains a CVMFS-based RPM db.
- Add correct version of the compiled py files for python 3 in the rpm.

### Changed
- Use the new getConfigurationPath function to locate the conf files.
  into account the /usr/share locations.
- Use the Elements module name as search possibitlity for conf. First default tried
  is the name of the executable with a ".conf" extension. If not found,
  <module>/<executable>.conf is tried as a fallback.

### Fixed
- Fix a bug which was giving priority to the options from the configuration file over
  the ones from the command line (Nikos Apostolakos)
- Bug fixed: https://euclid.roe.ac.uk/issues/2305 (Nicolas Morisset)
- Fix the description parsing in the elements_project macro
- Fix issue with wrong default argument for the construction of Program
- Fix issue with incompatible -pg and -pie for executable. It is somehow related
  with the glibc 2.17. the fix will only be used if the build is "Profile" and
  the gcc version is less that 5.0. The problem appears on CentOS 7 (and not on
  Fedora) when linking executables.
- Fix the lookup order in the Environment XML files. There was a bug where the
  system directories where looked up first.
- Fix the usage of the "HEAD" keyword for the project version.


## [5.0.1] - 2017-04-06

### Fixed
- Fix the SPEC file generation for the squeezed installation.


## [5.0] - 2017-03-06


### Added
- The message from the exceptions is now shown after the stack trace.
- Add name check functionality for the creation scripts. CMake (project and modules),
  libraries and executables must be uniquely named (each). It relies on an URL pointed
  by the ELEMENTS_NAMING_DB_URL environment variable. The ElementsNameCheck script
  can be called directly.
- Add the squeezed install option:
    - It performs the installation in the standard CMAKE_INSTALL_PREFIX location (/usr/local
    by default for CMake). It is not multi-versioned
    - The build with the top wrapper Makefile has the -DSQUEEZED_INSTALL=OFF and thus stays in
    the local multiversioned directory layout
    - The build without the wrapper Makefile (with a direct configuration through CMake) is
    performed with the squeezed installation.
- Re-add the Napoleon Sphinx extension (Nikos Apostolakos)
    - the newest version is embedded in sphinx itself (since version 1.3)
    - before it was an external contribution.
- Add the usage of the cppreference doxygen tagfile
- Use SOFTWARE_BASE_VAR for the environment variable for base install
    - falls back on EULID_BASE if it exists
    - the final fallback value is /opt/euclid
- Implementation of the ElementsKernel/Configuration.py, ElementsKernel/Auxiliary.py,
  ElementsKernel/Configuration.h, ElementsKernel/Auxiliary.h files that implement:
    - getConfigurtionPath: to get the configuration file in the form "my_prog.conf" or
    "prefix/my_prog.conf"
    - getAuxiliaryPath: to get the auxiliary file in the form "my_aux" or "prefix/my_aux"

### Changed
- Doxygen now processes *.md files as well.
- Change the test executable name generated by AddCppClass to contain the _test
  prefix (Nikos Apostolakos)
- Conform to the -DCMAKE_BUILD_TYPE=... directive on the command line.
    - If it is passed the internal BINARY_TYPE is set accordingly
    - The types to be passed are the canonical ones:  Release, Debug,
    Coverage, Profile, RelWithDebInfo, MinSizeRel.
    - The type to be passed are not case-sensitive.
    - the corresponding BINARY_TAG extensions are: opt, dbg, cov, pro, o2g,min
    - If the BINARY_TAG is in the environment, it is used and the
    CMAKE_BUILD_TYPE is ignored.
- Change the RPM kits naming
    - The squeezed versions get <project>-<version>-1.x86_64.rpm
    - The non-squeezed versions get <project>_<version>-1.0-1.x86_64.rpm
    - The toolchain is also not used if the main build is not using it.
- Make the -Wfloat-equal warning optional (it is OFF by default)

### Fixed
- Fix the dependency onto a project with a 0 patch version.
- Fix the stream operator usage with Exception subclasses (Nikos Apostolakos)
- Fix AddPythonProgram script which was creating an "scripts" sub-directory. The later was
  then pruned by the "git clone" command and futher builds were failing.


## [4.1] - 2016-12-05

### Added
- Add backtrace feature. It displays the stack trace for the crashes. It equips
  the binary built ontop of the ElementsKernel/Program.h class and there is an
  example in the ElementsExamples module. This is especially helpful for the
  forensic study of production crashes.
- Add python ElementsKernel.Temporary.TempEnv class for using a temporary
  environment that will be forgotten after the destruction of the instance.
- Add C++ Elements::TempEnv class that has to same function has the one above
  in python. Very useful for the creation of elaborate tests.
- Add 2 CMake options, SPHINX_BUILD_OPTIONS and SPHINX_APIDOC_OPTIONS ot be
  added to the CMAKEFLAGS environment variable if needed.
- Add script to generate <project>_INSTALL.h header file and python <project>_INSTALL.py
  module that contain the install location.
- Add full RPATH handling for the install location.
- Add a flag to activate C++14 (ELEMENTS_CPP14). It is OFF by default.
- Add a flag to ignore warnings issued by external included files (like Eigen3
  for example). The flag is called HIDE_SYSINC_WARNINGS and it uses -isystem instead
  of -I for the included external path. It is OFF by default.
- Add the possibility to pass boolean option to the python scripts.
  This is the redmine issue #2708 (Marco Frailis)
- Add the CXX_SUGGEST_OVERRIDE option. It warns about overriding virtual
  functions that are not marked with the override keyword.
- Add the possibility to add an offset for the install location at install time only (for experts only). .
- Add a pure C program example: ElementsExamples/src/CProgram/cutout.c
- Add extra arguments passing to the rpmbuild (RPMBUILD_EXTRA_ARGS).
- Add a full description of the various Elements build flags to the Wiki at
  http://euclid.roe.ac.uk/projects/elements/wiki/GlobalSwitches.

### Changed
- New version of the documentation generation system
- Make the python and internal script compatible with python 3.

### Fixed
- Fix valgrind issues for unbalanced new/delete of smart pointers
- Fix the error reporting of the "make tests" command. Now it returns the exit
  code to the calling shell.
- Fix Python 3 compatibility issues.


## [4.0] - 2016-03-22
This release is foreseen for the Phosphoros version 0.5 project. It
also include the first integration of the Sphinx/Doxygen implementation.

### Added
- Add FindCCache, FindDistCC, FindSphinx, FindValgrind, FindPlantUML CMake modules
- Implement the generation of both Doxygen and Sphinx documentation
- Add the ChangeLog content to the generated SPEC file. The ChangeLog file (this file)
  must be located at the root of the project.
- Add a simple SWIG Python/C++ generation example.
- Add new RemoveCppClass, RemovePythonModule, RemovePythonProgram,
  RemoveCppProgram scripts (Nicolas Morisset).
- Add the CFITSIO_IS_REENTRANT CMake variable. It is computed during the
  configuration and then can be used further in the CMakeLists.txt files
- Add a timeout feature for the pure python tests. This is done by adding the
  TEST_TIMEOUT argument in seconds to the elements_install_python_modules CMake function.
- Add global options to enable Doxygen (USE_DOXYGEN) and Sphinx (USE_SPHINX) for
  the automatic documentation generation. They are both ON by default. The is also
  an option for the generation of the Sphinx API documentation (USE_SPHINX_APIDOC)
  which is also ON by default. Please have a look at the user manual for more informations
  about the usage of these switches.

### Changed
- Make the dependency check between projects mandatory. It will break if there
  is an inconsistency in the tree of projects.
- Update the compilation flags to support security hardening. Please have a
  look at this page https://fedoraproject.org/wiki/Changes/Harden_All_Packages
  for more information.
- Make the getAuxPathFile(file_name) python function generic. It can be used to
  locate resources pointed by the directory list in the ELEMENTS_AUX_PATH
  environment variable (Nicolas Morisset).
- Clean up of the main Elements Doxygen page.

### Fixed
- Fix the building of the RPMs file for a custom (general) build directory.
  It doesn't require that the build directory is placed inside the source directory.



## [3.10] - 2016-02-09
This release is essentially an update for the release of the EDEN 1.2 version.

### Added
- Add CMake build support for various libraries and tools:
    - GSL (issue #1907)
    - HealpixCpp
    - WCS
    - FFTW
    - PyXB
    - Xsd
- Add trivial examples to check that the build works if the following libraries
  are found (their build is conditional):
    - Eigen
    - FFTW
    - GSL
    - HealpixCpp
    - WCS
    - Xerces
- Add full support for the GCC 5 compiler series.
- Make the Log4CPP dependency mandatory.

### Fixed
- Fix a few issues for the symbol visibility and CMake 3.3
- Cure redundant sub-namespaces.
- Fix the CMake detection of the binary executables used at build time. The casing
  of the of the name of the find module file must be the same as the one used for
  the FIND_PACKAGE_HANDLE_STANDARD_ARGS macro.


## [3.9] - 2016-01-22
This is a release targeted for the ISDC December 2015 Phosphoros Workshop. This release is 
done together with the 1.14 version of the EuclidEnv standalone package.

### Added
- Implement the possibility of having generated aux directories (auxdir)
- Add the main creation helper scripts: CreateElementsProject, AddElementsModule, AddCppProgram, AddCppClass, AddPythonProgram, AddPythonModule (Nicolas Morisset)

### Fixed
- Fix the build for the MacPort distribution


## [0.0.0] - 2016-01-01

### Added

### Changed

### Deprecated

### Removed

### Fixed

### Security
