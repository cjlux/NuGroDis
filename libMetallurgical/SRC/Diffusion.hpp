class Diffusion
{

public:
    Diffusion();
    ~Diffusion();
    
    //Computes the diffusion coefficient for the given value of the temperature . Unit: K
    void ComputeCoefValue();

protected:

private:
    
    //Initial diffusion value. unit: m^2/s
    const double preExpDiffusionCoef_;
    
    //Activation energy of diffusion for a pair of chemical elements. Unit: J/mol
    const double activationEnergy_;
    

};