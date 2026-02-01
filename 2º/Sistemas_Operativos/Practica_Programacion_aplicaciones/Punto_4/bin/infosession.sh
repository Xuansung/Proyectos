#!/bin/bash

# Autor: Xuan Sun
# Fecha: 10/11/2024
# Resumen: Script que muestra información de la sesión y procesos.

### Variables
procesos=$(ps -eo sid,pgid,pid,user,tty,%mem,cmd --no-headers)
directorio_procesos=""
filtrado="$procesos"
usuario=""
usuario2=""
usuario3=""
usuario4=""
usuario5=""
usuario6=""
directorio=""
sid0="false"
solo_tty="false"
opcion_e="false"
opcion_sm="false"
opcion_sg="false"
opcion_r="false"

### Constantes
TITLE="Información de la sesión de $HOSTNAME"
PROGNOMBRE=$(basename $0)
ENCABEZADO_PROCESOS="    SID    PGID     PID USER    TTY      %MEM  CMD"
ENCABEZADO_SESIONES="SID    PGID     %MEM     PID      USER      TTY  CMD"
MAX_USUARIO=6
TEMP_FILE=$(mktemp)

### Estilos
TEXT_BOLD=$(tput bold)
TEXT_UNLINE=$(tput sgr 0 1)
TEXT_GREEN=$(tput setaf 2)
TEXT_RESET=$(tput sgr0)

### Funciones

