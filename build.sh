#!/bin/sh
# SEMPRE LEIA OS SCRIPTS ANTES DE RODÁ-LO EM SEU SISTEMA
# SCRIPT FEITO PARA COMPILAR E EXECUTAR O PROGRAMA FILHOS DE JORGE
mkdir ../build-GIS-git
cd ../build-GIS-git/
qmake ../GIS/GIS.pro
make
echo "O programa terminou de compilar."
read -rsp "Pressione uma tecla para continuar..."
clear
echo "Deseja executar o programa agora? (S ou N)"
read input
case $input in
    s|S|y|Y)
        exec ./GIS
    ;;
esac
