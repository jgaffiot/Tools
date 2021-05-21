// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2014-10-20
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//
/** Inspired by DCParam, the API for the Double Chooz simple "database"
    of numeric and string values, written by Glenn Horton-Smith.
*/
#ifndef DATABASE_HH
#define DATABASE_HH 1

#ifdef USE_ROOT
#    define DATABASE__USE_ROOT 1
#endif

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Exception.hh"
#include "ToolsConfig.hh"

#ifdef DATABASE__USE_ROOT
class TFile;
#endif

namespace tools
{
class DataTable;

////////////////////////////////////////////////////////////////
////////////////////////// exception ///////////////////////////
////////////////////////////////////////////////////////////////

class database_except: public Error {
public:
    explicit database_except(const std::string& arg): Error(arg) {}
    explicit database_except(const std::string& arg, std::ofstream& write):
        Error(arg, write) {}

    template<typename T>
    explicit database_except(const T& val): Error(val) {}
    template<typename T, typename... Args>
    database_except(const T& val, Args... args): Error(val, args...) {}
};

////////////////////////////////////////////////////////////////
////////////////////////// DataBase ///////////////////////////
////////////////////////////////////////////////////////////////

/** DataBase provides a simple "database" of numeric and string values:
  a string/value hashtable that is initialized from a simple text file.
  The file can also contain any number of table as defined as below (see DataTable).

  The text file looks something like this:
  @code
  # blank lines and lines beginning with '#' are ignored

    pi  3.14159265358979312  # any character after the '#' are ignored
    model  INCL++            # any space before the '#' are ignored
    e  2.71828182845904509e+00  This is 'e' as in exp(1), not the e+ charge.
    q_e 1.60217646200000007e-19 coulomb  # Charge of the positron in coulombs
   @endcode

   The above input would define four parameters: a numeric parameter
   named "pi", a string parameter named "model", and two parameters
   "e" and "q_e" having both numeric and string values.  Note that the
   word "coulomb" following the numeric value on the "q_e" line is
   the only word considered, while the string value of the "e" line is
   the whole phrase "This is 'e' as in exp(1), not the e+ charge.".
   The general format for a non-blank, non-comment line is:

   @code
     key [ numeric_value ] [ string_value ] [ # comments ]
   @endcode

   where brackets surround optional components.
   The string value extends to the end of the line if no '#' symbol is found.
   The space(s) between the last non-white character and the '#' symbol are ignored.

  - To determine wether an entry has a numeric value or not, the first "word" after
    the key is tested with std::stod method. Formats below should be valid:
      64320     6.4320     6,4320
      6.43e20   6.43E20    6,43e20
      6.43e-20  6.43E-20   6,43e-20 -6.43e+20

  - String values are supported. In fact, every parameter may
    have both a numeric and a string value.  For keys that are
    primarily numeric, the string value should be used for a
    description of the meaning of the quantity.

  - New parameters may only be defined via ReadFile, not directly through operator[].
    It guards against typos in the argument to operator[] or user input.

  - This class is a singleton: it can not be freely instantiated or deleted.
    Use the GetDataBase members to get the unique instance of the class per program.
    Creator and destructor are then private.

   Based on DCparam from DoubleChooz software, based on GLG4 package from Glenn
  Horton-Smith. Rewritten with std::string and regex by Jonathan Gaffiot.
*/

class DataBase {
public:
    /// Singleton management
    // Get reference to the read/write database, constructing as necessary. Use this
    // only to fill the database.
    static DataBase& GetInputDataBase() {
        if (theDataBase == nullptr) {
            theDataBase = new DataBase(false);
        } else {
            theDataBase->IsReadOnly = false;
        }
        return *theDataBase;
    }
    // Get the pointer to the read/write database, constructing as necessary. Use this
    // only to fill the database.
    static DataBase* GetInputDataBasePtr() {
        if (theDataBase == nullptr) {
            theDataBase = new DataBase(false);
        } else {
            theDataBase->IsReadOnly = false;
        }
        return theDataBase;
    }

