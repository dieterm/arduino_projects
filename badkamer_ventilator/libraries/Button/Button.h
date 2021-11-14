/*
	Button - a small library for Arduino to handle button debouncing
	
	MIT licensed.
*/

#ifndef Button_h
#define Button_h
#include "Arduino.h"

class Button
{
	public:
		Button(uint8_t pin);
		void poll(); //dmoe
		void begin();
		bool read();
		bool toggled();
		bool pressed();
		bool released();
		bool has_changed();
		
		const static bool PRESSED = LOW;
		const static bool RELEASED = HIGH;
		const static int POLL_SAMPLE_COUNT = 10;
	private:
		uint8_t  _pin;
		uint16_t _delay;
		bool     _state;
		bool     _has_changed;
		bool	 _pollBuffer[POLL_SAMPLE_COUNT];
		uint16_t _pollBufferPos;
		uint32_t _pollLastTime;
		uint32_t _ignore_until;
};

#endif
