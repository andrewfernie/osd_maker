#include "events.hpp"
#if 0
DEFINE_EVENT_TYPE(wxEvent_AircraftPositionChanged)
DEFINE_EVENT_TYPE(wxEvent_RemoteDistanceChanged)
DEFINE_EVENT_TYPE(wxEvent_RemoteBearingChanged)
DEFINE_EVENT_TYPE(wxEvent_RemoteElevationChanged)
#endif
wxEvent* QuanTrackerSimEvent ::Clone() const 
{ return new QuanTrackerSimEvent(*this);}