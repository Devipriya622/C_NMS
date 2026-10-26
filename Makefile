
BUILD_DIR = build
.PHONY: all configure build test coverage valgrind clean

all: test

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON

build: configure
	cmake --build $(BUILD_DIR) --parallel

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

coverage: test
	lcov --capture --directory $(BUILD_DIR) --output-file $(BUILD_DIR)/coverage.info --ignore-errors mismatch
	lcov --remove $(BUILD_DIR)/coverage.info '*/_deps/*' '*/tests/*' '/usr/*' --output-file $(BUILD_DIR)/coverage_filtered.info --ignore-errors unused
	genhtml $(BUILD_DIR)/coverage_filtered.info --output-directory $(BUILD_DIR)/coverage_html

clean:
	cmake -E rm -rf $(BUILD_DIR)


valgrind: build
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./build/nms_gtest
