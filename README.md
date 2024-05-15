# Plants Vs. Zombies Game Mod API

This project is intended to act as a foundation for further modding, as at the moment the functionality is quite limited. <br>
Unfortunately, I can't share the game exe here because it runs through Steam (and also thats probably illegal), so unless you have the game you can't test this app. <br>
When running the exe, there will be a message to show if the app successfully attached to the game. from there, you can type 'help' for a command list. <br>
All the code in this project is written 100% by me. I chose to make this project because I have used tools like Cheat Engine in the past and I wanted to apply similar concepts with cpp. Notable functions include: <br>
- findProcessId() in main.cpp <br>
- aobScan() in process.cpp <br>

Note: This code likely only works on Windows machines because it makes use of the windows.h library, though I have not tried elsewhere. <br>

### To compile
If opened on VS Code, pressing ctrl+shift+b will compile for you. <br>
Otherwise, navigate to the project root directory, and run 'g++ main.cpp -o main.exe' (this requires the gnu compiler to already be installed on your computer) <br>

### To run
The main.exe file, when run, will try to find a running application under the name of the game, so if it fails to do so, you will get a message saying 'No Process Found!" <br>
When the game is open, the console should attach to the game, and it will print "Successfully attached to process!" <br>
The console window now acts as a command line for the game, so anything typed in will affect game variables or behavior. <br>
type 'help' for a list of commands. Some commands take arguments that should be separated by spaces. (ex. 'setsun 1000') <br>
