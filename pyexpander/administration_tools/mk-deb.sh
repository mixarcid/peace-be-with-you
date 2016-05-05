#!/bin/sh
cd ..
python setup.py --command-packages=stdeb.command bdist_deb
