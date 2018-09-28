#include "mcDetectorConstruction.hh"
#include "mcDetectorMessenger.hh"
#include "mcSensorSD.hh"
#include "mcAnalyzer.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4NistManager.hh" // [yy] for use of G4materials

#include <iostream>

mcDetectorConstruction::mcDetectorConstruction()
://defaultMaterial(0),sensorMaterial(0),
WorldRadius(100*cm),
//solidWorld(0),logicWorld(0),physWorld(0),
solidSensor(0),logicSensor(0),physSensor(0),
magField(0),pUserLimits(0),maxStep(100.0*cm)
{
    
    // default parameter values of Sensor
    DefineMaterials();
    
    //SetSensorMaterial("NaI"); // [yy] comment
    //SetSensorMaterial("GAGG"); // [yy] comment
    
    // create commands for interactive definition of the calorimeter
    //detectorMessenger = new mcDetectorMessenger(this); // [yy] comment
}

mcDetectorConstruction::~mcDetectorConstruction()
{ 
    delete detectorMessenger;
}

//------------------------------------------------------------------------//
// Begin of Construct()
G4VPhysicalVolume* mcDetectorConstruction::Construct()
{
    // Clean old geometry, if any
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    
    /* -------------------------------------------------------------------- */
    // Create geometry
    
   // --- Overview of geometry ---
   // world - module -  scabox  - Sensor* (copy no.: 0-63)
   //                             absbox  - Sensor* (copy no: 64-127)    
   // * means "Sensitive Detector"
   
     // --- Parameters for geometry ---

  // world
  G4double world_sizeX = 100*cm; 
  G4double world_sizeY = 100*cm; 
  G4double world_sizeZ = 100*cm;
  WorldRadius = world_sizeX;

  // scabox
  G4double scabox_sizeX =     30.0  *mm;  // > scavoxel_sizeX
  G4double scabox_sizeY =     24.0  *mm;  // > scavoxel_pitchY*number_of_scavoxel_dimY
  G4double scabox_sizeZ =     24.0  *mm;  // > scavoxel_pitchZ*number_of_scavoxel_dimZ

  // Sensor
  G4double scavoxel_sizeX =   30.0  *mm;
  G4double scavoxel_sizeY =     2.0  *mm;
  G4double scavoxel_sizeZ =     2.0  *mm;
  G4double scavoxel_pitchY =    3.0  *mm;
  G4double scavoxel_pitchZ =    3.0  *mm;
  G4int number_of_scavoxel_dimY  =   8; 
  G4int number_of_scavoxel_dimZ  =   8; 

  // absbox
  G4double absbox_sizeX =   30.0  *mm;  // > absvoxel_sizeX
  G4double absbox_sizeY =   24.0  *mm;  // > absvoxel_pitchY*number_of_absvoxel_dimY
  G4double absbox_sizeZ =   24.0  *mm;  // > absvoxel_pitchZ*number_of_absvoxel_dimZ
  // absvoxel
  G4double absvoxel_sizeX =    30.0  *mm;
  G4double absvoxel_sizeY =      2.0  *mm;
  G4double absvoxel_sizeZ =      2.0  *mm;
  G4double absvoxel_pitchY =     3.0  *mm;
  G4double absvoxel_pitchZ =     3.0  *mm;
  G4int number_of_absvoxel_dimY  =  8; 
  G4int number_of_absvoxel_dimZ  =  8;  
    
    // distances
    G4double distance_source_sca  = 100.0  *mm; // distance between scatter and origin(0,0,0)
    G4double distance_sca_abs        =  80.0  *mm; // distance between absorber and scatter

    // module
    G4double module_sizeX = scabox_sizeX+distance_sca_abs+absbox_sizeX; 
    G4double module_sizeY = absbox_sizeY; // under supposition that scabox and absbos are same  
    G4double module_sizeZ = absbox_sizeZ; // under supposition that scabox and absbos are same  

    // if create multiple modules as circle
    //G4int number_of_modules = 1;  // 1: single Compton camera module
    //G4double angle_rot_modules = 360.0/number_of_modules *deg;
    
    // -- World
    // solid definition (size)
    G4Box* solidWorld =  new G4Box("World",                     // its name
	                                                       0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);      // its size
  
    // logical volume definition (material)
    G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(
                        solidWorld,          // its solid
                        Air,                 // its material
                        "World");            // its name

  // physical volume definition (position, relation, copy number)
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(
                      0,                                // no rotation
                      G4ThreeVector(),       // at (0,0,0)
                      logicWorld,          // its logical volume
                      "World",               // its name
                      0,                         // its mother  volume
                      false,                   // no boolean operation
                      0,                         // copy number
                      false);       // checking overlaps 

  // --- Module
  
  // solid definition (size)
  G4Box* solidModule =    
    new G4Box("Module",                       // its name
	      0.5*module_sizeX, 0.5*module_sizeY, 0.5*module_sizeZ); // its size

  // logical volume definition (material)
  G4LogicalVolume* logicModule =                         
    new G4LogicalVolume(solidModule,          // its solid
                        Air,                  // its material
                        "Module");            // its name

    // physical volume definition (position, relation, copy number)
    G4VPhysicalVolume* physModule = 
    new G4PVPlacement(0,         // no rotation
                      G4ThreeVector((distance_source_sca+module_sizeX*0.5),0,0),
                      logicModule,          // its logical volume
                      "Module",               // its name
                      logicWorld,            // its mother  volume
                      false,                 // no boolean operation
                      0,                       // copy number
                      false);                // checking overlaps 
                      
  /* // if create multiple modules as circle
  for (G4int i = 0; i < number_of_modules ; i++) {
  
    G4ThreeVector armVec= G4ThreeVector((distance_source_sca+module_sizeX*0.5),0,0);
    G4RotationMatrix armRot= G4RotationMatrix();
    armVec.rotateZ(i*angle_rot_modules);
    armRot.rotateZ(i*angle_rot_modules);
  
    new G4PVPlacement(G4Transform3D(armRot, armVec),    
                      logicModule,          // its logical volume
                      "Module",               // its name
                      logicWorld,            // its mother  volume
                      false,                 // no boolean operation
                      i,                       // copy number
                      true);                // checking overlaps 
   }
   */
   

  // -- scatter box

  // solid definition (size)
  G4Box* solidScabox =    
    new G4Box("Scabox",                       // its name
	      0.5*scabox_sizeX, 0.5*scabox_sizeY, 0.5*scabox_sizeZ); // its size
  
  // logical volume definition (material)
  G4LogicalVolume* logicScabox =                         
    new G4LogicalVolume(solidScabox,          // its solid
                        Air,              // its material 
                        "Scabox");            // its name

  new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(scabox_sizeX*0.5 - module_sizeX*0.5, 0, 0),  
                      logicScabox,            // its logical volume
                      "Scabox",               // its name
                      logicModule,            // its mother  volume
                      false,                  // no boolean operation
                      0,                      // copy number
                      true);        // checking overlaps 

  // -- absober box

  // solid definition (size)
  G4Box* solidAbsbox =    
    new G4Box("Absbox",                       // its name
	      0.5*absbox_sizeX, 0.5*absbox_sizeY, 0.5*absbox_sizeZ); // its size
  
  // logical volume definition (material)
   G4LogicalVolume* logicAbsbox =                         
    new G4LogicalVolume(solidAbsbox,          // its solid
                        Air,                  // its material
                        "Absbox");            // its name

  // physical volume definition (position, relation, copy number)
    new G4PVPlacement(0,                      // no rotation
                      G4ThreeVector(module_sizeX*0.5 - absbox_sizeX*0.5, 0, 0),      
                      logicAbsbox,            // its logical volume
                      "Absbox",               // its name
                      logicModule,             // its mother  volume
                      false,                  // no boolean operation
                      1,                      // copy number
                      true);        // checking overlaps 
  
  // -- scatter voxel

  // solid definition (size)
  solidSensor =    
    new G4Box("Sensor",                       // its name
	                    0.5*scavoxel_sizeX, 0.5*scavoxel_sizeY, 0.5*scavoxel_sizeZ); // its size
  
  // logical volume definition (material)
  logicSensor =                         
    new G4LogicalVolume(solidSensor,          // its solid
                        EJ200,                   // its material
                        "Sensor");            // its name

  // physical volume definition (position, relation, copy number)
  for (G4int i = 0; i < number_of_scavoxel_dimY ; i++) {
    for (G4int j = 0; j < number_of_scavoxel_dimZ ; j++) {
       new G4PVPlacement(0,                     // no rotation
                      G4ThreeVector(0,(i+0.5)*scavoxel_pitchY-scabox_sizeY*0.5,(j+0.5)*scavoxel_pitchZ-scabox_sizeZ*0.5), //location
                      logicSensor,            // its logical volume
                      "Sensor",               // its name
                      logicScabox,              // its mother  volume
                      false,                    // no boolean operation
                      i*number_of_scavoxel_dimZ+j,      // copy number
                      true);                       // checking overlaps
    }
  } 

  // physical volume definition (position, relation, copy number)
  for (G4int i = 0; i < number_of_absvoxel_dimY ; i++) {
    for (G4int j = 0; j < number_of_absvoxel_dimZ ; j++) {
      new G4PVPlacement(0,                    // no rotation
                      G4ThreeVector(0,(i+0.5)*absvoxel_pitchY-absbox_sizeY*0.5,(j+0.5)*absvoxel_pitchZ-absbox_sizeZ*0.5), // location
                      logicSensor,          // its logical volume
                      "Sensor",             // its name
                      logicAbsbox,            // its mother  volume
                      false,                  // no boolean operation
                      i*number_of_absvoxel_dimZ+j + number_of_scavoxel_dimY*number_of_scavoxel_dimZ,  // copy number
                      true);        // checking overlaps
    }
  } 
   
   /*
    // Sensor
    G4RotationMatrix matSensor  = G4RotationMatrix();
    solidSensor = new G4Tubs("Sensor",0.0*cm,5.*cm,10*cm,0,CLHEP::twopi);
    logicSensor = new G4LogicalVolume(solidSensor, EJ200, "Sensor");
    
    physSensor = new G4PVPlacement(0,G4ThreeVector(0,0,15*cm),logicSensor,"Sensor",logicWorld,false,1);
    matSensor.rotateX(-135 * deg);
    physSensor = new G4PVPlacement(G4Transform3D( matSensor,G4ThreeVector(0,15*cm,-15*cm)),logicSensor,"Sensor",logicWorld,false,2);
    matSensor.rotateX(135 * deg);
    matSensor.rotateX(135 * deg);
    physSensor = new G4PVPlacement(G4Transform3D( matSensor,G4ThreeVector(0,-15*cm,-15*cm)),logicSensor,"Sensor",logicWorld,false,3);
    */
    
    /*
    // source
    const G4double sourceThickness = 5.0 * mm;
    G4Tubs* env22NaCase = new G4Tubs( "env22NaCase", 0, 25.0/2.0 * mm, 5.0 / 2.0 * mm, 0, CLHEP::twopi);
    G4Tubs* solid22NaCase = new G4Tubs( "solid22NaCase", 8.0 /2.0 * mm, 25.0/2.0 * mm, sourceThickness /2.0, 0.0, CLHEP::twopi);
    G4Tubs* solid22NaCaseBottom = new G4Tubs( "solid22NaCaseBottom", 0, 8.0/2.0 * mm, 2.0 / 2.0 * mm, 0.0, CLHEP::twopi);
    G4Tubs* solid22NaCaseWindow = new G4Tubs( "solid22NaCaseWindow", 0, 8.0/2.0 * mm, 0.1 / 2.0 * mm, 0.0, CLHEP::twopi);
    
    G4Material* mat_Aluminium = new G4Material("mat_Aluminium", 13.,  26.98*g/mole, 2.70*g/cm3);
    G4Element* H  = new G4Element("Hydrogen","H" , 1., 1.01*g/mole);
    G4Element* C  = new G4Element("Carbon"  ,"C" , 6., 12.01*g/mole);
    G4Material* Sci =
    new G4Material("Scintillator", 1.032*g/cm3, 2);
    Sci->AddElement(C, 9);
    Sci->AddElement(H, 10);
    */
    /*
    G4Material* Vacuum =
    new G4Material("Galactic", 1., 1.01*g/mole, universe_mean_density,
                   kStateGas, 2.73*kelvin, 3.e-18*pascal);
    
    G4LogicalVolume* logenv22NaCase = new G4LogicalVolume( env22NaCase, Vacuum, "logenv22NaCase", 0,0,0,true);
    G4LogicalVolume* logSide22NaCase = new G4LogicalVolume( solid22NaCase, Sci, "logSide22NaCase", 0,0,0,true);
    G4LogicalVolume* logBottom22NaCase = new G4LogicalVolume( solid22NaCaseBottom, Sci, "logBottom22NaCase", 0,0,0,true);
    G4LogicalVolume* logWindow22NaCase = new G4LogicalVolume( solid22NaCaseWindow, mat_Aluminium, "logWindow22NaCase", 0,0,0,true);
                                                             
    new G4PVPlacement(
                      G4Transform3D(), //rotation and vector
                      logSide22NaCase,    //logical volume
                      "physEnv22NaCaseSide",//name
                      logenv22NaCase, //mother logicall volume
                      false,                //set to false
                      9001,            //copy number
                      true);            // check
    
    new G4PVPlacement(
                      G4Transform3D(G4RotationMatrix(),G4ThreeVector(0,0,-1.5*mm)), //rotation and vector
                      logBottom22NaCase,    //logical volume
                      "physEnv22NaCaseBottom",//name
                      logenv22NaCase, //mother logicall volume
                      false,                //set to false
                      9002,            //copy number
                      true);            // check
    
    new G4PVPlacement(
                      G4Transform3D(G4RotationMatrix(),G4ThreeVector(0,0,2.0*mm)), //rotation and vector
                      logWindow22NaCase,    //logical volume
                      "physEnv22NaCaseWindow",//name
                      logenv22NaCase, //mother logicall volume
                      false,                //set to false
                      9003,            //copy number
                      true);            // check
    new G4PVPlacement(
                      G4Transform3D(
                                    G4RotationMatrix(),G4ThreeVector(0 *cm,0,0)),
                      logenv22NaCase,
                      "sourceCase",
                      logicWorld,
                      false,
                      9004,
                      true);
    */
    /*
    //Gel
    const G4double radiusSilicagel = 4.0 * mm;
    const G4double depthSilicagel = 15.0 * mm;
    G4Tubs* solidSilicagel = new G4Tubs("solidSilicagel"  , 0, radiusSilicagel,depthSilicagel/2.0,0, CLHEP::twopi);

    G4Element* O  = new G4Element("Oxygen"  ,"O" , 8., 16.00*g/mole);
    G4Element* Si = new G4Element("Silicon","Si" , 14., 28.09*g/mole);
    
    G4Material* SilicaGel = new G4Material("SilicaGel", 0.55*g/cm3, 2);
    SilicaGel->AddElement(Si, 1);
    SilicaGel->AddElement(O, 2);
    G4LogicalVolume* logSilicagel = new G4LogicalVolume( solidSilicagel, SilicaGel, "logSilicagel", 0,0,0,true);
    new G4PVPlacement(
                      G4Transform3D(G4RotationMatrix(),G4ThreeVector(0,0,2*cm)),
                      logSilicagel,    //logical volume
                      "physSilicaGel",//name
                      logicWorld, //mother logicall volume
                      false,                //set to false
                      3001,            //copy number
                      true);            // check
    */


    
    //------------------------------------------------
    // Sensitive detectors
    //------------------------------------------------
    
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    mcSensorSD* aSensorSD = (mcSensorSD*)SDman->FindSensitiveDetector("mc/SensorSD");
    if ( aSensorSD == 0){
        aSensorSD = new mcSensorSD("mc/SensorSD");
        SDman->AddNewDetector( aSensorSD );
    }
    aSensorSD->SetAnalyzer(analyzer);
    
    logicSensor->SetSensitiveDetector(aSensorSD);
    
    // Set UserLimits
    G4double maxTrkLen = 10.0*WorldRadius;
    G4double maxTime   = 1000.0 * ns;
    pUserLimits = new G4UserLimits(maxStep, maxTrkLen, maxTime);
    logicWorld->SetUserLimits(pUserLimits);
    
    // Visualization attributes
    logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
    
    G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
    simpleBoxVisAtt->SetVisibility(true);
    logicSensor->SetVisAttributes(simpleBoxVisAtt);
    
    return physWorld;
}
// End of Construct()
//------------------------------------------------------------------------//


