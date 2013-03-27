if [[ "x$GAUDI_NO_STRIP_PATH" == "x" ]] ; then
  scriptdir=$GAUDIPOLICYROOT/scripts

  tmpfile=`python $scriptdir/PathStripper.py --shell=sh --mktemp -e PATH -e LD_LIBRARY_PATH -e PYTHONPATH -e JOBOPTSEARCHPATH -e HPATH `
  . $tmpfile
  rm -f $tmpfile
  unset tmpfile
  unset scriptdir
fi
