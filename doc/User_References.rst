User References
===============


Global CMake Build Options
--------------------------


One can pass several options to the build system to tweak its behavior.
This is done by passing some special CMake variables (called CMake
options) to the build system.

There are 2 kinds of CMake options

-  temporary options, used to create a special build for debugging or
   testing,
-  permanent ones that changes the global build behavior. They usually
   imply changes in the source code itself.

For the first category, the temporary ones, the usual way to modify
these options is to use the CMAKEFLAGS environment variable. By changing
its value, the top wrapper Makefile of an Elements-based project passes
new option to the CMake underlying process.

The List of Options
~~~~~~~~~~~~~~~~~~~

+---------+---------+---------+---------+---------+---------+-----+
| Name    | Default | Type    | Desc    | Remark  | Since   |     |
|         | Value   |         | ription |         | Version |     |
+=========+=========+=========+=========+=========+=========+=====+
| ``EL    | ``OFF`` | Pe      | enable  | This    | N/A     |     |
| EMENTS_ |         | rmanent | e       | switch  |         |     |
| HIDE_SY |         |         | xplicit | is      |         |     |
| MBOLS`` |         |         | symbol  | exp     |         |     |
|         |         |         | vis     | licitly |         |     |
|         |         |         | ibility | ``ON``  |         |     |
|         |         |         | on      | for     |         |     |
|         |         |         | gcc-4.  | E       |         |     |
|         |         |         | With    | lements |         |     |
|         |         |         | that    | only    |         |     |
|         |         |         | flag,   |         |         |     |
|         |         |         | all     |         |         |     |
|         |         |         | symbols |         |         |     |
|         |         |         | of a    |         |         |     |
|         |         |         | library |         |         |     |
|         |         |         | are     |         |         |     |
|         |         |         | hidden  |         |         |     |
|         |         |         | by      |         |         |     |
|         |         |         | d       |         |         |     |
|         |         |         | efault. |         |         |     |
|         |         |         | They    |         |         |     |
|         |         |         | have to |         |         |     |
|         |         |         | be      |         |         |     |
|         |         |         | exp     |         |         |     |
|         |         |         | licitly |         |         |     |
|         |         |         | exposed |         |         |     |
|         |         |         | with    |         |         |     |
|         |         |         | the     |         |         |     |
|         |         |         | ``      |         |         |     |
|         |         |         | ELEMENT |         |         |     |
|         |         |         | S_API`` |         |         |     |
|         |         |         | macro   |         |         |     |
|         |         |         | defined |         |         |     |
|         |         |         | in the  |         |         |     |
|         |         |         | El      |         |         |     |
|         |         |         | ementsK |         |         |     |
|         |         |         | ernel/E |         |         |     |
|         |         |         | xport.h |         |         |     |
|         |         |         | file    |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``EL    | ``ON``  | Pe      | enable  | It is   | N/A /   |     |
| EMENTS_ | /       | rmanent | C++11   | only    | 5.12    |     |
| CPP11`` | ``OFF`` |         | l       | active  |         |     |
|         |         |         | anguage | for     |         |     |
|         |         |         | dialect | co      |         |     |
|         |         |         |         | mpilers |         |     |
|         |         |         |         | that    |         |     |
|         |         |         |         | support |         |     |
|         |         |         |         | it      |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``EL    | ``OFF`` | Pe      | enable  | It is   | N/A /   |     |
| EMENTS_ | /       | rmanent | C++14   | only    | 5.12    |     |
| CPP14`` | ``ON``  |         | l       | active  |         |     |
|         |         |         | anguage | for     |         |     |
|         |         |         | dialect | co      |         |     |
|         |         |         |         | mpilers |         |     |
|         |         |         |         | that    |         |     |
|         |         |         |         | support |         |     |
|         |         |         |         | it      |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``ELEME | ``OFF`` | Pe      | enable  | Experi  | N/A     |     |
| NTS_PAR |         | rmanent | C++11   | mental. |         |     |
| ALLEL`` |         |         | p       | U       |         |     |
|         |         |         | arallel | ntested |         |     |
|         |         |         | support |         |         |     |
|         |         |         | with    |         |         |     |
|         |         |         | OpenMP. |         |         |     |
|         |         |         | Please  |         |         |     |
|         |         |         | look    |         |         |     |
|         |         |         | `here < |         |         |     |
|         |         |         | https:/ |         |         |     |
|         |         |         | /gcc.gn |         |         |     |
|         |         |         | u.org/o |         |         |     |
|         |         |         | nlinedo |         |         |     |
|         |         |         | cs/libs |         |         |     |
|         |         |         | tdc++/m |         |         |     |
|         |         |         | anual/p |         |         |     |
|         |         |         | arallel |         |         |     |
|         |         |         | _mode.h |         |         |     |
|         |         |         | tml>`__ |         |         |     |
|         |         |         | for     |         |         |     |
|         |         |         | more    |         |         |     |
|         |         |         | infor   |         |         |     |
|         |         |         | mations |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``ELEM  | ``ON``  | Pe      | enable  | It is   | N/A     |     |
| ENTS_FO |         | rmanent | g       | only    |         |     |
| RTIFY`` |         |         | fortify | used    |         |     |
|         |         |         | option. | for the |         |     |
|         |         |         | The     | builds  |         |     |
|         |         |         | ``      | with    |         |     |
|         |         |         | -D_FORT | some    |         |     |
|         |         |         | IFY_SOU | kind of |         |     |
|         |         |         | RCE=2`` | optim   |         |     |
|         |         |         | option  | ization |         |     |
|         |         |         | is      | (``-    |         |     |
|         |         |         | passed  | O...``) |         |     |
|         |         |         | to the  |         |         |     |
|         |         |         | compi   |         |         |     |
|         |         |         | lation. |         |         |     |
|         |         |         | Please  |         |         |     |
|         |         |         | look    |         |         |     |
|         |         |         | `here < |         |         |     |
|         |         |         | https:/ |         |         |     |
|         |         |         | /access |         |         |     |
|         |         |         | .redhat |         |         |     |
|         |         |         | .com/bl |         |         |     |
|         |         |         | ogs/766 |         |         |     |
|         |         |         | 093/pos |         |         |     |
|         |         |         | ts/1976 |         |         |     |
|         |         |         | 213>`__ |         |         |     |
|         |         |         | for     |         |         |     |
|         |         |         | more    |         |         |     |
|         |         |         | d       |         |         |     |
|         |         |         | etailed |         |         |     |
|         |         |         | expla   |         |         |     |
|         |         |         | nations |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``US    | ``OFF`` | Te      | Use     | The top | N/A     |     |
| E_LOCAL |         | mporary | local   | wrapper |         |     |
| _INSTAL |         |         | Inst    | M       |         |     |
| LAREA`` |         |         | allArea | akefile |         |     |
|         |         |         | for the | of the  |         |     |
|         |         |         | Dev     | project |         |     |
|         |         |         | elopers | is      |         |     |
|         |         |         |         | always  |         |     |
|         |         |         |         | passing |         |     |
|         |         |         |         | ``      |         |     |
|         |         |         |         | -DUSE_L |         |     |
|         |         |         |         | OCAL_IN |         |     |
|         |         |         |         | STALLAR |         |     |
|         |         |         |         | EA=ON`` |         |     |
|         |         |         |         | to      |         |     |
|         |         |         |         | CMake   |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``BUILD | ``      | Te      | Prefix  | The top | N/A     |     |
| _PREFIX | build`` | mporary | name    | wrapper |         |     |
| _NAME`` |         |         | for the | M       |         |     |
|         |         |         | build   | akefile |         |     |
|         |         |         | di      | of the  |         |     |
|         |         |         | rectory | project |         |     |
|         |         |         |         | is      |         |     |
|         |         |         |         | always  |         |     |
|         |         |         |         | passing |         |     |
|         |         |         |         | `       |         |     |
|         |         |         |         | `-DBUIL |         |     |
|         |         |         |         | D_PREFI |         |     |
|         |         |         |         | X_NAME: |         |     |
|         |         |         |         | STRING= |         |     |
|         |         |         |         | build`` |         |     |
|         |         |         |         | to      |         |     |
|         |         |         |         | CMake.  |         |     |
|         |         |         |         | It      |         |     |
|         |         |         |         | creates |         |     |
|         |         |         |         | the     |         |     |
|         |         |         |         | ``      |         |     |
|         |         |         |         | build.$ |         |     |
|         |         |         |         | {BINARY |         |     |
|         |         |         |         | _TAG}`` |         |     |
|         |         |         |         | dir     |         |     |
|         |         |         |         | ectory. |         |     |
|         |         |         |         | If      |         |     |
|         |         |         |         | there   |         |     |
|         |         |         |         | is no   |         |     |
|         |         |         |         | BIN     |         |     |
|         |         |         |         | ARY_TAG |         |     |
|         |         |         |         | in the  |         |     |
|         |         |         |         | envir   |         |     |
|         |         |         |         | onment, |         |     |
|         |         |         |         | the     |         |     |
|         |         |         |         | build   |         |     |
|         |         |         |         | di      |         |     |
|         |         |         |         | rectory |         |     |
|         |         |         |         | becomes |         |     |
|         |         |         |         | just    |         |     |
|         |         |         |         | “``b    |         |     |
|         |         |         |         | uild``” |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``OPT_  | ``ON``  | Te      | Enable  | Full    | N/A     |     |
| DEBUG`` |         | mporary | optim   | de      |         |     |
|         |         |         | isation | bugging |         |     |
|         |         |         | for the | c       |         |     |
|         |         |         | Debug   | apacity |         |     |
|         |         |         | v       | r       |         |     |
|         |         |         | ersion. | equires |         |     |
|         |         |         | It adds | to set  |         |     |
|         |         |         | the     | this    |         |     |
|         |         |         | ``-Og`` | option  |         |     |
|         |         |         | for the | to      |         |     |
|         |         |         | debug   | ``OFF`` |         |     |
|         |         |         | build.  |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``ELEM  | ``OFF`` | Pe      | Enable  | Experi  | N/A     |     |
| ENTS_LI |         | rmanent | Link    | mental. |         |     |
| NKOPT`` |         |         | Time    | It will |         |     |
|         |         |         | Optim   | only be |         |     |
|         |         |         | isation | active  |         |     |
|         |         |         |         | for     |         |     |
|         |         |         |         | op      |         |     |
|         |         |         |         | timized |         |     |
|         |         |         |         | builds  |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``USE   | ``OFF`` | Pe      | Prefer  | By      | N/A     |     |
| _PYTHON |         | rmanent | the     | d       |         |     |
| _NOSE`` |         |         | Nose    | efault, |         |     |
|         |         |         | test    | Nose    |         |     |
|         |         |         | runner  | will    |         |     |
|         |         |         | instead | only be |         |     |
|         |         |         | of the  | used as |         |     |
|         |         |         | default | a       |         |     |
|         |         |         | Py.Test | f       |         |     |
|         |         |         | one     | allback |         |     |
|         |         |         |         | if      |         |     |
|         |         |         |         | Py.Test |         |     |
|         |         |         |         | is not  |         |     |
|         |         |         |         | found.  |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| `       | ``ON``  | Pe      | Use     | By      | 4.1     |     |
| `ELEMEN |         | rmanent | full    | d       |         |     |
| TS_USE_ |         |         | RPATH   | efault, |         |     |
| RPATH`` |         |         | for     | CMake   |         |     |
|         |         |         | both    | only    |         |     |
|         |         |         | build   | uses    |         |     |
|         |         |         | and     | RPATH   |         |     |
|         |         |         | insta   | for the |         |     |
|         |         |         | llation | local   |         |     |
|         |         |         |         | build   |         |     |
|         |         |         |         | and     |         |     |
|         |         |         |         | strips  |         |     |
|         |         |         |         | the     |         |     |
|         |         |         |         | RPATH   |         |     |
|         |         |         |         | during  |         |     |
|         |         |         |         | instal  |         |     |
|         |         |         |         | lation. |         |     |
|         |         |         |         | With    |         |     |
|         |         |         |         | this    |         |     |
|         |         |         |         | flag    |         |     |
|         |         |         |         | e       |         |     |
|         |         |         |         | nabled, |         |     |
|         |         |         |         | The     |         |     |
|         |         |         |         | RPATH   |         |     |
|         |         |         |         | is also |         |     |
|         |         |         |         | used at |         |     |
|         |         |         |         | install |         |     |
|         |         |         |         | time.   |         |     |
|         |         |         |         | It is   |         |     |
|         |         |         |         | r       |         |     |
|         |         |         |         | elative |         |     |
|         |         |         |         | for the |         |     |
|         |         |         |         | links   |         |     |
|         |         |         |         | of the  |         |     |
|         |         |         |         | same    |         |     |
|         |         |         |         | project |         |     |
|         |         |         |         | and     |         |     |
|         |         |         |         | a       |         |     |
|         |         |         |         | bsolute |         |     |
|         |         |         |         | for the |         |     |
|         |         |         |         | e       |         |     |
|         |         |         |         | xternal |         |     |
|         |         |         |         | ones.   |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``H     | ``OFF`` | Pe      | Hide    | Useful  | 4.1     |     |
| IDE_SYS |         | rmanent | System  | for     |         |     |
| INC_WAR |         |         | i       | li      |         |     |
| NINGS`` |         |         | ncludes | braries |         |     |
|         |         |         | w       | like    |         |     |
|         |         |         | arnings | Eigen3  |         |     |
|         |         |         | by      |         |         |     |
|         |         |         | using   |         |         |     |
|         |         |         | ``-is   |         |         |     |
|         |         |         | ystem`` |         |         |     |
|         |         |         | instead |         |         |     |
|         |         |         | of      |         |         |     |
|         |         |         | ``-I``  |         |         |     |
|         |         |         | for 3rd |         |         |     |
|         |         |         | party   |         |         |     |
|         |         |         | s       |         |         |     |
|         |         |         | oftware |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``C     | ``OFF`` | Pe      | Enable  | Warn    | 4.1     |     |
| XX_SUGG |         | rmanent | the     | about   |         |     |
| EST_OVE |         |         | `       | ove     |         |     |
| RRIDE`` |         |         | `-Wsugg | rriding |         |     |
|         |         |         | est-ove | virtual |         |     |
|         |         |         | rride`` | fu      |         |     |
|         |         |         | gcc     | nctions |         |     |
|         |         |         | warning | that    |         |     |
|         |         |         |         | are not |         |     |
|         |         |         |         | marked  |         |     |
|         |         |         |         | with    |         |     |
|         |         |         |         | the     |         |     |
|         |         |         |         | o       |         |     |
|         |         |         |         | verride |         |     |
|         |         |         |         | k       |         |     |
|         |         |         |         | eyword. |         |     |
|         |         |         |         | This is |         |     |
|         |         |         |         | a good  |         |     |
|         |         |         |         | but     |         |     |
|         |         |         |         | i       |         |     |
|         |         |         |         | nvasive |         |     |
|         |         |         |         | option  |         |     |
|         |         |         |         | that    |         |     |
|         |         |         |         | has to  |         |     |
|         |         |         |         | be      |         |     |
|         |         |         |         | decided |         |     |
|         |         |         |         | by the  |         |     |
|         |         |         |         | project |         |     |
|         |         |         |         | ma      |         |     |
|         |         |         |         | nagers. |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``USE_S | ``ON``  | Pe      | Use     | It will | 4.1     |     |
| PHINX`` |         | rmanent | sphinx  | only    |         |     |
|         |         |         | docume  | work if |         |     |
|         |         |         | ntation | the     |         |     |
|         |         |         | gen     | sphin   |         |     |
|         |         |         | eration | x-build |         |     |
|         |         |         |         | binary  |         |     |
|         |         |         |         | can be  |         |     |
|         |         |         |         | found   |         |     |
|         |         |         |         | by      |         |     |
|         |         |         |         | CMake   |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| `       | ``ON``  | Pe      | Use     | It will | 4.1     |     |
| `USE_DO |         | rmanent | doxygen | only    |         |     |
| XYGEN`` |         |         | docume  | work if |         |     |
|         |         |         | ntation | the     |         |     |
|         |         |         | gen     | doxygen |         |     |
|         |         |         | eration | binary  |         |     |
|         |         |         |         | can be  |         |     |
|         |         |         |         | found   |         |     |
|         |         |         |         | by      |         |     |
|         |         |         |         | CMake   |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``USE_S | ``ON``  | Pe      | Use     | This    | 4.1     |     |
| PHINX_A |         | rmanent | sphinx  | setting |         |     |
| PIDOC`` |         |         | API     | is      |         |     |
|         |         |         | docume  | po      |         |     |
|         |         |         | ntation | intless |         |     |
|         |         |         | gen     | if the  |         |     |
|         |         |         | eration | `       |         |     |
|         |         |         |         | `USE_SP |         |     |
|         |         |         |         | HINX fl |         |     |
|         |         |         |         | ag is o |         |     |
|         |         |         |         | n OFF`` |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| `       | ``OFF`` | Pe      | Use     | idem    | 4.1     |     |
| `USE_SP |         | rmanent | sphinx  |         |         |     |
| HINX_BR |         |         | C API   |         |         |     |
| EATHE`` |         |         | Breathe |         |         |     |
|         |         |         | plugin  |         |         |     |
|         |         |         | to bind |         |         |     |
|         |         |         | to      |         |         |     |
|         |         |         | doxygen |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``S     | (empty) | Te      | pass    |         | 4.1     |     |
| PHINX_B |         | mporary | some    |         |         |     |
| UILD_OP |         |         | extra   |         |         |     |
| TIONS`` |         |         | options |         |         |     |
|         |         |         | to the  |         |         |     |
|         |         |         | sphin   |         |         |     |
|         |         |         | x-build |         |         |     |
|         |         |         | exe     |         |         |     |
|         |         |         | cutable |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``SP    | (empty) | Te      | pass    |         | 4.1     |     |
| HINX_AP |         | mporary | some    |         |         |     |
| IDOC_OP |         |         | extra   |         |         |     |
| TIONS`` |         |         | options |         |         |     |
|         |         |         | to the  |         |         |     |
|         |         |         | sphinx  |         |         |     |
|         |         |         | -apidoc |         |         |     |
|         |         |         | exe     |         |         |     |
|         |         |         | cutable |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``CMAK  | ``OFF`` | Te      | Use     | Only    | N/A     |     |
| E_USE_C |         | mporary | ccache  | works   |         |     |
| CACHE`` |         |         | to      | if the  |         |     |
|         |         |         | speed   | ccache  |         |     |
|         |         |         | up      | exe     |         |     |
|         |         |         | comp    | cutable |         |     |
|         |         |         | ilation | is      |         |     |
|         |         |         |         | found   |         |     |
|         |         |         |         | by      |         |     |
|         |         |         |         | CMake   |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``CMAK  | ``OFF`` | Te      | Use     | Only    | N/A     |     |
| E_USE_D |         | mporary | distcc  | works   |         |     |
| ISTCC`` |         |         | to      | if the  |         |     |
|         |         |         | speed   | distcc  |         |     |
|         |         |         | up      | exe     |         |     |
|         |         |         | comp    | cutable |         |     |
|         |         |         | ilation | is      |         |     |
|         |         |         |         | found   |         |     |
|         |         |         |         | by      |         |     |
|         |         |         |         | CMake.  |         |     |
|         |         |         |         | This    |         |     |
|         |         |         |         | enables |         |     |
|         |         |         |         | the     |         |     |
|         |         |         |         | distr   |         |     |
|         |         |         |         | ibution |         |     |
|         |         |         |         | of the  |         |     |
|         |         |         |         | build   |         |     |
|         |         |         |         | over    |         |     |
|         |         |         |         | several |         |     |
|         |         |         |         | ma      |         |     |
|         |         |         |         | chines. |         |     |
|         |         |         |         | Please  |         |     |
|         |         |         |         | read    |         |     |
|         |         |         |         | the     |         |     |
|         |         |         |         | distcc  |         |     |
|         |         |         |         | docume  |         |     |
|         |         |         |         | ntation |         |     |
|         |         |         |         | first   |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``ELEM  | ``OFF`` | Te      | Use the | If the  | N/A     |     |
| ENTS_US |         | mporary | same    | option  |         |     |
| E_STRIC |         |         | build   | is      |         |     |
| T_BINAR |         |         | variant | `       |         |     |
| Y_DEP`` |         |         | (opt,   | `OFF``, |         |     |
|         |         |         | dbg,    | the     |         |     |
|         |         |         | o2g,    | same    |         |     |
|         |         |         | etc)    | build   |         |     |
|         |         |         | for the | variant |         |     |
|         |         |         | lookup  | is      |         |     |
|         |         |         | of the  | tried   |         |     |
|         |         |         | de      | first   |         |     |
|         |         |         | pendent | and     |         |     |
|         |         |         | p       | then it |         |     |
|         |         |         | roject. | falls   |         |     |
|         |         |         | St      | back to |         |     |
|         |         |         | rictly. | try the |         |     |
|         |         |         | The     | other   |         |     |
|         |         |         | co      | ones    |         |     |
|         |         |         | nfigure | (o2g,   |         |     |
|         |         |         | step    | opt,    |         |     |
|         |         |         | will    | dbg,    |         |     |
|         |         |         | break   | pro,    |         |     |
|         |         |         | if it   | cov,    |         |     |
|         |         |         | is not  | min in  |         |     |
|         |         |         | found.  | that    |         |     |
|         |         |         |         | order)  |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``E     | ``ON``  | Te      | Set to  |         | N/A     |     |
| LEMENTS |         | mporary | ``ON``  |         |         |     |
| _BUILD_ |         |         | to      |         |         |     |
| TESTS`` |         |         | disable |         |         |     |
|         |         |         | the     |         |         |     |
|         |         |         | build   |         |         |     |
|         |         |         | of the  |         |         |     |
|         |         |         | tests   |         |         |     |
|         |         |         | (li     |         |         |     |
|         |         |         | braries |         |         |     |
|         |         |         | and     |         |         |     |
|         |         |         | execut  |         |         |     |
|         |         |         | ables). |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``USE_D | ``OFF`` | Te      | Use     | It the  | N/A     |     |
| EFAULT_ |         | mporary | default | option  |         |     |
| RPMBUIL |         |         | RPM     | is set  |         |     |
| D_DIR`` |         |         | build   | to      |         |     |
|         |         |         | di      | `       |         |     |
|         |         |         | rectory | `OFF``, |         |     |
|         |         |         | (the    | the     |         |     |
|         |         |         | value   | default |         |     |
|         |         |         | of the  | l       |         |     |
|         |         |         | %       | ocation |         |     |
|         |         |         | _topdir | is set  |         |     |
|         |         |         | r       | to      |         |     |
|         |         |         | pmbuild | ``<p    |         |     |
|         |         |         | var     | roject> |         |     |
|         |         |         | iable). | /build. |         |     |
|         |         |         | You can | ${BINAR |         |     |
|         |         |         | get the | Y_TAG}/ |         |     |
|         |         |         | l       | Package |         |     |
|         |         |         | ocation | s/RPM`` |         |     |
|         |         |         | by      |         |         |     |
|         |         |         | calling |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``      | (empty) | Te      | Pass    |         | 4.1     |     |
| RPMBUIL |         | mporary | extra   |         |         |     |
| D_EXTRA |         |         | a       |         |         |     |
| _ARGS`` |         |         | rgument |         |         |     |
|         |         |         | to the  |         |         |     |
|         |         |         | r       |         |         |     |
|         |         |         | pmbuild |         |         |     |
|         |         |         | command |         |         |     |
|         |         |         | line    |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``SQUE  | ``ON``  | Te      | This    | The     | 5.0     |     |
| EZED_IN |         | mporary | option  | default |         |     |
| STALL`` |         |         | enables | value   |         |     |
|         |         |         | the     | is      |         |     |
|         |         |         | special | ``ON``  |         |     |
|         |         |         | insta   | for a   |         |     |
|         |         |         | llation | pure    |         |     |
|         |         |         | in a    | CMake   |         |     |
|         |         |         | pure    | build   |         |     |
|         |         |         | ``C     | (       |         |     |
|         |         |         | MAKE_IN | without |         |     |
|         |         |         | STALL_P | the top |         |     |
|         |         |         | REFIX`` | Mak     |         |     |
|         |         |         | lo      | efile). |         |     |
|         |         |         | cation. | If the  |         |     |
|         |         |         | Without | top     |         |     |
|         |         |         | the     | wrapper |         |     |
|         |         |         | project | M       |         |     |
|         |         |         | di      | akefile |         |     |
|         |         |         | rectory | is used |         |     |
|         |         |         | and the | the     |         |     |
|         |         |         | project | default |         |     |
|         |         |         | version | value   |         |     |
|         |         |         |         | is      |         |     |
|         |         |         |         | ``OFF`` |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``CPACK | ``OFF`` | Te      | This    | Useful  | 5.2.1   |     |
| _REMOVE |         | mporary | option  | with a  |         |     |
| _SYSTEM |         |         | removes | non-s   |         |     |
| _DEPS`` |         |         | the     | tandard |         |     |
|         |         |         | depen   | RPM     |         |     |
|         |         |         | dencies | d       |         |     |
|         |         |         | onto    | atabase |         |     |
|         |         |         | the     | lo      |         |     |
|         |         |         | system  | cation. |         |     |
|         |         |         | pa      |         |         |     |
|         |         |         | ckages. |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``CPAC  | ``OFF`` | Te      | Use     | Only    | 5.2.1   |     |
| K_USE_C |         | mporary | ccache  | works   |         |     |
| CACHE`` |         |         | to      | if the  |         |     |
|         |         |         | speed   | ccache  |         |     |
|         |         |         | up      | exe     |         |     |
|         |         |         | comp    | cutable |         |     |
|         |         |         | ilation | is      |         |     |
|         |         |         | for the | found   |         |     |
|         |         |         | p       | by      |         |     |
|         |         |         | ackages | CMake   |         |     |
|         |         |         | c       |         |         |     |
|         |         |         | reation |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``CPAC  | ``OFF`` | Te      | Use     | Only    | 5.2.1   |     |
| K_USE_D |         | mporary | distcc  | works   |         |     |
| ISTCC`` |         |         | to      | if the  |         |     |
|         |         |         | speed   | distcc  |         |     |
|         |         |         | up      | exe     |         |     |
|         |         |         | comp    | cutable |         |     |
|         |         |         | ilation | is      |         |     |
|         |         |         | for the | found   |         |     |
|         |         |         | p       | by      |         |     |
|         |         |         | ackages | CMake.  |         |     |
|         |         |         | c       | This    |         |     |
|         |         |         | reation | enables |         |     |
|         |         |         |         | the     |         |     |
|         |         |         |         | distr   |         |     |
|         |         |         |         | ibution |         |     |
|         |         |         |         | of the  |         |     |
|         |         |         |         | build   |         |     |
|         |         |         |         | over    |         |     |
|         |         |         |         | several |         |     |
|         |         |         |         | ma      |         |     |
|         |         |         |         | chines. |         |     |
|         |         |         |         | Please  |         |     |
|         |         |         |         | read    |         |     |
|         |         |         |         | the     |         |     |
|         |         |         |         | distcc  |         |     |
|         |         |         |         | docume  |         |     |
|         |         |         |         | ntation |         |     |
|         |         |         |         | first   |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``SANI  | ``OFF`` | Te      | A       | It has  | 5.2.1   |     |
| TIZE_OP |         | mporary | ctivate | to be   |         |     |
| TIONS`` |         |         | the     | su      |         |     |
|         |         |         | s       | pported |         |     |
|         |         |         | anitize | by the  |         |     |
|         |         |         | feature | version |         |     |
|         |         |         | of gcc  | of GCC. |         |     |
|         |         |         |         | Please  |         |     |
|         |         |         |         | also    |         |     |
|         |         |         |         | read    |         |     |
|         |         |         |         | the     |         |     |
|         |         |         |         | desc    |         |     |
|         |         |         |         | ription |         |     |
|         |         |         |         | and     |         |     |
|         |         |         |         | remark  |         |     |
|         |         |         |         | of the  |         |     |
|         |         |         |         | option  |         |     |
|         |         |         |         | below   |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``SA    | ``unde  | Te      | Sets    | The     | 5.2.1   |     |
| NITIZE_ | fined`` | mporary | the     | corres  |         |     |
| STYLE`` |         |         | s       | ponding |         |     |
|         |         |         | anitize | s       |         |     |
|         |         |         | style   | anitize |         |     |
|         |         |         | in use. | library |         |     |
|         |         |         | By      | has to  |         |     |
|         |         |         | default | be      |         |     |
|         |         |         | it is   | ins     |         |     |
|         |         |         | the     | talled. |         |     |
|         |         |         | ``unde  | For     |         |     |
|         |         |         | fined`` | e       |         |     |
|         |         |         | san     | xample, |         |     |
|         |         |         | itizer. | ``lib   |         |     |
|         |         |         | But     | ubsan`` |         |     |
|         |         |         | other   | is the  |         |     |
|         |         |         | are     | Un      |         |     |
|         |         |         | po      | defined |         |     |
|         |         |         | ssible: | B       |         |     |
|         |         |         | ``      | ehavior |         |     |
|         |         |         | leak``, | Sa      |         |     |
|         |         |         | ``add   | nitizer |         |     |
|         |         |         | ress``, | library |         |     |
|         |         |         | ``th    |         |         |     |
|         |         |         | read``. |         |         |     |
|         |         |         | Please  |         |         |     |
|         |         |         | have a  |         |         |     |
|         |         |         | look at |         |         |     |
|         |         |         | the GCC |         |         |     |
|         |         |         | documen |         |         |     |
|         |         |         | tation. |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``RPM_  | ``ON``  | Te      | Forward |         | 5.2.1   |     |
| FORWARD |         | mporary | the     |         |         |     |
| _PREFIX |         |         | CMA     |         |         |     |
| _PATH`` |         |         | KE_PREF |         |         |     |
|         |         |         | IX_PATH |         |         |     |
|         |         |         | to the  |         |         |     |
|         |         |         | RPM     |         |         |     |
|         |         |         | c       |         |         |     |
|         |         |         | reation |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``DATA  | ``${XD  | Pe      | Lookup  | data    | If the  | 5.4 |
| _MODULE | G_DATA_ | rmanent | path    | mo      | X       |     |
| _PATH`` | HOME}:$ |         | entries | dules]] | DG_DATA |     |
|         | {XDG_DA |         | for the |         | envi    |     |
|         | TA_DIRS |         | [[N     |         | ronment |     |
|         | }:/usr/ |         | ewUserM |         | va      |     |
|         | share`` |         | anual#D |         | riables |     |
|         |         |         | ata-Mod |         | don’t   |     |
|         |         |         | ules-si |         | exist,  |     |
|         |         |         | nce-ver |         | the     |     |
|         |         |         | sion-54 |         | default |     |
|         |         |         |         |         | is      |     |
|         |         |         |         |         | ``/usr/ |     |
|         |         |         |         |         | share`` |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``ELE   | `       | Pe      | Lookup  | data    |         | 5.4 |
| MENTS_D | `DBASE; | rmanent | path    | mo      |         |     |
| ATA_SUF | PARAM;E |         | s       | dules]] |         |     |
| FIXES`` | XTRAPAC |         | uffixes |         |         |     |
|         | KAGES`` |         | for the |         |         |     |
|         |         |         | [[N     |         |         |     |
|         |         |         | ewUserM |         |         |     |
|         |         |         | anual#D |         |         |     |
|         |         |         | ata-Mod |         |         |     |
|         |         |         | ules-si |         |         |     |
|         |         |         | nce-ver |         |         |     |
|         |         |         | sion-54 |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``PYTH  | (empty) | Pe      | use the | A       | N/A     |     |
| ON_EXPL |         | rmanent | `       | lthough |         |     |
| ICIT_VE |         |         | `python | it is   |         |     |
| RSION`` |         |         | ${PYTHO | meant   |         |     |
|         |         |         | N_EXPLI | to be   |         |     |
|         |         |         | CIT_VER | per     |         |     |
|         |         |         | SION}`` | manent, |         |     |
|         |         |         | exec    | it is   |         |     |
|         |         |         | utable. | passed  |         |     |
|         |         |         | Useful  | through |         |     |
|         |         |         | to      | the     |         |     |
|         |         |         | switch  | ``CMAKE |         |     |
|         |         |         | from    | FLAGS`` |         |     |
|         |         |         | python  | envi    |         |     |
|         |         |         | to      | ronment |         |     |
|         |         |         | python3 | v       |         |     |
|         |         |         |         | ariable |         |     |
|         |         |         |         | with    |         |     |
|         |         |         |         | the top |         |     |
|         |         |         |         | level   |         |     |
|         |         |         |         | M       |         |     |
|         |         |         |         | akefile |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``CO    | ``OFF`` | Te      | This    |         | 5.4     |     |
| NCEPT_C |         | mporary | add the |         |         |     |
| HECKS`` |         |         | `       |         |         |     |
|         |         |         | `-D_GLI |         |         |     |
|         |         |         | BCXX_CO |         |         |     |
|         |         |         | NCEPT_C |         |         |     |
|         |         |         | HECKS`` |         |         |     |
|         |         |         | compile |         |         |     |
|         |         |         | macro.  |         |         |     |
|         |         |         | This    |         |         |     |
|         |         |         | a       |         |         |     |
|         |         |         | ctivate |         |         |     |
|         |         |         | some    |         |         |     |
|         |         |         | i       |         |         |     |
|         |         |         | nternal |         |         |     |
|         |         |         | check   |         |         |     |
|         |         |         | from    |         |         |     |
|         |         |         | the std |         |         |     |
|         |         |         | c       |         |         |     |
|         |         |         | l       |         |         |     |
|         |         |         | ibrary. |         |         |     |
|         |         |         | See     |         |         |     |
|         |         |         | https:/ |         |         |     |
|         |         |         | /gcc.gn |         |         |     |
|         |         |         | u.org/o |         |         |     |
|         |         |         | nlinedo |         |         |     |
|         |         |         | cs/libs |         |         |     |
|         |         |         | tdc/man |         |         |     |
|         |         |         | ual/usi |         |         |     |
|         |         |         | ng_macr |         |         |     |
|         |         |         | os.html |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``U     | ``OFF`` | Pe      | It      |         | 5.8     |     |
| SE_ENV_ |         | rmanent | allows  |         |         |     |
| FLAGS`` |         |         | the     |         |         |     |
|         |         |         | envi    |         |         |     |
|         |         |         | ronment |         |         |     |
|         |         |         | va      |         |         |     |
|         |         |         | riables |         |         |     |
|         |         |         | ``CXX   |         |         |     |
|         |         |         | FLAGS`` |         |         |     |
|         |         |         | and     |         |         |     |
|         |         |         | ``C     |         |         |     |
|         |         |         | FLAGS`` |         |         |     |
|         |         |         | to be   |         |         |     |
|         |         |         | used by |         |         |     |
|         |         |         | the     |         |         |     |
|         |         |         | builder |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``      | ``OFF`` | Pe      | It      | The     | 5.8     |     |
| USE_RPM |         | rmanent | forces  | ``U     |         |     |
| _CMAKE_ |         |         | the RPM | SE_ENV_ |         |     |
| MACRO`` |         |         | spec    | FLAGS`` |         |     |
|         |         |         | file to | is set  |         |     |
|         |         |         | use the | to      |         |     |
|         |         |         | ``%__   | ``ON``  |         |     |
|         |         |         | cmake`` | when    |         |     |
|         |         |         | RPM     | this    |         |     |
|         |         |         | macro   | one is  |         |     |
|         |         |         | for the | enabled |         |     |
|         |         |         | build   |         |         |     |
|         |         |         | instead |         |         |     |
|         |         |         | of the  |         |         |     |
|         |         |         | plain   |         |         |     |
|         |         |         | ``      |         |         |     |
|         |         |         | cmake`` |         |         |     |
|         |         |         | command |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| `       | `       | Pe      | set the | the     | 5.8     |     |
| `ELEMEN | `INFO`` | rmanent | default | default |         |     |
| TS_DEFA |         |         | log     | is      |         |     |
| ULT_LOG |         |         | level   | ``      |         |     |
| LEVEL`` |         |         | for     | DEBUG`` |         |     |
|         |         |         | E       | for a   |         |     |
|         |         |         | lements | build   |         |     |
|         |         |         | proper  | in      |         |     |
|         |         |         | m       | s       |         |     |
|         |         |         | essages | queezed |         |     |
|         |         |         | at      | (non-   |         |     |
|         |         |         | compile | euclid) |         |     |
|         |         |         | time    | mode    |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``TES   | ``OFF`` | Te      | g       |         | 5.12    |     |
| T_XML_R |         | mporary | enerate |         |         |     |
| EPORT`` |         |         | XML     |         |         |     |
|         |         |         | reports |         |         |     |
|         |         |         | for the |         |         |     |
|         |         |         | boost   |         |         |     |
|         |         |         | tests   |         |         |     |
|         |         |         | (       |         |         |     |
|         |         |         | instead |         |         |     |
|         |         |         | of HRF) |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``U     | ``ON``  | Pe      | Use     |         | 5.12    |     |
| SER_SPH |         | rmanent | numpy   |         |         |     |
| INX_NUM |         |         | doc     |         |         |     |
| PYDOC`` |         |         | format  |         |         |     |
|         |         |         | for     |         |         |     |
|         |         |         | sphinx  |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``      | (empty) | Te      | set the | It has  | 5.14.0  |     |
| DEBUG_F |         | mporary | debug   | to be   |         |     |
| ORMAT`` |         |         | format  | used    |         |     |
|         |         |         | at      | with    |         |     |
|         |         |         | compile | `       |         |     |
|         |         |         | time    | `DEBUG_ |         |     |
|         |         |         | tuning  | LEVEL`` |         |     |
|         |         |         | the     | to form |         |     |
|         |         |         | ``-g``  | ``-g${D |         |     |
|         |         |         | gcc     | EBUG_FO |         |     |
|         |         |         | option  | RMAT}${ |         |     |
|         |         |         |         | DEBUG_L |         |     |
|         |         |         |         | EVEL}`` |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| `       | (empty) | Te      | set the | It has  | 5.14.0  |     |
| `DEBUG_ |         | mporary | debug   | to be   |         |     |
| LEVEL`` |         |         | level   | used    |         |     |
|         |         |         | at      | with    |         |     |
|         |         |         | compile | ``      |         |     |
|         |         |         | time    | DEBUG_F |         |     |
|         |         |         | tuning  | ORMAT`` |         |     |
|         |         |         | the     | to form |         |     |
|         |         |         | ``-g``  | ``-g${D |         |     |
|         |         |         | gcc     | EBUG_FO |         |     |
|         |         |         | option  | RMAT}${ |         |     |
|         |         |         |         | DEBUG_L |         |     |
|         |         |         |         | EVEL}`` |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| `       | ``ON``  | Pe      | add the |         | 5.14.0  |     |
| `GCOVR_ |         | rmanent | ``--ex  |         |         |     |
| EXCLUDE |         |         | clude-u |         |         |     |
| _UNREAC |         |         | nreacha |         |         |     |
| HABLE`` |         |         | ble-bra |         |         |     |
|         |         |         | nches`` |         |         |     |
|         |         |         | gcovr   |         |         |     |
|         |         |         | option  |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``      | ``ON``  | Pe      | add the |         | 5.14.0  |     |
| GCOVR_E |         | rmanent | ``--exc |         |         |     |
| XCLUDE_ |         |         | lude-th |         |         |     |
| THROW`` |         |         | row-bra |         |         |     |
|         |         |         | nches`` |         |         |     |
|         |         |         | gcovr   |         |         |     |
|         |         |         | option  |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+
| ``      | (empty) | Pe      | allows  |         | 5.14.0  |     |
| GCOVR_E |         | rmanent | extra   |         |         |     |
| XTRA_OP |         |         | options |         |         |     |
| TIONS`` |         |         | to be   |         |         |     |
|         |         |         | passed  |         |         |     |
|         |         |         | to the  |         |         |     |
|         |         |         | gcovr   |         |         |     |
|         |         |         | command |         |         |     |
+---------+---------+---------+---------+---------+---------+-----+

The Top Wrapper Makefile
~~~~~~~~~~~~~~~~~~~~~~~~


The top wrapper Makefile is located at the root of each Elements-based
project. It is designed to simply the command for the developer of the
project and it wraps most of the CMake/CTest commands into pain Makefile
target. Please note:

-  the Makefile of each project should **be updated each time that there
   is a new version of Elements**. It contains the last available
   features.
-  A non-permanent options is passed internally by default to the CMake
   command by the top wrapper Makefile: the
   ``-DUSE_LOCAL_INSTALLAREA=ON`` to create a local install area within
   the project tree.
-  Some extra user temporary options can be passed to the underlying
   CMake process by using the ``CMAKEFLAGS`` envrionment variable. For
   example:
   ::

      export CMAKEFLAGS="-DCMAKE_USE_CCACHE=YES"

Please note that plain CMake commands can also be used directly.

Adding Permanent Options to Your Project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In order to change the global behavior of the project build, **the main
CMakeLists.txt file has to be edited** in the following way.

::

   set(ELEMENTS_HIDE_SYMBOLS ON
       CACHE STRING "enable explicit symbol visibility on gcc-4"
       FORCE)

| 
| This enables for the current project (and the current project only),
  the symbols hiding in the generated libraries.

These lines have to be placed *after* the call to
``find_package(ElementsProject)`` and *before* the call to
``elements_project``.

Adding Temporary Options to Your Project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In order to change temporarily the behavior of the project build, 2
settings are possible:

-  add an option to the CMAKEFLAGS environment variables if your are
   using the top wrapper Makefile
-  pass directly the option to CMake if your are using it directly