///////////////////////////////////////////////////////
void mcDetectorConstruction::DefineMaterials()
{ 
    //This function illustrates the possible ways to define materials
    
    G4String symbol, name;
    G4double a, z, density, temperature, pressure;
    // No use temperature & pressure in Geant4
    G4int iz, n, in;
    
    G4int ncomponents, natoms;
    G4double abundance, fractionmass;
    
    // a =mass of a mole;
    // z =mean number of protons;
    // iz=number of protons in an isotope;
    // n =number of nucleons in an isotope;
    
    //
    // define Elements
    //
    G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
    G4Element* B  = new G4Element("Boron",symbol="B" , z= 5., a= 10.81*g/mole);
    G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
    G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
    G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);
    G4Element* Na = new G4Element("Sodium",symbol="Na" , z= 11., a= 22.99*g/mole);
    G4Element* Mg = new G4Element("Magnesium",symbol="Mg" , z= 12., a= 24.31*g/mole);
    G4Element* Al = new G4Element("Aluminium",symbol="Al" , z= 13., a= 26.98*g/mole);
    G4Element* Si = new G4Element("Silicon",symbol="Si" , z= 14., a= 28.09*g/mole);
    G4Element* P = new G4Element("Phosphorus",symbol="P" , z= 15., a= 30.97*g/mole);
    G4Element* K = new G4Element("Kalium",symbol="K" , z= 19., a= 39.10*g/mole);
    G4Element* Ca = new G4Element("Calcium",symbol="Ca" , z= 20., a= 40.01*g/mole);
    G4Element* Ti = new G4Element("Titan",symbol="Ti" , z= 22., a= 47.87*g/mole);
    G4Element* Mn = new G4Element("Manganese",symbol="Mn" , z= 25., a= 54.94*g/mole);
    G4Element* Fe = new G4Element("Iron",symbol="Fe" , z= 26., a= 55.85*g/mole);
    G4Element* Ni = new G4Element( "Nickel", "Ni", 28., 58.69*g/mole);
    //G4Element* Mn = new G4Element( "Manganese", "Mn", 25., 54.93*g/mole);
    G4Element* Cr = new G4Element( "Chromium", "Cr", 24., 51.996*g/mole);
    G4Element* I = new G4Element( "Iodine", "I", 53., 126.9*g/mole);
    G4Element* Gd = new G4Element("Gadolinium", "Gd", z=64, a=157.25    *g/mole); // [yy]
    G4Element* Ga = new G4Element("Gallium"   , "Ga", z=31, a= 69.723   *g/mole); // [yy]
    
    //
    // define an Element from isotopes, by relative abundance
    //
    
    
    G4Isotope* U5 = new G4Isotope("U235", iz=92, n=235, a=235.01*g/mole);
    G4Isotope* U8 = new G4Isotope("U238", iz=92, n=238, a=238.03*g/mole);
    
    G4Element* U  = new G4Element("enriched Uranium",symbol="U",ncomponents=2);
    U->AddIsotope(U5, abundance= 90.*perCent);
    U->AddIsotope(U8, abundance= 10.*perCent);
    
    // define simple materials
    
    new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
    new G4Material("Lead"     , z=82., a= 207.19*g/mole, density= 11.35*g/cm3);
    new G4Material("Tungsten" , z=74., a= 183.84*g/mole, density= 19.25*g/cm3);
    
    // define a material from elements.   case 1: chemical molecule
    
    G4Material* H2O =
    new G4Material("Water", density= 1.000*g/cm3, ncomponents=2);
    H2O->AddElement(H, natoms=2);
    H2O->AddElement(O, natoms=1);
    // overwrite computed meanExcitationEnergy with ICRU recommended value
    H2O->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);
    
    G4Material* Sci =
    new G4Material("Scintillator", density= 1.032*g/cm3, ncomponents=2);
    Sci->AddElement(C, natoms=9);
    Sci->AddElement(H, natoms=10);
    
    G4Material* Myl =
    new G4Material("Mylar", density= 1.397*g/cm3, ncomponents=3);
    Myl->AddElement(C, natoms=10);
    Myl->AddElement(H, natoms= 8);
    Myl->AddElement(O, natoms= 4);
    
    G4Material* SiO2 =
    new G4Material("quartz",density= 2.200*g/cm3, ncomponents=2);
    SiO2->AddElement(Si, natoms=1);
    SiO2->AddElement(O , natoms=2);
    
    // define a material from elements.   case 2: mixture by fractional mass
    
    G4Material* RockKam =
    new G4Material("RockKam", density= 3.0*g/cm3, ncomponents=13);
    RockKam->AddElement(H, fractionmass=0.006);
    RockKam->AddElement(C, fractionmass=0.003);
    RockKam->AddElement(O, fractionmass=0.607);
    RockKam->AddElement(Na, fractionmass=0.039);
    RockKam->AddElement(Mg, fractionmass=0.003);
    RockKam->AddElement(Al, fractionmass=0.106);
    RockKam->AddElement(Si, fractionmass=0.185);
    RockKam->AddElement(P, fractionmass=0.001);
    RockKam->AddElement(K, fractionmass=0.021);
    RockKam->AddElement(Ca, fractionmass=0.018);
    RockKam->AddElement(Ti, fractionmass=0.001);
    RockKam->AddElement(Mn, fractionmass=0.000);
    RockKam->AddElement(Fe, fractionmass=0.010);
    
    //define for detector
    // stailess SUS304
    
    G4Material* sus304 = new G4Material(name="Sus304", density=8.03*g/cm3, ncomponents=5);
    sus304->AddElement(Ni, 0.09);
    sus304->AddElement(C, 0.005);
    sus304->AddElement(Mn, 0.01);
    sus304->AddElement(Cr, 0.18);
    sus304->AddElement(Fe, 0.715);
    G4Material* polyethylene=new G4Material(name="polyethylene",density=0.92*g/cm3,ncomponents=2);
    polyethylene->AddElement(C,1);
    polyethylene->AddElement(H,2);
    G4Isotope* he3 = new G4Isotope(name="he3", iz=2, in=3, a=3.0160293191*g/mole);
    G4Element* He3 = new G4Element(name="He3", symbol="He3", ncomponents=1);
    He3->AddIsotope(he3, abundance=100.*perCent);
    G4Material* Herium3 = new G4Material(name="Herium3",density=1.34644166*mg/cm3,ncomponents=1,kStateGas,temperature=300.15*kelvin,pressure=10.*atmosphere);
    Herium3->AddElement(He3, 1);
    
    
    //the follow definition from CANDLES MC
    // Sillion rubber + B4C 40%
    // http://www.askcorp.co.jp/sanshin/work/engineering/pdf/material.pdf
    const G4double R_B4C  = 40;    // target value, wt-%
    const G4double R0_B4C = 20;    // original value, wt-%
    const G4double W0_H   = 0.065; // fraction Mass
    const G4double W0_O   = 0.173;
    const G4double W0_Si  = 0.303;
    const G4double W0_B   = 0.157;
    const G4double W0_C   = 0.302;
    
    G4double totalMass = R_B4C / R0_B4C * (W0_B + W0_C) + (100.0 - R_B4C) / (100.0 - R0_B4C) * (W0_H + W0_O + W0_Si);
    G4double W_H       = (100.0 - R_B4C) / (100.0 - R0_B4C) * W0_H  / totalMass;
    G4double W_O       = (100.0 - R_B4C) / (100.0 - R0_B4C) * W0_O  / totalMass;
    G4double W_Si      = (100.0 - R_B4C) / (100.0 - R0_B4C) * W0_Si / totalMass;
    G4double W_B       = R_B4C / R0_B4C * W0_B / totalMass;
    G4double W_C       = R_B4C / R0_B4C * W0_C / totalMass;
    
    // ref. density (2015/7/30 from T.Iida)
    G4Material* RubberB4C = new G4Material("RubberB4C", density = 1.42 *g/cm3, ncomponents = 5);
    RubberB4C -> AddElement(H,  fractionmass = W_H);
    RubberB4C -> AddElement(O,  fractionmass = W_O);
    RubberB4C -> AddElement(Si, fractionmass = W_Si);
    RubberB4C -> AddElement(B,  fractionmass = W_B);
    RubberB4C -> AddElement(C,  fractionmass = W_C);
    
    //define a material from elements and/or others materials (mixture of mixtures)
    
    G4Material* Aerog =
    new G4Material("Aerogel", density= 0.200*g/cm3, ncomponents=3);
    Aerog->AddMaterial(SiO2, fractionmass=62.5*perCent);
    Aerog->AddMaterial(H2O , fractionmass=37.4*perCent);
    Aerog->AddElement (C   , fractionmass= 0.1*perCent);
    
    // examples of gas in non STP conditions
    
    G4Material* CO2 =
    new G4Material("CarbonicGas", density= 27.*mg/cm3, ncomponents=2,
                   kStateGas, 325.*kelvin, 2.*atmosphere);
    CO2->AddElement(C, natoms=1);
    CO2->AddElement(O, natoms=2);
    
    G4Material* NaI =
    new G4Material("NaI", density= 3.67*g/cm3, ncomponents=2);
    NaI->AddElement(Na, natoms=1);
    NaI->AddElement(I, natoms=1);
    
    G4Material* steam =
    new G4Material("WaterSteam", density= 0.3*mg/cm3, ncomponents=1,
                   kStateGas, 500.*kelvin, 2.*atmosphere);
    steam->AddMaterial(H2O, fractionmass=1.);
    
    // examples of vacuum
    G4Material* Vacuum =
    new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                   kStateGas, 2.73*kelvin, 3.e-18*pascal);
                   
    /*
    G4Material* beam =
    new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1,
                   kStateGas, STP_Temperature, 2.e-2*bar);
    beam->AddMaterial(Air, fractionmass=1.);
    */
    
    // GAGG scintillator [yy]
    G4Material* GAGG = new G4Material("GAGG", density=6.63*g/cm3, 4);
    GAGG -> AddElement(Gd,  3);
    GAGG -> AddElement(Al,  2);
    GAGG -> AddElement(Ga,  3);
    GAGG -> AddElement(O , 12);
    
    // Getting the NIST Material Manager [yy]
	nistMan = G4NistManager::Instance();
	nistMan->SetVerbose(0);
	
	// Air
	 Air = nistMan->FindOrBuildMaterial("G4_AIR"); // [yy]
	/*
    Air = new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=2);
    Air->AddElement(N, fractionmass=0.7);
    Air->AddElement(O, fractionmass=0.3);
	*/
	
	// Eljen EJ200 [yy]
	EJ200 = nistMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    
    
    //default materials of the World
    //defaultMaterial  = Vacuum;
    //defaultMaterial  = Air;
}

