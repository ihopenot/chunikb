#include "mpr121_arduino.h"
#include "chunikb.h"
#include "chuni_serial.h"
#include "touch.h"
#include "air.h"
#include "Hid-Project.h"

extern KB_Mode kb_mode = DEFAULT_KB_MODE;

KeyboardKeycode air_key[6] = { KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9};
KeyboardKeycode touch_key[16] = { KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H };

int air_key_state[6] = { 0, 0, 0, 0, 0, 0 };
int touch_key_state[16] = { 0 };

void slider_scan() {
	Touch_Check();
	Air_Check();
	// Air_LED();
	if (touch_scan) {
		slider_resp.syn = 0xff;
		slider_resp.cmd = SLIDER_CMD_AUTO_SCAN;
		slider_resp.size = sizeof(slider_resp.pressure);
		// TODO
	}
	for (int i = 0; i < 6; i++) {
		if (air_state[i] && !air_key_state[i]) {
			air_key_state[i] = 1;
			NKROKeyboard.press(air_key[i]);
		} else if (!air_state[i] && air_key_state[i]) {
			air_key_state[i] = 0;
			NKROKeyboard.release(air_key[i]);
		}
		// Serial.print(air_state[i]);
		// Serial.print(' ');
	}
	// Serial.println();
	for (int i = 0; i < 16; i++) {
		int ts = touch_state[2*i] | touch_state[2*i+1];
		if (ts && !touch_key_state[i]) {
			touch_key_state[i] = 1;
			NKROKeyboard.press(touch_key[i]);
		} else if (!ts && touch_key_state[i]) {
			touch_key_state[i] = 0;
			NKROKeyboard.release(touch_key[i]);
		}
		// Serial.print(touch_state[i]);
		// Serial.print(' ');
	}
	// Serial.println();
}

// NO LED
void slider_set_led() {
	return;
}

void slider_scan_start() {
	touch_scan = true;
	slider_scan();
}

void slider_scan_stop() {
	touch_scan = false;
	slider_resp.syn = 0xff;
	slider_resp.cmd = SLIDER_CMD_AUTO_SCAN_STOP;
	slider_resp.size = 0;
	slider_req.cmd = 0;
}

void slider_reset() {
	Touch_Reset();
	slider_resp.syn = 0xff;
	slider_resp.cmd = SLIDER_CMD_RESET;
	slider_resp.size = 0;
	slider_req.cmd = 0;
}

void slider_get_board_info() {
	slider_resp.syn = 0xff;
	slider_resp.cmd = SLIDER_CMD_GET_BOARD_INFO;
	slider_resp.size = sizeof(slider_resp.version);
	strcpy(slider_resp.version, "15330   \xA0""06712\xFF""\x90");
	slider_req.cmd = 0;
}

void loop() {
	if (kb_mode == KEYBOARD) {
		slider_scan();
	}
	else {
		switch (sliderserial_readreq()) {
		case SLIDER_CMD_SET_LED:
			slider_set_led();
			break;
		case SLIDER_CMD_AUTO_SCAN_START:
			slider_scan_start();
			break;
		case SLIDER_CMD_AUTO_SCAN_STOP:
			slider_scan_stop();
			break;
		case SLIDER_CMD_RESET:
			slider_reset();
			break;
		case SLIDER_CMD_GET_BOARD_INFO:
			slider_get_board_info();
			break;
		default:
			slider_scan();
		}
		sliderserial_writeresp();
	}
}

void setup() {
	Serial.begin(115200);
	while (!Serial) { // needed to keep leonardo/micro from starting too fast!
		delay(10);
	}

	Touch_Setup();
	Air_Setup();
	NKROKeyboard.begin();
}

