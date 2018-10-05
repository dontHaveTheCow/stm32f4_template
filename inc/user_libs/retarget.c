
int fputc(int ch, FILE *f) {

	return ITM_SendChar(ch);
}

int fgetc(FILE *f){

	int r;
 
	/* if we just backspaced, then return the backspaced character */
        /* otherwise output the next character in the stream */
        if (backspace_called == 1){
		
		backspace_called = 0;
	}
	else {
		do {
			r = ITM_ReceiveChar();
		} while (r == -1);
       	
		last_char_read = (unsigned char)r;
	
		#ifdef ECHO_FGETC
       		ITM_SendChar(r);
       	}

	return last_char_read;
}



