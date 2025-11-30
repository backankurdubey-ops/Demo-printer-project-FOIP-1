all:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build -- -j$(shell nproc)

check: all
	ctest --test-dir build --output-on-failure || true

clean:
	rm -rf build
