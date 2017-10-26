#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include "celestialbody.h"
#include <vector>
#include <string>
#include <fstream>

class SolarSystem
{
public:
    SolarSystem();
    // Speed of light in AU/Year
   const double speedOfLightAU_Year   = 63239.7263;
    //const double mercury_perihelion_AU  = 0.3075;

    void createCelestialBody(vec3 position, vec3 velocity, double mass, std::string objectName);
    void calculateForcesAndEnergy();
    void calculateRelativisticForcesAndEnergy();
    vec3 calculateAngularMomentum(CelestialBody &body);
    double relativisticCorrectionFactor(double distance_square, double norm_square_angular_mass,double lightspeedsquared);
    double calculatePerihelionAngle(double current_distance_AU,vec3 current_solar_position);
    int numberOfBodies() const;

    double totalEnergy() const;
    double potentialEnergy() const;
    double kineticEnergy() const;
    vec3 angularMomentum() const;
    double perihelionAngle() const;
    void writeToFile(std::string filename);
    std::vector<CelestialBody> &bodies();

private:
    std::vector<CelestialBody> m_bodies;
    std::ofstream m_file;
    vec3 perihelionRange = vec3(1.0e10,1.0e10,1.0e10);
    double m_kineticEnergy;
    double m_potentialEnergy;
    double m_perihelionAngle;
    vec3 m_ang_mom;
};

#endif // SOLARSYSTEM_H
