#include "nrf_pwm.h"

void pwm_init(void);

void pwm_enable(void);

void pwm_disable(void);

void pwm_set_pin(uint32_t pin);

void pwm_set_value(uint32_t value);

uint32_t pwm_get_max_value(void);