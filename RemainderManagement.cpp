#include <fstream>
#include "RemainderManagement.h"

void RemainderManagement::printRemainders(Json::Value remainders) {
    cout << "Remainders: " << endl;
    for (Json::Value::ArrayIndex i = 0; i != remainders.size(); i++) {
        cout << remainders[i]["summary"] << " || " << remainders[i]["date"] << " || "
            << remainders[i]["time"] << remainders[i]["recurrence"] << endl;
    }
}

void RemainderManagement::printRemaindersInMap(unordered_map<int, Json::Value> remaindersMap) {
    cout << "Remainder Id || Remainder, Date, Time" << endl;

    for (auto& remainder : remaindersMap)
    {
        cout << remainder.first << " || " << remainder.second["summary"] << ", " << remainder.second["date"] << ", "
            << remainder.second["time"] << ", " << remainder.second["recurrence"] << endl;
    }
}

ifstream RemainderManagement::getAFileToRead() {
    ifstream fileToWorkWith;

    fileToWorkWith.open(FILE_NAME, std::fstream::in);

    // If file does not exist, Create new file
    if (!fileToWorkWith)
    {
        cout << "Cannot open file, file does not exist. Creating new file..";
        fileToWorkWith.open(FILE_NAME, fstream::in | fstream::trunc);
    }
    else
    {    // use existing file
        cout << "success " << FILE_NAME << " found. \n";
    }

    return fileToWorkWith;
}

ofstream RemainderManagement::getAFileToWrite() {
    ofstream fileToWorkWith;
    fileToWorkWith.open(FILE_NAME, std::fstream::out | std::ios::trunc);

    return fileToWorkWith;
}

string RemainderManagement::getUUID(size_t len) {
    static const char x[] = "0123456789abcdef";

    string uuid;
    uuid.reserve(len);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution < > dis(0, sizeof(x) - 2);
    for (size_t i = 0; i < len; i++)
        uuid += x[dis(gen)];

    return uuid;
}

string RemainderManagement::readAFile(ifstream file) {
    std::string content((std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>()));
    file.close();

    return content;
}

void RemainderManagement::writeInAFile(ofstream file, Json::Value content) {
    Json::StyledWriter writer;

    file << writer.write(content);
    file.close();
}

void RemainderManagement::addRemainder(string summary, string description, string date, string time, string recur) {
    Json::Reader reader;
    Json::Value root;
    Json::Value newValue;

    newValue["id"] = getUUID(32);
    newValue["summary"] = summary;
    newValue["description"] = description;
    newValue["date"] = date;
    newValue["time"] = time;
    newValue["recurrence"] = recur;

    string content = readAFile(getAFileToRead());
    // check if file is empty
    
    if (content.size() == 0) {
        root["remainders"] = Json::Value(Json::arrayValue);
    }
    else {
        if (!reader.parse(content, root)) {
            cout << "Error parsing the JSON file" << endl;
        }
    }

    root["remainders"].append(newValue);
    // write the changes
    writeInAFile(getAFileToWrite(), root);
}

Json::Value RemainderManagement::readRemainder() {
    string content = readAFile(getAFileToRead());
    Json::Reader reader;
    Json::Value root;

    reader.parse(content, root);
    
    return root["remainders"];
}

unordered_map<int, Json::Value> RemainderManagement::getRemaindersInMap(Json::Value remainders) {
    unordered_map<int, Json::Value> remaindersMap;

    for (Json::Value::ArrayIndex i = 0; i != remainders.size(); i++) {
        remaindersMap.insert(make_pair(i + 1, remainders[i]));
    }

    return remaindersMap;
}

void RemainderManagement::writeRemaindersFromMap(unordered_map<int, Json::Value> remaindersMap) {
    Json::Value root;

    root["remainders"] = Json::Value(Json::arrayValue);
    for (auto& remainder : remaindersMap)
    {
        root["remainders"].append(remainder.second);
    }
    // write the changes
    writeInAFile(getAFileToWrite(), root);
}

Json::Value RemainderManagement::getLatestRemainder() {
    Json::Value remainders = readRemainder();
    if (remainders.size() <= 0) return NULL;

    Json::Value latest = remainders[0];
    for (Json::Value::ArrayIndex i = 1; i != remainders.size(); i++) {
        int minDate = strcmp(latest["date"].asCString(), remainders[i]["date"].asCString());
        if (minDate == -1) {
            continue;
        }
        if (minDate == 0) {
            int minTime = strcmp(latest["time"].asCString(), remainders[i]["time"].asCString());

            if (minTime == -1 || minTime == 0) {
                continue;
            }
        }

        latest = remainders[i];
    }

    return latest;
}

void RemainderManagement::deleteOrModifyById(string id) {
    unordered_map<int, Json::Value> remaindersMap = getRemaindersInMap(readRemainder());

    for (auto& remainder : remaindersMap)
    {
        if (strcmp(remainder.second["id"].asCString(), id.c_str()) == 0) {
            remaindersMap.erase(remainder.first);

            // to delete or make changes in the recurrence remainders
            /*
            if (strcmp(remainder.second["recurrence"].asCString(), "ONCE") == 0) {
                
            }
            else {
                string recurrence = remainder.second["recurrence"].asCString();

                if (strcmp(recurrence.c_str(), "HOURLY") == 0) {

                }
                else if (strcmp(recurrence.c_str(), "DAILY") == 0) {

                }
                else if (strcmp(recurrence.c_str(), "WEEKLY") == 0) {

                }
                else if (strcmp(recurrence.c_str(), "MONTHLY") == 0) {

                }
                else if (strcmp(recurrence.c_str(), "YEARLY") == 0) {

                }
            }
            */
        }
    }

    // write in remainders.json file
    writeRemaindersFromMap(remaindersMap);
}

