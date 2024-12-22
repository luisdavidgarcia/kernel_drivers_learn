#!/usr/bin/python3

def main():
    with open('/proc/lg_driver') as driver_handle:
        message_from_kernel_space = driver_handle.readline()
        print(message_from_kernel_space)
    return

main()