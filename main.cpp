#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <iomanip>
#include <sstream>

using namespace std;

class Date {
public:
    Date(int year, int month, int day) {
        this->year = year;

        if (month < 1 || month > 12) {
            throw logic_error("Invalid month value: " + to_string(month));
        }
        this->month = month;

        if (day < 1 || day > 31) {
            throw logic_error("Invalid day value: " + to_string(day));
        }
        this->day = day;
    }

    int GetYear() const {
        return year;
    }

    int GetMonth() const {
        return month;
    }

    int GetDay() const {
        return day;
    }

private:
    int year;
    int month;
    int day;
};

bool operator<(const Date &lhs, const Date &rhs) {
    return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} < vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

ostream &operator<<(ostream &stream, const Date &date) {
    stream << setw(4) << setfill('0') << date.GetYear() << "-" << setw(2) << setfill('0') << date.GetMonth() <<
        "-" << setw(2) << setfill('0') << date.GetDay();
    return stream;
}

ostream &operator<<(ostream &stream, const set<string> &elements) {
    for (const auto &element : elements) {
        stream << element << " ";
    }
    return stream;
}

class Database {
public:
    void AddEvent(const Date &date, const string &event) {
        data[date].insert(event);
    }

    bool DeleteEvent(const Date &date, const string &event) {
        if (data[date].find(event) != data[date].end()) {
            data[date].erase(event);
            return true;
        }
        return false;
    }

    int DeleteDate(const Date &date) {
        if (data.find(date) != data.end()) {
            int amount = data[date].size();
            data.erase(date);
            return amount;
        }
        return 0;
    }

    set<string> Find(const Date &date) const {
        if (data.find(date) != data.end()) {
            return data.at(date);
        }
        return {};
    }

    void Print() {
        for (const auto &item : data) {
            for (const auto &event : item.second) {
                cout << item.first << " " << event << "\n";
            }
        }
    }

private:
    map<Date, set<string>> data;
};

Date ParseDate(const string &date_str) {
    stringstream input(date_str);
    int year, month, day;
    bool ok = true;

    ok = ok && (input >> year);
    ok = ok && (input.peek() == '-');
    input.ignore(1);

    ok = ok && (input >> month);
    ok = ok && (input.peek() == '-');
    input.ignore(1);

    ok = ok && (input >> day);
    ok = ok && (input.eof());

    if (ok) {
        return {year, month, day};
    }
    throw logic_error("Invalid date format " + date_str);
}

int main() {
    try {
        Database db;
        string command_line;

        while (getline(cin, command_line)) {
            stringstream stream(command_line);
            string command;

            stream >> command;

            if (command == "Add") {
                string date_str, event;
                stream >> date_str >> event;

                const Date date = ParseDate(date_str);
                db.AddEvent(date, event);
            } else if (command == "Del") {
                string date_str, event;
                stream >> date_str;

                if (!stream.eof()) {
                    stream >> event;
                }

                const Date date = ParseDate(date_str);

                if (!event.empty()) {
                    if (db.DeleteEvent(date, event)) {
                        cout << "Successfully deleted\n";
                    } else {
                        cout << "Event doesn't exist\n";
                    }
                } else {
                    int amount = db.DeleteDate(date);
                    cout << "Deleted " << amount << "events\n";
                }
            } else if (command == "Find") {
                string date_str;
                stream >> date_str;

                const Date date = ParseDate(date_str);
                const set<string> events = db.Find(date);

                if (events.empty()) {
                    cout << "No events for " << date << "\n";
                } else {
                    cout << events << "\n";
                }
            } else if (command == "Print") {
                db.Print();
            } else if (!command.empty()) {
                throw logic_error("Command not found: " + command);
            }
        }
    } catch (const exception &ex) {
        cout << ex.what() << "\n";
    }
    return 0;
}