#include <string>
#include <regex>

using namespace std;

class CallEvent {
public:
    string from = "";   // the person originating the event
    string to = "";     // the person receiving the event
    long timestamp = -1;     // seconds since the beginning of the Unix epoch

    CallEvent() {}

    CallEvent(smatch matches) {
        from = matches[2];
        to = matches[3];
        timestamp = stol(matches[4]);
    }

    bool isEqual(CallEvent other) {
        bool from_equal = from.compare(other.from) == 0;
        bool to_equal = to.compare(other.to) == 0;
        bool timestamp_equal = timestamp == other.timestamp;
        return from_equal && to_equal && timestamp_equal;
    }
};
