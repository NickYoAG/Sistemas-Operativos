#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

class Tenedor{//Clase Tenedor
  public:
    Tenedor(){;}
    mutex mu;
};

class Tiempo{ //Clase Tiempo, que controla cuanto tiempo duro la cena en total
  public:
    int time;
    auto tiempo_cena(float cena_tiempo){
      cout <<"\nLa cena duro: "<< cena_tiempo <<" S"<<endl;
    }
};

int main(){
  int num_fil; //cantidad de filosofos
  cout<<"Ingrese el numero de filosofos: ";
  cin>>num_fil;
  cout<<endl;

  Tiempo clock; //Creamos una instancia en nuestro timer
  clock.time=100; //Le damos este tiempo a nuestra instancia para encapsularla en la variable ms
  int ms = clock.time;
  auto start = chrono::high_resolution_clock::now(); //Se inicia el contador de tiempo
  auto eat = [ms](Tenedor &izq_Fork, Tenedor &der_Fork, int id_filosofo, int estomago=100, int plato_fideo=500){
    while(plato_fideo>0){ //funcion lambda que recoge los parametros necesarios
    if(estomago>100) { //condicional del estomago mayor a 100, es decir, esta lleno
      cout<<"X El filosofo "<<id_filosofo<<" ya no puede comer mas y pensará en momento\n";
      estomago=estomago-25; //su estomago libera espacio
    }
    else if(estomago<=0){ //condicional del estomago en 0
      cout<<"+ El filosofo "<<id_filosofo<<" murio\n";
      break;
    }
    else{
        //Bloqueamos los tenedores con un mutex
        unique_lock<mutex> llock(izq_Fork.mu);  
        unique_lock<mutex> rlock(der_Fork.mu); 
        //Una vez que cogio los tenedoresel filosofo empieza a comer
        cout<<"→ El filosofo "<<id_filosofo<<" está comiendo.\n";
        estomago=estomago+25; //El estomago 
        plato_fideo=plato_fideo-100; //su porcion de comida decrece
        //Esperamos el tiempo encapsulado en la variable ms
        chrono::microseconds timeout(ms);
        this_thread::sleep_for(timeout); // el hilo descansa un instante de tiempo
      }
      if(plato_fideo<=0){ // si el plato de comida del filosofo esta vacio, termina de comer
        cout<<"✔ El filosofo " <<id_filosofo<<" termino de comer.\n";
        break;
      }
      cout<<"○ El filosofo " <<id_filosofo<<" termino de comer una porcion.\n"; //si aun no ha terminado de comer, sigue comiendo
    }
  
  };
  //creamos un par de arrays para manejar la secuencia de filosofos
  Tenedor chp[num_fil];//EN BASE AL NRO DE 
  //EL segundo array sera para controlar mediante los threads 
  thread philosopher[num_fil];//UN THREAD PARA CADA FILOSOFO
  
  cout<<"... El filosofo 1 está pensando.\n";
  //Indicamos que el primer filosofo en comer va tomar un tenedor y el de la izquierda
  philosopher[0] =thread(eat, ref(chp[0]),ref(chp[num_fil-1]), 1);
  
  for (int i = 1;i < num_fil;++i){//empieza la rueda, inicializando todos pensando
    cout<<"... El filosofo "<<(i+1)<<" está pensando.\n";
    philosopher[i] = thread(eat,ref(chp[i]), ref(chp[i-1]),(i+1));
  }
  //Hilos de los filosofos, inicio de la cena.
  for (auto &ph: philosopher){
    ph.join();
  }
  auto end = chrono::high_resolution_clock::now(); //termina el contador del teimpo de la cena en total

  chrono::duration <float> duration = end - start; //calculamos el tiempo
  clock.tiempo_cena(duration.count()); //llamamos a la función tiempo_cena de nuestra clase Tiempo
  return 0;
}
