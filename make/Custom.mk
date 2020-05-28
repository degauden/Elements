# Looking a some custom Makefile according to the CUSTOM_MAKEFILE environment variable.

CURRENT_MAKEFILE_LIBRARY := $(lastword $(MAKEFILE_LIST))

$(info Using the $(CURRENT_MAKEFILE_LIBRARY) make library)

ifneq (${CUSTOM_MAKEFILE},)

  $(info Looking for the ${CUSTOM_MAKEFILE} make file)

  ifneq ($(wildcard ${CUSTOM_MAKEFILE}),)
    # checking if it is an absolute file. It has priority.
    CUSTOM_MAKEFILE_FILE := ${CUSTOM_MAKEFILE}
  else
    # lookup in the common places
    ifneq ($(wildcard $(CURDIR)/make/${CUSTOM_MAKEFILE}),)
      CUSTOM_MAKEFILE_FILE := $(CURDIR)/make/${CUSTOM_MAKEFILE}
    else
      ifneq ($(CMAKE_PREFIX_PATH),)
        PREFIX_LIST := $(subst :, ,$(CMAKE_PREFIX_PATH))
        CUSTOM_MAKEFILE_LIST := $(foreach dir,$(PREFIX_LIST),$(wildcard $(dir)/share/Elements/make/${CUSTOM_MAKEFILE} $(dir)/../make/${CUSTOM_MAKEFILE}))
      endif
      CUSTOM_MAKEFILE_LIST += /usr/share/Elements/make/${CUSTOM_MAKEFILE}
      CUSTOM_MAKEFILE_FILE := $(firstword $(CUSTOM_MAKEFILE_LIST))
    endif
  endif

  -include $(CUSTOM_MAKEFILE_FILE)

endif
