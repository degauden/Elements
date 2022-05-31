
#[=======================================================================[.rst:
HelloWorldExample
-------------------

This outputs Hello World with a name and a given severity level.

.. cmake:command:: hello_world_example

  .. code-block:: cmake

    hello_world_example(NAME [SEVERITY])

  Output "Hello World <NAME>" with the given severity level.
  ``SEVERITY`` is optional. Setting the :cmake:variable:`HELLO_WORLD_SEVERITY` variable prior to calling,
  that severity value is used.

#]=======================================================================]


# NOTE: This is just a comment which is not added to the generated documentation.

# Default value for the Severity
set(HELLO_WORLD_SEVERITY "STATUS")

function(hello_world_example NAME)
    set(severity ${HELLO_WORLD_SEVERITY})

    set(options )
    set(oneValueArgs SEVERITY)
    set(multiValueArgs )
    cmake_parse_arguments(hello_world_example "${options}" "${oneValueArgs}"
            "${multiValueArgs}" ${ARGN} )
    if(hello_world_example_SEVERITY)
        set(severity ${hello_world_example_SEVERITY})
    endif()
    message(${severity} "Hello World ${NAME}!")

endfunction()
