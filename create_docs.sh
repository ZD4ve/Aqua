
#!/bin/bash
cd "$(dirname "$0")"
cd Aqua
set -e

echo 'Building documentation'
echo $'-------------------------------------------------\n\n'
doxygen Doxyfile

echo $'\n\n'
echo 'Creating pdf'
echo $'-------------------------------------------------\n\n'
cd docs/latex
make > /dev/null
cd ../../..
mv Aqua/docs/latex/refman.pdf Documentation.pdf
rm -r Aqua/docs

echo $'\n\n'
echo 'Done'
echo $'-------------------------------------------------\n\n'