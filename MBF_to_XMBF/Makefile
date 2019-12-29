CXX           = g++
CXXFLAGS      = -Wall

#note: use "pkg-config libxml++-2.6 --cflags --libs" to determine includes and libs for libxml++

#32 bit
#INCPATH       = -I/usr/include/libxml++-2.6 -I/usr/lib/libxml++-2.6/include -I/usr/include/libxml2 -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include

#64 bit
INCPATH       = -I/usr/include/libxml++-2.6 -I/usr/lib64/libxml++-2.6/include -I/usr/include/libxml2 -I/usr/include/glibmm-2.4 -I/usr/lib64/glibmm-2.4/include -I/usr/include/sigc++-2.0 -I/usr/lib64/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include

LIBS          = -lm -lxml++-2.6 -lxml2 -lglibmm-2.4 -lgobject-2.0 -lsigc-2.0 -lglib-2.0

LINK          = g++
LFLAGS        =
DEL_FILE      = rm -f

TARGET        = MBF_to_XMBF

SOURCES       = main.cpp \
		string_tools.cpp \
		xml_tools.cpp \
		settingsmap.cpp

OBJECTS       = main.o \
		string_tools.o \
		xml_tools.o \
		settingsmap.o

$(TARGET):  $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)


main.o: 	main.cpp \
		string_tools.h \
		xml_tools.h \
		settingsmap.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

string_tools.o: string_tools.cpp string_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o string_tools.o string_tools.cpp

xml_tools.o: xml_tools.cpp xml_tools.h string_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o xml_tools.o xml_tools.cpp

settingsmap.o: settingsmap.cpp settingsmap.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o settingsmap.o settingsmap.cpp

.PHONY : clean
clean :
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~
