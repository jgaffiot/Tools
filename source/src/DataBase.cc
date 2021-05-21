// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//
/** Inspired by DCparam, the API for the Double Chooz simple "database"
    of numeric and string values, written by Glenn Horton-Smith.
*/

#include "DataBase.hh"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <string>

#include "Regex.hh"
#include "String.hh"

#ifdef DATABASE__USE_ROOT
#    include "TDirectory.h"
#    include "TFile.h"
#    include "TObjString.h"
#endif

using namespace tools;
using namespace std;

////////////////////////////////////////////////////////////////
////////////////////////// DataBase ///////////////////////////
////////////////////////////////////////////////////////////////

DataBase* DataBase::theDataBase = nullptr;

////////////////////////////////////////////////////////////////
// Get the absolute path from environment
string DataBase::GetPath(const char* env_path) const {
    if (env_path == nullptr) {
        throw DataBaseError(" ***Error DataBase::GetPath()1: env_path is empty");
    }

    char* env = getenv(env_path);
    if (not env) {
        throw DataBaseError(
            " ***Error DataBase::GetPath()2: env_path is not defined: ", env_path);
    }

    return scat(env, "/");
}

////////////////////////////////////////////////////////////////
/** ReadFile: read a file.  See class documentation for file format.
    If a parameter is defined more than once, e.g., due to calling ReadFile
    multiple times with different input files, the overwrite parameter decides
    whether the first or last definition is used.
    @param  is         input stream
    @param  prefix     prefix prepended (with a dot) to all the parameters of this file
    @param  overwrite  indicates how to handle parameter redefinitions
 */
void DataBase::ReadFile(istream& is, string prefix, DataBase::EOverwrite overwrite) {
    if (IsReadOnly) {
        throw DataBaseError(" ***Error DataBase::ReadFile1: Read-only database");
    }

    if (not prefix.empty()) {
        prefix += ".";
    }

    int NbMatch = 0, n = 0;
    string str, key;

    const string number("([+-]?\\d+(\\.\\d*)?([eE][+-]?\\d+)?)");
    Regex regNum(number);
    Regex regTable("^=");
    Regex regComment("\\s*#.*$");
    Regex regParse(scat("^(\\w+)(\\s+", number, ")?(\\s+(.+))?"));  // to parse the line
    while (getline(is, str).good()) {  // get a line from the file
        if (is.fail()) {
            break;
        }

        // skip blank lines and lines beginning with '#'
        if (str.empty() || str.front() == '#'
            || str.find_first_not_of(' ') == string::npos) {
            continue;
        }
        while (regComment.Match(str)) {
            regComment.Substitute(str, "");
        }
        // check if a comment is a the end of the line
        if (str.empty()) {
            continue;
        }

        // look for a table
        if (regTable.Match(str)) {
            // Get the table name
            Regex regName("=(\\w+)=");
            if (not regName.Match(str)) {
                throw DataBaseError(
                    " ***Error DataBase::ReadFile2: Can not find table name in line: ",
                    str);
            }
            const string tablename = regName.at(1);

            if (TableMap.count(tablename) != 0) {
                throw DataBaseError(
                    " ***Error DataBase::ReadFile3: Attempt to redefine already "
                    "defined table: ",
                    tablename);
            }

            //       TableMap[tablename] = make_shared<DataTable>(is,str);
            //    Here using make_shared is not possible because the constructor of
            //    DataTable is private
            TableMap[tablename] = shared_ptr<DataTable>(new DataTable(is, str));
            StringMap[tablename] = tablename;
            NumMap[tablename] = TableMap[tablename]->GetNbRows();
            KeyList.push_back(tablename);
            continue;
        }

        // no table, parse the string
        NbMatch = regParse.Match(str);
        if (NbMatch < 2) {  // nothing was found => error
            cerr << " *Warning DataBase::ReadFile1: Bad format of line " << str
                 << "\nSkipping...\n";
            continue;
        }
        key = prefix + regParse.at(1);

        // Now save value(s).  By convention, any defined parameter will
        // always have an entry in both the numeric and string value dictionaries,
        // possibly zero(-length) if no number or no string value was supplied. First
        // check: does name already exist in hash?
        if (StringMap.count(regParse[1]) or NumMap.count(regParse[1])) {
            // name exists in hash, so override or keep existing value as appropriate
            cerr << " *Warning DataBase::ReadFile2: "
                 << (overwrite != kKeep ? "OVERRIDING" : "PRESERVING")
                 << " previous setting of " << key << endl;
            if (overwrite != kKeep) {
                continue;
            }
        } else {
            // name does not exist in hash, so add to vector of keys
            KeyList.push_back(key);
        }

        if (NbMatch > 7) {
            StringMap[key] = regParse.at(7);
        }  // case 'name numeric string [comment]'
        if (NbMatch > 3) {  // case 'name numeric [comment]' or 'name string [comment]'
            if (regNum.Match(regParse.at(3))) {
                NumMap[key] = regParse.stod(3);
                StringMap[key];
            } else {
                NumMap[key] = 0.;
                StringMap[key] = regParse.at(3);
            }
        }
        n++;
    }
#ifdef DEBUG
    cout << " --- Database successfully read: " << n << " elements found ---\n";
#endif
}

