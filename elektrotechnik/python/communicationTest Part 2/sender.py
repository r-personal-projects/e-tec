import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

GPIO.setmode(GPIO.BCM);

pipes = [[0xe7, 0xe7, 0xe7, 0xe7, 0xe7], [0xc2, 0xc2, 0xc2, 0xc2, 0xc2]]

GPIO.cleanup()

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 17)
radio.setPayloadSize(32)
radio.setChannel(0x60)
radio.setDataRate(NRF24.BR_2MBPS)
radio.setPALevel(NRF24.PA_MIN)
radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()

radio.openWritingPipe(pipes[1])
radio.printDetails()

while True:
    message = list("Hello World")
    radio.write(message)
    print("{} was sent".format(message))

    if radio.isAckPayloadAvailable():
        returnedPayload = []
        radio.read(returnedPayload, radio.getDynamicPayloadSize())
        print("Payload was: {}".format(returnedPayload))
    else:
        print("No Payload sent")

        time.sleep(1)