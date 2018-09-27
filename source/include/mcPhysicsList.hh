#ifndef mcPhysicsList_h
#define mcPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"


class mcPhysicsList: public G4VUserPhysicsList
{
    public:
        //! Constructor
        mcPhysicsList();
        //! Destructor
        ~mcPhysicsList();
    
    protected:
        // Construct particle and physics
        void ConstructParticle();
        void ConstructProcess();
        //! Define user cuts
        void SetCuts();
    
        //! Construct particles
        void ConstructBosons();
        void ConstructLeptons();
        void ConstructMesons();
        void ConstructBaryons();

    //protected:
    private:
        // these methods Construct physics processes and register them
        //! Define general decays
        void ConstructGeneral();
        //! Define electro magnetic interactions
        void ConstructEM();
        //! Define decays
        void ConstructDecay(); // [yy]
    
        void ConstructOp();
};


#endif



