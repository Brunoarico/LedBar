import time
from pythonosc import udp_client
import sys
import colorsys
import random
NUM_LEDS = 60

client = udp_client.SimpleUDPClient("192.168.0.198", 2807)

def generate_pattern():
    arr = [0]*3*NUM_LEDS
    c = 255
    for i in range(0, len(arr), 3):
        (r, g, b) = colorsys.hsv_to_rgb(i/len(arr), 1.0, 1.0)
        arr[i] = r
        arr[i+1] = g
        arr[i+2] = b
    return arr

def rotate ():
    arr = generate_pattern()
    print(arr)
    while(True):
        r = arr.pop(0)
        g = arr.pop(0)
        b = arr.pop(0)
        arr.append(r)
        arr.append(g)
        arr.append(b)
        client.send_message("/LEDS", arr)
        time.sleep(0.2)
        print(arr)

def unique ():
    arr = [0]*3*NUM_LEDS
    c = 16
    arr[0] = 1
    while(True):
        r = arr.pop(0)
        g = arr.pop(0)
        b = arr.pop(0)
        arr.append(r)
        arr.append(g)
        arr.append(b)
        client.send_message("/LEDS", arr)
        time.sleep(0.2)
        print(arr)

def off ():
    arr = [0.0]*3*NUM_LEDS
    client.send_message("/LEDS", arr)
    print(arr)

def inc ():
    
    arr = []
    for i in range(NUM_LEDS):
        r = random.uniform(0, 0.5)
        g = random.uniform(0, 0.5)
        b = random.uniform(0, 0.5)
        arr.append(r)
        arr.append(g)
        arr.append(b)
        print(r,g,b)
        print(len(arr)/3)
        client.send_message("/LEDS", arr)
        time.sleep(1)

#rotate()
off()
inc()
#client.send_message("/LEDS", [1,0.0,0.0, 0.0,1.,0.0, 0.,.0,1.0, 0.,01.,1.])
