engine:
	@mkdir -p build
	cd build && cmake .. && make

	rm -rf /usr/include/WarpEngine
	rm -rf /usr/lib/libWarpEngine.so
	cp -r include/WarpEngine/ /usr/include
	cp -r build/libWarpEngine.so /usr/lib/libWarpEngine.so


	g++ main.cpp -Lbuild/ -lWarpEngine -O3 -Wall -fPIC -g -w -lGL -lGLEW -lGLU -lglfw -o export/WarpEngine -Iinclude/ -O3
	export/WarpEngine
app:
	g++ main.cpp -Lbuild/ -lWarpEngine -lGL -lGLU -lglfw -o export/WarpEngine -Iinclude/ -O3
	export/WarpEngine
clean:
	rm -rf build/
	rm -rf obj/
