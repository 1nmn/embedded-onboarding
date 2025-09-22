#include "stm32f0xx_hal_gpio.h"
#include "stm32f030x8.h"

// rewrite HAL_GPIO_Init

void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    uint32_t pinmask = GPIO_Init->Pin;
    uint32_t moder = GPIOx->MODER;
    uint32_t otyper = GPIOx->OTYPER;
    uint32_t ospeedr = GPIOx->OSPEEDR;
    uint32_t pupdr = GPIOx->PUPDR;
    uint32_t afr[2] = {GPIOx->AFR[0], GPIOx->AFR[1]};

    uint32_t exticr[4] = {SYSCFG->EXTICR[0], SYSCFG->EXTICR[1], SYSCFG->EXTICR[2], SYSCFG->EXTICR[3]};
    uint32_t rtsr = EXTI->RTSR;
    uint32_t ftsr = EXTI->FTSR;
    uint32_t imr = EXTI->IMR;
    uint32_t emr = EXTI->EMR;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
    assert_param(IS_GPIO_MODE(GPIO_Init->Mode));

    /* Configure the port pins */
    while (pinmask)
    {
        // Get current io position
        uint32_t position = __builtin_ctz(pinmask); // index of least-significant set bit
        uint32_t iocurrent = (1UL << position);

        /*--------------------- GPIO Mode Configuration ------------------------*/
        /* In case of Output or Alternate function mode selection */
        if (((GPIO_Init->Mode & GPIO_MODE) == MODE_OUTPUT) || ((GPIO_Init->Mode & GPIO_MODE) == MODE_AF))
        {
            /* Check the Speed parameter */
            assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));

            /* Configure the IO Speed */
            ospeedr &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2u));
            ospeedr |= (GPIO_Init->Speed << (position * 2u));

            /* Configure the IO Output Type */
            otyper &= ~(GPIO_OTYPER_OT_0 << position);
            otyper |= (((GPIO_Init->Mode & OUTPUT_TYPE) >> OUTPUT_TYPE_Pos) << position);
        }

        if ((GPIO_Init->Mode & GPIO_MODE) != MODE_ANALOG)
        {
            /* Check the Pull parameter */
            assert_param(IS_GPIO_PULL(GPIO_Init->Pull));

            /* Activate the Pull-up or Pull down resistor for the current IO */
            pupdr &= ~(GPIO_PUPDR_PUPDR0 << (position * 2u));
            pupdr |= ((GPIO_Init->Pull) << (position * 2u));
        }

        /* In case of Alternate function mode selection */
        if ((GPIO_Init->Mode & GPIO_MODE) == MODE_AF)
        {
            /* Check the Alternate function parameters */
            assert_param(IS_GPIO_AF_INSTANCE(GPIOx));
            assert_param(IS_GPIO_AF(GPIO_Init->Alternate));

            /* Configure Alternate function mapped with the current IO */
            afr[position >> 3u] &= ~(0xFu << ((position & 0x07u) * 4u));
            afr[position >> 3u] |= ((GPIO_Init->Alternate) << ((position & 0x07u) * 4u));
        }

        /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
        moder &= ~(GPIO_MODER_MODER0 << (position * 2u));
        moder |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2u));

        /*--------------------- EXTI Mode Configuration ------------------------*/
        /* Configure the External Interrupt or event for the current IO */
        if ((GPIO_Init->Mode & EXTI_MODE) != 0x00u)
        {
            /* Enable SYSCFG Clock */
            __HAL_RCC_SYSCFG_CLK_ENABLE();

            exticr[position >> 2u] &= ~(0x0FuL << (4u * (position & 0x03u)));
            exticr[position >> 2u] |= (GPIO_GET_INDEX(GPIOx) << (4u * (position & 0x03u)));

            /* Clear Rising Falling edge configuration */
            rtsr &= ~(iocurrent);
            if ((GPIO_Init->Mode & TRIGGER_RISING) != 0x00u)
                rtsr |= iocurrent;

            ftsr &= ~(iocurrent);
            if ((GPIO_Init->Mode & TRIGGER_FALLING) != 0x00u)
                ftsr |= iocurrent;

            /* Clear EXTI line configuration */
            emr &= ~(iocurrent);
            if ((GPIO_Init->Mode & EXTI_EVT) != 0x00u)
                emr |= iocurrent;

            imr &= ~(iocurrent);
            if ((GPIO_Init->Mode & EXTI_IT) != 0x00u)
                imr |= iocurrent;
        }
        pinmask &= ~iocurrent; // clear processed pin
    }

    /* Write all registers back at once */
    GPIOx->MODER = moder;
    GPIOx->OTYPER = otyper;
    GPIOx->OSPEEDR = ospeedr;
    GPIOx->PUPDR = pupdr;
    GPIOx->AFR[0] = afr[0];
    GPIOx->AFR[1] = afr[1];

    SYSCFG->EXTICR[0] = exticr[0];
    SYSCFG->EXTICR[1] = exticr[1];
    SYSCFG->EXTICR[2] = exticr[2];
    SYSCFG->EXTICR[3] = exticr[3];

    EXTI->RTSR = rtsr;
    EXTI->FTSR = ftsr;
    EXTI->IMR = imr;
    EXTI->EMR = emr;
}

// rewrite HAL_GPIO_WritePin

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_PIN_ACTION(PinState));

    /* Directly set or reset the pin using ODR register */
    GPIOx->BSRR = (PinState != GPIO_PIN_RESET) ? GPIO_Pin : (GPIO_Pin << 16u);
}

// rewrite HAL_GPIO_TogglePin

void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    /* Toggle the selected pin */
    GPIOx->ODR ^= GPIO_Pin;
}