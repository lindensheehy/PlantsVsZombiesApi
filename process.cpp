#ifndef processcpp
#define processcpp

#include <iostream>
#include <windows.h>

#include "log.cpp"
#include "aob.h"

class Process {

    public:

        // Instance variables
        int8bytes processId;
        HANDLE process;

        // Constructor
        Process(uintptr_t processId) {
            
            // Open the process as a HANDLE
            this->processId = processId;
            this->process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

        }

        ~Process() {
            CloseHandle(this->process);
        }

        // Instance functions
        BYTE readByte(int8bytes address) {
            BYTE returnValue;
            ReadProcessMemory(this->process, reinterpret_cast<LPCVOID>(address), &returnValue, sizeof(BYTE), NULL);
            return returnValue;
        }

        BYTE readByte(int4bytes address) {
            return this->readByte( (int8bytes) address );
        }

        AOB* read4Bytes(int8bytes address) {
            BYTE bytes[4];
            ReadProcessMemory(this->process, reinterpret_cast<LPCVOID>(address), &bytes, sizeof(BYTE) * 4, NULL);
            return new AOB(bytes, 4);
        }

        AOB* read8Bytes(int8bytes address) {
            BYTE bytes[8];
            ReadProcessMemory(this->process, reinterpret_cast<LPCVOID>(address), &bytes, sizeof(BYTE) * 8, NULL);
            return new AOB(bytes, 8);
        }

        void readxBytes(int8bytes address, BYTE* array, int size) {
            ReadProcessMemory(this->process, reinterpret_cast<LPCVOID>(address), array, size, NULL);
            return;
        }

        void writeByte(int8bytes address, BYTE value) {
            WriteProcessMemory(this->process, reinterpret_cast<LPVOID>(address), &value, sizeof(BYTE), NULL);
        }

        void write4bytesValue(int8bytes address, int4bytes value) {
            WriteProcessMemory(this->process, reinterpret_cast<LPVOID>(address), &value, sizeof(int4bytes), NULL);
        }

        void write4bytesAOB(int8bytes address, int4bytes value) {
            int4bytes effectiveValue = 0;
            int4bytes valueCopy = value;
            // Load the bytes backwards from the input into a new variable
            // This is just how instructions are handled between actual memory and cheat engine where I got the bytes
            for (int i = 0; i < 3; i++) {
                effectiveValue += valueCopy & 0xFF;
                effectiveValue <<= 8;
                valueCopy >>= 8;
            }
            effectiveValue += valueCopy & 0xFF;
            WriteProcessMemory(this->process, reinterpret_cast<LPVOID>(address), &effectiveValue, sizeof(int4bytes), NULL);
        }

        void write8bytesValue(int8bytes address, int8bytes value) {
            WriteProcessMemory(this->process, reinterpret_cast<LPVOID>(address), &value, sizeof(int8bytes), NULL);
        }

        void write8bytesAOB(int8bytes address, int8bytes value) {
            int8bytes effectiveValue = 0;
            int8bytes valueCopy = value;
            // Load the bytes backwards from the input into a new variable
            // This is just how instructions are handled between actual memory and cheat engine where I got the bytes
            for (int i = 0; i < 7; i++) {
                effectiveValue += valueCopy & 0xFF;
                effectiveValue <<= 8;
                valueCopy >>= 8;
            }
            effectiveValue += valueCopy & 0xFF;
            WriteProcessMemory(this->process, reinterpret_cast<LPVOID>(address), &effectiveValue, sizeof(int8bytes), NULL);
        }

        int8bytes aobScan(BYTE* requestedAOB, int length, int8bytes from = 0, int8bytes to = 0xFFFFFFFF) {
            
            // These are indexes for searching
            int8bytes memoryReadLocation = from; // location in memory where the next aob will come from
            int memoryByte = 0; // index of the byte within the memory aobs
            int aobByte = 0; // index of the byte within the requestedAOB

            // This list stores the AOBs from memory which are being searched
            // Size is input aob length / 8 because it needs to be able to hold enough bytes to get through the requested aob
            int4bytes memoryBytesSize = 0x10040; // 0x40 bytes of overlap between reads to avoid missed bytes, this allows aobs of size < 64
            int4bytes memoryReadLocationIncrement = 0x10000; // The read memory location changes by this much every iteration
            BYTE* memoryBytes = new BYTE[memoryBytesSize];
            this->readxBytes(memoryReadLocation, memoryBytes, memoryBytesSize);

            boolean searchFound;

            while ( (memoryReadLocation + memoryBytesSize) < to ) {
                while(memoryByte < memoryReadLocationIncrement) {

                    searchFound = true;
                    for (; aobByte < length; aobByte++) {

                        BYTE byte1 = requestedAOB[aobByte];
                        BYTE byte2 = memoryBytes[memoryByte + aobByte];

                        if (byte1 != byte2) {
                            searchFound = false;
                            break;
                        }
                    }

                    // AOB found case
                    if (searchFound) return (memoryReadLocation + (int8bytes) memoryByte);

                    // Iteration change
                    aobByte = 0;
                    memoryByte++;
                }

                // Iteration change
                memoryByte = 0;
                memoryReadLocation += memoryReadLocationIncrement;

                delete[] memoryBytes;
                memoryBytes = new BYTE[memoryBytesSize];

                this->readxBytes(memoryReadLocation, memoryBytes, memoryBytesSize);
                
            }

            return -1;
        }