////////////////////////////////////////////////////////////////
/// ReadFile: read from a file of the given name.
void DataBase::ReadFile(
    const char* filename,
    const char* env_path,
    const char* prefix,
    DataBase::EOverwrite overwrite) {
    string FileWithPath = filename;
    if (env_path) {
        FileWithPath = GetPath(env_path) + FileWithPath;
    }

    if (IsReadOnly) {
        throw DataBaseError(
            " ***Error DataBase::ReadFile2: Read-only database, can not update: ",
            FileWithPath);
    }

#ifdef DEBUG
    cout << " Attempting to read database from file " << FileWithPath << endl;
#endif

    ifstream ifs;
    ifs.open(FileWithPath, std::ios::in);
    if (! ifs.is_open() || ! ifs.good()) {
        throw DataBaseError(
            " ***Error DataBase::ReadFile3: Could not open: ", FileWithPath);
    }

    if (prefix) {
        ReadFile(ifs, prefix, overwrite);
    } else {
        ReadFile(ifs, "", overwrite);
    }
    ifs.close();
}

////////////////////////////////////////////////////////////////

void DataBase::WriteText(ostream& os) const {
    // write out parameters in the same order in which they were read in
    os.precision(15);
    for (size_t i = 0; i != KeyList.size(); i++) {
        const string& key(KeyList[i]);
        os << key;
        if (HasNumValue(key)) {
            os << ' ' << NumMap.at(key);
        }
        if (HasStrValue(key)) {
            os << " : " << StringMap.at(key);
        }
        os << endl;
    }
    os.flush();

    // write out tables (in the map order, i.e. not ordered)
    for (auto itMap = TableMap.begin(), endMap = TableMap.end(); itMap != endMap;
         ++itMap) {
        os << "\n\n" << (*itMap).first << "\n";
        (*itMap).second->Print(os);
    }
    os.flush();

    cout << " --- Database successfully written in text file: " << KeyList.size()
         << " parameters and " << TableMap.size() << " tables ---\n";
}
////////////////////////////////////////////////////////////////
/// WriteFile: write to a file of the given name.
void DataBase::WriteText(const char* filename, const char* env_path) const {
    string FileWithPath = filename;
    if (env_path) {
        FileWithPath = GetPath(env_path) + FileWithPath;
    }

    cout << " Attempting to write database on text file: " << FileWithPath << endl;
    ofstream ofs;
    ofs.open(filename, std::ios::out);
    if (! ofs.is_open() || ! ofs.good()) {
        throw DataBaseError(" ***Error DataBase::WriteText: Could not open ", filename);
    }

    WriteText(ofs);
    ofs.close();
}

////////////////////////////////////////////////////////////////

double DataBase::GetNumValue(const string& key) const {
    if (NumMap.count(key) == 0) {
        // if application code is using this method, it's just reading the db, not
        // assigning to it. This is always an error in DataBase if the key is not yet
        // defined.
        throw DataBaseError(
            " ***Error DataBase::GetNumValue: Attempt to retrieve undefined key ", key);
    }
    return NumMap.at(key);
}

