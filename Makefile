CC = gcc
CFLAGS = -Wall -Wextra

LOOK_DIRS = . * */* */*/*

HEADERS = $(wildcard $(addsuffix /*.h, $(LOOK_DIRS)))
INCLUDE_DIRS = $(sort $(dir $(HEADERS)))
INCLUDES = $(addprefix -I, $(INCLUDE_DIRS))

CFLAGS += $(INCLUDES)

TARGET = test.out
SRC = test.c

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

debug:
	@echo "=== MAKEFILE DIAGNOSTICS ==="
	@echo "1. Current Directory Contents:"
	@ls -R || dir /s
	@echo ""
	@echo "2. Found Header Files (HEADERS):"
	@echo "   $(HEADERS)"
	@echo ""
	@echo "3. Generated Include Directories (INCLUDES):"
	@echo "   $(INCLUDES)"
	@echo "============================"

.PHONY: all clean debug

