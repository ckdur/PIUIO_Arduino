#ifndef PIUIO_CTRL_H
#define PIUIO_CTRL_H

extern int nControl;
void SetupHardware(void);
void USB_Loop(void);

extern unsigned char LampData[8];
extern unsigned char InputData[8];

void go_next_device(void);

#endif
