#!/usr/bin/python3
from dataclasses import dataclass
import binascii

@dataclass
class info:
    P6: bool
    size: int
    Values: list

#opens the ppm file
def openPPMFile(filename, valueList):

    ppmInfo = info(False, 2, valueList)

    try:

        #opens file to read as a text file
        with open(filename, "r") as fileInfo:

            ppmInfo = P3Parser(fileInfo, ppmInfo, filename)
            fileInfo.close()
    
    except UnicodeDecodeError:

        #opens file to read as a binary file
        fileInfo = open(filename, "rb")
        ppmInfo = P6Parser(fileInfo, ppmInfo)
        fileInfo.close()

    return valueList

#gets each line of the P3 file
def P3Parser(fileInfo, ppmInfo, filename):
    
    #reads in a line
    line = fileInfo.readline()

    while line :

        #extracts info for the line
        ppmInfo = storingInfo(line.strip(), ppmInfo)

        #if a binary file is opened in "r" mode then it opens it up and send it to P6Parser
        if ppmInfo.P6 == True:
            fileInfo.close()
            
            fileInfo = open(filename, "rb")
            ppmInfo = P6Parser(fileInfo, ppmInfo)
            break

        #reads in next line
        line = fileInfo.readline()

    return ppmInfo

#stores the information provided by each line accordingly
def storingInfo(line, ppmInfo):
    
    temp = line.split()

    #checks if temp is empty
    if len(temp) >= 1:

        #checks if a file is a P6 file
        if temp[0] == "P6":

            ppmInfo.P6 = True

        #if the file is a P3 file
        elif ppmInfo.P6 == False:

            #stores the height and wide information
            if ppmInfo.Values[0] == -1 and ppmInfo.Values[1] == -1 and len(temp) == 2:

                if(temp[0].isdigit() and temp[1].isdigit()):

                    ppmInfo.Values[0] = int(temp[0])
                    ppmInfo.Values[1] = int(temp[1])

            #stores the depth information
            elif ppmInfo.Values[0] != -1 and ppmInfo.Values[1] != -1 and ppmInfo.Values[2] == -1:

                if(temp[0].isdigit()):

                    ppmInfo.Values[2] = int(temp[0])

            #stores the rbg information
            elif ppmInfo.Values[0] != -1 and ppmInfo.Values[1] != -1 and ppmInfo.Values[2] != -1:

                for x in range(len(temp)):

                    if temp[x].isdigit():

                        ppmInfo.size = ppmInfo.size + 1
                        ppmInfo.Values.append(int(temp[x]))
                        print(ppmInfo.size)

        #checks if the file is a P6 file
        elif ppmInfo.P6:

            #stores the height and wide information
            if ppmInfo.Values[0] == -1 and ppmInfo.Values[1] == -1 and len(temp) == 2:

                if(temp[0].isdigit() and temp[1].isdigit()):

                    ppmInfo.Values[0] = int(temp[0])
                    ppmInfo.Values[1] = int(temp[1])

            #stores the depth information
            elif ppmInfo.Values[0] != -1 and ppmInfo.Values[1] != -1 and ppmInfo.Values[2] == -1:

                if temp[0].isdigit():

                    ppmInfo.Values[2] = int(temp[0])
            
            #stores the rbg information
            elif ppmInfo.Values[0] != -1 and ppmInfo.Values[1] != -1 and ppmInfo.Values[2] != -1 and temp[0] != "P6" and temp[0] != "#":

                #converts value from binary to ascii
                for value in line:

                    token = (str(value)).split()

                    for i in range(len(token)):

                        if (token[i]).isdigit():
                    
                            ppmInfo.size = ppmInfo.size + 1
                            ppmInfo.Values.append(int(token[i]))


    return ppmInfo

#gets each line of the P6 file
def P6Parser(fileInfo, ppmInfo):

    line = fileInfo.readline()

    while line:

        storingInfo(line.strip(), ppmInfo)
        line = fileInfo.readline()

    return ppmInfo