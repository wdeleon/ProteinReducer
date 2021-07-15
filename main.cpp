/*****************************************************************
*   (C) 2018 Winston Deleon                                      *
*                                                                *
*   Compiles and runs under Linux using g++ with the commands:   *
*   g++ task2.cpp -o task2                                       *
*   ./task2                                                      *
*                                                                *
*   Compiles and runs under Windows using MinGW++:               *
*   mingw32-g++ task2.cpp -o task2.exe                           *
*                                                                *
*   Tested with 1aky.pdb                                         *
*****************************************************************/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

/****************
*   Functions   *
****************/

// Translate a string of text to an integer
// Requires no particular format
int int_from_string (string s) {
    int result = 0;
    int temp = 0;
    int power = 1;
    for (unsigned int i = 0; i < s.size(); i++) {

        if (s.substr(i, 1) == "1") {
            temp = 1;
        }
        else if (s.substr(i, 1) == "2") {
            temp = 2;
        }
        else if (s.substr(i, 1) == "3") {
            temp = 3;
        }
        else if (s.substr(i, 1) == "4") {
            temp = 4;
        }
        else if (s.substr(i, 1) == "5") {
            temp = 5;
        }
        else if (s.substr(i, 1) == "6") {
            temp = 6;
        }
        else if (s.substr(i, 1) == "7") {
            temp = 7;
        }
        else if (s.substr(i, 1) == "8") {
            temp = 8;
        }
        else if (s.substr(i, 1) == "9") {
            temp = 9;
        }
        else {
            temp = 0;
        }

        power = 1;
        for (unsigned int j = 0; j < ((s.size() - 1) - i); j++) {
            power = power * 10;
        }
        result += temp * power;
    }
    return result;
}

// Translate a string of text to a floating point number
// Requires strings in the format ####.###
float float_from_string (string s) {
    float result = 0;

    int n = 1;
    for (unsigned int i = 0; i < 4; i++) {
        if (s.substr(i, 1) == "-") {
            n *= -1;
        }
    }

    result += int_from_string(s.substr(0, 1)) * 1000;
    result += int_from_string(s.substr(1, 1)) * 100;
    result += int_from_string(s.substr(2, 1)) * 10;
    result += int_from_string(s.substr(3, 1));

    result += int_from_string(s.substr(5, 1)) * 0.1;
    result += int_from_string(s.substr(6, 1)) * 0.01;
    result += int_from_string(s.substr(7, 1)) * 0.001;

    result *= n;
    return result;
}

// Right-justify a text string s by adding whitespace padding to achieve a total string length of p
string right_justify(string s, unsigned int p) {
    string r = s;
    for (unsigned int i = s.size(); i < p; i++) {
        r = " " + r;
    }
    return r;
}


/**************
*   Classes   *
**************/

// Helix class
class Helix {
    private:
        int firstResID;
        int lastResID;
        string chain;
        string ID;
        string rawLine;

    public:
        Helix(string pdb_line) {
            this->rawLine = pdb_line;
            this->chain = pdb_line.substr(19, 1);
            this->ID = pdb_line.substr(7, 7);
            this->firstResID = int_from_string(pdb_line.substr(21, 4));
            this->lastResID = int_from_string(pdb_line.substr(33, 4));
        }

        int getFirstRes () {
            return this->firstResID;
        }

        int getLastRes () {
            return this->lastResID;
        }

        string getChain() {
            return this->chain;
        }

        string getID() {
            return this->ID;
        }

        string getRawLine() {
            return this->rawLine;
        }
};


// Sheet class
class Sheet {
    private:
        int firstResID;
        int lastResID;
        string chain;
        string ID;
        string rawLine;

    public:
        Sheet(string pdb_line) {
            this->rawLine = pdb_line;
            this->firstResID = int_from_string(pdb_line.substr(22, 4));
            this->lastResID = int_from_string(pdb_line.substr(33, 4));
            this->chain = pdb_line.substr(21, 1);
            this->ID = pdb_line.substr(7, 7);
        }

        int getFirstRes () {
            return this->firstResID;
        }

        int getLastRes () {
            return this->lastResID;
        }

        string getChain () {
            return this->chain;
        }

        string getID () {
            return this->ID;
        }

        string getRawLine() {
            return this->rawLine;
        }
};


// Atom class
class Atom {
    private:
        float x;
        float y;
        float z;
        int ID;
        int resID;
        string resName;
        string chain;
        string label;
        string element;
        string rawLine;

