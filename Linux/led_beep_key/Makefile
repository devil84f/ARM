CROSS_COMPILE 	?= arm-linux-gnueabihf-
TARGET        	?= led_beep_key

CC            	:= $(CROSS_COMPILE)gcc
LD 			  	:= $(CROSS_COMPILE)ld
OBJCOPY		  	:= $(CROSS_COMPILE)objcopy
OBJDUMP		  	:= $(CROSS_COMPILE)objdump 

INCDIRS       	:= imx6ul   \
				   bsp/clk \
				   bsp/led \
				   bsp/delay  \
				   bsp/beep \
				   bsp/gpio \
				   bsp/key

SRCDIRS 	  	:= project  \
				   bsp/clk \
				   bsp/led \
				   bsp/delay  \
				   bsp/beep \
				   bsp/gpio \
				   bsp/key

INCLUDE 		:= $(patsubst %, -I %, $(INCDIRS))

SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
				 
SFILENDIR		:= $(notdir $(SFILES))
CFILENDIR		:= $(notdir $(CFILES))

SOBJS			:= $(patsubst %.S, obj/%.o, $(SFILENDIR))
COBJS			:= $(patsubst %.c, obj/%.o, $(CFILENDIR))
OBJS			:= $(SOBJS) $(COBJS)

VPATH			:= $(SRCDIRS)

$(TARGET).bin	: $(OBJS)
	$(LD) -Timx6ul.lds -o $(TARGET).elf $^
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

.PHONY: clean

clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(SOBJS) $(COBJS)
