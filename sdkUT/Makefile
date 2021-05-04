INCFLAGS = -I..
DEF_WORK_DIR = -DWORK_DIR=\"../../WorkData\"
DEBUG_CC_FLAGS = $(INCFLAGS) -c -g -D_DEBUG -Wno-unknown-pragmas $(DEF_WORK_DIR)
RELEASE_CC_FLAGS = $(INCFLAGS) -c -O3 -DNDEBUG -Wno-unknown-pragmas $(DEF_WORK_DIR)
DEBUG_TARGET_DIR = ../debug
RELEASE_TARGET_DIR = ../release
DEBUG_OBJECT_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_OBJECT_DIR = $(RELEASE_TARGET_DIR)/o
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/sdkUT.exe
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/sdkUT.exe
DEBUG_LIBS = -L$(DEBUG_TARGET_DIR) -lUASDK -lbase -pthread
RELEASE_LIBS = -L$(RELEASE_TARGET_DIR) -lUASDK -lbase -pthread
DEBUG_OBJECTS = \
	$(DEBUG_OBJECT_DIR)/ut_main.o \
	$(DEBUG_OBJECT_DIR)/pkt.o \
	$(DEBUG_OBJECT_DIR)/iobase.o
RELEASE_OBJECTS = \
	$(RELEASE_OBJECT_DIR)/ut_main.o \
	$(RELEASE_OBJECT_DIR)/pkt.o \
	$(DEBUG_OBJECT_DIR)/iobase.o
.PHONY	:	debug
debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS) ../debug/libUASDK.a
	$(CC) -o $@ $^ $(DEBUG_LIBS)

$(DEBUG_OBJECT_DIR)/ut_main.o	:	ut_main.c
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/pkt.o	:	pkt.c
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/iobase.o	:	iobase.c
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

../debug/libUASDK.a	:
	cd ../sdk && $(MAKE) debug

.PHONY	:	clean_debug
clean_debug	:
	cd ../sdk && $(MAKE) $@
	rm -f $(DEBUG_OBJECTS)
	rm -f $(DEBUG_TARGET)
