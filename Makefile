# Set build directory
BUILD_DIR = build

# Set Pico SDK path
PICO_SDK_PATH = /Users/alssdev/pico/pico-sdk
PICO_BOARD=pico_w

# Create build directory and run CMake
all: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DPICO_BOARD=pico_w .. && make -j4

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)

# Flash the project to the Pico using picotool
flash: all
	picotool load $(BUILD_DIR)/router.uf2

.PHONY: all clean flash

