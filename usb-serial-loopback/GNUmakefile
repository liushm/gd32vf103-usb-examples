
ELF = firmware.elf
OBJ = app.o cdc_acm_core.o gd32vf103_hw.o gd32vf103_it.o system_gd32vf103.o
FRM = Firmware/RISCV/env_Eclipse/init.o \
      Firmware/RISCV/env_Eclipse/start.o \
      Firmware/RISCV/env_Eclipse/entry.o \
      Firmware/RISCV/env_Eclipse/handlers.o \
      Firmware/RISCV/drivers/n200_func.o \
      Firmware/GD32VF103_usbfs_driver/Source/drv_usb_core.o \
      Firmware/GD32VF103_usbfs_driver/Source/drv_usb_dev.o \
      Firmware/GD32VF103_usbfs_driver/Source/drv_usbd_int.o \
      Firmware/GD32VF103_usbfs_driver/Source/usbd_core.o \
      Firmware/GD32VF103_usbfs_driver/Source/usbd_enum.o \
      Firmware/GD32VF103_usbfs_driver/Source/usbd_transc.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_adc.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_bkp.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_can.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_crc.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_dac.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_dbg.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_dma.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_eclic.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_exmc.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_exti.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_fmc.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_fwdgt.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_gpio.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_i2c.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_pmu.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_rcu.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_rtc.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_spi.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_timer.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_usart.o \
      Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_wwdgt.o

CC = riscv-nuclei-elf-gcc
OBJCOPY = riscv-nuclei-elf-objcopy

INCS = -I . \
       -I Firmware/RISCV/drivers \
       -I Firmware/GD32VF103_usbfs_driver/Include \
       -I Firmware/GD32VF103_standard_peripheral \
       -I Firmware/GD32VF103_standard_peripheral/Include
LIBS = 

DEFS = -D HXTAL_VALUE=8000000 \
       -D USE_STDPERIPH_DRIVER \
       -D USE_USB_FS \
       -D __packed=__attribute((__packed__))

ASFLAGS = -x assembler-with-cpp

CFLAGS = -Wall -Os -march=rv32imac -mabi=ilp32 -mcmodel=medlow \
		-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -std=gnu11

LDFLAGS = -march=rv32imac -mabi=ilp32 -mcmodel=medlow -nostartfiles -Xlinker --gc-sections --specs=nano.specs \
          -T Firmware/RISCV/env_Eclipse/GD32VF103xB.lds

$(ELF): $(OBJ) $(FRM)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.S
	$(CC) -c $(ASFLAGS) $(CFLAGS) $(INCS) $(DEFS) -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCS) $(DEFS) -o $@ $<

clean:
	@del $(ELF) $(OBJ)
	@rm.cmd

flash:
	@ocd.cmd

.PHONY: clean
