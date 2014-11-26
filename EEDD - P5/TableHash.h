/* 
 * File:   tableHash.h
 * Author: alf
 *
 * Created on 21 de noviembre de 2014, 18:16
 */

#ifndef TABLEHASH_H
#define	TABLEHASH_H
#include <vector>
#include <bitset>
#include <math.h>

unsigned long prime, _hash1, _hash2;        // Variables globales

using namespace std;

// Declaracion de funciones hechas al final de la clase
long getPrime(long num);
void getHash (unsigned long key, unsigned long _size);


template<class T>
class Entry {
public:
    long key;
    T data;
    
    Entry (long aKey, T aData) : key(aKey), data(aData) {};
};

template<class T, long L>
class TableHash {
private:
    unsigned long       size;
    vector<Entry<T> >  table;
    bitset<L>           chek;       // Estructura de datos para llevar las posiciones "ocupadas"/"vacías"/"libres"
    
    // Función de dispersión de la tabla
    unsigned long doubleDispersion (unsigned long key, int numColi) {
        unsigned long _hash = 0;
        _hash = key % size;
        return _hash;
    }
    
public:
    MyHastTable<T, L> () {
        size = L;
    }
    
    bool insert     (const long key, const T &data);
    T* search       (const long key);
    int size        ();
};

/**
 * 
 * @return devuelve el tamaño ocupado en la tabla
 */
template<class T, long L>
int TableHash<class T,long L>::size() {
    return chek.count();
}

/**
 * 
 * @param key clave de búsqueda del objeto
 * @return devuelbe un puntero al objeto encontrado
 * @description 
 */
template<class T, long L>
T* TableHash<class T,long L>::search(const long key) {
    int i = 0;
    getHash(key, size);
    unsigned long pos;
    do {
        pos = doubleDispersion(key, i);
        if (table[pos].key == key)
            return &table[pos].data;
        else
            i++;
    } while (chek.test(pos));
    return 0;
}

/**
 * 
 * @param key clave de búsqueda para el dato
 * @param data
 * @return devuelbe true si lo ha insertado y false en caso de que el número de colisiones sea superior a 6
 */
template<class T, long L>
bool TableHash<class T,long L>::insert(const long key, const T& data) {
    int numColisions = 0;
    getHash(key, size);
    while (numColisions < 6) {
        unsigned long pos = doubleDispersion(key, numColisions);
        if (!chek.test(pos)) {
            table[pos].key = key;
            table[pos].data = data;
            chek.flip(pos);
            return true;
        } else
            ++numColisions;
    }
    return false;
}

/**
 * 
 * @param key clave del dato
 * @param _size tamaño de la tabal
 * @description método para generar los valores del cálculo de la tabla de dispersión
 */
void getHash(unsigned long key, unsigned long _size) {
    long p = getPrime(15000);
    _hash1 = (key % p) % _size;
    _hash2 = prime - (key % prime);
}

/**
 * 
 * @param num
 * @return primer número primo inferior al num
 * @description calcula el siguiente número primo inferior comprobando por todos
 *              los números inferiores al resultado de su raíz
 */
long getPrime(long num) {
    
    long n = num - 1;
    int m = sqrt(n);
    
    while (m > 1) 
    {
        if (n%m == 0) 
        {            
            n -= 1;
            m = sqrt(n);            
        } 
        else
            --m;
        
        if (m == 1)
            return n;
    }
}
#endif	/* TABLEHASH_H */

