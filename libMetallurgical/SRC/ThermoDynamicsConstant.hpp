class ThermoDynamicsConstant
{

public:
    ThermoDynamicsConstant();
    ~ThermoDynamicsConstant();

protected:

private:

    //Perfect gas constant. Unit: J/mol.K 
    const double R_;
    
    //Boltzman constant. Unit: J/K
    const double kB_;
    
    //Avogadro's number. Unit: at/mol
    const double Na_;

};