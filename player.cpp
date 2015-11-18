#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;
const double masinf=9999999999.0, menosinf=-9999999999.0;

bool principio=false;

// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

double alineacion2(int jugador, const Environment &estado){
    double contador=0;
    for (int filas=0; filas<7; filas++){
      for (int columnas=0; columnas<7; columnas++){
    if(columnas<4){
        if(estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas,columnas+1)%3 && estado.See_Casilla(filas,columnas+1)%3==jugador && estado.See_Casilla(filas,columnas+2)%3==0 && estado.See_Casilla(filas,columnas+3)%3==0){
            contador++;
        }
    }
	if(columnas>=4){
         if(estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas,columnas-1)%3 && estado.See_Casilla(filas,columnas-1)%3==jugador && estado.See_Casilla(filas,columnas-2)%3==0 && estado.See_Casilla(filas,columnas-3)%3==0)
            contador++;
    }
    if(filas<4){
         if(estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas+1,columnas)%3 && estado.See_Casilla(filas+1,columnas)%3==jugador && estado.See_Casilla(filas+2,columnas)%3==0 && estado.See_Casilla(filas+3,columnas)%3==0){
            contador++;
        }
    }
    if (filas<4 and columnas<4){
      if (estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas+1,columnas+1)%3 && estado.See_Casilla(filas+1,columnas+1)%3 == jugador && estado.See_Casilla(filas+2,columnas+2)%3==0 && estado.See_Casilla(filas+3,columnas+3)%3==0)
            contador++;
      }
      if (filas>2 and columnas<4){
            if(estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas-1,columnas+1)%3 && estado.See_Casilla(filas-1,columnas+1)%3==jugador && estado.See_Casilla(filas-2,columnas+2)%3==0 && estado.See_Casilla(filas-3,columnas+3)%3==0)
                contador++;
      }
      }
    }
        return contador;
    }
    double alineacion3(int jugador, const Environment &estado){
        double contador=0;
        for (int filas=0; filas<7; filas++){
      for (int columnas=0; columnas<7; columnas++){
    if(columnas<4){
        if(estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas,columnas+1)%3 && estado.See_Casilla(filas,columnas+1)%3==estado.See_Casilla(filas,columnas+2)%3 && estado.See_Casilla(filas,columnas+2)%3==jugador && estado.See_Casilla(filas,columnas+3)%3==0){
            contador++;
        }
    }
    if(columnas>=4){
        if(estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas,columnas-1)%3 && estado.See_Casilla(filas,columnas-1)%3==estado.See_Casilla(filas,columnas-2)%3 && estado.See_Casilla(filas,columnas-2)%3==jugador && estado.See_Casilla(filas,columnas-3)%3==0)
            contador++;
    }
    if(filas<4){
         if(estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas+1,columnas)%3 && estado.See_Casilla(filas+1,columnas)%3==estado.See_Casilla(filas+2,columnas)%3 && estado.See_Casilla(filas+2,columnas)%3==jugador && estado.See_Casilla(filas+3,columnas)%3==0){
            contador++;
        }
    }
    if (filas<4 and columnas<4){
      if (estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas+1,columnas+1)%3 && estado.See_Casilla(filas+1,columnas+1)%3== estado.See_Casilla(filas+2,columnas+2)%3 && estado.See_Casilla(filas+2,columnas+2)%3==jugador && estado.See_Casilla(filas+3,columnas+3)%3==0)
            contador++;
      }
      if (filas>2 and columnas<4){
            if(estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas-1,columnas+1)%3 && estado.See_Casilla(filas-1,columnas+1)%3==estado.See_Casilla(filas-2,columnas+2)%3 && estado.See_Casilla(filas-2,columnas+2)%3==jugador && estado.See_Casilla(filas-3,columnas+3)%3==0)
                contador++;
      }
        }
    }
        return contador;
    }
    double FaltaUnHueco(int jugador, const Environment &estado){
        double contador=0;
        for (int filas=0; filas<7; filas++){
      for (int columnas=0; columnas<7; columnas++){
        if(columnas<4){
            if((estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas,columnas+2)%3 && estado.See_Casilla(filas,columnas+1)%3==0 &&estado.See_Casilla(filas,columnas+2)%3==estado.See_Casilla(filas,columnas+3)%3 && estado.See_Casilla(filas,columnas+3)%3==jugador) || (estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas,columnas+1)%3 && estado.See_Casilla(filas,columnas+2)%3==0 &&estado.See_Casilla(filas,columnas+1)%3==estado.See_Casilla(filas,columnas+3)%3 && estado.See_Casilla(filas,columnas+3)%3==jugador))
                contador++;
        }
	 if(columnas>=4){
            if((estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas,columnas-2)%3 && estado.See_Casilla(filas,columnas-1)%3==0 &&estado.See_Casilla(filas,columnas-2)%3==estado.See_Casilla(filas,columnas-3)%3 && estado.See_Casilla(filas,columnas-3)%3==jugador) || (estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas,columnas-1)%3 && estado.See_Casilla(filas,columnas-2)%3==0 &&estado.See_Casilla(filas,columnas-1)%3==estado.See_Casilla(filas,columnas-3)%3 && estado.See_Casilla(filas,columnas-3)%3==jugador))
                contador++;
        }
        if (filas<4 and columnas<4){
                if((estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas+1,columnas+1)%3 && estado.See_Casilla(filas+1,columnas+1)%3==estado.See_Casilla(filas+3,columnas+3)%3 && estado.See_Casilla(filas+3,columnas+3)%3==jugador && estado.See_Casilla(filas+2,columnas+2)%3==0) || (estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas+2,columnas+2)%3 && estado.See_Casilla(filas+2,columnas+2)%3==estado.See_Casilla(filas+3,columnas+3)%3 && estado.See_Casilla(filas+3,columnas+3)%3==jugador && estado.See_Casilla(filas+1,columnas+1)%3==0)||(estado.See_Casilla(filas+1,columnas+1)%3==estado.See_Casilla(filas+2,columnas+2)%3 && estado.See_Casilla(filas+2,columnas+2)%3==estado.See_Casilla(filas+3,columnas+3)%3 && estado.See_Casilla(filas+3,columnas+3)%3==jugador && estado.See_Casilla(filas,columnas)%3==0)||(estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas+1,columnas+1)%3 && estado.See_Casilla(filas+1,columnas+1)%3==estado.See_Casilla(filas+2,columnas+2)%3 && estado.See_Casilla(filas+2,columnas+2)%3==jugador && estado.See_Casilla(filas+3,columnas+3)%3==0))
                    contador++;
        }
        if (filas>2 and columnas<4){
                if((estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas-1,columnas+1)%3 && estado.See_Casilla(filas-1,columnas+1)%3==estado.See_Casilla(filas-3,columnas+3)%3 && estado.See_Casilla(filas-3,columnas+3)%3==jugador && estado.See_Casilla(filas-2,columnas+2)%3==0) || (estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas-2,columnas+2)%3 && estado.See_Casilla(filas-2,columnas+2)%3==estado.See_Casilla(filas-3,columnas+3)%3 && estado.See_Casilla(filas-3,columnas+3)%3==jugador && estado.See_Casilla(filas-1,columnas+1)%3==0)||(estado.See_Casilla(filas-1,columnas+1)%3==estado.See_Casilla(filas-2,columnas+2)%3 && estado.See_Casilla(filas-2,columnas+2)%3==estado.See_Casilla(filas-3,columnas+3)%3 && estado.See_Casilla(filas-3,columnas+3)%3==jugador && estado.See_Casilla(filas,columnas)%3==0)||(estado.See_Casilla(filas,columnas)%3==estado.See_Casilla(filas-1,columnas+1)%3 && estado.See_Casilla(filas-1,columnas+1)%3==estado.See_Casilla(filas-2,columnas+2)%3 && estado.See_Casilla(filas-2,columnas+2)%3==jugador && estado.See_Casilla(filas-3,columnas+3)%3==0))
                    contador++;
        }
    }
    }
    return contador;
    }

