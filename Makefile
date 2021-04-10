
install-libs:
	@echo "Instalando bibliotecas utilizadas no programa..."
	sudo apt-get install libsdl1.2-dev 
	sudo apt-get install libsdl-mixer1.2-dev 
	sudo apt-get install libsdl-ttf2.0-dev 
	sudo apt-get install freeglut3-dev
	sudo apt-get install libglew-dev
	sudo apt-get install libsoil-dev


all:
	@echo "Compilando o jogo..."
	g++ tp2.cpp -lglut -lGLU -lGL -lGLEW -lSOIL -lSDL -lSDLmain -lSDL_mixer


run:
	@echo "Abrindo o Sistema Solar"
	./a.out

clean:
	@echo "Obrigado por visitar o Sistema Solar"
	@echo "Removendo..."
	rm ./a.out
