
#ifndef muPrimaryGeneratorAction_h
#define muPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4ParticleTable;
class G4Event;
class muDetectorConstruction;

class muPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    muPrimaryGeneratorAction(const muDetectorConstruction*);
    ~muPrimaryGeneratorAction();
    
public:
    void GeneratePrimaries(G4Event*);
    
private:
    G4ParticleGun*              particleGun;	  //pointer a to G4  class
    G4ParticleTable*			particleTable;
    //const muDetectorConstruction* muDetector;
};


#endif