    public:
        Atom(string pdb_line) {
            this->rawLine = pdb_line;
            this->x = float_from_string(pdb_line.substr(30, 8));
            this->y = float_from_string(pdb_line.substr(38, 8));
            this->z = float_from_string(pdb_line.substr(46, 8));
            this->ID = int_from_string(pdb_line.substr(6, 5));
            this->resID = int_from_string(pdb_line.substr(22, 4));
            this->chain = pdb_line.substr(21, 1);
            this->label = pdb_line.substr(12, 4);
        }

        Atom(float x, float y, float z, int ID, int resID, string resName, string chain, string label, string element) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->ID = ID;
            this->resID = resID;
            this->resName = resName;
            this->chain = chain;
            this->label = label;
            this->element = element;

            // ATOM entry identifier
            this->rawLine = "ATOM  ";

            // atom serial number
            ostringstream strm;
            strm.clear();
            strm.fill(' ');
            strm.width(5);
            strm << this->ID;
            this->rawLine += strm.str();
            strm.str(string());

            // Atom label, residue name, and chain identifier
            this->rawLine += right_justify(this->label, 3);
            this->rawLine += "   ";
            this->rawLine += this->resName;
            this->rawLine += " ";
            this->rawLine += this->chain;

            // residue ID
            strm.width(4);
            strm << this->resID;
            this->rawLine += strm.str();
            strm.str(string());

            this->rawLine += "    ";

            // x, y, z
            strm.width(8);
            strm.precision(3);
            strm.setf(ios::fixed, ios::floatfield);
            strm << this->x;
            this->rawLine += right_justify(strm.str(), 8);
            strm.str(string());
            strm << this->y;
            this->rawLine += right_justify(strm.str(), 8);
            strm.str(string());
            strm << this->z;
            this->rawLine += right_justify(strm.str(), 8);
            strm.str(string());

            // occupancy and temperature factor are left at these default values for axis points
            this->rawLine += "  1.00  0.00          ";

            // Element symbol and ending whitespace
            this->rawLine += right_justify(this->element, 2);
            this->rawLine += "  ";
        }

        float getX() {
            return this->x;
        }

        float getY() {
            return this->y;
        }

        float getZ() {
            return this->z;
        }

        int getID() {
            return this->ID;
        }

        int getResID() {
            return this->resID;
        }

        string getChain() {
            return this->chain;
        }

        string getLabel() {
            return this->label;
        }

        string getRawLine() {
            return this->rawLine;
        }
};


/*******************
*   Main program   *
*******************/

