# **CG-TP2**

##### **Trabalho de Computação Gráfica do Segundo Período de Engenharia de Computação do CEFET-MG**
---
>## Integrantes do grupo:
>####	Fernando Lorenzeto Rodrigues
>####	Rafael Assunção Ladeira Torga
>####	Rafael Augusto de Souza
---
# Instruções para compilação e execução:

>## Importante ressaltar que é necessário verificar se seus programas seguem atualizados antes de prosseguir:
>	sudo apt-get update
>	sudo apt-get upgrade

## Bibliotecas utilizadas no desenvolvimento do Sistema Solar:

**As bibliotecas podem ser instaladas via comando _make install-libs__**

### SDL e SDL_mixer: (para músicas apenas)
>	sudo apt-get install libsdl1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev 
### FreeGlut
>	sudo apt-get install freeglut3-dev
### Glew
>	sudo apt-get install libglew-dev
### SOIL: 
>	sudo apt-get install libsoil-dev

## Execução
>### Existem dois modos:
> Via Terminal manualmente onde você irá utilzar o comando **cd/diretório** e vai colocar o diretório/pasta onde foi baixado o jogo. **Ex: cd 'Área de Trabalho'/TP2**
> Após isso você deve digitar o comando g++ tp2.cpp -lglut -lGLU -lGL -lGLEW -lSOIL -lSDL -lSDLmain -lSDL_mixer para compilar o programa
e em seguida ./a.out para executar.

>### O outro modo é via terminal porém utilzando os comandos make:
### Instala todas as bibliotecas necessárias.
>	make install-libs 
### Compila o Game
>	make all
### Executa o Game
>	make run
### Limpa o arquivo compilado
>	make clean
---
# Lista de itens adicionais implementados:

	Relativas a texturas e gráficos:

        	1-Modo de órbitas visíveis (6%): com o pressionar de uma tecla (o), torne possível mostrar/esconder a trajetória de 
		todos os elementos orbitantes.

        	2-[Satélites naturais] (10%): Criar objetos orbitando os que estão orbitando o principal (eg, satélites naturais) 
		Esses objetos devem possuir uma rotação em torno do seu próprio eixo também


        	3-Anéis planetários (6%): coloque os anéis ao redor dos planetas gigantes gasosos (Saturno)
		
		4- Reshape (6%): permitir o redimensionamento da janela de visualização sem perda da razão de aspecto (sem que haja distorção de achatamento/alongamento)



	Outras funcionalidades:

         	1-Música (8%): você pode incluir uma música de fundo para seu sistema estelar (ou de outra coisa)

			
>#####	*As imagens e sons utilzados no game podem ser encontradas nesses sites: (Tudo com free download).
>#####	Música: https://freesound.org/people/DaveJf/sounds/566083/
>#####	Imagens: https://www.solarsystemscope.com/textures/

# **VIDEO**
>Link do vídeo https://youtu.be/LqFkw0xMnuM
[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/LqFkw0xMnuM/0.jpg)](https://youtu.be/LqFkw0xMnuM)

