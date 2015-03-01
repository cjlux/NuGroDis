class Material
{

public:
    Material();
    ~Material();

    void ReadData(std::string dataFileName);

protected:

private:
    
        //The temperature of the material. Unit : K
        double temperature_;
};