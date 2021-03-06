#!/usr/bin/env python
"""
setup.py file for pymsi.

See http://docs.python.org/install
on how to use setup.py
"""

from distutils.core import setup
import os.path
import subprocess
import glob

join= os.path.join

# note: the following version number should be equal with the one
# shown by expander.py:
my_version= "1.5.10"

doc_dir= join("share","doc","pyexpander-%s" % my_version)
doc_html_dir= join(doc_dir, "html")

# a collection of all html files that should be part of the
# distribution:
html_files= glob.glob(join("doc","*.html"))
html_files.extend(glob.glob(join("doc","*.css")))

setup(name='pyexpander',
      version= my_version,
      description='a powerful macro processing language',
      author='Goetz Pfeiffer',
      author_email='Goetz.Pfeiffer@helmholtz-berlin.de',
      url='http://pyexpander.sourceforge.net',
      py_modules= ['expanderparser','pyexpander'],
      # the data_files parameter is especially needed for the 
      # rpm file generation:
      data_files=[(doc_dir, ['README', 'LICENSE']),
                  (doc_html_dir, html_files),
                 ],
      license= "GPLv3",
      scripts=['expander.py', 'msi2pyexpander.py'],
     )
