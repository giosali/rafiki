# Source directory containing CMakeLists.txt
SRC_DIR := tests

# Build directory
BUILD_DIR := $(SRC_DIR)/build

# Default build type (Debug/Release)
BUILD_TYPE ?= Debug

# Targets
.PHONY: test clean rebuild

test: $(BUILD_DIR)
	cmake -S $(SRC_DIR) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $(BUILD_DIR)
	./$(BUILD_DIR)/test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean test