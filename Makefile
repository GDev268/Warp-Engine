engine:
	@mkdir -p build
	cd build && cmake .. && make

	rm -rf /usr/include/SpeackerEngine
	rm -rf /usr/lib/libSpeackerEngine.so
	cp -r include/ /usr/include
	cp -r build/libSpeackerEngine.so /usr/lib/libSpeackerEngine.so

app:
	g++ main.cpp -Lbuild/ -lSpeackerEngine -lGL -lGLU -lglfw -o export/SpeackerEngine -Iinclude/ -O3

