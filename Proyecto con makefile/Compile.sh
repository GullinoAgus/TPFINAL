#!/bin/bash
echo 'Bienvenidos a la instalacion de Super Matias Bros!'
echo 'Seleccione en que plataforma desea jugar:'
echo '(1) PC o Notebook'
echo '(2) RaspberryPi con matriz de 16x16'
#leemos la seleccion del usuario
read seleccion

until [ "$seleccion" == "1" ] || [ "$seleccion" == "2" ]
do
    echo $seleccion
    echo 'Seleccione en que plataforma desea jugar:'
    echo '(1) PC o Notebook'
    echo '(2) RaspberryPi con matriz de 16x16'
    read seleccion
done

#Mostrar el valor de la variable de usuario
if [ $seleccion == "1" ]
then
    make -s -f ./MakePC.mak
    make -s -f ./MakePC.mak clean
else
    make -s -f ./MakeRP.mak
    make -s -f ./MakeRP.mak clean
fi
echo
#Avisar al usuario que se ha terminado de ejecutar el script 
echo ---------Fin del script.-------------
