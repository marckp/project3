#include "solarsystem.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <string>
using namespace std;

SolarSystem::SolarSystem() :
    m_kineticEnergy(0),
    m_potentialEnergy(0)
{
}

void SolarSystem::createCelestialBody(vec3 position, vec3 velocity, double mass, std::string objectName) {
    m_bodies.push_back( CelestialBody(position, velocity, mass, objectName) );
}

void SolarSystem::calculateForcesAndEnergy()
{
    m_kineticEnergy = 0;
    m_potentialEnergy = 0;
    m_ang_mom.zeros();

    for(CelestialBody &body : m_bodies) {
        // Reset forces on all bodies
        body.force.zeros();
    }

    for(int i=0; i<numberOfBodies(); i++) {
        CelestialBody &body1 = m_bodies[i];

        //calculate angular momentum
        body1.angularMomentum   = calculateAngularMomentum(body1);

        m_ang_mom += body1.angularMomentum;

        for(int j=i+1; j<numberOfBodies(); j++) {
            CelestialBody &body2        = m_bodies[j];
            vec3 deltaRVector           = body1.position - body2.position;
            double dr                   = deltaRVector.length();
            double G                    = 4*pow(M_PI,2);
            vec3 tempForce              = G*body1.mass * body2.mass * deltaRVector/pow(dr,3);
            body1.force                 -= tempForce;
            body2.force                 += tempForce;
            m_potentialEnergy           += -G*body1.mass*body2.mass/dr;

        }

        m_kineticEnergy += 0.5*body1.mass*body1.velocity.lengthSquared();
    }
}

//Calculate Relativistic Force, Energy, and displacement
void SolarSystem::calculateRelativisticForcesAndEnergy()
{
    m_kineticEnergy        = 0;
    m_potentialEnergy      = 0;
    m_ang_mom.zeros();
    CelestialBody &Sun     = m_bodies[0];
    CelestialBody &Mercury = m_bodies[1];
    cout << "Mercury mass: " << Mercury.mass << endl;

    for(CelestialBody &body : m_bodies)
    {
        // Reset forces on all bodies
        body.force.zeros();
    }

    //calculate angular momentum
    Mercury.angularMomentum     = calculateAngularMomentum(Mercury);
    m_ang_mom                   = Mercury.angularMomentum;
    double dr                   = Mercury.position.length();
    double theta                = calculatePerihelionAngle(dr,Mercury.position);
    double G                    = 4 * pow(M_PI,2);
    vec3 tempForce              = G * Sun.mass * Mercury.mass * Mercury.position/pow(dr,3);
    relativisticCorrectionFactor(dr*dr,m_ang_mom.lengthSquared()/pow(Mercury.mass,2),speedOfLightAU_Year * speedOfLightAU_Year);
    Mercury.force               = tempForce;
    m_potentialEnergy           = -G * Sun.mass * Mercury.mass/dr;
    m_kineticEnergy             = 0.5 * Mercury.mass * Mercury.velocity.lengthSquared();
}

//Calculate Periheilion Angle for Mercury
double SolarSystem::calculatePerihelionAngle(double current_distance_AU,vec3 current_solar_position)
{
   double mercury_perihelion_angle = -M_PI/2.0;
   if (current_distance_AU <= 0.30750)
   {
       mercury_perihelion_angle += atan2(current_solar_position.x(),current_solar_position.y());
       cout << "Perihelion angle and position: " << setprecision(10) << mercury_perihelion_angle  << " 0.3075 " << " " << setprecision(10) << current_distance_AU << " " << setprecision(10) << endl;
   }
   return mercury_perihelion_angle;
}

int SolarSystem::numberOfBodies() const
{
    return m_bodies.size();
}

double SolarSystem::totalEnergy() const
{
    return m_kineticEnergy + m_potentialEnergy;
}

double SolarSystem::potentialEnergy() const
{
    return m_potentialEnergy;
}

double SolarSystem::kineticEnergy() const
{
    return m_kineticEnergy;
}
double SolarSystem::perihelionAngle() const
{
    return m_perihelionAngle;
}

vec3 SolarSystem::angularMomentum() const
{
    return m_ang_mom;
}

vec3 SolarSystem::calculateAngularMomentum(CelestialBody &body)
{
    vec3 pos                = body.position;
    vec3 vel                = body.velocity;
    double m                = body.mass;
    vec3 angularMomentum  = pos.cross(vel)*m;
    return angularMomentum;
}

double SolarSystem::relativisticCorrectionFactor(double distance_square, double norm_square_angular_mass,double lightspeedsquared)
{
    double RCF = 1.0 + (3.0 * norm_square_angular_mass)/(lightspeedsquared*distance_square);
    return RCF;
}

void SolarSystem::writeToFile(string filename)
{
    if(!m_file.good()) {
        m_file.open(filename.c_str(), ofstream::out);
        if(!m_file.good()) {
            cout << "Error opening file " << filename << ". Aborting!" << endl;
            terminate();
        }
    }

    for(CelestialBody &body : m_bodies) {
        m_file << setprecision(10) << body.position.x() << " " << setprecision(10) << body.position.y() << " " << setprecision(10) << body.position.z()<< " ";
    }
    m_file << "\n";
}

std::vector<CelestialBody> &SolarSystem::bodies()
{
    return m_bodies;
}