int main()
{
    // Tested with 1aky.pdb, 2p8y.pdb, 1aop.pdb, 1s04.pdb, 1atz.pdb, 2qtr.pdb, 1d5t.pdb, 1a12.pdb, 1b3a.pdb
    string infile_name;
    cout << "Enter input file name: ";
    cin >> infile_name;

    ifstream input_file;
    input_file.open(infile_name.c_str(), ios::in);

    if (input_file.is_open()) {
        cout << "Opened " << infile_name << ". Reading..." << endl;

        /**************************************************************
        *   Step 1:                                                   *
        *   Process .pdb file text into helices, strands, and atoms   *
        **************************************************************/

        vector<string> pdb_line;
        string file_text_line = "";
        while (getline(input_file, file_text_line)) {
            pdb_line.push_back(file_text_line);
        }
        input_file.close();
        cout << "Read " << pdb_line.size() << " lines from " << infile_name << endl;

        // Bin relevant lines from the input PDB file into dynamically expanding arrays of objects
        vector<Helix> helix;
        vector<Sheet> sheet;
        vector<Atom> atom;
        vector<Atom> point;
        unsigned int next_atom_ID = 1;
        vector<unsigned int> ter;

        for (unsigned int i = 0; i < pdb_line.size(); i++) {
            if (pdb_line[i].substr(0, 6) == "HELIX ") {
                helix.push_back(Helix(pdb_line[i]));
            }
            else if (pdb_line[i].substr(0, 6) == "SHEET ") {
                sheet.push_back(Sheet(pdb_line[i]));
            }
            else if (pdb_line[i].substr(0, 6) == "ATOM  ") {
                atom.push_back(Atom(pdb_line[i]));
            }
        }
        cout << "Found " << helix.size() << " helices" << endl;
        cout << "Found " << sheet.size() << " strands" << endl;
        cout << "Found " << atom.size() << " atoms" << endl;


        /***************************************
        *   Step 2:                            *
        *   Generate axis points for helices   *
        ***************************************/

        for (unsigned int i = 0; i < helix.size(); i++) {
            int first_resID = helix[i].getFirstRes();
            int last_resID = helix[i].getLastRes();
            string chain = helix[i].getChain();

            // Find all CA atoms in residues for this helix
            vector<unsigned int> atom_number;
            for (unsigned int j = 0; j < atom.size(); j++) {
                if ((atom[j].getResID() >= first_resID)
                    && (atom[j].getResID() <= last_resID)
                    && atom[j].getLabel() == " CA "
                    && atom[j].getChain() == chain) {
                    atom_number.push_back(j);
                }
            }

             // At least four CA atoms are needed in a helix to calculate axis points
            for (unsigned int k = 0; k < (atom_number.size() - 3); k++) {
                float point_x = (atom[atom_number[k]].getX() +
                                atom[atom_number[k + 1]].getX() +
                                atom[atom_number[k + 2]].getX() +
                                atom[atom_number[k + 3]].getX()) / 4;

                float point_y = (atom[atom_number[k]].getY() +
                                atom[atom_number[k + 1]].getY() +
                                atom[atom_number[k + 2]].getY() +
                                atom[atom_number[k + 3]].getY()) / 4;

                float point_z = (atom[atom_number[k]].getZ() +
                                atom[atom_number[k + 1]].getZ() +
                                atom[atom_number[k + 2]].getZ() +
                                atom[atom_number[k + 3]].getZ()) / 4;

                point.push_back(Atom(point_x, point_y, point_z, next_atom_ID, 1, "HLX", helix[i].getChain(), "S", "S"));
                next_atom_ID++;
            }
            ter.push_back(next_atom_ID - 2);
        }


        /**************************************
        *   Step 3:                           *
        *   Generate axis points for sheets   *
        **************************************/

        for (unsigned int i = 0; i < sheet.size(); i++) {
            int first_resID = sheet[i].getFirstRes();
            int last_resID = sheet[i].getLastRes();
            string chain = sheet[i].getChain();

            vector<unsigned int> atom_number;
            for (unsigned int j = 0; j < atom.size(); j++) {
                if ((atom[j].getResID() >= first_resID) && (atom[j].getResID() <= last_resID) && (atom[j].getChain() == chain)) {
                    // CA, C, and N atoms are used to find axis points for strands
                    if ((atom[j].getLabel() == " CA ") || (atom[j].getLabel() == " N  ") || (atom[j].getLabel() == " C  ")) {
                        atom_number.push_back(j);
                    }
                }
            }

            // At least four atoms are needed in a strand to find axis points
            for (unsigned int k = 0; k < (atom_number.size() - 3); k++) {
                float point_x = (atom[atom_number[k]].getX() +
                                atom[atom_number[k + 1]].getX() +
                                atom[atom_number[k + 2]].getX() +
                                atom[atom_number[k + 3]].getX()) / 4;

                float point_y = (atom[atom_number[k]].getY() +
                                atom[atom_number[k + 1]].getY() +
                                atom[atom_number[k + 2]].getY() +
                                atom[atom_number[k + 3]].getY()) / 4;

                float point_z = (atom[atom_number[k]].getZ() +
                                atom[atom_number[k + 1]].getZ() +
                                atom[atom_number[k + 2]].getZ() +
                                atom[atom_number[k + 3]].getZ()) / 4;

                point.push_back(Atom(point_x, point_y, point_z, next_atom_ID, 1, "STR", sheet[i].getChain(), "S", "S"));
                next_atom_ID++;
            }
            ter.push_back(next_atom_ID - 2);
        }


        /********************************************
        *   Step 4:                                 *
        *   Output axis points to a new .pdb file   *
        ********************************************/

        // New output file name
        string outfile_name = infile_name.substr(0, infile_name.size() - 4) + "_simplified.pdb";

        ofstream output_file;
        output_file.open(outfile_name.c_str(), ios::out | ios::trunc);
        if (output_file.is_open()) {
            cout << "Writing " << outfile_name << "..." << endl;

            for (unsigned int i = 0; i < point.size(); i++) {
                output_file << point[i].getRawLine() << endl;
                for (unsigned j = 0; j < ter.size(); j++) {
                    if (ter[j] == i) {
                        output_file << "TER" << endl;
                    }
                }
            }
            output_file.close();
            cout << "Done." << endl;
        }
        else {
            cout << "Couldn't open output file." << endl;
        }
    }

    else {
        cout << "Couldn't open input file." << endl;
    }

    return 0;
}
