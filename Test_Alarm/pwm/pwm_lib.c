#include "mira.h"
#include "nrf_gpio.h"
#include "nrf_pwm.h"
#include "pwm_lib.h"
#include <stdio.h>

#define NRF_PIN_1 NRF_GPIO_PIN_MAP(0, 20)
#define PWM_MAX_VALUE 100
static uint32_t pending_pin = NRF_PIN_1;
static uint32_t pending_value = 0;
nrf_pwm_values_individual_t seq_values;

static nrf_pwm_sequence_t const seq =
{
    .values.p_individual = &seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 1,
    .end_delay       = 0
};

void pwm_init(void){
    printf("PWM - init\n");
    seq_values.channel_0 = PWM_MAX_VALUE;
    seq_values.channel_1 = PWM_MAX_VALUE;
    seq_values.channel_2 = PWM_MAX_VALUE;
    seq_values.channel_3 = PWM_MAX_VALUE;

    // nrf_pwm_clk_t base_clock = NRF_PWM_CLK_1MHz;
    nrf_pwm_clk_t base_clock = NRF_PWM_CLK_250kHz;
    nrf_pwm_mode_t mode = NRF_PWM_MODE_UP;
    uint16_t top_value = PWM_MAX_VALUE;
    nrf_pwm_dec_load_t dec_load = NRF_PWM_LOAD_INDIVIDUAL;
    nrf_pwm_dec_step_t dec_step = NRF_PWM_STEP_AUTO;

    nrf_gpio_cfg_output(NRF_PIN_1);
    nrf_gpio_pin_clear(NRF_PIN_1);
    NRF_PWM0->PSEL.OUT[0] = NRF_PIN_1;

    nrf_pwm_enable(NRF_PWM0);
    nrf_pwm_configure(NRF_PWM0, base_clock, mode, top_value);
    nrf_pwm_decoder_set(NRF_PWM0, dec_load, dec_step);
    nrf_pwm_shorts_set(NRF_PWM0, 0);
    nrf_pwm_int_set(NRF_PWM0, PWM_INTENSET_SEQEND1_Msk);
    nrf_pwm_event_clear(NRF_PWM0, NRF_PWM_EVENT_LOOPSDONE);
    nrf_pwm_event_clear(NRF_PWM0, NRF_PWM_EVENT_SEQEND0);
    nrf_pwm_event_clear(NRF_PWM0, NRF_PWM_EVENT_SEQEND1);
    nrf_pwm_event_clear(NRF_PWM0, NRF_PWM_EVENT_STOPPED);
    NVIC_ClearPendingIRQ(PWM0_IRQn);
    NVIC_SetPriority(PWM0_IRQn, 3);
    NVIC_EnableIRQ(PWM0_IRQn);

    // start it
    nrf_pwm_sequence_set(NRF_PWM0, 0, &seq);
    nrf_pwm_sequence_set(NRF_PWM0, 1, &seq);

    nrf_pwm_loop_set(NRF_PWM0, 2);
    nrf_pwm_shorts_set(NRF_PWM0, NRF_PWM_SHORT_LOOPSDONE_SEQSTART1_MASK);
    nrf_pwm_task_trigger(NRF_PWM0, NRF_PWM_TASK_SEQSTART1);
}

void pwm_enable(void){
    nrf_pwm_task_trigger(NRF_PWM0, NRF_PWM_TASK_SEQSTART1);
}

void pwm_disable(void){
    nrf_pwm_task_trigger(NRF_PWM0, NRF_PWM_TASK_STOP);
}

void pwm_set_value(uint32_t value){
    if (value > PWM_MAX_VALUE){
        value = PWM_MAX_VALUE;
    }
    pending_value = PWM_MAX_VALUE - value;
}

void pwm_set_pin(uint32_t pin){
    pending_pin = pin;
}

uint32_t pwm_get_max_value(void){
    return PWM_MAX_VALUE;
}

void PWM0_IRQHandler(void){
    if (nrf_pwm_event_check(NRF_PWM0, NRF_PWM_EVENT_SEQEND1)){
        nrf_pwm_event_clear(NRF_PWM0, NRF_PWM_EVENT_SEQEND1);
        if (pending_value != seq_values.channel_0){
            seq_values.channel_0 = pending_value;
        }
        if (pending_pin != NRF_PWM0->PSEL.OUT[0]){
            NRF_PWM0->PSEL.OUT[0] = pending_pin;
        }
    }
}