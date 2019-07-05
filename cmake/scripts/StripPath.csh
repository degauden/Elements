
if (! ${?ELEMENTS_NO_STRIP_PATH} ) then
set scriptdir=${ELEMENTS_PROJECT_ROOT}/cmake/scripts

  set tmpfile = `python $scriptdir/PathStripper.py --shell=csh --mktemp -e PATH -e LD_LIBRARY_PATH -e PYTHONPATH -e ELEMENTS_AUX_PATH -e ELEMENTS_CONF_PATH -e HPATH `
  source $tmpfile
  rm -f $tmpfile
  unset tmpfile
  unset scriptdir
endif

