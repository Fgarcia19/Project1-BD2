#include <ctime>
#define N 17
#define BUCKETS 13
// unsigned t0,t1;
/*
N es la cantidad maxima de registros en un BUCKET
BUCKETS es la cantidad de Bucket que seran creados inicialmente
En otras palabras sera el valor con el que saque residuo en mi funcion hash
*/

struct Bucket
{
  //Posiciones de los registros en su archivo de datos
  int registros[N];
  int overflow;
  int ocupados;
  Bucket()
  {
    for(int i=0;i<N;i++)
      registros[i]=-1;
    overflow=-1;
    ocupados=0;

  };




  int add(int s)
  {
    //Como se inicia en -1 el overflow, si este cambia es porque este bucket tiene
    //Buckets de overflow por lo tanto no se podra agregar aca
    if(overflow!=-1)
      return overflow;
    if(ocupados==N)
    {
      //Si el bucket esta lleno no puedo agregar aca, mas bien debo crear el overflow
      return -1;
    }
    //Cuando no entra a ningun if quiere decir que puedo agregarlo
    registros[ocupados]=s;
    ocupados++;

    return 0;
  }

  void ver()
  {
    //Funcion que use para ver los buckets
    cout<<"--------------------"<<endl;
    cout<<"BUCKET"<<endl;
    for(int i=0;i<N;i++)
      cout<<registros[i]<<" ";
    cout<<endl;
    cout<<"O "<<overflow<<" Oc "<<ocupados<<endl;

  }

};

struct Hash_Index
{
  string filename;
  string indexname;
  //Columna en el cual hare el hash
  int columna;
  //tamano del indice que me sirve guardarlo para cuando creo buckets de overflow
  int size;

  string atributo(string linea)
  {
    //Como los atributos de los registros estan separados por ','
    //Esta funcion me devolvera el atributo por el cual tengo que hacer el hash
    stringstream registro(linea);
    string dato;
    for(int k=0;k<columna;k++)
    {
      getline(registro,dato,',');
    }
    return dato;
  };


  int funcion_hash(string arg)
  {
    //Funcion para obtener el indice hash
    int h=0;
    for(auto i:arg)
    {
      int a=i;
      h+=a;
    }

    return h%BUCKETS;
  };

  Hash_Index(string f,string i):filename(f),indexname(i)
  {
    ifstream ver(indexname,ios::binary);
    //Si no existe crear uno, caso contrario no hacerlo
    if(ver.fail())
    {
      ver.close();
      ofstream ind(indexname,ios::binary);
      cout<<"Ingrese numero de columna para hacer el HASH"<<endl;
      int c;
      cin>>c;
      columna=c;
      //Primero guardo la columna en la cual se hara el hash para saberlo en el caso el indice ya este creado
      ind.write((char*)(&c),sizeof(int));
      //Creando BUCKETS vacios y guardandolos en memoria
      for(int i=0;i<BUCKETS;i++)
      {
        Bucket b;
        // cout<<"B "<<i<<" "<<ind.tellp()<<endl;
        ind.write((char*)(&b),sizeof(Bucket));
      }
      size=ind.tellp();
      ind.close();

      //Cargando los datos y guardando sus posiciones en el indice hash
      ifstream datos(filename);
      datos.seekg(0);

      while(!datos.eof())
      {

        string t;
        int p=datos.tellg();
        getline(datos,t);
        if(t.size()==0)
          break;
        // cout<<t<<endl;
        add(t,p);
      }

      datos.close();

    }
    else
    {
      //caso el indice ya este creado, solo cargo la columna en la cual se hizo el hash
      ifstream indi(indexname,ios::binary);
      indi.seekg(0);
      int c;
      indi.read((char*)(&c),sizeof(int));
      columna=c;
      indi.close();

    }
  };

