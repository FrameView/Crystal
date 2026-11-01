# Crystal

[Features] 
-> Run any binary or linux command!
-> Manage apps, groups and colors through userconfig.json! 
-> Switch between dark and light mode as needed!

[Build]
You can build Crystal using the following command:
gcc -Wall -Wextra crystal.c json.c ui.c -o crystal -lX11

[Run] 
-> Create the following directory : $HOME/.config/crystal/ 
-> Place your userconfig.json file inside before running Crystal