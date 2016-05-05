#!/bin/sh

pydoc -w ../expanderparser.py
# creates expanderparser.html

PYTHONPATH=..:$PYTHONPATH pydoc -w ../pyexpander.py
# creates pyexpander.html

echo "<pre>" > expander.html
python ../expander.py -h >> expander.html
echo "</pre>" >> expander.html
echo "<pre>" > msi2pyexpander.html
python ../msi2pyexpander.py -h >> msi2pyexpander.html
echo "</pre>" >> msi2pyexpander.html

RSTOPTS="--stylesheet-path=docStyle.css --cloak-email-addresses"

rst2html $RSTOPTS python.rst > python.html
rst2html $RSTOPTS Pyexpander.rst > Pyexpander.html
rst2html $RSTOPTS index.rst > index.html

