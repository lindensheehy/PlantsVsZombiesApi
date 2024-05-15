# Plants Vs. Zombies Game Mod API

This project is intended to act as a foundation for further modding, as at the moment the functionality is quite limited. <br>
Unfortunately, I cant share the game exe in here because it runs through steam (and also thats probably illegal), so unless you have the game you cant test this app. <br>
When running the exe, there will be a message to show if the app successfully attached to the game. from there you can type 'help' for a command list. <br>
All the code in this project is written 100% by me. I chose to make this project becuase i have used tools like Cheat Engine in the past and i wanted to apply similar concepts with cpp. Notable functions include: <br>
- findProcessId() in main.cpp <br>
- aobScan() in process.cpp <br> <br>
Note: This code likely only works on windows machines because it makes use of the windows.h library, though I have not tried elsewhere. <br>

### To compile
If opened on VS Code, pressing ctrl+shift+b will compile for you. <br>
Otherwise, navigate to the project root directory, and run 'g++ main.cpp -o main.exe' (this requires the gnu compiler to already be installed on your computer) <br>

### To run
The main.exe file, when run, will try to find a running application, so if it fails to do so, you will get a message saying 'No Process Found!" <br>
When the game is open, the console should attach to the game, and it will print "Successfully attached to process!" <br>
The console window acts now as a command line for the game, so anything typed in will affect game variables or behaviour. <br>
type 'help' for a list of commands. Some commands take arguments which should be given seperated by spaces. (ex. 'setsun 1000') <br>