////////////////////////////////////////////////////////////////

const string& DataBase::GetStrValue(const string& key) const {
    if (StringMap.count(key) == 0) {
        throw DataBaseError(
            " ***Error DataBase::GetStrValue: Attempt to retrieve undefined key ", key);
    }
    return StringMap.at(key);
}

////////////////////////////////////////////////////////////////

double& DataBase::operator[](const string& key) {
    if (IsReadOnly) {
        throw DataBaseError(
            " ***Error DataBase::operator[]: Attempt to modify read-only base: ", key);
    }
    if (NumMap.count(key) != 0) {
        //     throw parambase_except(" ***Error DataBase::operator[]2: Attempt to
        //     create already existing key: ", key);
        cerr << " *Warning DataBase::operator[]: Overriding existing key: " << key
             << "\n";
    }
    if (StringMap.count(key) == 0) {
        KeyList.push_back(key);
    }  // the key may have been already created with just a string value
    return NumMap[key];
}

////////////////////////////////////////////////////////////////

string& DataBase::operator()(const string& key) {
    if (IsReadOnly) {
        throw DataBaseError(
            " ***Error DataBase::operator()1: Attempt to modify read-only base: ", key);
    }
    if (StringMap.count(key) != 0) {
        //     throw parambase_except(" ***Error DataBase::operator()2: Attempt to
        //     create already existing key: ", key);
        cerr << " *Warning DataBase::operator(): Overriding existing key: " << key
             << "\n";
    }
    if (NumMap.count(key) == 0) {
        KeyList.push_back(key);
    }  // the key may have been already created with just a numeric value
    return StringMap[key];
}

////////////////////////////////////////////////////////////////

const std::shared_ptr<DataTable> DataBase::GetTablePtr(const char* name) const {
    if (TableMap.count(string(name)) == 0) {
        throw DataBaseError(
            " ***Error DataBase::GetTable:Attempt to access undefined table ", name);
    }
    return TableMap.at(string(name));
}

////////////////////////////////////////////////////////////////

vector<double> DataTable::GetColumnAsNum(const string& colname) const {
    if (ColMap.count(colname) == 0) {
        throw DataBaseError(" ***Error DataTable::GetColumnAsNum: Unknown : ", colname);
    }

    vector<double> v;
    for (auto& el : ColMap.at(colname)) {
        v.push_back(stod(el));
    }
    return v;
}

////////////////////////////////////////////////////////////////
///////////////////////// DataTable ///////////////////////////
////////////////////////////////////////////////////////////////

/** Create a DataTable object and initialize it from a file.
    It may seem strange that no "ReadFile()" function is provided, and
    no default DataTable() constructor is provided, but this is intentional:
    no uninitialized tables are allowed, and once initialized, tables are
    intended to be immutable. */
DataTable::DataTable(std::istream& is, std::string& line) {
#ifdef DEBUG
    cout << " Attempting to read table from file " << filename << endl;
#endif

    size_t NbCol = 0;
    string header = line;
    Regex regSharp("#");
    Regex regSpace("\\s+");
    Regex regComment("\\s*#.*$");

    // parse header line
    // remove '=tablename=' and possible spaces
    Regex("^=\\w+=\\s*").Substitute(header, "");
    // check if a comment is a the end of the line
    while (regSharp.Match(header)) {
        regComment.Substitute(header, "");
    }  // remove if necessary

    // split the string on spaces
    NbCol = regSpace.Split(header).size();
    for (size_t i = 0; i < NbCol; i++) {
        ColNames.push_back(regSpace.at(i));
        ColMap[regSpace.at(i)].reserve(10);  // forces column vector to be defined
    }

    // process the data lines
    while (getline(is, line).good()) {
        if (is.fail()) {
            break;
        }

        // skip lines beginning with '#'
        if (line.front() == '#') {
            continue;
        }
        while (regSharp.Match(line)) {
            regComment.Substitute(line, "");
        }  // remove if necessary

        // end of the table at the first blank line
        if (line.empty() || line.find_first_not_of(' ') == string::npos) {
            break;
        }

        // parse the line
        if (regSpace.Split(line).size() - 1 != NbCol) {
            throw DataBaseError(
                " ***Error DataTable::DataTable1: Bad line, expected ",
                NbCol,
                " columns, got ",
                regSpace.Nb(),
                " values on row ",
                NbRow);
        }

        // all is well, push back the data!
        for (size_t i = 0; i < NbCol; i++) {
            ColMap[ColNames.at(i)].push_back(regSpace.at(i + 1));
        }
        NbRow++;
    }

#ifdef DEBUG
    cout << " --- Table successfully defined with " << NbRow << " rows and "
         << ColNames.size() << " columns found ---\n";
#endif
}

