
Use	`make release`		to compile release executable
	`make debug` 		to compile debug executable
	`make flash_swd` 	to upload release executable
	`make flash_debug` 	to upload debug executable and start gdb
	`make start_gdb` 	to just start the openocd and gdb
	`make clean`		to clean project

TODO:
	*Add debugging defines in main and additional functionality in Makefile
	*Repair the printf in debugging mode
	*.gdbint has static path to executable

