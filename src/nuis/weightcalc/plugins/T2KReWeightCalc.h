#pragma once

#include "nuis/weightcalc/plugins/IWeightCalcPlugin.h"
#include "nuis/eventinput/plugins/neutvectEventSource.h"

#include "boost/dll/alias.hpp"

#include "yaml-cpp/yaml.h"

#include <filesystem>

namespace t2krew {
  class T2KReWeight;
}

namespace nuis {

class T2KReWeightCalc : public IWeightCalcPlugin {
public:

  std::unique_ptr<t2krew::T2KReWeight> fT2KRW;
  std::shared_ptr<neutvectEventSource> nevs;

  double calc_weight(HepMC3::GenEvent const &ev);
  void set_parameters(std::map<std::string, double> const &params);
  bool good() const;

  T2KReWeightCalc(IEventSourcePtr evs, YAML::Node const &cfg);
  

  virtual ~T2KReWeightCalc();
};

} // namespace nuis
