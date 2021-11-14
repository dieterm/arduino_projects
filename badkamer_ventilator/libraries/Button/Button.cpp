/*
	Button - a small library for Arduino to handle button debouncing
	
	MIT licensed.
*/
// #define DEBUG
#include "Button.h"
#include <Arduino.h>

Button::Button(uint8_t pin)
:  _pin(pin)
,  _delay(200)
,  _state(RELEASED)
,  _ignore_until(0)
,  _pollBuffer{RELEASED, RELEASED, RELEASED, RELEASED, RELEASED, RELEASED, RELEASED, RELEASED, RELEASED, RELEASED}
,  _pollBufferPos(0)
,  _pollLastTime(0)
,  _has_changed(false)
{
}

// call this every application main loop iteration
void Button::poll() {
	unsigned long _millis = millis();
	unsigned long pollSampleInterval = _delay / (uint16_t)POLL_SAMPLE_COUNT;
	if(_has_changed){
		_has_changed = false;
	}

	if ((_pollLastTime + pollSampleInterval) <= _millis)
	{
		bool currentButtonState = digitalRead(_pin);
		// update buffer
		_pollBuffer[_pollBufferPos++] = currentButtonState;
		if(_pollBufferPos==10){
			_pollBufferPos=0;
		}
		_pollLastTime = _millis;

		// check if state has changed
		bool allPollValuesDifferent = true;
	
		for(int i = 0; i<=9; i++){
			allPollValuesDifferent = allPollValuesDifferent && (_pollBuffer[i] != _state);
		}

		if(allPollValuesDifferent){
			_state = currentButtonState;
			_has_changed = true;
		} 
	}
}

void Button::begin()
{
	pinMode(_pin, INPUT_PULLUP);
}

// 
// public methods
// 

// get current state of the button
bool Button::read()
{
	return _state;
}

// has the button been toggled from on -> off, or vice versa
bool Button::toggled()
{
	//read();
	return has_changed();
}

// mostly internal, tells you if a button has changed after calling the read() function
bool Button::has_changed()
{
	return _has_changed;
	/*if (_has_changed == true)
	{
		_has_changed = false;
		return true;
	}
	return false;*/
}

// has the button gone from off -> on
bool Button::pressed()
{
	//return read() == PRESSED;
	if (read() == PRESSED && has_changed() == true) // 
		return true;
	else
		return false;
}

// has the button gone from on -> off
bool Button::released()
{
	//return read() == RELEASED;
	if (read() == RELEASED && has_changed() == true)
		return true;
	else
		return false;
}