    // Get reference to the read-only database, constructing as necessary
    static const DataBase& GetDataBase() {
        if (theDataBase == nullptr) {
            theDataBase = new DataBase(true);
        } else {
            theDataBase->IsReadOnly = true;
        }
        return *theDataBase;
    }
    // Get the pointer to the read-only database, constructing as necessary
    static DataBase const* GetDataBasePtr() {
        if (theDataBase == nullptr) {
            theDataBase = new DataBase(true);
        } else {
            theDataBase->IsReadOnly = true;
        }
        return theDataBase;
    }

    /// Getters, constant methods -> no need to check for IsReadOnly
    std::string GetPath(const char* env_path) const;
    inline std::string GetPath(const std::string& env_path) const {
        return GetPath(env_path.c_str());
    }

    void WriteText(const char* filename, const char* env_path = nullptr) const;
    void WriteText(std::ostream& os) const;
    inline void Print() const { WriteText(std::cout); }

#ifdef DATABASE__USE_ROOT
    void WriteRoot(const char* filename) const;
    void WriteRoot(TFile* file) const;
#endif

    double GetNumValue(const std::string& key) const;
    const std::string& GetStrValue(const std::string& key) const;

    // bunch of classic accessors
    inline double operator[](const std::string& key) const { return GetNumValue(key); }
    inline const std::string& operator()(const std::string& key) const {
        return GetStrValue(key);
    }

    const std::shared_ptr<DataTable> GetTablePtr(const char* name) const;
    inline const std::shared_ptr<DataTable> GetTablePtr(const std::string& name) const {
        return GetTablePtr(name.c_str());
    }
    inline const DataTable& GetTable(const char* name) const {
        return *GetTablePtr(name);
    }
    inline const DataTable& GetTable(const std::string& name) const {
        return *GetTablePtr(name.c_str());
    }

    /** Test if key is in the numeric parameter table */
    inline bool HasNumValue(const std::string& key) const {
        return (NumMap.count(key) > 0);
    }
    /** Test if key is in the string parameter table */
    inline bool HasStrValue(const std::string& key) const {
        return (StringMap.count(key) > 0 && StringMap.at(key).size() > 0);
    }
    /** Test if key is in the table table */
    inline bool HasTable(const std::string& tablename) const {
        return (TableMap.count(tablename) > 0);
    }

    /** Get list of keys, in the order in which they were read in. */
    inline const std::vector<std::string>& GetListOfKeys() const { return KeyList; }

    /// Setters, NON constant methods -> DO need to check for IsReadOnly
    enum EOverwrite
    {
        kKeep,
        kOverride
    };
    void ReadFile(
        const char* filename,
        const char* env_path = nullptr,
        const char* prefix = nullptr,
        EOverwrite overwrite = kKeep);
    void ReadFile(
        std::istream& is, std::string prefix = "", EOverwrite overwrite = kKeep);

    double& operator[](const std::string& key);
    std::string& operator()(const std::string& key);

    inline void SetNumValue(const std::string& key, double value) {
        this->operator[](key) = value;
    }
    inline void SetStrValue(const std::string& key, const std::string& value) {
        this->operator()(key) = value;
    }

private:
    /// Constructors
    explicit DataBase(bool isreadonly) { IsReadOnly = isreadonly; }
    DataBase() = delete;
    DataBase(const DataBase&) = delete;
    DataBase(DataBase&&) = delete;
    DataBase& operator=(const DataBase&) = delete;
    DataBase& operator=(DataBase&&) = delete;

    /// Members
    bool IsReadOnly;  // read-only most of time, when reading values from map
    static DataBase* theDataBase;  // common instance for database

