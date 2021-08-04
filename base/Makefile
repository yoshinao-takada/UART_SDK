INCFLAGS = -I..
DEBUG_CCFLAGS = -c -g $(INCFLAGS) -Wno-unknown-pragmas
RELEASE_CCFLAGS = -c -O2 $(INCFLAGS) -Wno-unknown-pragmas
DEBUG_TARGET_DIR = ../debug
RELEASE_TARGET_DIR = ../release
DEBUG_OBJ_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_OBJ_DIR = $(RELEASE_TARGET_DIR)/o

DEBUG_OBJECTS = \
	$(DEBUG_OBJ_DIR)/BLts.o \
	$(DEBUG_OBJ_DIR)/BLthread.o \
	$(DEBUG_OBJ_DIR)/BLringbuf.o \
	$(DEBUG_OBJ_DIR)/BLcrc.o \
	$(DEBUG_OBJ_DIR)/BLbsearch.o \
	$(DEBUG_OBJ_DIR)/BLheapdebug.o \
	$(DEBUG_OBJ_DIR)/BLfile.o \
	$(DEBUG_OBJ_DIR)/BLprocess.o \
	$(DEBUG_OBJ_DIR)/BLsystick.o \
	$(DEBUG_OBJ_DIR)/BLdispatcher.o \
	$(DEBUG_OBJ_DIR)/BLsettings.o
RELEASE_OBJECTS = \
	$(RELEASE_OBJ_DIR)/BLts.o \
	$(RELEASE_OBJ_DIR)/BLthread.o \
	$(RELEASE_OBJ_DIR)/BLringbuf.o \
	$(RELEASE_OBJ_DIR)/BLcrc.o \
	$(RELEASE_OBJ_DIR)/BLbsearch.o \
	$(RELEASE_OBJ_DIR)/BLheapdebug.o \
	$(RELEASE_OBJ_DIR)/BLfile.o \
	$(RELEASE_OBJ_DIR)/BLprocess.o \
	$(RELEASE_OBJ_DIR)/BLsystick.o \
	$(RELEASE_OBJ_DIR)/BLdispatcher.o \
	$(RELEASE_OBJ_DIR)/BLsettings.o
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

$(DEBUG_OBJ_DIR)/BLringbuf.o	:	BLringbuf.c BLringbuf.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLcrc.o	:	BLcrc.c BLcrc.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLbsearch.o	:	BLbsearch.c BLbsearch.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLheapdebug.o	:	BLheapdebug.c BLheapdebug.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -w -o $@ $<

$(DEBUG_OBJ_DIR)/BLfile.o	:	BLfile.c BLfile.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLprocess.o	:	BLprocess.c BLprocess.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BLsystick.o	:	BLsystick.c BLsystick.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -std=gnu99 -o $@ $<

$(DEBUG_OBJ_DIR)/BLdispatcher.o	:	BLdispatcher.c BLdispatcher.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) -std=gnu99 -o $@ $<

$(DEBUG_OBJ_DIR)/BLsettings.o	:	BLsettings.c BLsettings.h
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

$(RELEASE_OBJ_DIR)/BLringbuf.o	:	BLringbuf.c BLringbuf.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLcrc.o	:	BLcrc.c BLcrc.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLbsearch.o	:	BLbsearch.c BLbsearch.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLheapdebug.o	:	BLheapdebug.c BLheapdebug.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -w -o $@ $<

$(RELEASE_OBJ_DIR)/BLfile.o	:	BLfile.c BLfile.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLprocess.o	:	BLprocess.c BLprocess.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

$(RELEASE_OBJ_DIR)/BLsystick.o	:	BLsystick.c BLsystick.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -std=gnu99 -o $@ $<

$(RELEASE_OBJ_DIR)/BLdispatcher.o	:	BLdispatcher.c BLdispatcher.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -std=gnu99 -o $@ $<

$(RELEASE_OBJ_DIR)/BLsettings.o	:	BLsettings.c BLsettings.h
	mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

.PHONY	:	clean_release
clean_release	:
	rm -f $(RELEASE_OBJECTS)
	rm -f $(RELEASE_TARGET)
