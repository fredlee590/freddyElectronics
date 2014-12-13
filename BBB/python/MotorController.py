#!/usr/bin/python2.7

import serial

class MotorController:
	def info(self):
		print 'Motor Controller object'
		print 'Name: %s' % self.__name
		print 'Channel: %x' % self.__channel
		print 'Values: %x %x' % (self.__value_hi, self.__value_lo)

	def set_position(self, value_hi, value_lo):
		self.__value_hi = int(value_hi)
		self.__value_lo = int(value_lo)

	def send_command(self):
		ser = serial.Serial("/dev/ttyACM0")
		# use compact protocol
		header = chr(84)
		ser.write(header)
		ser.flush()

		command = '%c%c%c' % (self.__channel, self.__value_hi, self.__value_lo)
		print command

		ser.write(command)
		ser.flush()
		ser.close()

	def __init__(self, name, channel, value_hi, value_lo):
		self.__name = name
		self.__channel = channel
		self.__value_hi = value_hi
		self.__value_lo = value_lo
