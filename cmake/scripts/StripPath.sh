if [[ "x$ELEMENTS_NO_STRIP_PATH" == "x" ]] ; then
 scriptdir=${ELEMENTS_PROJECT_ROOT}/cmake/scripts

tmpfile=`python $scriptdir/PathStripper.py --shell=sh --mktemp -e PATH -e LD_LIBRARY_PATH -e PYTHONPATH -e ELEMENTS_AUX_PATH -e ELEMENTS_CONF_PATH -e HPATH `
  . $tmpfile
  rm -f $tmpfile
  unset tmpfile
  unset scriptdir
fi
