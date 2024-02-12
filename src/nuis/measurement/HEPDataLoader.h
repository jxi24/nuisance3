// Copyright 2016-2021 L. Pickering, P Stowell, R. Terri, C. Wilkinson, C. Wret

/*******************************************************************************
 *    This file is part of NUISANCE.
 *
 *    NUISANCE is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    NUISANCE is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with NUISANCE.  If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************/
#pragma once
#include <vector>
#include <string>
#include <memory>

#include "yaml-cpp/yaml.h"
#include "HepMC3/GenEvent.h"
#include "ProSelecta/ProSelecta.h"

#include "nuis/measurement/Record.h"
#include "nuis/measurement/Variables.h"
#include "nuis/measurement/Document.h"
#include "nuis/measurement/MeasurementLoader.h"

namespace nuis {
namespace measurement {

using RecordPtr = std::shared_ptr<nuis::measurement::Record>;

class HEPDataLoader : public MeasurementLoader {
public:
  HEPDataLoader() {}

  explicit HEPDataLoader(YAML::Node config);

  virtual ~HEPDataLoader() {}

  std::vector<double> ProjectEvent(const HepMC3::GenEvent& event);

  bool FilterEvent(const HepMC3::GenEvent& event);

  double WeightEvent(const HepMC3::GenEvent& event);

  Record CreateRecord(const std::string label = "MC");

  bool FillRecordFromEvent(RecordPtr h,
    const HepMC3::GenEvent& event, const double weight);

  bool FillRecordFromProj(RecordPtr h,
    const std::vector<double>& val, const double weight);

  void FinalizeRecord(RecordPtr h, double scaling);
};

}  // namespace measurement
}  // namespace nuis
