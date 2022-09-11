#include "CircuitIoTDecoder.h"
#include <Arduino.h>
#if (defined(__AVR__) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

const char PROGMEM _Base64AlphabetTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

int CircuitIoTDecoder::decode(char * output, char * input, int inputLength) {
	int i = 0, j = 0;
	int decodedLength = 0;
	unsigned char A3[3];
	unsigned char A4[4];


	while (inputLength--) {
		if(*input == '=') {
			break;
		}

		A4[i++] = *(input++);
		if (i == 4) {
			for (i = 0; i <4; i++) {
				A4[i] = lookupTable(A4[i]);
			}

			fromA4ToA3(A3,A4);

			for (i = 0; i < 3; i++) {
				output[decodedLength++] = A3[i];
			}
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++) {
			A4[j] = '\0';
		}

		for (j = 0; j <4; j++) {
			A4[j] = lookupTable(A4[j]);
		}

		fromA4ToA3(A3,A4);

		for (j = 0; j < i - 1; j++) {
			output[decodedLength++] = A3[j];
		}
	}
	output[decodedLength] = '\0';
	return decodedLength;
}

int CircuitIoTDecoder::decodedLength(char * input, int inputLength) {
	int i = 0;
	int numEq = 0;
	for(i = inputLength - 1; input[i] == '='; i--) {
		numEq++;
	}

	return ((6 * inputLength) / 8) - numEq;
}

inline void CircuitIoTDecoder::fromA4ToA3(unsigned char * A3, unsigned char * A4) {
	A3[0] = (A4[0] << 2) + ((A4[1] & 0x30) >> 4);
	A3[1] = ((A4[1] & 0xf) << 4) + ((A4[2] & 0x3c) >> 2);
	A3[2] = ((A4[2] & 0x3) << 6) + A4[3];
}

inline unsigned char CircuitIoTDecoder::lookupTable(char c) {
	if(c >='A' && c <='Z') return c - 'A';
	if(c >='a' && c <='z') return c - 71;
	if(c >='0' && c <='9') return c + 4;
	if(c == '+') return 62;
	if(c == '/') return 63;
	return -1;
}

CircuitIoTDecoder Decoder;
