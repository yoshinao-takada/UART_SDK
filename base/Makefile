INCFLAGS = -I..
DEBUG_CCFLAGS = -c -g $(INCFLAGS)
RELEASE_CCFLAGS = -c -O2 $(INCFLAGS)
DEBUG_TARGET_DIR = ../debug
RELEASE_TARGET_DIR = ../release
DEBUG_OBJ_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_OBJ_DIR = $(RELEASE_TARGET_DIR)/o

DEBUG_OBJECTS = $(DEBUG_OBJ_DIR)/BLts.o $(DEBUG_OBJ_DIR)/BLthread.o
RELEASE_OBJECTS = $(RELEASE_OBJ_DIR)/BLts.o $(RELEASE_OBJ_DIR)/BLthread.o

DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libbase.a
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libbase.a

# ----------- debug -----------------------
.PHONY	:	debug
debug	:	$(DEBUG_TARGET)
$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	ar r $@ $^
	ranlib $@

$(DEBUG_OBJ_DIR)/BLts.o	:	BLts.c BLts.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLthread.o	:	BLthread.c BLthread.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

.PHONY	:	clean_debug
clean_debug	:
	rm -f $(DEBUG_OBJECTS)
	rm -f $(DEBUG_TARGET)
	
# ----------- release -----------------------
.PHONY	:	release
release	:	$(RELEASE_TARGET)
$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(RELEASE_TARGET_DIR)
	ar r $@ $^
	ranlib $@

$(RELEASE_OBJ_DIR)/BLts.o	:	BLts.c BLts.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLthread.o	:	BLthread.c BLthread.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

.PHONY	:	clean_release
clean_release	:
	rm -f $(RELEASE_OBJECTS)
	rm -f $(RELEASE_TARGET)
