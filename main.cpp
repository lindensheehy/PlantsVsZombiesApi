#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

#include "log.cpp"
#include "process.cpp"

// Stores all the base pointers which will be reference often
class Game {
    public:

        // Subclasses
        class Plant {
            public:

                // Offsets
                const int4bytes plantId = 0x24;
                const int4bytes healthOffset = 0x40;
                const int4bytes maxHealthOffset = 0x44;
                const int4bytes animationState = 0x2C; // Not sure on this one
                const int4bytes produceSunTimer = 0x58;
                const int4bytes produceSunTimerMax = 0x5C;

                // Instance variables
                int4bytes baseAddress;


                Plant(int4bytes baseAddress) {
                    this->baseAddress = baseAddress;
                }
        };

        class Zombie {
            public:

                // Instance variables
                int4bytes baseAddress;

                Zombie(int4bytes baseAddress) {
                    this->baseAddress = baseAddress;
                }
        };

        // AOBs - A lot of these arent used as most addresses are static
        const BYTE sunCollectAOB[12] = { 0x01, 0x88, 0x78, 0x55, 0x00, 0x00, 0x8B, 0x88, 0x78, 0x55, 0x00, 0x00 }; // First instruction in the sun collect function
        const BYTE sunValueAOB[8] = { 0xE8, 0xB4, 0xAD, 0xFE, 0xFF , 0xE9, 0xC7, 0x00 }; // 4 bytes after the location of the sun value added on sun pick up

        // Addresses (Values)
        const int4bytes mainBasePointer = 0x731C50;
        //const int4bytes sunCollectHook;
        const int4bytes sunValueAddress = 0x434713;

        // Addresses (Instructions)
        const int4bytes allPlantsBaseHook = 0x46A2F7; // Plant health instruction
        const int4bytes allSunPlantsBaseHook = 0x465DFB; // Plant sun timer instruction
        const int4bytes plantSunSpawnHook = 0x465E32; // Runs when a plant spawns sun
        const int4bytes skySunSpawnHook = 0x417075; // Runs when a plant spawns sun

        /*  ---  Instance Variables  ---  */
        Process* process;
        int4bytes mainBase; // This is a base pointer used to find game state stuff like sun count
        bool autoCollentSunEnabled;

        Game(Process* process) {
            this->process = process;
            this->mainBase = process->getPointer4(mainBasePointer);
            this->autoCollentSunEnabled = (this->process->readByte(this->plantSunSpawnHook) == 0x50); // when the cheats on this byte is changed to 50
        }

        ~Game() {
            if (this->process != NULL) {
                delete this->process;
            }
        }

