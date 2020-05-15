
#include<ctime>
#include<cerrno>
#include <cstdlib>
#include <pthread.h>
#include <sys/time.h>
#define NUM_THREADS 2
//Cree un indice hash con la segunda columna/atributo de la tabla

//Esta hace relacion al modelo de las maquinas en la base de datos

//Usare dos consultas que buscaran los registros con esas marcas


void *T1(void *) {
   // cout << "Hello World! Thread ID, " << tid << endl;
   Hash_Index h("Maquina_10k.csv","Maquinas_10k_2.hash");
   h.search("Bronco");
   pthread_exit(NULL);
}

void *T2(void *) {
  Hash_Index h("Maquina_10k.csv","Maquinas_10k_2.hash");
   h.search("CL-Class");
   pthread_exit(NULL);
}
void ejecutar()
{
unsigned t_t0,t_t1;

  pthread_attr_t attr;
  Hash_Index h("Maquina_10k.csv","Maquinas_10k_2.hash");
  void *status;
   pthread_attr_init(&attr);
  pthread_t threads[NUM_THREADS];
  int rc;
  t_t0=clock();
  rc = pthread_create(&threads[0],&attr,T1,NULL);
  rc = pthread_create(&threads[1],&attr,T2,NULL);

  pthread_attr_destroy(&attr);
pthread_join(threads[0],&status);
pthread_join(threads[1],&status);
t_t1=clock();
double time = (double(t_t1-t_t0)/CLOCKS_PER_SEC);

printf("Execution time with threads: %.16g milisegundos\n", time * 1000.0);

//En consola se pareciara el tiempo de ejecucion de aca funcion de la transaccion
//Por separado y al final el tiempo en total en threads
//Como se podra a preciar, la suma de estas duraciones por separado es mayor
//al tiempo de esta funcion ejecutar

pthread_exit(NULL);





}
