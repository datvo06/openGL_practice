all:
	g++ -std=c++11 -o window.exe hello_cubemap.cpp ../src/Shader.cpp\
	 	../src/stb_image.cpp \
		../src/CustomizedModelLoading/Mesh.cpp \
	 	../src/CustomizedModelLoading/Texture.cpp \
	 	../src/CustomizedModelLoading/Model.cpp \
		-I../include -L../lib \
		-lglfw3 -lGL -lGLU -lX11 -lpthread -lXrandr -lXi -lglad -ldl -ldatstringutils -lassimp
