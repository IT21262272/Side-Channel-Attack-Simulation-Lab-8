#!/usr/bin/env python3
import csv
with open('in.csv', newline='') as csvfile, open('out.bin','ab') as binaryfile:
    csvreader = csv.reader(csvfile, delimiter='\t')
    binaryfile.seek(0)
    binaryfile.truncate()
    for row in csvreader:
        for value in row:
            binary = int(value).to_bytes(2, byteorder='little')
            binaryfile.write(binary)