#include "nuis/eventinput/INormalizedEventSource.h"

#include "NuHepMC/EventUtils.hxx"
#include "NuHepMC/FATXUtils.hxx"

#include "HepMC3/GenParticle.h"

#include "spdlog/spdlog.h"

namespace nuis {

std::optional<EventCVWeightPair>
INormalizedEventSource::process(std::optional<HepMC3::GenEvent> ev) {
  if (!ev) {
    return std::optional<EventCVWeightPair>();
  }
#ifndef NUIS_NDEBUG
  if (NuHepMC::Event::ToMeVFactor(ev.value()) != 1) {
    spdlog::critical(
        "[INormalizedEventSource]: Processing event not in MeV. This breaks "
        "a critical contract to users, fix the underlying IEventSource.");
    abort();
  }

  auto fsprot =
      NuHepMC::Event::GetParticles_AllRealFinalState(ev.value(), {
                                                                     2212,
                                                                 });
  if (fsprot.size()) {
    if (fsprot.front()->momentum().m() < 10) {
      spdlog::critical(
          "[INormalizedEventSource]: Processing event with a real final "
          "state proton with a reported mass of {} MeV, the units look "
          "incorrectly set. This breaks "
          "a critical contract to users, fix the underlying IEventSource.");
      abort();
    }
  }
#endif

  double cvw = xs_acc->process(ev.value());
  return EventCVWeightPair{std::move(ev.value()), cvw};
}

INormalizedEventSource::INormalizedEventSource(
    std::shared_ptr<IEventSource> evs)
    : IEventSourceWrapper(evs) {}

std::optional<EventCVWeightPair> INormalizedEventSource::first() {
  if (!wrapped_ev_source) {
    return std::optional<EventCVWeightPair>();
  }

  try {
    xs_acc = NuHepMC::FATX::MakeAccumulator(
        wrapped_ev_source->first().value().run_info());
  } catch (NuHepMC::except const &ex) {
    return std::optional<EventCVWeightPair>();
  }
  return process(wrapped_ev_source->first());
}

std::optional<EventCVWeightPair> INormalizedEventSource::next() {
  return process(wrapped_ev_source->next());
}

NormInfo INormalizedEventSource::norm_info() {
  return {xs_acc->fatx(), xs_acc->sumweights(), xs_acc->events()};
}

INormalizedEventSource::~INormalizedEventSource() {}

} // namespace nuis
