#ifndef PORT_CONFIG_H
#define PORT_CONFIG_H

/**@brief       Priority of critical sections in kernel
 * @details     Specify the priority in range: <code>0</code>\f$\leq\f$
 *              <code>CONFIG_ISR_MAX_PRIO</code> \f$\leq\f$ <code>15</code>.
 *              The lower the number the higher the priority.
 * @note        When priority is set to @b 0 then critical code section will not
 *              use priority levels but instead it will just disable interrupts
 *              on entry and enable interrupts on exit.
 */
#if !defined(CONFIG_ISR_MAX_PRIO)
# define CONFIG_ISR_MAX_PRIO                7u
#endif

/**@brief       This field determines the split of priority from subpriority.
 * @note        This configuration option is port dependent.
 * @warning     Change this value only if you are familiar with Cortex interrupt
 *              priority system and how Neon RT Kernel protects its critical
 *              code sections.
 */
#if !defined(PORT_CONFIG_ISR_SUBPRIORITY)
# define PORT_CONFIG_ISR_SUBPRIORITY        0u
#endif

/**@brief       The frequency of clock which is used for the system timer
 * @details     System timer SysTick uses core clock (sometimes referred to as
 *              HCLK) for counting. Specify here the core clock so the OS can
 *              properly manage system tick event generation.
 */
#if !defined(CONFIG_SYSTIMER_CLOCK_FREQ)
# define CONFIG_SYSTIMER_CLOCK_FREQ         24000000ul
#endif

/**@brief       The frequency of system timer tick event
 * @note        This setting is valid only if configuration option
 *              @ref CONFIG_SYSTIMER_CLOCK_FREQ is properly set.
 */
#if !defined(CONFIG_SYSTIMER_EVENT_FREQ)
# define CONFIG_SYSTIMER_EVENT_FREQ         100ul
#endif

#if !defined(CONFIG_CORE_TIMER_HANDLERS)
# define CONFIG_CORE_TIMER_HANDLERS         4
#endif


#if !defined(ncore_timer_isr)
# define ncore_timer_isr                    SysTick_Handler
#endif

#if !defined(ncore_kernel_isr)
# define ncore_kernel_isr                   PendSV_Handler
#endif


#endif /* PORT_CONFIG_H */
