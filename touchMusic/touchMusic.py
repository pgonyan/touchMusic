#!/usr/bin/python
  # -*- coding: latin-1 -*-

import fcntl
import os
import sys
import termios

## pymedia

from pygame import mixer # Load the required library


def getch():
    fd = sys.stdin.fileno()

    oldattr = termios.tcgetattr(fd)
    newattr = termios.tcgetattr(fd)
    newattr[3] = newattr[3] & ~termios.ICANON & ~termios.ECHO
    termios.tcsetattr(fd, termios.TCSANOW, newattr)

    oldflags = fcntl.fcntl(fd, fcntl.F_GETFL)
    fcntl.fcntl(fd, fcntl.F_SETFL, oldflags | os.O_NONBLOCK)

    try:
        while True:
            try:
                c = sys.stdin.read(1)
            except IOError:
                pass
            else:
                return c
    finally:
        termios.tcsetattr(fd, termios.TCSAFLUSH, oldattr)
        fcntl.fcntl(fd, fcntl.F_SETFL, oldflags)



print("Please press a key to see its value")
mixer.init()


s1 = mixer.Sound("./music/loopsA/beep1.ogg")
s2 = mixer.Sound("./music/loopsA/beep2.ogg")
s3 = mixer.Sound("./music/loopsA/beep3.ogg")
s4 = mixer.Sound("./music/loopsA/another1.ogg")



print ("Numero de canales %d",  mixer.get_num_channels() )

firstTimeC1 = 1
firstTimeC2 = 1
firstTimeC3 = 1
firstTimeC4 = 1

	
while 1:
    a = getch()
    print("the key is")
    print(a)
    print (ord(a))
    if a == 'A':
	print ("A")
	if firstTimeC1 :
		c1 = s1.play()
		firstTimeC1 = 0
	elif not c1.get_busy():	
		c1 = s1.play()     

    if a == 'a':
	if not firstTimeC4 :
		c1 = s1.stop()
		firstTimeC1 = 1


    if a == 'B':
	print ("B")
	if firstTimeC2 :
		c2 = s2.play()
		firstTimeC2 = 0
	elif not c2.get_busy():	
		c2 = s2.play()     

    if a == 'b':
	if not firstTimeC2 :
		c2 = s2.stop()
		firstTimeC2 = 1

    if a == 'C':
	print ("C")
	if firstTimeC3 :
		c3 = s3.play()
		firstTimeC3 = 0
	elif not c3.get_busy():	
		c3 = s3.play()     

    if a == 'c':
	if not firstTimeC3 :
		c3 = s3.stop()
		firstTimeC3 = 1

    if a == 'D':
	if firstTimeC4 :
		c4 = s4.play()
		firstTimeC4 = 0
	elif not c4.get_busy():	
		c4 = s4.play()      


    if a == 'd':
	if not firstTimeC4 :
		c4 = s4.stop()
		firstTimeC4 = 1




    if a == 'Q':
	quit()


#set_num_channels(count)











