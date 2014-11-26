/* 
 * File:   ItemCancion.h
 * Author: alf
 *
 * Created on 20 de noviembre de 2014, 13:42
 */

#ifndef ITEMCANCION_H
#define	ITEMCANCION_H

using namespace std;

class ItemCancion {
public:
    ItemCancion             (string word): palabra(word){};
    ItemCancion             (string word, int cod): palabra(word), code(cod) {};
    ItemCancion             (const ItemCancion& orig);
    virtual ~ItemCancion    () {};
    bool operator<          (const ItemCancion& orig) const;
    bool operator>          (const ItemCancion& orig) const;
    bool operator==         (const ItemCancion& orig) const;
    void    SetPalabra      (string palabra);
    string  GetPalabra      () const;
    void    SetCod          (int cod);
    int     GetCod          () const;
private:
    string  palabra;
    int code;
};

ItemCancion::ItemCancion(const ItemCancion& orig) {
    palabra = orig.palabra;
}

bool ItemCancion::operator< (const ItemCancion& orig) const {
    return palabra < orig.palabra;
    
}

bool ItemCancion::operator> (const ItemCancion& orig) const {
   return palabra > orig.palabra;
}

bool ItemCancion::operator ==(const ItemCancion& orig) const {
    return palabra == orig.palabra;
}

void ItemCancion::SetPalabra(string palabra) {
    this->palabra = palabra;
}

string ItemCancion::GetPalabra() const {
    return palabra;
}

void ItemCancion::SetCod(int cod) {
    this->code = cod;
}

int ItemCancion::GetCod() const {
    return code;
}

#endif	/* ITEMCANCION_H */

