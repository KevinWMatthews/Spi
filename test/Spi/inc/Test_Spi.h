#ifndef Test_Spi_H_
#define Test_Spi_H_

class Test_Spi
{
  public:
    explicit Test_Spi();
    virtual ~Test_Spi();

  private:
    Test_Spi(const Test_Spi&);
    Test_Spi& operator=(const Test_Spi&);
};

#endif
