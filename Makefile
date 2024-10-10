# Set build directory
BUILD_DIR = build

# Set Pico board
PICO_BOARD=pico

# Create build directory and run CMake
all: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DPICO_BOARD=$(PICO_BOARD) .. && make -j4

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)

# Flash the project to the Pico using picotool
flash: all
	picotool load $(BUILD_DIR)/main.uf2

.PHONY: all clean flash
