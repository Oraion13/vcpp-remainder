#include <fstream>
#include "RemainderManagement.h"

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

void RemainderManagement::addRemainder(string summary, string description, string date, string time, string recur) {
    Json::Reader reader;
    Json::Value root;
    Json::Value newValue;

    Json::StyledWriter writer;

    newValue["id"] = getUUID(32);
    newValue["summary"] = summary;
    newValue["description"] = description;
    newValue["date"] = date;
    newValue["time"] = time;
    newValue["recur"] = recur;

    ifstream fileToWorkWithRead = getAFileToRead();
    std::string content((std::istreambuf_iterator<char>(fileToWorkWithRead)),
        (std::istreambuf_iterator<char>()));
    fileToWorkWithRead.close();
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
    ofstream fileToWorkWithWrite = getAFileToWrite();
    fileToWorkWithWrite << writer.write(root);
    fileToWorkWithWrite.close();
}