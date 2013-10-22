ddt
===

Building DDT from source

== Linux ==
You will need
- git
- clang or gcc
- Qt5.1.1
- qmake
- subversion
- gnu make

- bison flex python-setuptools python-dev autoconf libtool zlib1g-dev

svn checkout http://or-tools.googlecode.com/svn/trunk/ or-tools-read-only
make third_party
make install_python_modules
