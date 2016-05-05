#!/bin/sh

setup_version=`grep my_version= ../setup.py | sed -e 's/^.*"\([^"]\+\)".*/\1/'`

expander_version=`grep my_version= ../expander.py | sed -e 's/^.*"\([^"]\+\)".*/\1/'`

msi2pyexpander_version=`grep my_version= ../msi2pyexpander.py | sed -e 's/^.*"\([^"]\+\)".*/\1/'`

err="no"

if [ $setup_version != $expander_version ]; then err="yes"; fi
if [ $setup_version != $msi2pyexpander_version ]; then err="yes"; fi

if [ "$err" != "no" ]; then
        echo "error: setup.py version is         : $setup_version"
        echo "       msi2pyexpander.py version is: $msi2pyexpander_version"
        echo "       expander.py version is      : $expander_version"
        exit 1
fi
echo "version check OK"
exit 0

