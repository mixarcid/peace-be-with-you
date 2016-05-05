#!/bin/sh
cd ../doc
echo "cd /home/project-web/pyexpander/htdocs
put docStyle.css
put expander.html
put expanderparser.html
put index.html
put msi2pyexpander.html
put pyexpander.html
put python.html
put Pyexpander.html
exit" | sftp -b - goetzpf@web.sourceforge.net
