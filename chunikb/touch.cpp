#include "mpr121_arduino.h"
#include "touch.h"

extern int touch_scan = false;

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap[4] = { Adafruit_MPR121(), Adafruit_MPR121(), Adafruit_MPR121(), Adafruit_MPR121() };
extern int touch_state[32] = { 0 };

#define CLAMP(x, min, max) (x < min ? min : (x > max ? max : x))

int CalCheck(int bl, int fd) {//触摸数值计算
	int cal = bl - fd;
	return cal > 4 ? 5 * cal : (cal > 0 ? cal : 0);
}

void Touch_Check() {
	// int16_t bl, fl, cal, calpress[32];
	// for (uint8_t i = 0; i < 8; i++) {//计算数值
	// 	for (int j = 0; j < 4; j++) {
	// 		bl = cap[j].baselineData(i);
	// 		fl = cap[j].filteredData(i);
	// 		cal = CalCheck(bl, fl);
	// 		calpress[i + j * 8] = CLAMP(cal, 0, 255);
	// 		touch_state[i + j * 8] = calpress[i + j * 8] > 0 ? 1 : 0;
	// 		// Serial.print(fl);
	// 		// Serial.print(",");
	// 		// Serial.print(bl);
	// 		// Serial.print("\t");
	// 	}
	// 	// Serial.println();
	// }
	// for (uint8_t i = 0; i < 8; i++) {//打印数值
	// 	for (int j = 0; j < 4; j++) {
	// 		Serial.print(calpress[i + j * 8]);
	// 		Serial.print("\t");
	// 	}
	// 	Serial.println();
	// }
	for(int i=0; i<4; i++) {
		int state = cap[i].touched();
		for(int j=0; j<8; j++) {
			touch_state[i*8+j] = (state >> j) & 1;
		}
	}
}

void Touch_Reset() {
	Touch_Setup();
}

void Touch_Setup() {
	memset(touch_state, 0, sizeof(touch_state));
    Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

    // Default address is 0x5A, if tied to 3.3V its 0x5B
    // If tied to SDA its 0x5C and if SCL then 0x5D
	for (int i = 0; i < 4; i++) {
		if (!cap[i].begin(0x5A+i, &Wire, 6, 3)) {
			Serial.print("MPR121 ");
			Serial.print(i);
			Serial.println(" not found, check wiring?");
			while (1);
		}
	}
}
