#include "./screen.h"
#include "./generic_keyboard.h"
#include "../cpu/isr.h"
#include "../drivers/ports.h"
#include "../cathesimc/queue.h"
#include "../cathesimc/string.h"
#include "../shell/shell.c"
#define BACKSPACE 0x0e
#define ENTER 0x1c


		static char keyboard_buffer[256];

#define SC_MAX 57

const char *sc_name[] =  { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};



static void keyboard_callback(registers_t regs) {
		u8 sc = port_byte_in(0x60);
		if (sc>SC_MAX) return;
		if(sc == ENTER) {
				append(keyboard_buffer, '\n');
				user_input(keyboard_buffer);
				keyboard_buffer[0] = '\n';
		}else if(sc == BACKSPACE){
				append(keyboard_buffer, "<");
				append(keyboard_buffer, "-");
				user_input(keyboard_buffer);
		}else{
				char lit_key[] = {sc_ascii[(int)sc], '\0'};
				kprint(lit_key);

		}
		UNSUSED(regs);

}


void init_generic_keyboard(void* keyboard_event_queue, char* simple_key_buff){
		register_interrupt_handler(IRQ1, keyboard_callback); 
		struct keyboard_event_queue_head* keyboard_event_queue_ptr = &keyboard_event_qhead;
		QUEUE_INIT(keyboard_event_queue_ptr);
		keyboard_event_queue = keyboard_event_queue_ptr;
		simple_key_buff = keyboard_buffer;



};
