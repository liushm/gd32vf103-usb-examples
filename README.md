# gd32vf103-usb-examples

买到了一个Sipeed Longan Nano的板子，编译USB相关示例的时候遇到了一些问题，记录一下

## PlatformIO & gd32v

* 网上找到的例子都是GD32VF103-EVAL这个板子，并不是Longan Nano的，不过也就是LED_PIN之类的差别
* 默认把GD32VF103_usbfs_driver全注释了，原因大概是这个“库”反过来依赖由用户代码提供的usb_conf.h和usbd_conf.h，就很奇葩
* 最简单的解决办法就是，把USB相关的库直接拷贝到自己的工程目录里面来（库里面也分device和host，只拷device相关的）
* 覆盖了库里面的system_gd32vf103.c，因为需要把CK_SYS应设置为96MHz，这样CK_PLL二分频之后才能提供USBFS需要的48MHz
* 修改usb_conf.h，添加
> #include <sys/cdefs.h>

> #define USE_USE_USB_FS
* 改了一下cdc_acm_core.c，STR_IDX_SERIAL，这样win10下才有反应，原因未知，待研究
> [STR_IDX_SERIAL] = "\x08\x03CDCACM"

## using makefile

## gd32vf103-usb-examples
* usb-serial-loopback
* usb-serial-uart0
* usb-msc-internal-64k
* usb-msc-tfcard
* usb-hid-keyboard
