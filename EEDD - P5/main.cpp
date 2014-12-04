/* 
 * File:   main.cpp
 * Author: zackreynolt
 *
 * Created on 21 de noviembre de 2014, 17:44
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>

#include "tinythread.h"
#include "millisleep.h"
#include "Song.h"
#include "Request.h"
#include "ItemCancion.h"
#include "TableHash.h"

using namespace std;
using namespace tthread;

void CargarListaCaciones(vector<Song> &vCanciones);
long djb2 (char *str);

class RadioApp {
    vector<Request>  vPeticiones;
    vector<Song>     vCanciones;
    
    TableHash<ItemCancion, 787>  tablaAutores;
    TableHash<ItemCancion, 1583> tablaTitulos;
   
    thread  threadReproducirCanciones;
    mutex   semaforo;
    bool    pinchar;

    static void hebraReproducirCanciones(void *arg) {
        RadioApp *radioApp = static_cast<RadioApp *> (arg);
        radioApp->reproducirCanciones();
    }

public:

    RadioApp() : threadReproducirCanciones(hebraReproducirCanciones, this) {
        pinchar = true;
        CargarListaCaciones(vCanciones);
        
        int pArtista = 0, pTitulo = 0;
        
        // Construcción de las tablas de dispersión
        for (int i = 0; i < vCanciones.size(); ++i) {
            
            string artista, titulo;
            string lineArtist = vCanciones[i].GetArtist();
            string lineTittle = vCanciones[i].GetTitle();
            stringstream lineStreamArtist(lineArtist);
            stringstream lineStreamTittle(lineTittle);
            
            while (getline(lineStreamArtist, artista, ' ')) {
                long key = djb2((char*) artista.c_str());
                ItemCancion *p = tablaAutores.search(key);
                if (!p) {
                    if (!tablaAutores.insert(key, ItemCancion(artista, &vCanciones[i])))
                        cout << "Demasiadas colisiones para pArtista: " << artista << endl;
                    pArtista++;
                } else
                    p->addSong(&vCanciones[i]);
            };
            
            while (getline(lineStreamTittle, titulo, ' ')) {
                long key = djb2((char*) titulo.c_str());
                ItemCancion *p = tablaTitulos.search(key);
                if (!p) {
                    if(!tablaTitulos.insert(key, ItemCancion(titulo, &vCanciones[i])))
                        cout << "Demasiadas colisiones para pTitulo: " << titulo << endl;
                    pTitulo++;
                } else
                    p->addSong(&vCanciones[i]);
            };
        };
        
//        ItemCancion *p = tablaTitulos.search(djb2((char*) "Who"));
//        vector<Song*> *songs = p->getSongs();
//        for (int i = 0; i < songs->size(); i++)
//            cout << songs->at(i)->GetCode() << endl;
        cout << "Número de palabras entre Autores: " << pArtista << endl;
        cout << "Número de palabras entre Titulos: " << pTitulo << endl;
        cout << "Data size de al tabla de autores: " << tablaAutores.tableSize() << endl;
        cout << "Data size de al tabla de titulos: " << tablaTitulos.tableSize() << endl;
    };

    void reproducirCanciones() {
        do {
            // Esperar 1 seg. hasta ver si hay alguna canción nueva en la lista de peticiones
            millisleep(1000); 
            
            while (pinchar && !vPeticiones.empty()) {
                // Sacar una canción y reproducirla
                semaforo.lock();
                // Coge la que tenga más prioridad (última)
                int cancion = vPeticiones.back().getCod();
                vPeticiones.pop_back();
                semaforo.unlock();
                
                cout << "Reproduciendo canción " << 
                        vCanciones[cancion - 1].GetTitle() << 
                        " de " << vCanciones[cancion - 1].GetArtist() <<
                        "... (" << vCanciones[cancion - 1].GetCode() << ")" << endl;
                // Simular el tiempo de reproducción de la canción (entre 2 y 12 seg.)
                
                millisleep(2000 + 1000 * (rand() % 10));
            }
            
        } while (pinchar);
    }

    void solicitarCanciones() {
        int cancion; //Código de la canción que se añadirá a vPeticiones
        
        cout << "¡Bienvenido a Radionauta v4!" << endl;
        cout << "Introduce el código de la canción que quieres reproducir:" 
                << endl;        
        
        // Pedir canciones hasta que se introduce "0"
        do {
            cin >> cancion;

            //Asegurarse de que la canción introducida es válida
            while (cancion < 0 || cancion > vCanciones.size()) {
                cin.clear();
                cin.ignore(100, '\n');
                cout << "Lo sentimos, no disponemos de esa canción." << endl;
                cin >> cancion;
            }
            
            semaforo.lock();
            //Código fuente para añadir canciones a vPeticiones
            Request peticion(cancion); 
            vPeticiones.push_back(peticion);            
            semaforo.unlock();

        } while (cancion != 0);
        
      
        pinchar = false;
        threadReproducirCanciones.join();
    }
};

/**
 * 
 * @param lSongs    Lista con las canciones (por referencia)
 * @return          void
 * @description     Este procedimiento carga en la lista de canciones todas las
 *                  canciones que se encuentran en el archivo de canciones para
 *                  tal fin ("canciones.txt") en el directorio del proyecto.
 */
void CargarListaCaciones(vector<Song> &vCanciones) {
    try {
        // Opens a file
        fstream fi("canciones.txt");
        string line, atribute[3];

        while (!fi.eof()) {
            getline(fi, line);
            stringstream lineStream(line);

            int i = 0;
            while (getline(lineStream, atribute[i], '|')) {
                i++;
            };

            int cod = atoi(atribute[0].c_str());
            Song s(cod, atribute[1], atribute[2]);
            vCanciones.push_back(s);
        }
        fi.close();
        vCanciones.pop_back();
    } catch (exception &e) {
        cout << "The file could not be open";
    }
}

long djb2 (char *str) {
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    return hash;
}

int main(int argc, char** argv) {
    RadioApp app;
    app.solicitarCanciones();

    // # de palabras en Autores 413
    // # de palabras en Titulos 829
    cout << endl;
    cout << "El primo más cercano a 413*1.9=" << 413*1.9 << " es: " << getPrime(413*1.9+10) << endl;
    cout << "El primo más cercano a 829*1.9=" << 829*1.9 << " es: " << getPrime(829*1.9+10) << endl;
    
    return 0;
}

