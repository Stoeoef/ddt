
# make or-tools
mkdir deps
cd deps
svn checkout http://or-tools.googlecode.com/svn/trunk/ or-tools-read-only
cd or-tools-read-only
make third_party
make cplibs
