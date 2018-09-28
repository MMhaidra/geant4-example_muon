# README

Created on Sep.27th, 2018.

# Reference
[Geant4 Training at Desy Zeuthen, 2011/3 (Geant4 and Calorimetry in HEP)](https://www-zeuthen.desy.de/geant4/g4course2011/day1/overview.html) > [Day 2 Basics of Geant4](https://www-zeuthen.desy.de/geant4/g4course2011/day2/index.html) > [Proxesses and Particles](https://www-zeuthen.desy.de/geant4/g4course2011/day2/4_particlesNprocesses/index.html)

# How to get source files?
```
wget http://www.ifh.de/geant4/g4course2011/day2d.tgz
```

# How changed?

- Geometry -> copy number:0-127 "Sensor", EJ-200
- Muon beam

## EJ-200/EJ-212
Plastic scintillators such as EJ-200 often seems to be supposed as 'PVT bases'.  
(ref: [murffer/DetectorSim/blob/master/ScintillationSlab/src/Material.cc](https://github.com/murffer/DetectorSim/blob/master/ScintillationSlab/src/Material.cc))  
Thus, EJ-200 is defined as **G4\_PLASTIC\_SC\_VINYLTOLUENE** in this code.

```
EJ200 = nistMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

```  
  
G4\_PLASTIC\_SC\_VINYLTOLUENE is defined at 1468-1470 Lines in **G4NistMaterialBuilder.cc**.  
(ref: [Id: G4NistMaterialBuilder.cc 67044 2013-01-30 08:50:06Z gcosmo](http://www.apc.univ-paris7.fr/~franco/g4doxy/html/G4NistMaterialBuilder_8cc-source.html))

```G4NistMaterialBuilder.cc
AddMaterial("G4_PLASTIC_SC_VINYLTOLUENE", 1.032, 0, 64.7, 2);
AddElementByWeightFraction( 1, 0.085);
AddElementByWeightFraction( 6, 0.915);
```

Since EJ-200 and EJ-212 have similar properties, EJ-212 can be also defined as G4\_PLASTIC\_SC\_VINYLTOLUENE.

# Schedule
- Add muon libraries to PhysicsList 9/27
- Generate muon beam 9/27
- Change detector geometry 9/28
- Change detector materials 9/28
- remove unnecessary definitions 9/29
- change file name 9/29