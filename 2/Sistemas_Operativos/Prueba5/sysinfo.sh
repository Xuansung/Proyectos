#!/bin/bash
# sysinfo - Un script que informa del estado del sistema

#Constantes
TITLE="Información del sistema para $HOSTNAME"
RIGHT_NOW=$(date +"%x %r%Z")
TIME_STAMP="Actualizada el $RIGHT_NOW por $USER"

TEXT_BOLD=$(tput bold)
TEXT_GREEN=$(tput setaf 2)
TEXT_RESET=$(tput sgr0)
#Funciones
system_info(){
	uname -a
	system_info
}

show_uptime(){
	echo "${TEXT_UNLINE}Tiempo de encendido del sistema$TEXT_RESET"
	echo
	uptime
}

drive_space(){
	echo "drive_space"
	echo
	df -a
	echo
	echo " Memoria total usada:"
	echo 
	awk '{sum += $5}END {print sum}' cat df -a
	
}

home_space(){
echo "home_space"
}
cat << _EOF_
$TEXT_BOLD$TITLE$TEXT_RESET
home_space
$TEXT_GREEN$TIME_STAMP$TEXT_RESET
_EOF_
drive_space
system_info
show_uptime
buscador_ejecutador
