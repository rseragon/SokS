echo ""
echo "Running Unit tests"

VALGRIND='valgrind'

t="tests/test.run"

if test -f $t
then
  if $VALGRIND ./$t 2>>tests/test.log
  then
    echo $t PASS
  else
    echo "Error in test $t"
    echo "Here's test.log"
    tail tests/test.log
    exit 1
  fi
fi

echo ""
