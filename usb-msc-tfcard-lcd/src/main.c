#include <gd32vf103.h>
#include <stdio.h>
#include <unistd.h>
#include <lcd.h>
#include <tf_card.h>
#include <systick.h>

unsigned char image[12800];

int main()
{
    SystemCoreClockUpdate();

    // use GPIOA-5/(6)/7 GPIOB-0/1/2
    LCD_Init(WHITE);

    // TF-card use GPIOB-12/13/14/15
    // LED use GPIOA-1/2 GPIOC-13

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);

    GPIO_BOP(GPIOA) = GPIO_PIN_1;
    GPIO_BOP(GPIOA) = GPIO_PIN_2;
    GPIO_BOP(GPIOC) = GPIO_PIN_13;

    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);  // LED GREEN
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);  // LED BLUE
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13); // LED RED

    // printf("hello, world\n");
    // printf("%08x\n", (uint32_t)malloc(32));
    // printf("%08x\n", (uint32_t)RCU_CFG0);
    // for (int i = 0; i < 256; i++) {
    //     printf("%u\n", mtime_lo()/26000); // mtime_lo() * 4000 / 108e6 ===> ms ////// 0xd1000000
    //     delay_1ms(10);
    // }
    // delay_1ms(2000);

    // dbgprint("%x", *(uint32_t *)0x40022100); // ============> JBC3
    // delay_1ms(5000);

    FATFS fs;
    FRESULT fr = f_mount(&fs, "", 1);
    for (int i = 0; i < 32 && fr != 0; i++) {
        fr = f_mount(&fs, "", 1);
        delay_1ms(500);
        LCD_Clear(WHITE);
        LCD_ShowString(8, 32, "Trying to mount!", BLACK);
    }
    while (fr == 0) {
        FIL fil;
        fr = f_open(&fil, "bmp.bin", FA_READ);
        if (fr != 0) {
            break;
        }

        while (1) {
            UINT len = 0;

            fr = f_read(&fil, image, sizeof(image), &len);
            if (fr != 0 || len != sizeof(image)) {
                break;
            }
            LCD_ShowPicture(0, 0, 159, 39);

            fr = f_read(&fil, image, sizeof(image), &len);
            if (fr != 0 || len != sizeof(image)) {
                break;
            }
            LCD_ShowPicture(0,40,159,79);
        }
        f_close(&fil);

        LCD_Clear(WHITE);
        LCD_ShowString(8, 32, "THX for watching!", BLACK);
        for (int i = 0; i < 40; i++) {
            gpio_bit_write(GPIOA, GPIO_PIN_1, 1 - gpio_input_bit_get(GPIOA, GPIO_PIN_1));
            delay_1ms(50);
        }
    }

    LCD_Clear(WHITE);
    LCD_ShowString(8, 32, "Failed to mount!", BLACK);

    while (1) {
        GPIO_BC(GPIOA) = GPIO_PIN_1;
        delay_1ms(100);
        GPIO_BOP(GPIOA) = GPIO_PIN_1;
        delay_1ms(100);
    }

    return 0;
}