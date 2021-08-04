INCFLAGS=-I..
DEBUG_CCFLAGS = -c -g $(INCFLAGS) -Wno-unknown-pragmas
RELEASE_CCFLAGS = -c -O2 $(INCFLAGS) -Wno-unknown-pragmas
DEBUG_TARGET_DIR = ../debug
RELEASE_TARGET_DIR = ../release
DEBUG_OBJ_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_OBJ_DIR = $(RELEASE_TARGET_DIR)/o

DEBUG_OBJECTS = \
	$(DEBUG_OBJ_DIR)/BLgetline.o \
	$(DEBUG_OBJ_DIR)/BLmultiwin.o \
	$(DEBUG_OBJ_DIR)/BLmultiwinfw.o \
	$(DEBUG_OBJ_DIR)/BLsubwin.o

RELEASE_OBJECTS = \
	$(RELEASE_OBJ_DIR)/BLgetline.o \
	$(RELEASE_OBJ_DIR)/BLmultiwin.o \
	$(RELEASE_OBJ_DIR)/BLmultiwinfw.o \
	$(RELEASE_OBJ_DIR)/BLsubwin.o

DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libcui.a
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libcui.a

# ------------- debug -----------------
.PHONY	:	debug
debug	:	$(DEBUG_TARGET)
$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	ar r $@ $^
	ranlib $@

$(DEBUG_OBJ_DIR)/BLgetline.o	:	BLgetline.c BLgetline.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLmultiwin.o	:	BLmultiwin.c BLmultiwin.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLmultiwinfw.o	:	BLmultiwinfw.c BLmultiwinfw.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLsubwin.o	:	BLsubwin.c BLsubwin.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<


# ------------- release -----------------
.PHONY	:	release
release	:	$(RELEASE_TARGET)
$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(RELEASE_TARGET_DIR)
	ar r $@ $^
	ranlib $@

$(RELEASE_OBJ_DIR)/BLgetline.o	:	BLgetline.c BLgetline.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLmultiwin.o	:	BLmultiwin.c BLmultiwin.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLmultiwinfw.o	:	BLmultiwinfw.c BLmultiwinfw.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLsubwin.o	:	BLsubwin.c BLsubwin.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<