////////////////////////////////////////////////////////////////

void DataTable::Print(std::ostream& os) const {
    for (auto& name : ColNames) {
        os << "\t" << name;
    }
    os << "\n";

    for (int iRow = 0; iRow < NbRow; iRow++) {
        for (auto it = ColNames.begin(), end = ColNames.end(); it != end; ++it) {
            os << "\t" << ColMap.at(*it).at(iRow);
        }
        os << "\n";
    }
}

////////////////////////////////////////////////////////////////
/* Text can not be written in ROOT TFile as easily as in (o)fstream:
 * we need to write a TObject or one of its daughters.
 * The simplest way is to concatenate the text in a single TString (which is NOT a
 * daughter of TObject), then push the TString in a TObjString (a TObject encapsulating
 * a TString), then write the TObjString.
 */

#ifdef DATABASE__USE_ROOT
void DataBase::WriteRoot(TFile* file) const {
    TDirectory* dir = TDirectory::CurrentDirectory();  // save the current directory
    file->cd();  // change (Root) directory to the output file

    TObjString ObjStr;
    string SaveStr;

    // write out parameters in the same order in which they were read in
    for (size_t i = 0; i != KeyList.size(); i++) {
        const string& key(KeyList[i]);
        SaveStr += key;
        if (HasNumValue(key)) {
            SaveStr += scat(" ", NumMap.at(key));
        }
        if (HasStrValue(key)) {
            SaveStr += scat(" : ", StringMap.at(key));
        }
        SaveStr += "\n";
    }
    ObjStr.SetString(SaveStr.c_str());
    ObjStr.Write("Parameters");

    // write out tables (in the map order, i.e. not ordered)
    for (auto itMap = TableMap.begin(), endMap = TableMap.end(); itMap != endMap;
         ++itMap) {
        SaveStr.clear();
        SaveStr = scat("\n\n", (*itMap).first, "\n");

        auto tab = (*itMap).second;
        const vector<string>* ColNames = &tab->GetColumnNames();
        for (auto itV = ColNames->begin(), end = ColNames->end(); itV != end; ++itV) {
            SaveStr += scat("\t", *itV);
        }
        SaveStr += "\n";

        for (int iRow = 0; iRow < tab->GetNbRows(); iRow++) {
            for (auto itV = ColNames->begin(), end = ColNames->end(); itV != end; ++itV)
            {
                SaveStr += scat("\t", (*tab)[*itV].at(iRow));
            }
            SaveStr += "\n";
        }

        ObjStr.SetString(SaveStr.c_str());
        ObjStr.Write(cat("Table ", (*itMap).first));
    }

    dir->cd();  // back to previous directory
    cout << " --- Database successfully written in ROOT file " << file->GetName()
         << ": " << KeyList.size() << " parameters and " << TableMap.size()
         << " tables ---\n";
}

////////////////////////////////////////////////////////////////

void DataBase::WriteRoot(const char* filename) const {
    cout << " Attempting to write database on ROOT file " << filename << endl;
    TFile file(filename, "RECREATE");
    if (! file.IsOpen()) {
        throw parambase_except(
            " ***Error DataBase::WriteRoot1: Could not open ", filename);
    }
    WriteRoot(&file);
    file.Close();
}
#endif  // DATABASE__USE_ROOT
