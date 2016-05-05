cd ..
DESTHOST=goetzpf,pyexpander@frs.sourceforge.net
DESTPATH=/home/frs/project/p/py/pyexpander/deb
scp deb_dist/*.deb $DESTHOST:$DESTPATH
scp deb_dist/*.dsc $DESTHOST:$DESTPATH