        int8bytes aobScan(const BYTE* requestedAOB, int length, int8bytes from = 0, int8bytes to = 0xFFFFFFFF) {
            return this->aobScan( (BYTE*) requestedAOB, length, from, to);
        }

        int8bytes aobScanDebug(BYTE* requestedAOB, int length, int8bytes from = 0, int8bytes to = 0xFFFFFFF) {

            // These are indexes for searching
            int8bytes memoryReadLocation = from; // location in memory where the next aob will come from
            int memoryByte = 0; // index of the byte within the memory aobs
            int aobByte = 0; // index of the byte within the requestedAOB

            // This list stores the AOBs from memory which are being searched
            // Size is input aob length / 8 because it needs to be able to hold enough bytes to get through the requested aob
            int memoryBytesSize = 0x2000;
            BYTE* memoryBytes = new BYTE[memoryBytesSize];
            this->readxBytes(memoryReadLocation, memoryBytes, memoryBytesSize);

            logWrite("Starting aob scan of length: ", false);
            logWrite(length, true);

            logWrite("aob: ", false);

            for (int i = 0; i < length; i++) {
                logWrite((BYTE) requestedAOB[i], false);
                logWrite(" ", false);
            }

            logNewLine();

            logWrite("address: ", false);
            logWrite((int8bytes) from, false);
            logWrite(" -> ", false);
            logWrite((int8bytes) to, false);

            logWrite("\n", true);
        

            boolean searchFound;

            while (((unsigned long long) memoryReadLocation + (unsigned long long) memoryBytesSize) < (unsigned long long) to) {

                logWrite("starting: ", false);
                logWrite((int8bytes) memoryReadLocation, false);
                logWrite(" -> ", false);
                logWrite((int8bytes) memoryReadLocation + memoryBytesSize, true);

                while(memoryByte < memoryBytesSize - length) {

                    logWrite("byte: ", false);
                    logWrite((int8bytes) memoryByte + memoryReadLocation, false);
                    logWrite("   ---   subBytes -> 1: ", false);
                    logWrite(requestedAOB[0], false);
                    logWrite("  2: ", false);
                    logWrite(memoryBytes[memoryByte], true);

                    searchFound = true;
                    for (; aobByte < length; aobByte++) {

                        BYTE byte1 = requestedAOB[aobByte];
                        BYTE byte2 = memoryBytes[memoryByte + aobByte];

                        if (byte1 != byte2) {
                            searchFound = false;
                            break;
                        }
                    }

                    // AOB found case
                    if (searchFound) return (memoryReadLocation + (int8bytes) memoryByte);

                    // Iteration change
                    aobByte = 0;
                    memoryByte++;
                }

                // Iteration change
                memoryByte = 0;
                memoryReadLocation += memoryBytesSize;

                delete[] memoryBytes;
                memoryBytes = new BYTE[memoryBytesSize];

                this->readxBytes(memoryReadLocation, memoryBytes, 0x2000);
                
            }

            return -1;
        }

        int4bytes getPointer4(int8bytes address, int8bytes offset = 0) {

            // Get bytes
            BYTE bytes[4];
            LPCVOID effectiveAddress = reinterpret_cast<LPCVOID>(address + offset);
            ReadProcessMemory(this->process, effectiveAddress, &bytes, sizeof(BYTE) * 4, NULL);

            // Load the bytes into a long type
            // Load them in backwards because thats how theyre treated in memory
            int4bytes ret = 0;
            ret += bytes[0];
            ret += ( (int4bytes) bytes[1] ) << 8;
            ret += ( (int4bytes) bytes[2] ) << 16;
            ret += ( (int4bytes) bytes[3] ) << 24;

            return ret;
        }

        int8bytes getPointer8(int8bytes address, int8bytes offset = 0) {

            // Get bytes
            BYTE bytes[8];
            LPCVOID effectiveAddress = reinterpret_cast<LPCVOID>(address + offset);
            ReadProcessMemory(this->process, effectiveAddress, &bytes, sizeof(BYTE) * 8, NULL);

            // Load the bytes into a int8byte type
            // Load them in backwards because thats how theyre treated in memory
            int8bytes ret = 0;
            ret += bytes[0];
            ret += ( (int8bytes) bytes[1] ) << 8;
            ret += ( (int8bytes) bytes[2] ) << 16;
            ret += ( (int8bytes) bytes[3] ) << 24;
            ret += ( (int8bytes) bytes[4] ) << 32;
            ret += ( (int8bytes) bytes[5] ) << 40;
            ret += ( (int8bytes) bytes[6] ) << 48;
            ret += ( (int8bytes) bytes[7] ) << 56;
            
            return ret;
        }

    private:
};

#endif