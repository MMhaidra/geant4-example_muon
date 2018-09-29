#ifndef mcDetectorConstruction_h
#define mcDetectorConstruction_h 1

#include "mcAnalyzer.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4NistManager.hh" // [yy]

class G4Box;
class G4Orb;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class mcDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    mcDetectorConstruction();
    ~mcDetectorConstruction();
    
public:
    
    G4VPhysicalVolume* Construct();
    
    void UpdateGeometry();
    
    const G4VPhysicalVolume* GetSensor()     const {return physSensor;};
    
    void SetAnalyzer(mcAnalyzer*);
    
    
private:

    G4Box*             solidSensor;
    G4LogicalVolume*   logicSensor;
    G4VPhysicalVolume* physSensor;
    
    G4NistManager* nistMan; // [yy]
    G4Material* EJ200;  // [yy]   Eljen EJ200 (assumed as PVT Scintillator)
    G4Material* Air;    // [yy]
    G4Material* GAGG;   // [yy]
    
    void DefineMaterials();
    mcAnalyzer* analyzer;

};

#endif
