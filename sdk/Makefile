DEBUG_CC_FLAGS = -c -I.. -g -Wno-unknown-pragmas
RELEASE_CC_FLAGS = -c -I.. -O3 -Wno-unknown-pragmas
DEBUG_TARGET_DIR = ../debug
RELEASE_TARGET_DIR = ../release
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libUASDK.a
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libUASDK.a
DEBUG_OBJECT_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_OBJECT_DIR = $(RELEASE_TARGET_DIR)/o
DEBUG_OBJECTS = \
	$(DEBUG_OBJECT_DIR)/UASDKpkt.o \
	$(DEBUG_OBJECT_DIR)/UASDKiobase.o \
	$(DEBUG_OBJECT_DIR)/UASDKpktio.o
RELEASE_OBJECTS = \
	$(RELEASE_OBJECT_DIR)/UASDKpkt.o \
	$(RELEASE_OBJECT_DIR)/UASDKiobase.o \
	$(RELEASE_OBJECT_DIR)/UASDKpktio.o

.PHONY	:	debug

debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p  $(DEBUG_TARGET_DIR)
	ar r $@ $^
	ranlib $@

$(DEBUG_OBJECT_DIR)/UASDKpkt.o	:	UASDKpkt.c UASDKpkt.h
	mkdir -p  $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/UASDKiobase.o	:	UASDKiobase.c UASDKiobase.h
	mkdir -p  $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/UASDKpktio.o	:	UASDKpktio.c UASDKpktio.h
	mkdir -p  $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/UASDKiofw.o	:	UASDKiofw.c UASDKiofw.h
	mkdir -p  $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJECT_DIR)/UASDKiofw_defaults.o	:	UASDKiofw_defaults.c UASDKiofw_defaults.h
	mkdir -p  $(DEBUG_OBJECT_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

.PHONY	:	clean_debug
clean_debug	:
	rm -rf $(DEBUG_TARGET)
	rm -rf $(DEBUG_OBJECTS)


