// Interfaz base para sistemas (update y lifecycle)
// define la forma en que los sistemas interactúan con el mundo

#pragma once 

class World; //un forward declaration para evitar inlcudes circulares

//Contrato para los demás sistemas a implementar
struct ISystem{
    virtual ~ISystem() = default;   //destructor virtual por defecto

    //actualización de la lógica del sistema
    virtual void update(World& world, float dt) = 0;    //Referencia a world y dt (Delta time)
                                                        //para tiempo transcurrido desde el ultimo frotograma 
};