/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *
 ****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ValidityInterval.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"

#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSRPAlignmentCorrectionsDataSequence.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSRPAlignmentCorrectionsMethods.h"

#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"

#include "Alignment/CTPPSExport/interface/alignment_classes.h"
#include "Alignment/CTPPSExport/interface/fill_info.h"

//----------------------------------------------------------------------------------------------------

class CTPPSExportAlignment : public edm::stream::EDAnalyzer<>
{
  public:
    explicit CTPPSExportAlignment(const edm::ParameterSet&);
    ~CTPPSExportAlignment() {}

    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    void doExport(const CTPPSGeometry &geometry) const;

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    std::vector<std::string> inputFiles;
    std::string outputFile;

    bool done;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

CTPPSExportAlignment::CTPPSExportAlignment(const edm::ParameterSet& iConfig) :
  inputFiles(iConfig.getParameter<vector<string>>("inputFiles")),
  outputFile(iConfig.getParameter<string>("outputFile")),
  done(false)
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSExportAlignment::analyze(const edm::Event&, const edm::EventSetup &iSetup)
{
  if (done)
    return;

  ESHandle<CTPPSGeometry> geometry;
  iSetup.get<VeryForwardRealGeometryRecord>().get(geometry);

  doExport(*geometry);

  done = true;
}

//----------------------------------------------------------------------------------------------------

void CTPPSExportAlignment::doExport(const CTPPSGeometry &geometry) const
{
  // load input
  AlignmentResultsCollection input;
  for (const auto &f : inputFiles)
  {
    FileInPath fip(f);
    if (input.Load(fip.fullPath()) != 0)
      throw cms::Exception("CTPPSExportAlignment") << "Cannot load alignment from file '" << f << "'.";
  }

  // load fill-alignment mapping
  InitFillInfoCollection();

  // export alignment
  CTPPSRPAlignmentCorrectionsDataSequence alSeq;
  for (const auto &fi : fillInfoCollection)
  {
    printf("* fill %u\n", fi.fillNumber);

    const IOVSyncValue beg(EventID(fi.runMin, 1, 1));
    const IOVSyncValue end(EventID(fi.runMax, EventID::maxLuminosityBlockNumber(), EventID::maxEventNumber()));
    edm::ValidityInterval iov(beg, end);

    const auto &alInput = input[fi.alignmentTag];

    // stop if no corrections given for the fill
    if (alInput.empty())
      continue;

    CTPPSRPAlignmentCorrectionsData alData;
    for (const auto &p : alInput)
    {
      const unsigned int rpDecId = p.first;
      const unsigned int arm = rpDecId / 100;
      const unsigned int station = (rpDecId / 10) % 10;
      const unsigned int rp = rpDecId % 10;

      unsigned int subDetector = CTPPSDetId::sdTrackingStrip;
      if (station == 0 && rp == 2)
        subDetector = CTPPSDetId::sdTrackingStrip;

      if (station == 0 && rp == 3)
      {
        if (fi.fillNumber < 6579)
          subDetector = CTPPSDetId::sdTrackingStrip;  // 2016 and 2017
        else
          subDetector = CTPPSDetId::sdTrackingPixel;  // 2018
      }

      if (station == 2 && rp == 3)
        subDetector = CTPPSDetId::sdTrackingPixel;

      CTPPSDetId rpId(subDetector, arm, station, rp);

      const AlignmentResult &ar = p.second;

      CTPPSRPAlignmentCorrectionData rpCorrection(
        ar.sh_x, ar.sh_x_unc,
        -ar.sh_y, ar.sh_y_unc,
        0., 0.,
        ar.rot_x, ar.rot_x_unc,
        ar.rot_y, ar.rot_y_unc,
        -ar.rot_z, ar.rot_z_unc
      );

      // extras for 2016: fill from 4947 to 5288
      if (rpDecId == 2 && fi.fillNumber >= 4947 && fi.fillNumber <= 5288)
      {
        for (unsigned int plane = 0; plane < 10; ++plane)
        {
          TotemRPDetId planeId(arm, station, rp, plane);

          auto c = geometry.getSensorTranslation(planeId);
          printf("  plane = %u, c_x = %.3f, c_y = %.3f, c_z = %.3f\n", plane, c.x(), c.y(), c.z());

          ROOT::Math::RotationZYX R(-ar.rot_z, ar.rot_y, ar.rot_x);
          ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>> cm(c.x(), c.y(), c.z());
          auto de = R * cm - cm;

          CTPPSRPAlignmentCorrectionData sensorCorrection(
            de.x(), 0., de.y(), 0., de.z(), 0.,
            0., 0., 0., 0., 0., 0.
          );

          alData.setSensorCorrection(planeId, sensorCorrection);
        }
      }

      // extras for 2017 post-TS2: add correction for RP inversion
      if (station == 0 && fi.fillNumber >= 6230 && fi.fillNumber <= 6417)
      {
        const double rot_x = M_PI;
        const double rot_y = 0.;
        const double rot_z = -2. * 8. / 180. * M_PI;

        rpCorrection.setRotX(rot_x);
        rpCorrection.setRotY(rot_y);
        rpCorrection.setRotZ(rot_z);

        ROOT::Math::RotationZYX R(rot_z, rot_y, rot_x);

        TotemRPDetId planeId0(arm, station, rp, 0);
        TotemRPDetId planeId1(arm, station, rp, 1);

        auto c0 = geometry.getSensorTranslation(planeId0);
        auto c1 = geometry.getSensorTranslation(planeId1);

        const double ref_x = (c0.x() + c1.x()) / 2.;
        const double ref_y = (c0.y() + c1.y()) / 2.;

        printf("    ref_x = %.3f, ref_y = %.3f\n", ref_x, ref_y);

        for (unsigned int plane = 0; plane < 10; ++plane)
        {
          TotemRPDetId planeId(arm, station, rp, plane);

          auto c = geometry.getSensorTranslation(planeId);

          c.setX(c.x() - ref_x);

          c.setY(c.y() - ref_y);

          if (c.z() > 0)
           c.setZ(c.z() - 212550.);
          else
           c.setZ(c.z() + 212550.);

          printf("  plane = %u, c_x = %.3f, c_y = %.3f, c_z = %.3f\n", plane, c.x(), c.y(), c.z());

          ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>> cm(c.x(), c.y(), c.z());
          auto de = R * cm - cm;

          printf("    de.z = %.3f\n", de.z());

          CTPPSRPAlignmentCorrectionData sensorCorrection(
            de.x(), 0., de.y(), 0., de.z(), 0.,
            0., 0., 0., 0., 0., 0.
          );

          alData.setSensorCorrection(planeId, sensorCorrection);
        }
      }

      // extras for 2018: fill from 6583 to 6660
      if (rpDecId == 103 && fi.fillNumber >= 6583 && fi.fillNumber <= 6660)
      {
        for (unsigned int plane = 0; plane < 6; ++plane)
        {
          CTPPSPixelDetId planeId(arm, station, rp, plane);

          auto c = geometry.getSensorTranslation(planeId);
          printf("  plane = %u\n", plane);
          printf("    centre: x = %.3f, y = %.3f, z = %.3f\n", c.x(), c.y(), c.z());

          ROOT::Math::RotationZYX R(-ar.rot_z, ar.rot_y, ar.rot_x);
          ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>> cm(
            c.x() + ar.sh_x,
            c.y() - ar.sh_y,
            c.z()
          );
          printf("    c - B: x = %.3f, y = %.3f, z = %.3f\n", cm.x(), cm.y(), cm.z());

          auto de = R * cm - cm;
          printf("    de: x = %.3f, y = %.3f, z = %.3f\n", de.x(), de.y(), de.z());

          printf("\n");

          CTPPSRPAlignmentCorrectionData sensorCorrection(
            de.x(), 0., de.y(), 0., de.z(), 0.,
            0., 0., 0., 0., 0., 0.
          );

          alData.setSensorCorrection(planeId, sensorCorrection);
        }
      }

      alData.setRPCorrection(rpId, rpCorrection);
    }

    alSeq.insert(iov, alData);
  }

  // save output
  CTPPSRPAlignmentCorrectionsMethods::writeToXML(alSeq, outputFile,
    false, false, true, true, true, true);
}

//----------------------------------------------------------------------------------------------------

void CTPPSExportAlignment::fillDescriptions(ConfigurationDescriptions& descriptions)
{
  ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CTPPSExportAlignment);
