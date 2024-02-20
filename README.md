# CMSIS STM32L4 Example projects

This repository contains examples used on STM32L412KB device, but it is
configurable in the Makefile for a different device.

## Setup

Setup the docker container (used to buidl and flash).

```bash
make docker-setup
```

## Build an app

Run the make command with the target as `<folder>.<subtarget>`.

```bash
make projects/01-blink/.all
```

## Flash an app

Run the make command with the target as `<folder>.<subtarget>`.

```bash
make projects/01-blink/.flash
```
