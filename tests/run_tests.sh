VALGRIND='valgrind'

if !(command -v $VALGRIND > /dev/null 2>&1)
then
  echo "[ERROR] valgrind not found, stopping tests..."
  exit
fi

echo ""
echo "Running Unit tests"


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
