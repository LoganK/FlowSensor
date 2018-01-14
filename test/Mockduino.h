#if !defined(MOCKDUINO_H)
#define MOCKDUINO_H

typedef enum pin_mode_t {
  LOW,
  HIGH
} pin_mode_t;

extern unsigned long _micros;
unsigned long micros();

#endif
