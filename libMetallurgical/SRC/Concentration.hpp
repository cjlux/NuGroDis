class Concentration
{

public:

    //The conctructor, taking as optional argument the initial  volumic concentration value
    Concentration();
     
    ~Concentration();
    
    //Compute the atomic concentration from  current volumic value. 
    //Because current atomic concentration is not stored , it is computed using chemicalElement::ConvertVolumicToAtomicConcentration
    void ComputeAtomicValue();
    
    // Compute massic from the current volumic value  Because current amassic concentration is not 
    // stored , it is computed using chemicalElement::ConvertVolumicToAtMassicncentration
    void ComputeMassicValue();
    
    //Set Initial atomic value
    void SetInitialAtomicValue();
    
    //Set Initial massic value
    void SetInitialMassicValue();
    
    //Set value of volumic concentration
    void SetVolumicValue();
    
    //This method return the value of the volumic concentration
    double GetVolumicValue() const;


protected:

private:

    //The initial atomic concentration of the chemical element. Unit: ratio in the range [0,1]
    const double initialAtomicValue_; 
    
    //The initial massic concentration of the chemical element. Unit: ratio in the range [0,1]
    const double initialMassicValue_;
    
    // new double[NbElements][NbElement] stoechiometricCoef_;
    
    //value of the current volumic concentration of the chemical element. Unit : ratio in the range[ 0, 1]. May be computed from massic concentration....
    double volumicValue_;

};

inline void
Concentration::SetInitialMassicValue()
{
}

inline void
Concentration::SetInitialAtomicValue()
{
}

inline void
Concentration::SetVolumicValue()
{
}

inline double
Concentration::GetVolumicValue() const
{
}
