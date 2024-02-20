#include "hgpio.h"
#include "stm32l4xx.h"

#include <stdint.h>

struct hgpio_port_map {
	uint8_t port;
	GPIO_TypeDef *pgpio;
};

struct hgpio_port_map port_map[] = {
	{HGPIO_PA, GPIOA}, {HGPIO_PB, GPIOB}, {HGPIO_PC, GPIOC}, {HGPIO_PD, GPIOD},
	{HGPIO_PE, 0},	   {HGPIO_PF, 0},	  {HGPIO_PG, 0},	 {HGPIO_PH, GPIOH},
};

void hgpio_set_mode(uint16_t gpio, enum hgpio_mode mode) {
	GPIO_TypeDef *p;
	uint8_t port = HGPIO_PORT(gpio);
	uint8_t io = HGPIO_IO(gpio);

	p = port_map[port].pgpio;

	io = io << 1;
	p->MODER &= ~(HGPIO_MODE_MSK << io);
	p->MODER |= (mode << io);
}

void hgpio_set_otype(uint16_t gpio, enum hgpio_otype type) {
	GPIO_TypeDef *p;
	uint8_t port = HGPIO_PORT(gpio);
	uint8_t io = HGPIO_IO(gpio);

	p = port_map[port].pgpio;

	io <<= 0;
	p->OTYPER &= ~(HGPIO_OTYPE_MSK << io);
	p->OTYPER |= (type << io);
}

void hgpio_set_ospeed(uint16_t gpio, enum hgpio_ospeed speed) {
	GPIO_TypeDef *p;
	uint8_t port = HGPIO_PORT(gpio);
	uint8_t io = HGPIO_IO(gpio);

	p = port_map[port].pgpio;

	io <<= 1;
	p->OSPEEDR &= ~(HGPIO_OSPEED_MSK << io);
	p->OSPEEDR |= (speed << io);
}

void hgpio_set_pupd(uint16_t gpio, enum hgpio_pupd pupd) {
	GPIO_TypeDef *p;
	uint8_t port = HGPIO_PORT(gpio);
	uint8_t io = HGPIO_IO(gpio);

	p = port_map[port].pgpio;

	io <<= 1;
	p->PUPDR &= ~(HGPIO_PUPD_MSK << io);
	p->PUPDR |= (pupd << io);
}

uint8_t hgpio_get_idr(uint16_t gpio) {
	GPIO_TypeDef *p;
	uint8_t port = HGPIO_PORT(gpio);
	uint8_t io = HGPIO_IO(gpio);

	p = port_map[port].pgpio;

	return (uint8_t)(p->IDR & (1 << io)) >> io;
}

void hgpio_set_od(uint16_t gpio, uint8_t val) {
	GPIO_TypeDef *p;
	uint8_t port = HGPIO_PORT(gpio);
	uint8_t io = HGPIO_IO(gpio);

	p = port_map[port].pgpio;

	io <<= 0;
	p->ODR &= ~((0x1) << io);
	p->ODR |= ((val & 0x1) << io);
}

void hgpio_set(uint16_t gpio) {
	GPIO_TypeDef *p;
	uint8_t port = HGPIO_PORT(gpio);
	uint8_t io = HGPIO_IO(gpio);

	p = port_map[port].pgpio;

	p->BSRR = io;
}

void hgpio_clr(uint16_t gpio) {
	GPIO_TypeDef *p;
	uint8_t port = HGPIO_PORT(gpio);
	uint8_t io = HGPIO_IO(gpio);

	p = port_map[port].pgpio;

	p->BRR = io;
}

void hgpio_set_af(uint16_t gpio, enum hgpio_af af) {
	GPIO_TypeDef *p;
	uint8_t port = HGPIO_PORT(gpio);
	uint8_t io = HGPIO_IO(gpio);

	p = port_map[port].pgpio;

	if (io < 8) {
		io = io * 4;
		p->AFR[0] &= ~(HGPIO_AF_MSK << io);
		p->AFR[0] |= af << io;
	} else {
		io = (io - 8) * 4;
		p->AFR[1] &= ~(HGPIO_AF_MSK << io);
		p->AFR[1] |= af << io;
	}
}