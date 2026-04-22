# ============================================================
# Temperature Monitor - Real-Time Build Configuration
# ============================================================

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2 -g
CFLAGS += -Wno-unused-parameter

# Include paths
CPPFLAGS = -I src -I src/hal -I src/app -I src/drivers

# Source directory
SRC_DIR = src

# Source files
SOURCES = \
    $(SRC_DIR)/main.c \
    $(SRC_DIR)/app/monitor_temperature.c \
    $(SRC_DIR)/drivers/mock_adc.c \
    $(SRC_DIR)/drivers/mock_eeprom.c \
    $(SRC_DIR)/drivers/mock_led.c \
    $(SRC_DIR)/drivers/mock_timer.c

# Output
BUILD_DIR = build
TARGET = $(BUILD_DIR)/temp_monitor

# ============================================================
# Targets
# ============================================================

.PHONY: all clean run docs

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@
	@echo ""
	@echo "Build complete: $(TARGET)"
	@echo "Run with: make run"

run: $(TARGET)
	@echo ""
	./$(TARGET)

docs: 
	plantuml docs/diagrams/*.puml

clean:
	rm -rf $(BUILD_DIR)
	@echo "Clean complete"