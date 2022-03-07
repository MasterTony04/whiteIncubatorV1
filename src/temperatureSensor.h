class TemperatureSensor{
private:
    int sensorReading;
    float R1 = 10000;
    float logR2, R2, T;
    float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

public:

    float getTemperature(int sensorValue){

        sensorReading = sensorValue;

        R2 = R1 * (1023.0 / (float)sensorReading - 1.0);
        logR2 = log(R2);
        T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
        T = T - 273.15;
        return T;
    }

};

TemperatureSensor ntc;