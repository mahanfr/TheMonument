CC = gcc
CFLAGS = -Wall -Wextra
INCLUDE_PATHS = -Iraylib/include
ifeq ($(OS),Windows_NT)
    LDFLAGS = -Lraylib/libs/win64
else
    # LDFLAGS = -Lraylib/libs/linux
endif
ifeq ($(OS),Windows_NT)
	LDLIBS = -lraylib -lwinmm -lopengl32 -lgdi32 -lcomdlg32 -lws2_32
else
	LDLIBS = -lpthread -lraylib -lm
endif
MK_FILE_PATH = $(abspath $(lastword $(MAKEFILE_LIST)))
PROJECT_DIR := $(dir $(MK_FILE_PATH))
SRC_DIR = $(PROJECT_DIR)src
PROGRAM_ENTRY = $(PROJECT_DIR)main.c
BUILD_DIR := $(PROJECT_DIR)build
TARGET_NAME := game
TARGET = $(BUILD_DIR)/$(TARGET_NAME)

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC = $(call rwildcard,$(SRC_DIR),*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

.PHONY: game clean

game: always $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE_PATHS) -c $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	@ echo DONE. CREATED $@

always:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
