from pynput import keyboard
import os

status = 0

def action_lock():
    global status
    status = 1
    os.system('ft_lock')

def action_unlock():
    global status
    status = 0
    os.system('killall -9 ft_lock')

def on_press(key):
    try:
        if status == 0 and key == keyboard.KeyCode.from_char('a'):
            action_lock()
        elif status == 1 and key == keyboard.KeyCode.from_char('a'):
            action_unlock()
    except AttributeError:
        pass

def main():
    with keyboard.Listener(on_press=on_press) as listener:
        listener.join()

if __name__ == "__main__":
    main()

# pip3 install --user pynput
# python3 -m pip install --user pynput
# python3 key_lock.py
