import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

GPIO.setmode(GPIO.BCM);

pipes = [[0xe7, 0xe7, 0xe7, 0xe7, 0xe7], [0xc2, 0xc2, 0xc2, 0xc2, 0xc2]]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 17)
radio.setPayloadSize(32)
radio.setChannel(0x60)
radio.setDataRate(NRF24.BR_2MBPS)
radio.setPALevel(NRF24.PA_MIN)
radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()


radio.openReadingPipe(1, pipes[0])
radio.printDetails()

radio.startListening()

while True:
    ackPayload = [1]
    while not radio.available(0):
        time.sleep(1 / 100)
    receivedMessage = []
    radio.read(receivedMessage, radio.getDynamicPayloadSize())
    print("Received: {}".format(receivedMessage))

    if len(receivedMessage) == 4:
        alpha = receivedMessage.pop()
        blue = receivedMessage.pop()
        green = receivedMessage.pop()
        red = receivedMessage.pop()
        print("Red = {} | Green = {} | Blue = {} | Alpha = {}".format(red, green, blue, alpha))

    radio.writeAckPayload(1, ackPayload, len(ackPayload))
    print("Loaded Payload reply")