#Muestra la ayuda
Help() {

  echo "Script que muestra la información de la sesión."
  echo "La forma de ejecutarlo es el siguiente:"
  echo "infosession.sh [-h] [-z] [-u user] [-t] [-d directorio/]"
  echo "[-h] Muestra la ayuda del comando."
  echo "[-e] Muestra los procesos activos."
  echo "[-z] Muestra también los procesos cuyo identificador de sesión sea 0."
  echo "[-u user1...] Deberá ir acompañada de un nombre de usuario o varios (máximo 6). Se mostrarán los procesos cuyo usuario efectivo sea el especificado."
  echo "[-t] Muestra los procesos cuyo TTY es conocido."
  echo "[-d directorio] Deberá de ir acompañado con un directorio. Se mostrarán los procesos abiertos en dicho directorio."
  echo "[-sm] Ordena por el total de memoria. (Incompatible con -e)"
  echo "[-sg] Ordena por el número de grupos de proceso. (Incompatible con -sh)"
  echo "[-r] Invierte el orden."
}
#Comprueba si los comandos usados son disponibles
Comandos_existe() {

  if ! command -v awk 2>&1 >/dev/null; then
    echo "El comando 'awk' no está disponible."
    exit 1
  fi 

  if ! command -v lsof 2>&1 >/dev/null; then
  echo "El comando 'lsof' no está disponible."
  exit 1
  fi
  
    if ! command -v printf 2>&1 >/dev/null; then
  echo "El comando 'printf' no está disponible."
  exit 1
  fi
}
#Salida de error.
Error_exit() {

  echo "${PROGNOMBRE}: ${1:-"ERROR EN LOS PARÁMETROS DADOS"}" 1>&2
  exit 1
}
#Procesa los parámetros.
Procesar_parametros(){

  #Lee todos los parámetros dados.
  while [ "$#" -gt 0 ]; do
    case "$1" in
    -e)
      opcion_e="true"
      shift
      ;;
    #Mostrar la ayuda
    -h)
      Help
      exit 0
      ;;
    #Mostrar los procesos con SID = 0.
    -z)
      sid0="true"
      shift
      ;;
    #Muestra los TTY con valor conocido.
    -t)
      solo_tty="true"
      shift
      ;;
    #Ordena la tabla en cuanto a %MEM de menor a mayor.
    -sm)
      opcion_sm="true"
      shift
      ;;
    #Ordena la tabla en cuanto a PGID.
    -sg)
      opcion_sg="true"
      shift
      ;;
    #Ordena de forma inversa.
    -r)
      opcion_r="true"
      shift
      ;;
      #Mostrar los procesos de los usuarios dados.
      #Se usa 6 ya que suele ser los que hay en total.
    -u)
      shift  # Saltar la opción -u.
      usuario_cont=0 #Contar el número de usuarios añadidos.
      while [ "$#" -gt 0 ] && [ "$usuario_cont" -lt "$MAX_USUARIO" ] && [[ "$1" != -* ]]; do
        case "$usuario_cont" in
          0) usuario="$1" ;;
          1) usuario2="$1" ;;
          2) usuario3="$1" ;;
          3) usuario4="$1" ;;
          4) usuario5="$1" ;;
          5) usuario6="$1" ;;
        esac
        usuario_cont=$((usuario_cont + 1))
        shift
      done
      if [ "$usuario_cont" -eq "$MAX_USUARIO" ] && [ -n "$1" ] && [[ "$1" != -* ]]; then
        echo "Se ha introducido el número máximo de usuarios permitidos."
        Error_exit
      fi
    ;;
    #Muestra los procesos abiertos en el directorio dado.
    -d)
      shift
      if [ -n "$1" ] && [ "$1" != -* ]; then
        directorio="$1"
        directorio_procesos=$(lsof +d "$directorio" -t | sort -u)
        shift
      else
        echo "No se ha introducido ningún directorio."
        Error_exit
      fi
      ;;
      #Correción de parámetros erróneos.
    -*)
      echo "Error en los parámetros, argumentos inválidos. Prueba con [-h] para ver la ayuda."
      Error_exit
      ;;
    *)
      echo "Error en los parámetros. Prueba con [-h] para ver la ayuda."
      Error_exit
      ;;
    esac
  done

  #Incompatibilidades

  if [ "$opcion_e" == "true" ] && [ "$opcion_sg" == "true" ]; then
    echo "Opciones -e y -sg incompatibles."
    Error_exit
  fi

  if [ "$opcion_sm" == "true" ] && [ "$opcion_sg" == "true" ]; then
    echo "Opciones -sm y -sg incompatibles."
    Error_exit
  fi  
}
#Función para filtrar los PID de los procesos abiertos por el directorio dado.
Filtrar_directorio(){
    #Lee linea por línea, y si el PID es el mismo los imprime.
    echo "$filtrado" | while read -r linea; do
    pid=$(echo "$linea" | awk '{print $3}')
    if echo "$directorio_procesos" | grep  -qw "^pid$"; then
      echo "$linea"
    fi
  done
}
#Función para filtrar los procesos/sesiones
Filtrador(){

  #Filtro de la tabla con SID != de 0.
  if [ "$sid0" == "false" ]; then
    filtrado=$(echo "$filtrado" | awk '$1 > 0 {print $0}' )
  fi
  #Filtro de la tabla con TTY conocido.
  if [ "$solo_tty" == "true" ]; then
    filtrado=$(echo "$filtrado" | awk '$5 != "?" {print $0}')
  fi
  if [ "$solo_no_tty" == "true" ]; then
    filtrado=$(echo "$filtrado" | awk '$5 == "?" {print $0}')
  fi
  #Filtro para la tabla de los usuarios.
 if [ -n "$usuario" ] || [ -n "$usuario2" ] || [ -n "$usuario3" ] || [ -n "$usuario4" ] || [ -n "$usuario5" ] ||[ -n "$usuario6" ]; then
    filtrado=$(echo "$filtrado" | awk -v usuario1="$usuario" -v usuario2="$usuario2" -v usuario3="$usuario3" -v usuario4="$usuario4" -v usuario5="$usuario5" -v usuario6="$usuario6" '$4 == usuario1 || $4 == usuario2 || $4 == usuario3 || $4 == usuario4 || $4 == usuario5 || $4 == usuario6 {print $0}')
  fi
  #Llamada a la función de filtrar directorio.
  if [ -n "$directorio_procesos" ]; then
  Filtrar_directorio
  fi
}
#Crea la tabla
Creador_tabla() {
  #Llama a la función filtrador para filtrar los procesos dado los argumentos.
  Filtrador
  #Si el argumento -e está activado imprime los procesos.
  if [ "$opcion_e" == "true" ]; then
    echo "$TEXT_BOLD$ENCABEZADO_PROCESOS$TEXT_RESET"
    #Ordena por opciones elegidos
    if [ "$opcion_sm" == "true" ]; then
      if [ "$opcion_r" == "true" ]; then
        echo "$filtrado" | sort -rnk6
      else
        echo "$filtrado" | sort -nk6
      fi
    else
      if [ "$opcion_r" == "true" ]; then
        echo "$filtrado" | sort -rk4
      else
        echo "$filtrado" | sort -k4
      fi
    fi
  # el argumento -e no está activado, por lo tanto imprime las sesiones.
  else
  echo "$TEXT_BOLD$ENCABEZADO_SESIONES$TEXT_RESET"
  #Declaración de la variable sids, que tendrá todos los SID.
  sids=$(echo "$filtrado" | awk '{print $1}' | sort -u )
  #Bucle for utilizando una variable sid que pasa por todos los SIDs.
  for sid in $sids; do
  #Declaración de la variable pgids que tendrá todos los pgids.
  pgids=$(echo "$filtrado" | awk -v sid="$sid" '$1 == sid {print $2}' | sort -u | wc -l )
  #Declaración de la variable mem que tendrá el porcentaje de memoria.
  mem_total=$(echo "$filtrado" | awk -v sid="$sid" '$1 == sid {mem+=$6} END {print mem}')
  #Declaración de la variable sesion_lider que serán los SID == PID
  sesion_lider=$(echo "$filtrado" | awk -v pid="$sid" '$3 == pid')
  #Si sesion_lider no es vacío se crea las demás columnas.
  if [ -n "$sesion_lider" ]; then
  user=$(echo "$sesion_lider" | awk '{print $4}')
  tty=$(echo "$sesion_lider" | awk '{print $5}')
  cmd=$(echo "$sesion_lider" | awk '{print $7}')
  #Si sesion_lider está vacío, las variables toman el signo '?'.
  else
  user="?"
  tty="?"
  cmd="?"
  fi
  #Saltarse las líneas donde "user" es desconocido
  if [ "$user" == "?" ]; then
    continue
  fi
  #Imprime las líneas en un archivo temporal $TEMP_FILE.
  printf "%8s %8s %8s %8s %8s %8s %s\n" "$sid" "$pgids" "$mem_total" "$sid" "$user" "$tty" "$cmd" >> "$TEMP_FILE"
  done
  #Imprime la tabla y la ordena según las opciones.
  if [ "$opcion_sm" == "true" ]; then
    if [ "$opcion_r" == "true" ]; then
      cat "$TEMP_FILE" | sort -rnk3
    else
      cat "$TEMP_FILE" | sort -nk3
    fi
  else
    if [ "$opcion_sg" == "true" ]; then
      if [ "$opcion_r" == "true" ]; then
        cat "$TEMP_FILE" | sort -rnk2
      else
        cat "$TEMP_FILE" | sort -nk2
      fi
    else
      if [ "$opcion_r" == "true" ]; then
        cat "$TEMP_FILE" | sort -rbk5
      else
        cat "$TEMP_FILE" | sort -bk5
      fi
    fi
  fi
  fi
}
#Comprueba si los comandos utilizados existen
Comandos_existe
#Llama a la función para procesar parámetros
Procesar_parametros "$@"
#Muestra por pantalla el enunciado
cat << _EOF_
$TEXT_BOLD$TITLE$TEXT_RESET
$(Creador_tabla)
_EOF_

#Elimina el archivo temporal
rm -f "$TEMP_FILE"