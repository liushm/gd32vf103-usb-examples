/*!
    \file  main.c
    \brief USB CDC ACM device

    \version 2019-6-5, V1.0.0, firmware for GD32VF103
*/

/*
    Copyright (c) 2019, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/


#include "drv_usb_hw.h"
#include "cdc_acm_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
#include "usart0.h"
#include "dbgprint.h"

usb_core_driver USB_OTG_dev = 
{
    .dev = {
        .desc = {
            .dev_desc       = (uint8_t *)&device_descriptor,
            .config_desc    = (uint8_t *)&configuration_descriptor,
            .strings        = usbd_strings,
        }
    }
};

void send(char *buf)
{
    int len = strlen(buf);
    for (int i = 0; i < len; i++) {
        usart_data_transmit(USART0, buf[i]);
        while (usart_flag_get(USART0, USART_FLAG_TC) == RESET) {}
    }
}

/*!
    \brief      main routine will construct a USB keyboard
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    eclic_global_interrupt_enable();	
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL2_PRIO2);

    /* enable GPIO LED */
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_bit_set(GPIOA, GPIO_PIN_1);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);

    LCD_Init(BLACK);

    usart0_init();

    usb_rcu_config();

    usb_timer_init();

    usb_intr_config();

    usbd_init (&USB_OTG_dev, USB_CORE_ENUM_FS, &usbd_cdc_cb);

    /* check if USB device is enumerated successfully */
    while (USBD_CONFIGURED != USB_OTG_dev.dev.cur_status) {
    }

    while (1) {
        if (USBD_CONFIGURED == USB_OTG_dev.dev.cur_status) {
            extern uint8_t packet_receive;
            if (packet_receive) {
                cdc_acm_data_receive(&USB_OTG_dev);
            }

            // TODO
            extern uint8_t data_buffer[64];
            extern uint8_t data_length;
            if (data_length) {
                uint8_t length = data_length;
                uint8_t buffer[64];
                for (uint8_t i = 0; i < length; i++)
                    buffer[i] = data_buffer[i];
                data_length = 0;

                cdc_acm_data_send(&USB_OTG_dev, buffer, length);
                
            }
        }
    }

    return 0;
}