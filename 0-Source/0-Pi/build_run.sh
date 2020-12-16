printf '\e[1;34m%-6s\e[m' "Building..."
printf "\n"
rm UUV.o
g++ Main.c UDPServer/UDPServer.c SensorDriver/SensorDriver.c Logger/Logger.c Utils/Utils.c -lm -lwiringPi -o UUV.o
printf '\e[1;34m%-6s\e[m' "Running..."
printf "\n"
./UUV.o