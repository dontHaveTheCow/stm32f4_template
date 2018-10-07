
Use	`make release`		to compile release executable
	`make debug` 		to compile debug executable
	`make flash_swd` 	to upload release executable
	`make flash_debug` 	to upload debug executable and start gdb
	`make start_gdb` 	to just start the openocd and gdb
	`make clean`		to clean project

TODO:
	
	* .gdbint has static path to executable
	* Code with debugging symbols, semihosting and minimal libc ir 14kB in size
	  But release variant is 11kB, which might be oversized.
	* Cleaner way of implementing debug code in overall source should be introduced.
