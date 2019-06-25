#include <iostream>
#include <stdlib.h>
#include <time.h>



using namespace std;

const int TAM = 8;	

int campo[TAM][TAM] = {0};
int quant[TAM][TAM] = {0};
int jog[TAM][TAM]= {};

struct pontuacao{
	int jogada;
	int marcacao;
};

//funcoes
int geraMina();
int verificaRedor();
int tranfQuantCampo();
int minasProx(int x, int y);
int jogo();
int liberaRecursiv(int x, int y);
pontuacao testaQuantJogadas();


int main() {
	
	srand (time(NULL));

	geraMina();

	verificaRedor();

	tranfQuantCampo();

	/*
	for(int j = 0; j < TAM; j++) {
		for(int k = 0; k < TAM; k++)
			cout<<campo[j][k]<<' ';
		cout<<endl;
	}
	*/
	

	jogo();

	return 0;
}

int geraMina() { //gera as minas no campo
	int geraMinaX, geraMinaY;

	for(int i = 0; i < TAM+1; i++) {
		geraMinaX = rand() % TAM;
		geraMinaY = rand() % TAM;

		if (campo[geraMinaX][geraMinaY] == -1)	
			i--;
		else
			campo[geraMinaX][geraMinaY] = -1;
	
	}

	return 0;
}

int verificaRedor() { //procura os locais das minas para dar inicio ao processo de pontuação

	for(int j = 0; j < TAM; j++) {
		for(int k = 0; k < TAM; k++) {
			if(campo[j][k] == -1) {
				minasProx(j,k);
			}
		}
	}

	return 0;
}



int minasProx(int x, int y) { //adiciona um ponto a cada espaço dos arredores
	for(int a = -1; a <= 1; a++) {
			for(int b = -1; b <= 1; b++) {
				if(x+a >= 0 && x+a < TAM && y+b >= 0 && y+b < TAM) {
					if(quant[x+a][y+b] != -1) {
						quant[x+a][y+b]++;
					}
				}
				if(a==0 && b==0) {
					quant[x][y] = -1;
				}
			}
	}

	return 0;
}

int tranfQuantCampo() {
	for(int i = 0; i < TAM; i++) {
			for(int j = 0; j < TAM; j++) {
				if(quant[i][j] != -1)
					campo[i][j] = quant[i][j];
			}
	}
	
	return 0;

}

int jogo() {

	system("clear");	//limpa a tela
	cout<<" Bem vindo ao Campo Minado "<<endl<<endl;

	int perdeu = 0, ganhou = 0;
	int um, dois, marcada=0;
	int quantJogadas=0;
	char marca;

	for(int j = 0; j < TAM; j++) {
		for(int k = 0; k < TAM; k++) 
			jog[j][k] = 9;   //o 9 é o espaço que não foi selecionado ainda, oculto
	}

	while (perdeu != 1 && ganhou != 1) {

		cin>>um>>dois>>marca;
		cout<<endl;

		//hack para ganhar
		if(um == 42 && dois == 42)
			ganhou = 1;
		
		if (marca == 'x' && jog[um][dois] == 9)
			jog[um][dois] = 8;
		else
			jog[um][dois] = campo[um][dois];

		if(jog[um][dois] == 0) {  //quando as coordenadas selecionadas forem zero, mostrar o campo dos vizinhos também
			liberaRecursiv(um, dois);
		}

		cout<<"   ";
		for(int j = 0; j < TAM; j++) {
			cout<<j<<" ";
		}		
		cout<<endl;
		cout<<endl;

		for(int j = 0; j < TAM; j++) {
			cout<<j<<"  ";
			for(int k = 0; k < TAM; k++){				
				
				if(jog[j][k] == 9)
					cout<<"-"<<' ';
				else if(jog[j][k] == 8)
					cout<<"x"<<' ';
				else if(jog[j][k] == -1)
						cout<<"#"<<' ';
				else
					cout<<jog[j][k]<<' ';				
			}
			cout<<endl;
		}
		cout<<endl;

		if(jog[um][dois] == -1) { //condição de perder
						
			for(int j = 0; j < TAM; j++) {
				for(int k = 0; k < TAM; k++) {	
					if(campo[j][k] == -1) {
						jog[j][k] = campo[j][k];
					}
				}
			}
			for(int j = 0; j < TAM; j++) {
				for(int k = 0; k < TAM; k++)
					if(jog[j][k] == 9)
						cout<<"-"<<' ';
					else if(jog[j][k] == -1)
						cout<<"#"<<' ';
					else
						cout<<jog[j][k]<<' ';
				cout<<endl;
			}
			perdeu = 1;
		}

		quantJogadas = testaQuantJogadas().jogada;
		marcada = testaQuantJogadas().marcacao;

		if(quantJogadas == TAM+1 || (marcada == TAM+1 && !perdeu)) 
			ganhou = 1;
			
	}

	if(perdeu)
		cout<<"Você perdeu!! ";
	if(ganhou)
		cout<<"Você ganhou!! ";
	cout<<endl;
	cout<<endl;

	return 0;

}

int prov[TAM][TAM] = {0};

int liberaRecursiv(int x, int y) {	//ainda tá com problema na recursividade
	prov[x][y] = -1;
	for(int a = -1; a <= 1; a++) {
		for(int b = -1; b <= 1; b++) {
			if(x+a >= 0 && x+a < TAM && y+b >= 0 && y+b < TAM) {
				jog[x+a][y+b] = campo[x+a][y+b];
		
				if(jog[x+a][y+b] == 0 && prov[x+a][y+b] != -1) 
					return liberaRecursiv(x+a, y+b);					
									
			}

		}
	}
	return 0;
}

pontuacao testaQuantJogadas() {
	int qJs = 0, marcs = 0;
	for(int j = 0; j < TAM; j++) {
				for(int k = 0; k < TAM; k++) {
					if(jog[j][k] == 9 || jog[j][k] == 8)
						qJs++;
					if(jog[j][k] == 8 && campo[j][k] ==  -1) 
						marcs++;
				}
	}
	return {qJs, marcs};
}


