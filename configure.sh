
# make or-tools
mkdir -p deps
cd deps
svn checkout http://or-tools.googlecode.com/svn/trunk/ or-tools-read-only
cd or-tools-read-only
make third_party
make cplibs
# remove shared objects to enforce static linking
rm -v lib/*.so lib/*.so.*
cd dependencies/install/lib
rm -v *.so *.so.*
