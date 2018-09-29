
#include "muPrimaryGeneratorAction.hh"
#include "muParticleGun.hh"
#include "muDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4MuonMinus.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4GeneralParticleSource.hh" // [yy] for gps

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

muPrimaryGeneratorAction::muPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), gpsParticleGun(0)
{
  //G4int n_particle = 1;

  gpsParticleGun = new G4GeneralParticleSource();

  // *** Note ***
  // If you fix the parameters here, you cannot change them from .mac file. I guess.

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

muPrimaryGeneratorAction::~muPrimaryGeneratorAction()
{

  delete gpsParticleGun;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void muPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  gpsParticleGun->GeneratePrimaryVertex(anEvent);
  
}

/*
muPrimaryGeneratorAction::muPrimaryGeneratorAction(const muDetectorConstruction* muDC)
:particleTable(G4ParticleTable::GetParticleTable())
{
    particleGun = new muParticleGun();
    G4ParticleDefinition* particle = particleTable->FindParticle("geantino");
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(1.0,0.0,0.0));
    particleGun->SetParticleEnergy(electron_mass_c2);
    particleGun->SetParticlePosition(G4ThreeVector(0.0,0.0,0.0));
    
    
}



muPrimaryGeneratorAction::~muPrimaryGeneratorAction()
{
    delete particleGun;
}



void muPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    particleGun->GeneratePrimaryVertex(anEvent);
    
}
*/


