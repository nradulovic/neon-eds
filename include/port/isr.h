#ifndef NISR_H_
#define NISR_H_

#include "arch/isr.h"
#include "arch/cpu.h"

struct nisr;

void nisr_enable(struct nisr * isr);
void nisr_disable(struct nisr * isr);
void nisr_set(struct nisr * isr);
void nisr_clear(struct nisr * isr);
void nisr_set_prio(struct nisr * isr, ncpu_reg prio);
ncpu_reg nisr_get_prio(struct nisr * isr);

#endif /* NISR_H_ */
