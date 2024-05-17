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
cd doc/latex
make >/dev/null
cd ../../..
mv Aqua/doc/latex/refman.pdf "Docs/Prog documentation.pdf"
rm -r Aqua/doc

echo $'\n\n'
echo 'Merging'
echo $'-------------------------------------------------\n\n'
pdftk "Docs/Quck Start Guide.pdf" "Docs/Prog documentation.pdf" cat output Docs/final.pdf

echo $'\n\n'
echo 'Done!'
echo $'-------------------------------------------------\n\n'
