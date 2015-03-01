class Polynomial
{

public:

    //the constructor takes 2 arguments: degree and coefficients to build the polynomial
    Polynomial(int degree, double * coefs);
    
    ~Polynomial();
    
    //Gives P(x)
    void ComputeValue(double x);


protected:

private:
    const int degree_;
    double * coefs_;

};