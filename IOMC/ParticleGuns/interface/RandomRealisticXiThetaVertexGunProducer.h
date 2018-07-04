/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *
 ****************************************************************************/

#ifndef RandomRealisticXiThetaVertexGunProducer_H
#define RandomRealisticXiThetaVertexGunProducer_H

#include "FWCore/Framework/interface/one/EDProducer.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandGeneral.h"

namespace edm {
  
class RandomRealisticXiThetaVertexGunProducer : public one::EDProducer<>
{

  public:
    RandomRealisticXiThetaVertexGunProducer(const ParameterSet &);

    virtual ~RandomRealisticXiThetaVertexGunProducer();

  private:
    virtual void produce(Event & e, const EventSetup& es) override;
    
    void GenerateParticle(double z_sign, double mass, unsigned int barcode, CLHEP::HepRandomEngine* engine, HepMC::GenVertex *vtx) const;

    unsigned int verbosity;

    unsigned int particleId;

    double energy;
    double xi_min, xi_max;
    double theta_x_mean, theta_x_sigma;
    double theta_y_mean, theta_y_sigma;
    double vertex_x_mean, vertex_x_sigma;
    double vertex_y_mean, vertex_y_sigma;

    unsigned int nParticlesSector45;
    unsigned int nParticlesSector56;

    int nBinsProbability;
    double *xiProbabilityPoints;
    // CLHEP::RandGeneral randGeneralXi;
};

} 

#endif
