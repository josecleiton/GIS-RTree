# FILHOS DE JORGE
Este projeto foi proposto como trabalho final da disciplina Estruturas de Dados II, pelo professor [**Jorge Sampaio Farias**](http://lattes.cnpq.br/6683499592786376), referente ao período 3 do curso de [Sistemas de Informação](http://www.csi.uneb.br/) na [**Universidade do Estado da Bahia (UNEB)**](https://portal.uneb.br/).
## Problema
Armazenamento, recuperação e representação de formas geométricas 2-D guardadas em armazenamento secundário.

## Solução
- Utilização de classes para representação dos dados geométricos
- Utilização de R-Tree e hashing para armazenamento e recuperação desses dados.
- Utilização do framework QT e C++ para a representação gráfica do programa (GUI).

## UML
Diagrama de classes feito no aplicativo Umbrello, basta importá-lo. [**PDF**](https://drive.google.com/file/d/1ZGOCYsQ1NC7MJfIM0cWefakV0_k9Ek-y/view?usp=sharing).

## Instalação
### Requisitos
Para rodar o programa é necessário ter instalado as bibliotecas do QT:
- Pacote qt5-default para Linux baseado em Ubuntu;
- Pacote qt5-base para Linux baseado em Arch;
- Windows/Mac: instale o QTCreator.

Também é necessário o compilador C++ (clang++ ou g++).

Como compilar usando terminal (Linux):\
OBS: É necessário ter o pacote `cmake`.
- Entre na pasta GIS
- `chmod +x build.sh`
- `./build.sh`
- Divirta-se

O programa ficará no diretório `build-GIS-git`.

Compilar usando QTCreator:
- Importe o `GIS.pro`
- Clique em `Run`
- Divirta-se

### Encontrou algum bug? 
Fique à vontade de reportar bugs ou resolvê-los. O código está disponível para ser melhorado.