///////////////////////////////////////////////////////
void mcDetectorConstruction::SetMaxStep(G4double value)
{
    //--------- example of User Limits -------------------------------
    // below is an example of how to set tracking constraints in a given
    // logical volume
    
    if (value >0.) {
        maxStep = value;
    } else {
        maxStep = DBL_MAX;
    }
    if (pUserLimits) {
        delete pUserLimits;
    }
    UpdateGeometry();
}
/////////////////////////////////////////////////////////////

void mcDetectorConstruction::SetSensorMaterial(G4String materialChoice)
{
    // search the material by its name
    G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
    if (pttoMaterial) {
        sensorMaterial = pttoMaterial;
        G4cout << " mcDetectorConstruction::SetSensorMaterial:  ";
        G4cout << "Sensor material is " << materialChoice << G4endl;
        UpdateGeometry();
    } else {
        G4cout << " mcDetectorConstruction::SetSensorMaterial:  ";
        G4cout << materialChoice << " is not in the Material Table.";
        G4cout <<G4endl;
    }
}


#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

void mcDetectorConstruction::SetMagField(G4double value)
{
    //apply a global uniform magnetic field along Y axis
    G4FieldManager* fieldMgr
    = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    
    if(magField) delete magField;		//delete the existing magn field
    
    if(value!=0.){			// create a new one if non nul
        fieldValue = value;
        magField = new G4UniformMagField(G4ThreeVector(0.,fieldValue,0.));
        fieldMgr->SetDetectorField(magField);
        fieldMgr->CreateChordFinder(magField);
    } else {
        magField = 0;
        fieldMgr->SetDetectorField(magField);
    }
}

#include "G4RunManager.hh"

void mcDetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

void mcDetectorConstruction::SetAnalyzer(mcAnalyzer * analyzer_in){
    analyzer = analyzer_in;
}

