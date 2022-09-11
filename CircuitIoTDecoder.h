#ifndef _CIRCUITIOT_DECODER_H
#define _CIRCUITIOT_DECODER_H

class CircuitIoTDecoder{
  public:
    int decode(char * output, char * input, int inputLength);
    int decodedLength(char * input, int inputLength);

  private:
    inline void fromA4ToA3(unsigned char * A3, unsigned char * A4);
    inline unsigned char lookupTable(char c);
};
extern CircuitIoTDecoder Decoder;
#endif
