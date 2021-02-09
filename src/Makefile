INCFLAGS = -I. -I..
DEBUG_CCFLAGS = -c -g
RELEASE_CCFALGS = -c -O2
DEBUG_TARGET_DIR = ../debug
RELEASE_TARGET_DIR = ../release
DEBUG_OBJ_DIR = $(DEBUG_TARGET_DIR)/o
RELEASE_OBJ_DIR = $(RELEASE_TARGET_DIR)/o
DEBUG_LIB_DIR = -L../debug
DEBUG_LIBS = $(DEBUG_LIB_DIR) -lbase -lpthread

DEBUG_OBJECTS = \
	$(DEBUG_OBJ_DIR)/UASDK_setup.o \
	$(DEBUG_OBJ_DIR)/UASDK_ascii.o \
	$(DEBUG_OBJ_DIR)/UASDK_buffer.o \
	$(DEBUG_OBJ_DIR)/UASDK_addressable.o \
	$(DEBUG_OBJ_DIR)/UASDK_thread.o \
	$(DEBUG_OBJ_DIR)/test_buffer.o \
	$(DEBUG_OBJ_DIR)/UASDK_string.o \
	$(DEBUG_OBJ_DIR)/test_UASDK_string.o \
	$(DEBUG_OBJ_DIR)/UASDK_i16array.o \
	$(DEBUG_OBJ_DIR)/test_UASDK_i16array.o \
	$(DEBUG_OBJ_DIR)/UASDK_r32array.o \
	$(DEBUG_OBJ_DIR)/test_UASDK_r32array.o \
	$(DEBUG_OBJ_DIR)/UASDK_packet.o

RELEASE_OBJECTS = \
	$(RELEASE_OBJ_DIR)/UASDK_setup.o \
	$(RELEASE_OBJ_DIR)/UASDK_ascii.o \
	$(RELEASE_OBJ_DIR)/UASDK_buffer.o \
	$(RELEASE_OBJ_DIR)/UASDK_addressable.o \
	$(RELEASE_OBJ_DIR)/UASDK_thread.o \
	$(RELEASE_OBJ_DIR)/test_buffer.o \
	$(RELEASE_OBJ_DIR)/UASDK_string.o \
	$(RELEASE_OBJ_DIR)/test_UASDK_string.o \
	$(RELEASE_OBJ_DIR)/UASDK_i16array.o \
	$(RELEASE_OBJ_DIR)/test_UASDK_i16array.o \
	$(RELEASE_OBJ_DIR)/UASDK_r32array.o \
	$(RELEASE_OBJ_DIR)/test_UASDK_r32array.o \
	$(RELEASE_OBJ_DIR)/UASDK_packet.o


DEBUG_TARGET = \
	$(DEBUG_TARGET_DIR)/test0 $(DEBUG_TARGET_DIR)/test1 $(DEBUG_TARGET_DIR)/test2 \
	$(DEBUG_TARGET_DIR)/test3
RELEASE_TARGET = \
	$(RELEASE_TARGET_DIR)/test0 $(RELEASE_TARGET_DIR)/test1 $(RELEASE_TARGET_DIR)/test2 \
	$(RELEASE_TARGET_DIR)/test3

.PHONY	:	debug
debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET_DIR)/libbase.a	:
	cd ../base && $(MAKE) debug
	
$(DEBUG_TARGET_DIR)/test0	:	$(DEBUG_OBJECTS) $(DEBUG_OBJ_DIR)/test0.o $(DEBUG_TARGET_DIR)/libbase.a
	$(CC) -o $@ $^ $(DEBUG_LIBS)

$(DEBUG_TARGET_DIR)/test1	:	$(DEBUG_OBJECTS) $(DEBUG_OBJ_DIR)/test1.o $(DEBUG_TARGET_DIR)/libbase.a
	$(CC) -o $@ $^ $(DEBUG_LIBS)

$(DEBUG_TARGET_DIR)/test2	:	$(DEBUG_OBJECTS) $(DEBUG_OBJ_DIR)/test2.o $(DEBUG_TARGET_DIR)/libbase.a
	$(CC) -o $@ $^ $(DEBUG_LIBS)

$(DEBUG_TARGET_DIR)/test3	:	$(DEBUG_OBJECTS) $(DEBUG_OBJ_DIR)/test3.o $(DEBUG_TARGET_DIR)/libbase.a
	$(CC) -o $@ $^ $(DEBUG_LIBS)

$(DEBUG_OBJ_DIR)/test0.o	:	test0.c UASDK_setup.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/test1.o	:	test1.c UASDK_setup.h UASDK_thread.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/test2.o	:	test2.c UASDK_setup.h UASDK_addressable.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@
	# $(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) -DUNIT_TEST $< -o $@

$(DEBUG_OBJ_DIR)/test3.o	:	test3.c UASDK_setup.h UASDK_addressable.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@
	# $(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) -DUNIT_TEST $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_setup.o	:	UASDK_setup.c UASDK_setup.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_ascii.o	:	UASDK_ascii.c UASDK_ascii.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_buffer.o	:	UASDK_buffer.c UASDK_buffer.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_addressable.o	:	UASDK_addressable.c UASDK_addressable.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_thread.o	:	UASDK_thread.c UASDK_thread.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/test_buffer.o	:	test_buffer.c
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_string.o	:	serialize/UASDK_string.c serialize/UASDK_string.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/test_UASDK_string.o	:	test_UASDK_string.c
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_i16array.o	:	serialize/UASDK_i16array.c serialize/UASDK_i16array.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/test_UASDK_i16array.o	:	test_UASDK_i16array.c
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_r32array.o	:	serialize/UASDK_r32array.c serialize/UASDK_r32array.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/test_UASDK_r32array.o	:	test_UASDK_r32array.c
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

$(DEBUG_OBJ_DIR)/UASDK_packet.o	:	UASDK_packet.c UASDK_packet.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) $< -o $@

clean_debug	:
	cd ../base && $(MAKE) $@
	rm -rf $(DEBUG_TARGET_DIR)
