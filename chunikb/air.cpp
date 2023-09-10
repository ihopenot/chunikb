#include "air.h"
#include <Arduino.h>

#define AIR_LED_DELAY 300//��ѯ�ӳ�
#define AIR_INPUT_SENSITIVITY 0.3//�������ж�
#define CALIBRATION_SAMPLES 256//У׼������
#define SKIP_SAMPLES 256//У׼����������

int ir_input_pins[6] = { 4,5,6,7,8,15 };//������ѯ����������
int ir_sensor_pins[6] = { A0,A1,A2,A3,A9,A10 };//������ѯ���պ������

extern int air_state[6] = { 0 };
int calibrationCounter;
bool calibrated;
int thresholds[6];
int maxReadings[6];

void Air_Setup() {
	Serial.println("Air Setup");
	calibrated = false;
	for (int i = 0; i < 6; i++) {
		maxReadings[i] = 0;
	}
	for (int i = 0; i < 6; i++) {
		pinMode(ir_sensor_pins[i], INPUT);
		pinMode(ir_input_pins[i], OUTPUT);
		air_state[i] = 0;
	}
	AirCalibrate();
}

void turnOffLight() {//�رշ��亯��
	for (int i = 0; i < 6; i++) {
		digitalWrite(ir_input_pins[i], LOW);
	}
}

void changeLight(int light) {//���ⷢ����ѯ
	switch (light) {
	case 0:
		digitalWrite(ir_input_pins[light], HIGH);//#1
		break;
	case 1:
		digitalWrite(ir_input_pins[light], HIGH);//#2
		break;
	case 2:
		digitalWrite(ir_input_pins[light], HIGH);//#3
		break;
	case 3:
		digitalWrite(ir_input_pins[light], HIGH);//#4
		break;
	case 4:
		digitalWrite(ir_input_pins[light], HIGH);//#5
		break;
	case 5:
		digitalWrite(ir_input_pins[light], HIGH);//#6
		break;
	default:
		turnOffLight();
		break;
	}
}

int getValue(int sensor) {//��ȡ������ֵ����
	changeLight(sensor);// ����
	delayMicroseconds(AIR_LED_DELAY / 2);// �����ӳٱ�֤��ȫ����
	return analogRead(ir_sensor_pins[sensor]);//����ֵ
}

bool getSensorState(int sensor) {//����Air״̬
	turnOffLight();//�ص�
	delayMicroseconds(AIR_LED_DELAY / 2);//�����ӳٱ�֤��ȫ�ر�
	int enviroment = analogRead(ir_sensor_pins[sensor]);//��ȡ������ֵ
	int value = getValue(sensor);//��ȡ������ֵ

	//����AIR�����
	// Serial.print(sensor);
	// Serial.print("\t");
	// Serial.print(value);
	// Serial.print("\t");
	// Serial.print(enviroment);
	// Serial.print("\t");
	// Serial.print(value - enviroment);
	// Serial.print("\t");
	// Serial.print(value - enviroment + 200);
	// Serial.print("\t");
	// Serial.print(maxReadings[sensor]);
	// Serial.print("\t");
	// Serial.print(thresholds[sensor]);
	// Serial.println();

	//�Ѷ�����ȥ������ֵ��У׼��ֵ�ȽϺ󷵻ز���ֵ��+200Ϊ��ֹ�����ıȽ�
	return (value - enviroment + 200) < (thresholds[sensor] + 200);

}

void AirCalibrate() {//У׼���ն�
	// ������ͷ����
	for (int i = 0; i < SKIP_SAMPLES; i++) {
		for (int sensor = 0; sensor < 6; sensor++) {
			getValue(sensor);
			turnOffLight();
		}
	}
	//��ʼУ׼
	for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
		for (int sensor = 0; sensor < 6; sensor++) {
			turnOffLight();//�ص�
			delayMicroseconds(AIR_LED_DELAY / 2);//�����ӳٱ�֤��ȫ�ر�
			int enviroment = analogRead(ir_sensor_pins[sensor]);//��ȡ������ֵ

			// Serial.print(sensor);
			// Serial.print("\t");
			// Serial.print(enviroment);
			// Serial.println();
			// changeLight(sensor);
			// delayMicroseconds(AIR_LED_DELAY / 2);//�����ӳٱ�֤��ȫ�ر�
			// int lvalue = analogRead(ir_sensor_pins[sensor]);//��ȡ������ֵ
			// Serial.print(sensor);
			// Serial.print("\t");
			// Serial.print(lvalue);
			// Serial.println();

			int value = getValue(sensor) - enviroment;//��ȡ������ֵ
			if (value > maxReadings[sensor])//��¼��߶���
				maxReadings[sensor] = value;
		}
	}
	for (int i = 0; i < 6; i++) {//ͨ�����ж�������ֵ
		thresholds[i] = (AIR_INPUT_SENSITIVITY * maxReadings[i]);
	}
	calibrated = true;//���У׼
}

void Air_Check() {
	if (calibrated) {
		for (int i = 0; i < 6; i++) {
			if (getSensorState(i)) {
				air_state[i] = 1;
			}
			else if (!getSensorState(i)) {
				air_state[i] = 0;
			}
		}
	}
}
