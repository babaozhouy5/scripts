#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <vector>

#define F16_EXPONENT_BITS 0x1F
#define F16_EXPONENT_SHIFT 10
#define F16_EXPONENT_BIAS 15
#define F16_MANTISSA_BITS 0x3ff
#define F16_MANTISSA_SHIFT (23 - F16_EXPONENT_SHIFT)
#define F16_MAX_EXPONENT (F16_EXPONENT_BITS << F16_EXPONENT_SHIFT)
#define F32_INFINITY 0x7f800000

inline uint16_t F32toF16(float val)
{
    uint32_t f32 = (*(uint32_t *) &val);
    uint16_t f16 = 0;

    /* Decode IEEE 754 little-endian 32-bit floating-point value */
    int sign = (f32 >> 16) & 0x8000;
    /* Map exponent to the range [-127,128] */
    int exponent = ((f32 >> 23) & 0xff) - 127;
    int mantissa = f32 & 0x007fffff;

    if (exponent == 128)
    { /* Infinity or NaN */
        f16 = sign | F16_MAX_EXPONENT;
        if (mantissa) f16 |= (mantissa & F16_MANTISSA_BITS);
    }
    else if (exponent > 15)
    { /* Overflow - flush to Infinity */
        f16 = sign | F16_MAX_EXPONENT;
    }
    else if (exponent > -15)
    { /* Representable value */
        exponent += F16_EXPONENT_BIAS;
        mantissa >>= F16_MANTISSA_SHIFT;
        f16 = sign | exponent << F16_EXPONENT_SHIFT | mantissa;
    }
    else
    {
        f16 = sign;
    }
    return f16;
}

inline float F16toF32(unsigned short val)
{
    union {
        float f;
        uint32_t ui;
    } f32;

    uint16_t f16 = (*(uint16_t*) &val);

    int sign        = (f16 & 0x8000) << 15;
    int exponent    = (f16 & 0x7c00) >> 10;
    int mantissa    = (f16 & 0x03ff);

    f32.f = 0.0;

    if(exponent == 0){
        if(mantissa != 0) {
            const float scale = 1.0 / (1 << 24);
            f32.f = scale * mantissa;
        }
    }
    else if(exponent == 31) {
        f32.ui = sign | F32_INFINITY | mantissa;
    }
    else {
        float scale, decimal;
        exponent -= 15;
        if(exponent < 0) {
            scale = 1.0 / (1 << -exponent);
        }
        else {
            scale = 1 << exponent;
        }
        decimal = 1.0 + (float) mantissa / (1 << 10);
        f32.f = scale * decimal;
    }

    if(sign) f32.f = -f32.f;

    return f32.f;
}

int main(int argc, char* argv[])
{
    int ret = 0;

    if (argc < 2)
    {
        printf("usage: ./half2float half.bin\n");
        return ret;
    }

    FILE *f = fopen(argv[1], "rb");

    printf("loading model %s\n", argv[1]);

    std::vector<uint16_t> buf;
    fseek(f, 0L, SEEK_END);
    int buf_len = ftell(f) / sizeof(uint16_t);
    buf.resize(buf_len);
    fseek(f, 0L, SEEK_SET);
    fread(buf.data(), sizeof(uint16_t), buf_len, f);
    fclose(f);

    for (int i = 0; i < buf.size(); ++i)
    {
        float val = F16toF32(buf[i]);
        printf("%f ", val);
    }
    printf("\n");

    return ret;
}