    std::vector<std::string> KeyList;  // to preserve order
    std::map<std::string, double> NumMap;  // numeric values
    std::map<std::string, std::string> StringMap;  // string values
    std::map<std::string, std::shared_ptr<DataTable>> TableMap;  // tables
};

////////////////////////////////////////////////////////////////
///////////////////////// DataTable ///////////////////////////
////////////////////////////////////////////////////////////////

/** A DataTable is a simple table of numeric values arranged in named
   columns and numbered rows, read from am ascii file and contained entirely in memory.
   (If more capability is needed, it may be time to use an SQL database or a TTree.)

   The ascii file for a table looks something like this:

   @code
   # comments
   # comments
   # comments
   =table-name= id  x      y      z     u    v    w
   10   1.0    0.0    1.0    0.0  0.0  -1.0
   11  -0.5    0.866  1.0    0.0  0.0  -1.0
   12  -0.5   -0.866  1.0    0.0  0.0  -1.0
   20   0.866  0.5   -1.0    0.0  0.0   1.0
   21  -0.866  0.5   -1.0    0.0  0.0   1.0
   22   0.0   -1.0   -1.0    0.0  0.0   1.0
   @endcode

   Every line starting with "=" will be parsed for the table name and the column names.
   The table name is enclosed between two '=' (including the one starting the table).
   Column names are separated by whitespace; column names cannot contain white space.
   The table continues until the first blank line.

   Used in conjuction with DataBase, which save each table under its name in a table
   base.

   Example code fragments:
    Accessing a table:
    @code
     const DataTable& pmt( DataBase::GetDataBase().GetTable("pmtcoordinates") );
     for (int ipmt=0; ipmt<pmt.NRows(); ipmt++)
       make_PMT( pmt["id"][ipmt].Atoi(), pmt["shape"][ipmt] );
    @endcode

    Accessing a table perhaps somewhat more efficiently:
    @code
     DataBase& db( DataBase::GetDataBase() );
     const DataTable& pmt( db.GetTable("pmtcoordinates") );
     DataTable::Col_t& id( pmt["id"] );
     std::vector<std::string>& shape( pmt["shape"] );
     for (int ipmt=0; ipmt<pmt.NRows(); ipmt++)
       make_PMT( id[ipmt].Atoi(), shape[ipmt] );
    @endcode
*/
class DataTable {
public:
    typedef std::vector<std::string> Col_t;
    inline int GetNbRows() const { return NbRow; }
    inline int GetNbColumns() const { return ColNames.size(); }
    inline const std::vector<std::string>& GetColumnNames() const { return ColNames; }

    std::vector<double> GetColumnAsNum(const std::string& colname) const;
    inline const Col_t& GetColumn(const std::string& colname) const {
        if (ColMap.count(colname) == 0) {
            throw database_except(
                " ***Error DataTable::operator[]: Unknown column ", colname);
        }
        return ColMap.at(colname);
    }

    inline const Col_t& operator[](const std::string& colname) const {
        return GetColumn(colname);
    }
    inline double operator()(const std::string& colname, int iRow) const {
        if (iRow < 0 or iRow >= NbRow) {
            throw database_except(
                " ***Error DataTable::operator(): No line number: ", iRow);
        }
        return std::stod(GetColumn(colname).at(iRow));
    }

    void Print(std::ostream& os) const;
    inline void Print() const { Print(std::cout); }

private:
    // Constructor is private, so that only DataBase creates DataTable, thanks to the
    // friend declaration below
    DataTable(std::istream& is, std::string& line);
    DataTable() = delete;
    DataTable(const DataTable&) = delete;
    DataTable(DataTable&&) = delete;
    DataTable& operator=(const DataTable&) = delete;
    DataTable& operator=(DataTable&&) = delete;

    friend void DataBase::ReadFile(std::istream&, std::string, EOverwrite);

    std::map<std::string, Col_t> ColMap;  // map of column vectors
    std::vector<std::string> ColNames;  // column names, in order
    int NbRow = 0;
};

}  // namespace tools

#endif /* DATABASE_HH */
