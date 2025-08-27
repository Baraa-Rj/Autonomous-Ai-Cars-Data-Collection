#include pragma once 

class Data{
    protected:
        std::DateTime timestamp;

    public:
        Data(std::DateTime timestamp);
        virtual ~Data();
      std::DateTime getTimestamp() const;

}