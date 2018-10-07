target remote localhost:3333
load build/debug/stm32f4_template.elf
monitor arm semihosting enable
monitor reset halt

