#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept> // For runtime_error and out_of_range
using namespace std;
class UserProfile {
protected:
string userID, name, email, businessType, location;
public:
UserProfile() {}
UserProfile(string id, string nm, string em, string bt, string loc)
: userID(id), name(nm), email(em), businessType(bt), location(loc) {}
virtual void displayProfile() const = 0;
virtual void connect() const = 0;
string getEmail() const { return email; }
string getIndustry() const { return businessType; }
string getLocation() const { return location; }
string getName() const { return name; }
};
class Entrepreneur : public UserProfile {
string startupName, industry, businessStage;
float fundingRequired;
public:
Entrepreneur() {}
Entrepreneur(string id, string nm, string em, string bt, string loc, string sn,
string ind, float fr, string bs)
: UserProfile(id, nm, em, bt, loc), startupName(sn), industry(ind),
fundingRequired(fr), businessStage(bs) {}
void pitchIdea() {
if (startupName == "") {
cout << "Error: Startup name missing!" << endl;
throw runtime_error("Startup name missing");
}
cout << name << " is pitching: " << startupName << " needs Rs." <<
fundingRequired << endl;
}
void connectWithInvestor() {
cout << name << " is connecting with investors in " << industry << endl;
}
void displayProfile() const override {
cout << "[Entrepreneur] " << name << ", Email: " << email << ", Startup: " <<
startupName << endl;
}
void connect() const override {
cout << name << " says: Let's collaborate and grow together!" << endl;
}
string getIndustry() const { return industry; }
float getFundingRequired() const { return fundingRequired; }
void saveToFile() const {
ofstream out("entrepreneurs.dat", ios::app);
if (out) {
out << userID << ',' << name << ',' << email << ',' << businessType << ','
<< location << ','
<< startupName << ',' << industry << ',' << fundingRequired << ',' <<
businessStage << endl;
out.close();
}
}
};
class Investor : public UserProfile {
float minInvestment, maxInvestment;
string industriesInterested[3];
int portfolioSize;
public:
Investor() {}
Investor(string id, string nm, string em, string bt, string loc, float minI, float
maxI, string ind[], int ps)
: UserProfile(id, nm, em, bt, loc), minInvestment(minI), maxInvestment(maxI),
portfolioSize(ps) {
for (int i = 0; i < 3; ++i)
industriesInterested[i] = ind[i];
}
void reviewPitch() {
cout << name << " is reviewing startup pitches." << endl;
}
void makeInvestmentOffer(float amount) {
if (amount < minInvestment || amount > maxInvestment) {
cout << "Error: Investment amount out of range!" << endl;
throw out_of_range("Investment amount out of range");
}
cout << name << " is offering Rs." << amount << " as investment." << endl;
}
void displayProfile() const override {
cout << "[Investor] " << name << ", Email: " << email << ", Portfolio Size: " <<
portfolioSize << endl;
}
void connect() const override {
cout << name << " says: I'm looking to fund promising ideas!" << endl;
}
string* getIndustries() { return industriesInterested; }
float getMin() const { return minInvestment; }
float getMax() const { return maxInvestment; }
};
template <typename T>
void searchByName(T users[], int count, string keyword) {
for (int i = 0; i < count; ++i) {
if (users[i].getName() == keyword) {
users[i].displayProfile();
}
}
}
template <typename T, typename U>
void searchByAttribute(T users[], int count, U getter, string keyword) {
for (int i = 0; i < count; ++i) {
if ((users[i].*getter)() == keyword) {
users[i].displayProfile();
}
}
}
void matchEntrepreneursWithInvestors(Entrepreneur entrepreneurs[], int eCount, Investor
investors[], int iCount) {
for (int i = 0; i < eCount; i++) {
for (int j = 0; j < iCount; j++) {
string* industries = investors[j].getIndustries();
for (int k = 0; k < 3; k++) {
if (entrepreneurs[i].getIndustry() == industries[k]) {
float fund = entrepreneurs[i].getFundingRequired();
if (fund >= investors[j].getMin() && fund <= investors[j].getMax())
{
cout << "Match found: " << entrepreneurs[i].getIndustry() << " -Rs." << fund << endl;
entrepreneurs[i].connectWithInvestor();
entrepreneurs[i].connect();
investors[j].connect();
}
}
}
}
}
}
int main() {
Entrepreneur entrepreneurs[5];
Investor investors[5];
string e_id, e_name, e_email, e_bt, e_loc, e_sn, e_ind, e_bs;
float e_fr;
cout << "Enter Entrepreneur Details:\n";
cout << "User ID: "; cin >> e_id;
cout << "Name: "; cin >> e_name;
cout << "Email: "; cin >> e_email;
cout << "Business Type: "; cin >> e_bt;
cout << "Location: "; cin >> e_loc;
cout << "Startup Name: "; cin >> e_sn;
cout << "Industry: "; cin >> e_ind;
cout << "Funding Required: "; cin >> e_fr;
cout << "Business Stage: "; cin >> e_bs;
entrepreneurs[0] = Entrepreneur(e_id, e_name, e_email, e_bt, e_loc, e_sn, e_ind,
e_fr, e_bs);
entrepreneurs[0].saveToFile();
string i_id, i_name, i_email, i_bt, i_loc;
float minI, maxI;
string i_ind[3];
int portfolioSize;
cout << "\nEnter Investor Details:\n";
cout << "User ID: "; cin >> i_id;
cout << "Name: "; cin >> i_name;
cout << "Email: "; cin >> i_email;
cout << "Business Type: "; cin >> i_bt;
cout << "Location: "; cin >> i_loc;
cout << "Minimum Investment: "; cin >> minI;
cout << "Maximum Investment: "; cin >> maxI;
cout << "Enter 3 Industries Interested:\n";
for (int i = 0; i < 3; ++i) {
cout << "Industry " << i + 1 << ": "; cin >> i_ind[i];
}
cout << "Portfolio Size: "; cin >> portfolioSize;
investors[0] = Investor(i_id, i_name, i_email, i_bt, i_loc, minI, maxI, i_ind,
portfolioSize);
try {
entrepreneurs[0].pitchIdea();
float offer;
cout << "\nEnter investment amount to offer: ";
cin >> offer;
investors[0].makeInvestmentOffer(offer);
} catch (exception &e) {
cout << "Exception caught: " << e.what() << endl;
}
matchEntrepreneursWithInvestors(entrepreneurs, 1, investors, 1);
cout << "\nSearch by Industry:\n";
string searchInd;
cout << "Enter industry to search: "; cin >> searchInd;
searchByAttribute(entrepreneurs, 1, &Entrepreneur::getIndustry, searchInd);
return 0;
}