  int add(string record,int pos)
  {
    //Cantidad de accesos a disco
    int accesos=0;
    int edit;
    //Atributo del registro en el cual se hara el hash
    string key=atributo(record);
    int ind_hash=funcion_hash(key);
    fstream prueba(indexname,std::ios::out | std::ios::in | std::ios::binary | std::ios::ate);
    ifstream read(indexname,ios::binary);
    Bucket aux;
    prueba.seekg(ind_hash*sizeof(Bucket)+sizeof(int));
    edit=prueba.tellg();
    accesos++;
    prueba.read((char*)(&aux),sizeof(Bucket));
    accesos++;
    int possible=aux.add(pos);
    //Como mencione antes, la funcion add de la clase bucket me devolvera un valor el
    //El cual me dira si es posible insertar en el bucket
    while(true)
    {
      if(possible==-1)
      {
        //Regresa 1 cuando la bucket esta llena y hay que crear el overflow
        Bucket b;
        b.add(pos);
        //Creo la nueva bucket e inserto
        //El valor que devuelve no interesa ya que sera 0 debido a
        //Que el objeto recien esta creado por lo que no esta lleno ni tiene overflow
        prueba.seekg(edit);
        //Como la nueva bucket estara al final, debo poner que el overflow de la actual me dirija a esta
        aux.overflow=size;
        //Como cree un nuevo bucket debo actualizar el size
        size+=sizeof(Bucket);
        accesos++;
        //Guardando la bucket actual, actualizando su overflow
        prueba.write((char*)(&aux),sizeof(Bucket));
        prueba.seekg(0,ios::end);
        accesos++;
        //Guardando la bucket de overflow
        prueba.write((char*)(&b),sizeof(Bucket));
        break;

      }
      else if(possible==0)
      {
        //Regresa 0 cuando es posible agregar a la bucket
        prueba.seekp(edit);
        //Guardo la bucket despues de agregarle el nuevo valor
        prueba.write((char*)(&aux),sizeof(Bucket));
        accesos++;
        prueba.close();
        break;

      }
      else
      {
        //Cuando sea cualquier otro valor de los mencionados, quiere decir que tiene overflow
        prueba.seekg(possible);
        edit=possible;
        accesos++;
        //Debo dirigirme al bucket de overflow y hacer lo mismo para ver si es posible guardar en esa
        prueba.read((char*)(&aux),sizeof(Bucket));
        //Veo si es posible guardar en el bucket de overflow antes de volver a loop
        possible=aux.add(pos);
      }

    }


    return accesos;
  };

  void add_hash(string registro)
  {
    //Funcion la cual es llamada desde el main para agregar el registro dado por el usuario
    t0=clock();
    ofstream ind(filename,ios::app);
    ind.seekp(0,ios::end);
    int pos=ind.tellp();
    ind <<registro<<"\n";
    ind.close();
    int a=add(registro,pos);
    //Llama a la funcion antes mendionada por la cual tendremos los accesos a disco que hizo
    t1=clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
    cout<<"Se realizaron un total de "<<a<<" accesos a disco. "<<endl;

  };

  void search(string key)
  {
    unsigned hash_t0,hash_t1;
    hash_t0=clock();

    int accesos_a_disco=0;
    ifstream indice(indexname,ios::binary);
    ifstream datos(filename);
    //Saco cual sera la ubicacion esperada de la key a raiz de su indice HASH
    //La suma del sizeof(int) es debido a que al inicio esta guardado el int que es la columna de hash
    int ind_hash=funcion_hash(key)*sizeof(Bucket)+sizeof(int);
    Bucket b;
    string r;

    do
    {
      //Busco en todos los valores de la bucket y del overflow
      // y muestro todos los qque tienen la key buscada
    indice.seekg(ind_hash);
    accesos_a_disco++;
    indice.read((char*)(&b),sizeof(Bucket));

    // b.ver();
    for(auto i:b.registros)
    {
      if(i!=-1){
      datos.seekg(i);
      accesos_a_disco++;
      getline(datos,r);
      if(key==atributo(r))
        cout<<r<<endl;
      }
      else
        break;
    }
    ind_hash=b.overflow;
  }while(b.overflow!=-1);

  hash_t1=clock();

  double time = (double(hash_t1-hash_t0)/CLOCKS_PER_SEC);
  // cout << "Execution Time: " << time << endl;
  printf("Execution time: %.16g milisegundos\n", time * 1000.0);

  cout<<"Se realizaron un total de "<<accesos_a_disco<<" accesos a disco. "<<endl;
};


};
