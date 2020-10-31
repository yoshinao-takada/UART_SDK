INCFLAGS = -I.
DEBUG_CCFLAGS = -c -g
RELEASE_CCFALGS = -c -O2
DEBUG_TARGET_DIR = ../debug
RELEASE_TARGET_DIR = ../release
DEBUG_OBJ_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_OBJ_DIR = $(RELEASE_TARGET_DIR)/o
LIBS = -lpthread

DEBUG_OBJECTS = $(DEBUG_OBJ_DIR)/test0.o \
	$(DEBUG_OBJ_DIR)/UASDK_setup.o \
	$(DEBUG_OBJ_DIR)/UASDK_ascii.o \
	$(DEBUG_OBJ_DIR)/UASDK_buffer.o

RELEASE_OBJECTS = $(RELEASE_OBJ_DIR)/test0.o \
	$(RELEASE_OBJ_DIR)/UASDK_setup.o \
	$(RELEASE_OBJ_DIR)/UASDK_ascii.o \
	$(RELEASE_OBJ_DIR)/UASDK_buffer.o

DEBUG_TARGET = $(DEBUG_TARGET_DIR)/test0
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/test0

debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	$(CC) -o $@ $^ $(LIBS)

$(DEBUG_OBJ_DIR)/test0.o	:	test0.c UASDK_setup.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_setup.o	:	UASDK_setup.c UASDK_setup.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_ascii.o	:	UASDK_ascii.c UASDK_ascii.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_buffer.o	:	UASDK_buffer.c UASDK_buffer.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

clean_debug	:
	rm -rf $(DEBUG_TARGET_DIR)
