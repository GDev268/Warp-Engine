engine:
	@mkdir -p build
	cd build && cmake .. && make

	rm -rf /usr/include/WarpEngine
	rm -rf /usr/lib/libWarpEngine.so
	cp -r include/WarpEngine/ /usr/include
	cp -r build/libWarpEngine.so /usr/lib/libWarpEngine.so


	clang++ -std=c++20 main.cpp -Lbuild/ -lWarpEngine -O3 -Wall -Wextra -g -w -lvulkan -lglfw -o export/WarpEngine -Iinclude/ -O3
	export/WarpEngine

debug:
	@mkdir -p build
	cd build && cmake .. && make

	rm -rf /usr/include/WarpEngine
	rm -rf /usr/lib/libWarpEngine.so
	cp -r include/WarpEngine/ /usr/include
	cp -r build/libWarpEngine.so /usr/lib/libWarpEngine.so


	clang++ -std=c++20 -fsanitize=address -fno-omit-frame-pointer main.cpp -Lbuild/ -lWarpEngine -O3 -Wall -Wextra -g -w -lvulkan -lglfw -o export/WarpEngine -Iinclude/ -O3
	export/WarpEngine
app:
	clang++ -std=c++20 -fsanitize=address -fno-omit-frame-pointer main.cpp -Lbuild/ -lWarpEngine -o export/WarpEngine -Iinclude/ -O3
	export/WarpEngine
clean:
	rm -rf build/
	rm -rf obj/

uninstall:
	rm -rf /usr/include/WarpEngine
	rm -rf /usr/lib/libWarpEngine.so
