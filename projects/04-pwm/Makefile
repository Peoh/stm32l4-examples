### Program Name
TARGET=$(CURDIR)/main

### Includes
INC_DIR = $(CURDIR)/inc
SRC_DIR = $(CURDIR)/src

### Sources
USER_SRC = $(wildcard $(SRC_DIR)/*.c )

### Setup
SRC_C = $(CMSIS_SYS_SRC) $(USER_SRC)
OBJS_C = $(patsubst %.c,%.o,$(SRC_C))
SRC_S = $(CMSIS_START_SRC)
OBJS_S = $(patsubst %.s,%.o,$(SRC_S))
OBJS = $(OBJS_C) $(OBJS_S)

INC = $(CMSIS_INC_DIR) $(CMSIS_ST_INC_DIR) $(INC_DIR)
INC_PARAMS = $(foreach d, $(INC), -I$d)

CFLAGS += $(INC_PARAMS)
CFLAGS += -specs=nosys.specs -Wl,-Map=$(TARGET).map  --specs=nano.specs

### Rules
.PHONY: all clean $(TARGET) $(TARGET).elf $(TARGET).bin flash size
all: clean $(TARGET) size

debug: CFLAGS += -O0
debug: all

$(TARGET): $(TARGET).elf

$(TARGET).elf: $(OBJS)
	@echo $(CC)
	$(CC) $(CFLAGS) -o $@ $^
	$(CP) -O binary $(TARGET).elf $(TARGET).bin

%.o:%.c
	echo $(CURDIR)
	$(CC) $(CFLAGS) -c $^ -o $@

%.o:%.s
	$(CC) $(CFLAGS) -c -x assembler-with-cpp $^ -o $@

disass: $(TARGET).elf
	$(OD) -drwCSg $(TARGET).elf > $(TARGET).cdasm

size:
	$(SZ) $(TARGET).elf

clean:
	rm -f *.o $(TARGET).elf $(TARGET).bin $(TARGET).map
	-@find . -type f -name '*.o' -delete 2>/dev/null

flash:
	$(DOCKER_FLASH) openocd -f board/st_nucleo_l4.cfg -c "program $(TARGET).elf verify reset exit"
