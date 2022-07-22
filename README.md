# 3d_hokuyo


### Dependências:
- OpenCV4
- urg_library (1.2.6 utilizada)

### Compilar e rodar:
```
make all
./hokuyo NAME_OF_FILE.dat #usa os dados salvos
sudo ./hokuyo #le diretamente do hokuyo - necessita 'sudo' ou mudar acesso da porta utilizada
```

### Salvar novo arquivo .dat:
Descomentar as linhas 51 e 62, substituindo o 'NAME_OF_FILE' na **src/main.cpp** pelo nome do arquivo desejado

![alt text](https://github.com/Gabriel-SGama/3d_hokuyo/blob/main/imgs/write_file.png?raw=true)


### Telas:
- current points - pontos detectados pelo Hokuyo
- filtered points - pontos detectados como parte de um mesmo objeto
- line screen - conjunto de pontos transformados em linhas
- match screen - matches entre as linhas
- trajectory - trajetória obtida pelo matching
- 3D visualization - visualização dos objetos (simplicados para retângulos) com a intensidade inversamente proporcional a profundidade

### Cenários:
#### Wall:
![alt text](https://github.com/Gabriel-SGama/3d_hokuyo/blob/main/imgs/wall.jpeg?raw=true, "Wall")

#### Boxs:
![alt text](https://github.com/Gabriel-SGama/3d_hokuyo/blob/main/imgs/boxs.jpeg?raw=true, "Wall")
