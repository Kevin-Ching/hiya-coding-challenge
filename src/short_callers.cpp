#include "short_callers.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <getopt.h>

using namespace std;

int cmd_line_error(string command) {
    cerr << "Incorrect command. Usage: " << command << " [[-a]] [filename]" << endl;
    return 1;
}

int input_error() {
    cerr << "Input error." << endl;
    return 1;
}

int call_error(string name) {
    cerr << "Call error. " << name << " is already on a call." << endl;
    return 1;
}

int hangup_error(string name) {
    cerr << "Hangup error. " << name << " is not on a call." << endl;
    return 1;
}

int main(int argc, char** argv) {
    // Default: do not show all callers' average time
    bool show_all = false;

    // Variables for command line argument parsing
    int opt;
    static struct option long_options[] =
    {
        {"show all", no_argument, 0, 'a'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "a", long_options, NULL)) != -1) {
        switch (opt) {
            case 'a':
                show_all = true;
                break;
            default:
                return cmd_line_error(argv[0]);
        }
    }

    if ((argc - optind) != 1) {
        return cmd_line_error(argv[0]);
    }

    ifstream infile(argv[optind]);
    if (!infile.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    /* Ongoing calls */
    map<string, CallEvent> ongoing_calls;

    /* List of call times by caller */
    map<string, vector<long>> call_times;
    
    string line;
    size_t pos;
    long last_timestamp = -1;
    // Regex pattern with capture groups to match and extract each part
    regex pattern(R"(\s*(\w+)\s+(\w+)\s+(\w+)\s+(\d+)\s*)");
    smatch matches;

    while(getline(infile, line)) {
        if (line.empty()) {
            continue;
        }
        // Strip comments that start with "//"
        pos = line.find("//");
        line = line.substr(0, pos);
        
        if (regex_match(line, matches, pattern)) {
            CallEvent event(matches);
            bool is_call;
            
            // First match
            if (matches[1].compare("call") == 0) {
                is_call = true;
            } else if (matches[1].compare("hangup") == 0) {
                is_call = false;
            } else {
                return input_error();
            }

            // Check timestamp
            if (event.timestamp < last_timestamp) {
                cout << "Not time-ordered error. Timestamp " << event.timestamp << " appears after " << last_timestamp  << "." << endl;
                return 1;
            } else {
                last_timestamp = event.timestamp;
            }

            if (is_call) {
                if (ongoing_calls.count(event.from) != 0) {
                    return call_error(event.from);
                }
                if (ongoing_calls.count(event.to) != 0) {
                    return call_error(event.to);
                }
                
                ongoing_calls[event.from] = event;
                ongoing_calls[event.to] = event;
            } else {
                if (ongoing_calls.count(event.from) == 0) {
                    return hangup_error(event.from);
                }
                if (ongoing_calls.count(event.to) == 0) {
                    return hangup_error(event.to);
                }

                // Check we are hanging up with the correct person
                CallEvent call_event = ongoing_calls[event.from];
                if (!call_event.isEqual(ongoing_calls[event.to])) {
                    cout << "Hangup error. " << event.from << " cannot hang up on " << event.to 
                        << " since they are not calling each other." << endl;
                    return 1;
                }

                // Record call time
                call_times[call_event.from].push_back(event.timestamp - call_event.timestamp);

                ongoing_calls.erase(event.from);
                ongoing_calls.erase(event.to);
            }
        } else {
            return input_error();
        }
    }

    infile.close();

    /* List of callers with an average call duration under 5 seconds */
    vector<string> short_callers;
    
    // Compute average call times
    for (const auto& pair : call_times) {
        string caller = pair.first;
        vector<long> times = pair.second;

        long sum = accumulate(times.begin(), times.end(), 0);
        double average = static_cast<double>(sum) / times.size();    // Size should not be 0

        if (show_all) {
            cout << "Caller: " << caller << ",\tAverage: " << average << endl;
        }
        if (average < 5) {
            short_callers.push_back(caller);
        }
    }

    if (show_all) {
        cout << endl;
    }

    // Print list of short_callers
    cout << "[";

    for (int i = 0; i < short_callers.size(); i++) {
        cout << short_callers[i];
        if (i != (short_callers.size() - 1)) {
            cout << ", ";
        }
    }

    cout << "]" << endl;

    return 0;
}