#include "CondFormats/Serialization/interface/Test.h"

#include "../src/headers.h"

int main() {
  testSerialization<PPSAlignmentConfig::PointErrors>();
  testSerialization<PPSAlignmentConfig::SelectionRange>();
  testSerialization<PPSAlignmentConfig::RPConfig>();
  testSerialization<PPSAlignmentConfig::SectorConfig>();
  testSerialization<PPSAlignmentConfig::Binning>();

  testSerialization<std::vector<PPSAlignmentConfig::PointErrors>>();
  testSerialization<std::map<unsigned int, std::vector<PPSAlignmentConfig::PointErrors>>>();
  testSerialization<std::map<unsigned int, PPSAlignmentConfig::SelectionRange>>();

  testSerialization<PPSAlignmentConfig>();
}