double Bombas(int jugador,int contricante,const Environment estado){
    double contador_3alineacion_jugador=alineacion3(jugador,estado);
    double contador_3alineacion_contricante=alineacion3(contricante,estado);
    double contador_2lineas_jugador=alineacion2(jugador,estado);
    double contador_2lineas_contricante=alineacion2(contricante,estado);
    double contador_hueco_jugador=FaltaUnHueco(jugador,estado);
    double contador_hueco_contricante=FaltaUnHueco(contricante,estado);
    double valor=6*contador_3alineacion_jugador+3*contador_2lineas_jugador+4*contador_hueco_jugador-3*contador_2lineas_contricante-6*contador_3alineacion_contricante-4*contador_hueco_contricante;
        return valor;
}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){
    double valor;
    int ganador = estado.RevisarTablero();
    int contricante;
    int bomba;
    int bomba_contricante;
    double bomb_contricante=0;
    Environment estado_bom;
    int ocu_columnas;
    double bomb_jugador=0;
    int jug=jugador;
    if(jugador==1){
        contricante=2;
	bomba=4;
	bomba_contricante=5;
    }
    else{
        contricante=1;
	bomba=5;
	bomba_contricante=4;
    }
/*
    if(estado.Have_BOOM(jugador)==true){
        estado_bom=estado;
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if((int)estado.See_Casilla(i,j)==bomba){
                    estado_bom.AplicarExplosion(i,j);
                    bomb_jugador=Bombas(jugador,contricante,estado_bom);
                }
            }
        }
    }
    if(estado.Have_BOOM(contricante)==true){
        estado_bom=estado;
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if((int)estado.See_Casilla(i,j)==bomba_contricante){
                    estado_bom.AplicarExplosion(i,j);
                    bomb_contricante=Bombas(contricante,jugador,estado_bom);
                }
            }
        }
    }*/
    double contador_3alineacion_jugador=alineacion3(jugador,estado);
    double contador_3alineacion_contricante=alineacion3(contricante,estado);
    double contador_2lineas_jugador=alineacion2(jugador,estado);
    double contador_2lineas_contricante=alineacion2(contricante,estado);
    double contador_hueco_jugador=FaltaUnHueco(jugador,estado);
    double contador_hueco_contricante=FaltaUnHueco(contricante,estado);
    if(ganador==jugador)
        return 9999999999999999999999999999999999999999999999.0;
    else if(ganador!=0)
        return -999999999999999999999999999999999999999999999.0;
    else if(estado.Get_Casillas_Libres()==0)
        return 0;
    else if(jugador==1 ||jugador==2){
            valor=(((contador_3alineacion_jugador*6)+(contador_hueco_jugador*4)+(contador_2lineas_jugador)+bomb_jugador*8)-((contador_3alineacion_contricante*6)+(contador_hueco_contricante*4)+(contador_2lineas_contricante*2)+bomba_contricante*8));
        return valor;
    }
    else{
        return Puntuacion(jugador,estado);
    }
    }





// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}

double Poda_alphaBetha(const Environment & tablero,int jug,int prof,int limite,Environment::ActionType & accion,double alpha,double betha){
    Environment::ActionType accion_anterior=accion;
    double aux;
    if(prof==limite or tablero.JuegoTerminado()){
            return Valoracion(tablero,jug);
    }
    else{
        int ult_act=-1;
        Environment hijo=tablero.GenerateNextMove(ult_act);
        while(!(hijo==tablero)){
            aux=Poda_alphaBetha(hijo,jug,prof+1,limite,accion_anterior,alpha,betha);
            if(prof%2==0){
                if(aux>alpha){
                    alpha=aux;
                        if(prof==0)
                    		accion=static_cast <Environment::ActionType>(ult_act);
                }

            }
            else{
                if(aux<betha){
                    betha=aux;
                    //if(prof==0)
                       // accion=static_cast<Environment::ActionType>(ult_act);
                }

            }
            if(betha<=alpha)
                return alpha;
            hijo=tablero.GenerateNextMove(ult_act);
        }
	if (prof%2==0)
		return alpha;
	else
		return betha;

        }
    }

double MiniMax(const Environment & tablero,int jug,int prof,int limite,Environment::ActionType & accion){
    const double INF=1000000000000.0, mINF=-1000000000000000.0, gano=999999999999999, pierdo=-999999999999999999;
    Environment::ActionType accion_anterior;
    double mejor, aux;
    if(prof==limite or tablero.JuegoTerminado()){
            return ValoracionTest(tablero,jug);
    }
    else{
        if(prof%2==0)
            mejor=mINF;
        else
            mejor=INF;
        int ult_act=-1;
        Environment hijo=tablero.GenerateNextMove(ult_act);
        while(!(hijo==tablero)){
            aux=MiniMax(hijo,jug,prof+1,limite,accion_anterior);
            if(prof%2==0){
                    if(aux>mejor){
                    mejor=aux;
                    accion=static_cast <Environment::ActionType>(ult_act);
                    }
                }
            else{
                if(aux<mejor){
                    mejor=aux;
                    accion=static_cast<Environment::ActionType>(ult_act);
                }
            }
            hijo=tablero.GenerateNextMove(ult_act);
        }
        return mejor;

        }
    }



// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acci\F3n que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;

/*
    //--------------------- COMENTAR Desde aqui
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }
*/
    //--------------------- COMENTAR Hasta aqui


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    alpha=-999999999999999999999999999999999999999999999999.0;
    beta=99999999999999999999999999999999999999999999999999.0;
    if((int)actual_.See_Casilla(0,1)==0 && (int)actual_.See_Casilla(0,0)==0 && (int)actual_.See_Casilla(0,2)==0 && (int)actual_.See_Casilla(0,3)==0 && (int)actual_.See_Casilla(0,4)==0 && (int)actual_.See_Casilla(0,5)==0 && (int)actual_.See_Casilla(0,6)==0){
        return Environment::PUT4;
}
    else{
    valor = Poda_alphaBetha(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion,alpha,beta);
    cout<<jugador_<<endl;
    cout << "Valor Poda: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;
    //valor=MiniMax(actual_,jugador_,0,PROFUNDIDAD_ALFABETA,accion);
    //cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;
    return accion;
    }
}

