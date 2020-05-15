// #include "Registro.h"
#include <map>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <ctime>
using namespace std;
unsigned t0, t1;
class Random_File
{
private:
  std::string filename;
  std::string indexname;
  int size_key;
  string first;


  map<string,int> leer_indice()
  {
    map<string,int> devolver;
    ifstream file(filename);

    if(file.fail())
    {
      cout<<"No such file"<<endl;
      file.close();
      return devolver;
    }
    ifstream ind(indexname);
    ind.seekg(0);
    if(!ind.fail())
    {
      //Entra aca si esque el indice ya existe y lo carga
      int pos;
      string key;
      string r;
      while(!ind.eof())
      {
        //En el indice esta puesto primero la posicion y luego la llave, para no tener problemas ya que la posicion siempre va a ser un solo numero
        string aux_pos;
        string key;
        //Leyendo
        getline(ind,r);
        if(r.size()==0)
          break;
        istringstream aux (r);
        //Separar la posicion y la llave de lo leido
        getline(aux,aux_pos,' ');
        getline(aux,key);
        stringstream geek(aux_pos);
        geek >> pos;
        if(key.size()==0)
          break;
        devolver[key]=pos;
        //Imprimia los valores para ver si todo estaba bien
        // cout<<key<<" ->> "<<devolver[key]<<endl;
        string c;
        if(pos==0)
        {
          //Guardando el primer registro para algo que me servira en la funcion buscar
            first=key;
        }
      }
      ind.close();
    }
    else
    {
      ind.close();
      //Leyendo archivo para crear indice
      ofstream new_index(indexname,ios::binary);
      if(!file.fail()){
        file.seekg(0);
        while(!file.eof())
        {
          string key;
          string r;
          //Sacando la posicion de este
          int pos=file.tellg();
          //Sacando el registro completo
          getline(file,r);
          //Extraer el primer elemento, que asumo siempre sera la key
          if(r.size()!=0)
          {
            //Guardando el elemento(key) en el indice
            istringstream aux (r);
            getline(aux,key,',');
            devolver[key]=pos;
            //Imprimia los valores para ver si todo estaba bien
            // cout<<key<<" -> "<<devolver[key]<<endl;
          }


        }
      }

    }
    file.close();
    return devolver;

  };

public:
  map<string,int> indice;

  Random_File(std::string f,std::string i)
  {

    filename=f;
    indexname=i;
    indice=leer_indice();

  };

  void add_random(string n)
  {
    t0=clock();
    //Asumo que n es un nuevo registro en formato csv
    //Y que este sera un registro correcto
    //Si quisiera agregar algo erroneo deberia evitar que pase
    //Pero eso seria con una funcion dentro de dicha clase para comprobar
    //Pero lo importante aca es el uso de los indices, asi que para no desviarme lo desarrollo asi
    //El n seria el registro completo y separado por comas cada atributo con su correcta longitud
    ofstream datos(filename,ios::app);
    int pos;
    datos.seekp(0,ios::end);
    //Guardando la posicion en la que se ingresera
    pos=datos.tellp();
    //Guardando el archivo
    datos << n << "\n";
    string c;
    istringstream aux (n);
    getline(aux,c,',');
    //Poner en el indice
    indice[c]=pos;
    datos.close();
    t1=clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
    cout<<"Se realizaron un total 1 acceso a disco, pero no se cuentan los tomados para guardar el indice al final del programa "<<endl;

  };

  void buscar(string key)
  {
    t0=clock();
    int pos=indice[key];
    if(pos==0&&key!=first)
    {
      //Guarde el primero ya que su indice me dara 0 y en un map<string,int> si el valor string no existe entonces su valor int sera 0
      //Por lo tanto si en el indice me da 0 y la key no es el primer archivo quiere decir que no existe
      cout<<"No such register"<<endl;
      //Borro lo que se habia creado en el map
      indice.erase(key);
    }
    else
    {
      ifstream datos(filename);
      //Me voy a la posicion del registro
      datos.seekg(pos);
      string registro;
      //Extraigo el registro
      getline(datos,registro);
      //Muestro el registro
      cout<<registro<<endl;
      datos.close();
    }
    t1=clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    printf("Execution time: %.16g milisegundos\n", time * 1000.0);
    // cout << "Execution Time: " << time << endl;
    cout<<"Se realizaron un total de 1 accesos a disco. "<<endl;


  };


  ~Random_File()
  {
    //Guardar el indice al finalizar el programa
    ofstream index(indexname);
    //
    for(auto i:indice)
    {
      //Recorro el indice y voy guardando de manera inversa en memoria secundaria
      //Imprimia los valores para ver si todo estaba bien
        // cout<<i.first<<" "<<indice[i.first]<<endl;
        index<<indice[i.first]<<" "<<i.first<<"\n";

    }
  };


};
