class GuinierPreston
{

public:
    GuinierPreston();
    ~GuinierPreston();
    
    //Compute the number of nucleation sites using RadiusDistribution::ComputeTotNbOfItems
    void ComputeNucleationSiteNb();
    
    //Compute the volumic fraction
    void ComputeVolumicFraction();
    
    void ReadDataValue(std::string fileName);


protected:

private:

};