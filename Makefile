CCC=/opt/homebrew/bin/g++-13

CCFLAGS = -std=c++11 -O3 -I /opt/homebrew/include

LIBS = /opt/homebrew/lib/libtiff.a  /opt/homebrew/lib/libturbojpeg.a \
    /opt/homebrew/lib/libz-ng.a /opt/homebrew/lib/libzlibstatic.a /opt/homebrew/lib/libzstd.a /opt/homebrew/lib/liblzma.a

all: doublewidth

clean:
	rm -rf *~ *# .??*
	rm -rf doublewidth

doublewidth: doublewidth.cpp
	$(CCC) $(CCFLAGS) doublewidth.cpp -o doublewidth $(LIBS)
