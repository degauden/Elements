
if (! ${?GAUDI_NO_STRIP_PATH} ) then
  set scriptdir=$GAUDIPOLICYROOT/scripts

  set tmpfile = `python $scriptdir/PathStripper.py --shell=csh --mktemp -e PATH -e LD_LIBRARY_PATH -e PYTHONPATH -e JOBOPTSEARCHPATH -e HPATH `
  source $tmpfile
  rm -f $tmpfile
  unset tmpfile
  unset scriptdir
endif

