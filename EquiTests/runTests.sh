SUCCESS=0
EQUI=$1

RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'
YELLOW='\033[0;33m'

shift

echo -e "${YELLOW}Using interpreter at: $EQUI ${NOCOLOR}"
echo -e "${YELLOW}With arguments: $@${NOCOLOR}"

for test in *.equi; do
  echo -e "${YELLOW}Beginning test $test${NOCOLOR}"
  if $EQUI $test $@; then
    echo -e "${GREEN}Test passed${NOCOLOR}"
  else
    echo -e "${RED}Test failed${NOCOLOR}"
    SUCCESS=1
  fi    
done

echo -e "${YELLOW}Tests complete, returning error code of: $SUCCESS${NOCOLOR}"

exit $SUCCESS

