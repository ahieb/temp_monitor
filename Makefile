# ============================================================
# Temperature Monitor - Real-Time Build Configuration
# ============================================================

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O3 -flto -DDEBUG 

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

# Test configuration
TEST_DIR = test
TEST_BUILD_DIR = $(BUILD_DIR)/test
TEST_TARGET = $(TEST_BUILD_DIR)/test_led_fsm
TEST_CPPFLAGS = $(CPPFLAGS) -I$(TEST_DIR)/Unity/src -I$(TEST_DIR)

# Test sources (Unity framework included separately)
TEST_SOURCES = \
    $(TEST_DIR)/unit/test_led_fsm.c \
    $(SRC_DIR)/drivers/mock_adc.c \
    $(SRC_DIR)/drivers/mock_eeprom.c \
    $(SRC_DIR)/drivers/mock_led.c \
    $(SRC_DIR)/drivers/mock_timer.c \
	$(TEST_DIR)/Unity/src/unity.c

# ============================================================
# Targets
# ============================================================

.PHONY: all clean run docs test

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

test: $(TEST_TARGET)
	@./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES) | $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_CPPFLAGS) $^ -o $@

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)