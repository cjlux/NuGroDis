#include <string>

class ThermalLoading
{
public:
    
    ThermalLoading();

    ~ThermalLoading();
    
    //Read the File giving the radius distribution of precipitates (NP1, NP2, ...
    void ReadDistributionFile(std::string);
    
    //Read the values for the temperature loading 
    //(equivalent to read TK1 and set time and temperature attributes)
    void ReadTemperatureLoading(std::string);
    
    //Read Vacancies concentration after hardening (maturation or tempering)
    void ReadVCAH(std::string);


protected:

private:

    //Thermal loading duration (was tmax - tmat). Unit: s
    double duration_;
    
    //Time vector of temperature loading. Unit: s
    double * time_;
    
    //Temperature vector of ThermalLoading. Unit: K
    double * temperature_;

};