        // Mods
        void autoCollectSunToggle() {

            // Enable mod
            if (!this->autoCollentSunEnabled) {

                /*  ---  From Sun Flowers  ---  */

                // Bytes to write
                int8bytes aob1 = 0x505152568B475C8D;
                int8bytes aob2 = 0xB06AFFFFFF29F040;
                int8bytes aob3 = 0x50E8A8F4190001F0;
                int8bytes aob4 = 0x8947588B0783C404;
                int8bytes aob5 = 0x5E5A59585E50B850;
                int8bytes aob6 = 0x1C73008B008B8068;
                int8bytes aob7 = 0x0800008380785500;
                int8bytes aob8 = 0x0019837F24290F85;
                int8bytes aob9 = 0x0C00000083807855;
                int8bytes aob10 = 0x000019E900000000;
                BYTE lastByte1 = 0x58;
                BYTE lastByte2 = 0xC3;

                // Writing
                this->process->write8bytesAOB(this->plantSunSpawnHook, aob1);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x8), aob2);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x10), aob3);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x18), aob4);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x20), aob5);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x28), aob6);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x30), aob7);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x38), aob8);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x40), aob9);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x48), aob10);
                this->process->writeByte((this->plantSunSpawnHook + 0x50), lastByte1);
                this->process->writeByte((this->plantSunSpawnHook + 0x51), lastByte2);

                /*  ---  From Sky  ---  */

                // Bytes to write
                aob1 = 0x5053515257682602;
                aob2 = 0x0000E86CE21E00FF;
                aob3 = 0x86545500008BF88B;
                aob4 = 0x86545500008D0480;
                aob5 = 0x8D8400A901000083;
                aob6 = 0xC40483C7643DB603;
                aob7 = 0x0000BBB60300007F;
                aob8 = 0x028BD86813010000;
                aob9 = 0xE836E21E008B8EA4;
                aob10 = 0x00000001D8898650;
                int8bytes aob11 = 0x55000083C4045F5A;
                int8bytes aob12 = 0x595B5850B8501C73;
                int8bytes aob13 = 0x008B008B80680800;
                int8bytes aob14 = 0x0083807855000019;
                int4bytes aob15 = 0x585F5BC3;

                // Writing
                this->process->write8bytesAOB(this->skySunSpawnHook, aob1);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x8), aob2);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x10), aob3);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x18), aob4);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x20), aob5);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x28), aob6);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x30), aob7);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x38), aob8);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x40), aob9);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x48), aob10);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x50), aob11);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x58), aob12);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x60), aob13);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x68), aob14);
                this->process->write4bytesAOB((this->skySunSpawnHook + 0x70), aob15);

                this->autoCollentSunEnabled = true;

            }

            // Disable mod
            else {
                
                /*  ---  From Sun Flowers  ---  */

                // Bytes to write
                int8bytes aob1 = 0x8B475C8DB06AFFFF;
                int8bytes aob2 = 0xFF29F04050E8ACF4;
                int8bytes aob3 = 0x190001F08947588B;
                int8bytes aob4 = 0x0783C404BE040000;
                int8bytes aob5 = 0x00E82830FFFF8B47;
                int8bytes aob6 = 0x2483F8097514837F;
                int8bytes aob7 = 0x3C176A0275328B57;
                int8bytes aob8 = 0x0C8B47086A055250;
                int8bytes aob9 = 0xEB5983F80175066A;
                int8bytes aob10 = 0x026A04EB4683F829;
                BYTE lastByte1 = 0x75;
                BYTE lastByte2 = 0x22;

                // Writing
                this->process->write8bytesAOB(this->plantSunSpawnHook, aob1);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x8), aob2);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x10), aob3);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x18), aob4);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x20), aob5);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x28), aob6);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x30), aob7);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x38), aob8);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x40), aob9);
                this->process->write8bytesAOB((this->plantSunSpawnHook + 0x48), aob10);
                this->process->writeByte((this->plantSunSpawnHook + 0x50), lastByte1);
                this->process->writeByte((this->plantSunSpawnHook + 0x51), lastByte2);

                /*  ---  From Sky  ---  */

                // Bytes to write
                aob1 = 0x6826020000E871E2;
                aob2 = 0x1E00FF8654550000;
                aob3 = 0x8BF88B8654550000;
                aob4 = 0x8D04808D8400A901;
                aob5 = 0x000083C40483C764;
                aob6 = 0x3DB6030000BBB603;
                aob7 = 0x00007F028BD86813;
                aob8 = 0x010000E83BE21E00;
                aob9 = 0x8B8EA400000003C3;
                aob10 = 0x89865055000083C4;
                int8bytes aob11 = 0x0483B91809000025;
                int8bytes aob12 = 0xB8040000007505B8;
                int8bytes aob13 = 0x060000006A00506A;
                int8bytes aob14 = 0x3C578BCEE8AA8EFF;
                int4bytes aob15 = 0xFF5F5BC3;

                // Writing
                this->process->write8bytesAOB(this->skySunSpawnHook, aob1);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x8), aob2);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x10), aob3);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x18), aob4);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x20), aob5);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x28), aob6);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x30), aob7);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x38), aob8);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x40), aob9);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x48), aob10);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x50), aob11);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x58), aob12);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x60), aob13);
                this->process->write8bytesAOB((this->skySunSpawnHook + 0x68), aob14);
                this->process->write4bytesAOB((this->skySunSpawnHook + 0x70), aob15);

                this->autoCollentSunEnabled = false;

            }

            return;
        }

};

int8bytes findProcessId(const char* processName) {

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        logWrite("Error creating process snapshot");
        return 0;
    }

    if (Process32First(hSnapshot, &processEntry)) {
        do {

            // Create a copy of the process name as a const
            const wchar_t* thisProcessName = (wchar_t*) processEntry.szExeFile;

            // Check all the chars of the process name against the requested name
            wchar_t charToPrint = thisProcessName[0];
            for (int i = 1; charToPrint != 0x0000; i++) {

                // get the two sub chars in the widechar
                char char1 = charToPrint & 0x00FF;
                char char2 = (charToPrint & 0xFF00) >> 8;

                // Check the sub chars against the requested char list
                int charIndex = (i - 1) * 2;
                if (char1 != processName[charIndex] || char2 != processName[charIndex + 1]) {
                    break;
                }

                // If one of the chars is a null char and the loop hasnt broken yet, this is the process
                if (!char1 || !char2) {
                    CloseHandle(hSnapshot);
                    return processEntry.th32ProcessID;
                }

                // For the next iteration
                charToPrint = thisProcessName[i];
            }

        // If the loop broke before hitting return, get next process and repeat
        } while (Process32Next(hSnapshot, &processEntry));
    }

    CloseHandle(hSnapshot);

    return -1;
}

