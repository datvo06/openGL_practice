all: librply.so
	g++ -o libctmmesh.so common.cpp 3ds.cpp ctm.cpp dae.cpp image.cpp lwo.cpp mesh.cpp meshio.cpp obj.cpp off.cpp ply.cpp stl.cpp wrl.cpp pnglite/pnglite.c -ltinyxml -fpermissive -I. -Ipnglite -lopenctm -L. -ljpeg -fPIC -shared -std=c++11 -lrply -Lrply -I../include -I./tinyxml -I./rply -L../lib
librply.so:
	cd rply;\
	gcc rply.c -o librply.so -O2 -W -fPIC -shared -std=c++11 -fpermissive -I.;\
	cd ..;
