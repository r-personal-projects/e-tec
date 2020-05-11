import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

GPIO.setmode(GPIO.BCM);

#ADRESSES
#- SENDER = pipes[0]
#- Bett = pipes[1]
pipes = [[0xe7, 0xe7, 0xe7, 0xe7, 0xe7], [0xc2, 0xc2, 0xc2, 0xc2, 0xc2]]

#SETUP
radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 17)
radio.setPayloadSize(32)
radio.setChannel(0x60)
radio.setDataRate(NRF24.BR_2MBPS)
radio.setPALevel(NRF24.PA_MAX)
radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()
radio.setRetries(20, 20)

radio.openWritingPipe(pipes[0])
radio.printDetails()

red =   0
green = 0
blue =  0
alpha = 0

message = [red, green, blue, alpha]
radio.write(message)
print("{} was sent".format(message))
time.sleep(5)
