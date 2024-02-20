#ifndef HGPIO_H
#define HGPIO_H

#define HGPIO_PORT_SHIFT 8
#define HGPIO_PORT_MASK (0xff << HGPIO_PORT_SHIFT)
#define HGPIO_IO_SHIFT 0
#define HPGIO_IO_MASK ((1 << HGPIO_PORT_SHIFT) - 1)

#define HGPIO_PORT(gpio) ((uint8_t)(gpio >> HGPIO_PORT_SHIFT))
#define HGPIO_IO(gpio) ((uint8_t)(gpio))
#define HGPIO_DEF_GPIO(port, io) \
	((port << HGPIO_PORT_SHIFT) | (io & HPGIO_IO_MASK))

enum hgpio_port {
	HGPIO_PA = 0,
	HGPIO_PB = 1,
	HGPIO_PC = 2,
	HGPIO_PD = 3,
	HGPIO_PE = 4,
	HGPIO_PF = 5,
	HGPIO_PG = 6,
	HGPIO_PH = 7,
};

enum hgpio_num {
	HGPIO_0 = 0,
	HGPIO_1 = 1,
	HGPIO_2 = 2,
	HGPIO_3 = 3,
	HGPIO_4 = 4,
	HGPIO_5 = 5,
	HGPIO_6 = 6,
	HGPIO_7 = 7,
	HGPIO_8 = 8,
	HGPIO_9 = 9,
	HGPIO_10 = 10,
	HGPIO_11 = 11,
	HGPIO_12 = 12,
	HGPIO_13 = 13,
	HGPIO_14 = 14,
	HGPIO_15 = 15,
};

enum hgpio_mode {
	HGPIO_MODE_IN = 0b00,
	HGPIO_MODE_OUT = 0b01,
	HGPIO_MODE_AF = 0b10,
	HGPIO_MODE_AN = 0b11,
	HGPIO_MODE_MSK = 0b11
};

enum hgpio_otype {
	HGPIO_OTYPE_PP = 0b0,
	HGPIO_OTYPE_OD = 0b1,
	HGPIO_OTYPE_MSK = 0b1
};

enum hgpio_ospeed {
	HGPIO_OSPEED_LO = 0b00,
	HGPIO_OSPEED_ME = 0b01,
	HGPIO_OSPEED_HI = 0b10,
	HGPIO_OSPEED_VH = 0b11,
	HGPIO_OSPEED_MSK = 0b11,
};

enum hgpio_pupd {
	HGPIO_PUPD_NO = 0b00,
	HGPIO_PUPD_PU = 0b01,
	HGPIO_PUPD_PD = 0b10,
	HGPIO_PUPD_MSK = 0b11,
};

enum hgpio_af {
	HGPIO_AF_0 = 0b0000,
	HGPIO_AF_1 = 0b0001,
	HGPIO_AF_2 = 0b0010,
	HGPIO_AF_3 = 0b0011,
	HGPIO_AF_4 = 0b0100,
	HGPIO_AF_5 = 0b0101,
	HGPIO_AF_6 = 0b0110,
	HGPIO_AF_7 = 0b0111,
	HGPIO_AF_8 = 0b1000,
	HGPIO_AF_9 = 0b1001,
	HGPIO_AF_10 = 0b1010,
	HGPIO_AF_11 = 0b1011,
	HGPIO_AF_12 = 0b1100,
	HGPIO_AF_13 = 0b1101,
	HGPIO_AF_14 = 0b1110,
	HGPIO_AF_15 = 0b1111,
	HGPIO_AF_MSK = 0b1111,
};

void hgpio_set_mode(uint16_t gpio, enum hgpio_mode mode);
void hgpio_set_otype(uint16_t gpio, enum hgpio_otype type);
void hgpio_set_ospeed(uint16_t gpio, enum hgpio_ospeed speed);
void hgpio_set_pupd(uint16_t gpio, enum hgpio_pupd pupd);
uint8_t hgpio_get_idr(uint16_t gpio);
void hgpio_set_od(uint16_t gpio, uint8_t val);
void hgpio_set(uint16_t gpio);
void hgpio_clr(uint16_t gpio);
void hgpio_set_af(uint16_t gpio, enum hgpio_af af);

#endif /* HGPIO_H */
