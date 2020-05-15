#include <iostream>
#include "random_file.h"
#include "hash.h"
#include <sstream>
#include <stdio.h>
#include <omp.h>
#include <string>
#include <pthread.h>
#include <assert.h>
#include "transacciones.h"

// map<string,Random_File> random_index;


void agregar()
{
  /*
    Para agregar lo tome de la siguiente manera:
      -Si no tengo indices creados, los agrego al csv
      -Si tengo alguno de los indices creados, lo guardo con su funcion add_hash
      -Si tengo los dos indices creados, lo guardo en ambos indices
      (Primero en el hash con su funcion, y en el random solo lo pongo al final porque igual cuando sea cargado se ordenara)
  */
  cout<<"Ingrese nombre de los datos"<<endl;
  string d,m;
  cin>>d;
  ifstream datos(d);
  if(datos.fail()){
    cout<<"No such file"<<endl;
    datos.close();
  }
  else{
    datos.close();
  stringstream aux(d);
  string ind_name;
  getline(aux,ind_name,'.');
  ifstream hash(ind_name+".hash");
  ifstream ran(ind_name+".random");
  string registro;
  cout<<"Escriba el registro"<<endl;
  // getline(cin,registro);
  cin>>registro;

  if(!hash.fail())
  {
    hash.close();
    ran.close();
    if(!ran.fail())
    {
      ofstream p(d,ios::app);
      p.seekp(0,ios::end);
      int pos=p.tellp();
      p.close();
      ofstream sali(ind_name+".random");
      string key;
      stringstream aux(registro);
      getline(aux,key,',');
      sali<<pos<<" "<<key<<"\n";


    }
    Hash_Index h(d,ind_name+".hash");
    // getline(cin,registro);
    h.add_hash(registro);
  }
  else
  {
    hash.close();
    ran.close();
    if(!ran.fail())
    {
      Random_File r(d,ind_name+".random");
      // getline(cin,registro);
      r.add_random(registro);

    }
    else
    {
      // getline(cin,registro);
      ofstream salida(d,ios::app);
      salida<<registro<<"\n"<<endl;
    }
  }


  }
}

void buscar()
{

  //Pido que digan con cual de los dos tipos de indices quieren hacer la busqueda

  //Si el indice no existe entonces lo crea

  cout<<"Ingrese nombre de los datos"<<endl;
  string d;
  cin>>d;
  ifstream datos(d);
  if(datos.fail())
    cout<<"No such file"<<endl;
  else
  {
  stringstream aux(d);
  string ind_name;
  getline(aux,ind_name,'.');
  cout<<"Que tipo de indice"<<endl;
  cout<<"1) Random File"<<endl;
  cout<<"2) Hash "<<endl;
  int r;
  cin>>r;
  if(r==1)
  {
    ind_name+=".random";
    Random_File r(d,ind_name);
    string key;
    cout<<"Ingrese llave a buscar"<<endl;
    cin>>key;
    r.buscar(key);

  }
  else
  {
    ind_name+=".hash";
    Hash_Index h(d,ind_name);
    cout<<"Ingrese llave a buscar"<<endl;
    string key,v;
    cin>>key;
    h.search(key);



  }
 }

}



int main()
{

  // Hash_Index h("Maquina_10k.csv","Maquinas_10k_2.hash");


  //Menu en consola
  int opcion;
  bool seguir=true;
  while(seguir){
    cout<<"Creacion de indice Random_File y Hash"<<endl;
    cout<<"---------------------------------------"<<endl;
  cout<<"1) Agregar registro"<<endl;
  cout<<"2) Buscar registro"<<endl;
    cin >>opcion;
    switch (opcion) {
      case 1:
        agregar();
        break;
      case 2:
        buscar();
        break;
      default:
        seguir=false;
        break;

    }
  cout<<endl;
  }
  // Hash_Index r("Maquina_10k.csv","Maquinas_10k.hash");
  // r.search("68804116");
  // ifstream s("Maquina_10k.csv");
  // unsigned t0,t1;
  // int accesos=0;
  // t0=clock();
  // s.seekg(0);
  // string a;
  // while(!s.eof())
  // {
  //   getline(s,a);
  //   stringstream registro(a);
  //   string dato;
  //   accesos++;
  //   // cout<<a;
  //   getline(registro,dato,',');
  //   if(dato=="68804116")
  //   {
  //     t1=clock();
  //     break;
  //   }
  // }
  // double time = (double(t1-t0)/CLOCKS_PER_SEC);
  // // cout << "Execution Time: " << time << endl;
  // printf("Execution time: %.16g milisegundos\n", time * 1000.0);
  // cout<<accesos<<endl;




}