void doInput(std::string string, bool* quit, Process* process, Game* game) {

    std::string command;
    std::string arg1;
    std::string arg2;
    int currentArg = 0; // 0 means its taking the command right now, after that its index

    // Finding the command and args
    char stringChar;
    for (int i = 0; i < string.length(); i++) {
        
        // Get next char
        stringChar = string.at(i);

        // Switch arg on space char
        if (stringChar == ' ') {
            currentArg++;
            continue;
        }

        // Add the char to a string
        switch (currentArg) {
            case 0:
                command.append(1, stringChar);
                break;
            case 1:
                arg1.append(1, stringChar);
                break;
            case 2:
                arg2.append(1, stringChar);
                break;
        }

    }

    // Logging
    logWrite(command, false);
    if (arg1.length() != 0) { logWrite(" -> ", false); logWrite(arg1, false); }
    if (arg2.length() != 0) { logWrite(", ", false); logWrite(arg2, true); }
    logWrite(": ", false);

    // Commands logic
    // for some reason i need the put the not to find if it equals a string
    if (!command.compare("quit")) {
        *quit = true;
        return;
    }

    if (!command.compare("help")) {
        std::cout << "\n\nCommands:" << std::endl;
        std::cout << "quit -------- exit program" << std::endl;
        std::cout << "help -------- list commands" << std::endl;
        std::cout << "printvar ---- prints a variable" << std::endl;
        std::cout << "setsun ------ set the sun count in the current game" << std::endl;
        std::cout << "setsunvalue - set the value of picked up suns" << std::endl;
        std::cout << "autosun ----- toggles suns auto collecting themselves ( only from plants )" << std::endl;
        std::cout << std::endl;
        return;
    }

    if (!command.compare("printvar")) {
        std::cout << "no implementation yet!" << std::endl;
        return;
    }

    if (!command.compare("setsun")) {

        // Check if there was no arg
        if (arg1.length() == 0 || arg2.length() != 0) {
            std::cout << "setsun requires one integer argument" << std::endl;
            return;
        }
        
        // Get arg1 value as int
        int newSunValue;
        try {
            newSunValue = std::stoi( arg1 );
        } catch (...) {
            std::cout << "illegal argument: " << arg1 << '\n';
            return;
        }

        // Pointers
        int4bytes pointer1 = process->getPointer4( (int8bytes) game->mainBase, 0x868);  // Midstep pointer
        int4bytes pointer2 = pointer1 + (int4bytes) 0x5578;                             // Pointer to sun value

        // There is no current game with a sun count to set
        if (pointer1 == 0) { std::cout << "couldnt set sun count! probably not in a game" << std::endl; return; }

        // Log
        logWrite("writing value of ", false);
        logWrite(newSunValue, false);
        logWrite(" at address ", false);
        logWrite(pointer2, true);

        // Write value
        process->write4bytesValue(pointer2, (int4bytes) newSunValue);

        return;

    }

    if (!command.compare("setsunvalue")) {
        
        // Check if there was no arg
        if (arg1.length() == 0 || arg2.length() != 0) {
            std::cout << "setsunvalue requires one integer argument" << std::endl;
            return;
        }

        // Get arg1 value as int
        int newSunValue;
        try {
            newSunValue = std::stoi( arg1 );
        } catch (...) {
            std::cout << "illegal argument: " << arg1 << '\n';
        }

        // Log
        logWrite("writing value of ", false);
        logWrite(newSunValue, false);
        logWrite(" at address ", false);
        logWrite(game->sunValueAddress, true);

        // Write value
        process->write4bytesValue(game->sunValueAddress, (int4bytes) newSunValue);

        return;

    }

    if (!command.compare("autosun")) {

        // Log
        logWrite("Toggling sunautocollect mod to ", false);
        if (!game->autoCollentSunEnabled) logWrite("true", true);
        else logWrite("false", true);

        // Toggle mod
        game->autoCollectSunToggle();

        return;

    }

    std::cout << command << " is not a valid command!" << std::endl;

    return;
    
}

int main( /* int argc, char** argv */ ) {

    // Prepare the log file
    logInit("log.txt");
    logClear();
    logWrite("Starting...", true);

    // Find the process id for plants vs zombies
    // process name -> "popcapgame1.exe"
    const char processName[16] = "popcapgame1.exe"; 

    int8bytes processId = findProcessId(processName);

    if (processId == -1) {
        logWrite("no process found");
        std::cout << "No Process Found!\n";
        system("pause");
        return -1;
    }

    logWrite("process found at id: ", false);
    logWrite(processId, true);

    // Main code
    Process* process = new Process(processId);
    Game* game = new Game(process);

    bool quit = false;
    std::string command;

    std::cout << "Successfully attached to process!" << std::endl;

    while ( !quit ) {

        // Take input
        std::getline(std::cin, command);

        // Run command
        doInput(command, &quit, process, game);

    }

    delete process;
    delete game;

    return 0;
}