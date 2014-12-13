#!/usr/bin/python2.7

import MotorController

def main():
	print 'Serial test begins'

	motor1 = MotorController.MotorController("motor1",1,0x10, 0x20)
	motor2 = MotorController.MotorController("motor2",5,112,46)

	print 'motor contollers declared'

	motor1.info()
	motor2.info()

	motor1.send_command()
	motor2.send_command()

	while True:
		response = raw_input('Enter a character: ')
		print 'You entered %s' % response
		if response is 'a':
			print 'Pressed a'
		elif response is 'b':
			print 'Pressed b'
		elif response is 'q':
			print 'Pressed q'
			print 'Quitting'
			break
		elif response.isdigit():
			print hex(int(response))
			motor2.set_position(response,0)
			motor2.send_command()
		else:
			print 'Not recognized'
		print
	print

if __name__ == '__main__':
	main()
