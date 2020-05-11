import traceback
import zipfile
import itertools
import string
from threading import Thread


def bruteforce():
    letters = string.ascii_letters + string.digits  # +punctuations
    for i in range(3, 30):
        for j in map(''.join, itertools.product(letters, repeat=i)):
            t = Thread(target=crack, args=(zipfile, j))
            t.start()


def dictionary():
    password = open("/Users/rubeen/Desktop/rockyou.txt")
    for line in password.readlines():
        pwd = line.strip('\n')
        t = Thread(target=crack, args=(zipfile, pwd))
        t.start()


def crack(zip, pwd):
    try:
        zip.extractall(pwd=str.encode(pwd))
        print('Success: Password is ' + pwd)
        exit(0)
    except:
        pass


zipfile = zipfile.ZipFile("/Users/rubeen/Desktop/testitest.zip")
dictionary()
