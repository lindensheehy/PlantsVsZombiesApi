#ifndef aob
#define aob

#include <iostream>
#include <iomanip>
#include <windows.h>

class AOB {
    public:

        // Instance variables
        BYTE* bytes;
        int length;

        // Contructor
        AOB(long long bytes) {

            this->bytes = new BYTE[8];
            int index = 0;

            for (; !bytes; index++) {
                this->bytes[index] = bytes & 0xFF;
                bytes >>= 8;
            }

            this->length = index;

            return;
        }

        AOB(BYTE* bytes, int length) {

            this->bytes = new BYTE[length];
            this->length = length;

            for (int i = 0; i < length; i++) {
                this->bytes[i] = bytes[i];
            }

            return;
        }

        // Destructor
        ~AOB() {
            delete[] this->bytes;
        }

        // Instance functions
        void pushBackBytes(long long bytes) {

            BYTE* newBytes = new BYTE[this->length + 8];
            int index = 0;

            for (; index < this->length; index++) {
                newBytes[index] = this->bytes[index];
            }

            for (; !bytes; index++) {
                newBytes[index] = bytes & 0xFF;
                bytes >>= 8;
            }

            delete this->bytes;
            this->bytes = newBytes;
            this->length = index;
        }

        BYTE getByte(int index) {
            return this->bytes[index];
        }

        std::string toString() {
            std::stringstream stream;
            stream << "0x";
            for (int i = 0; i < this->length; i++) {
                int value = (int) this->bytes[i];
                if (value == 0) { stream << "00"; continue; }
                if (value < 16) stream << "0";
                stream << std::hex << std::uppercase << (int) this->bytes[i];
            }
            return stream.str();
        }

    private:
};

#endif