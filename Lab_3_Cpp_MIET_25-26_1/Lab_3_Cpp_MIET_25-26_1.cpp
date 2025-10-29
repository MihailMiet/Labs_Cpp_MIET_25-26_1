#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

class date {
private:
    int day;
    int month;
    int year;

public:
    date() : day(1), month(1), year(2000) {}

    date(int d, int m, int y) : day(d), month(m), year(y) {}

    void setDate(int d, int m, int y) {
        day = d;
        month = m;
        year = y;
    }

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    date& operator=(const date& other) {
        if (this != &other) {
            day = other.day;
            month = other.month;
            year = other.year;
        }
        return *this;
    }

    bool operator<(const date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator==(const date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }

    void input() {
        cout << "Enter day: ";
        cin >> day;
        cout << "Enter month: ";
        cin >> month;
        cout << "Enter year: ";
        cin >> year;
    }

    void display() const {
        cout << setfill('0') << setw(2) << day << "."
            << setw(2) << month << "."
            << year;
    }
};

class product {
private:
    string name;
    int quantity;
    double price;
    date manufacturing_date;
    static int counter;

public:
    product() : name(""), quantity(0), price(0.0) {
        counter++;
    }

    product(string n, int q, double p, date d)
        : name(n), quantity(q), price(p), manufacturing_date(d) {
        counter++;
    }

    product(const product& other)
        : name(other.name), quantity(other.quantity), price(other.price),
        manufacturing_date(other.manufacturing_date) {
        counter++;
    }

    product(product&& other) noexcept
        : name(move(other.name)), quantity(other.quantity), price(other.price),
        manufacturing_date(other.manufacturing_date) {
        counter++;
    }

    ~product() {
        counter--;
    }

    static int getCounter() {
        return counter;
    }

    void setName(string n) { name = n; }
    void setQuantity(int q) { quantity = q; }
    void setPrice(double p) { price = p; }
    void setManufacturingDate(date d) { manufacturing_date = d; }

    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    date getManufacturingDate() const { return manufacturing_date; }

    product& operator=(const product& other) {
        if (this != &other) {
            name = other.name;
            quantity = other.quantity;
            price = other.price;
            manufacturing_date = other.manufacturing_date;
        }
        return *this;
    }

    bool operator<(const product& other) const {
        return price < other.price;
    }

    void input() {
        cout << "\nEnter product name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter quantity: ";
        cin >> quantity;
        cout << "Enter price: ";
        cin >> price;
        cout << "Enter manufacturing date:\n";
        manufacturing_date.input();
    }

    void display() const {
        cout << left << setw(20) << name
            << setw(12) << quantity
            << setw(12) << fixed << setprecision(2) << price
            << "  ";
        manufacturing_date.display();
        cout << endl;
    }
};

int product::counter = 0;

void displayHeader() {
    cout << "\n" << string(60, '=') << endl;
    cout << left << setw(20) << "Name"
        << setw(12) << "Quantity"
        << setw(12) << "Price"
        << "  Manufacturing Date" << endl;
    cout << string(60, '=') << endl;
}

void displayProducts(const vector<product>& products) {
    if (products.empty()) {
        cout << "No products to display.\n";
        return;
    }
    displayHeader();
    for (const auto& p : products) {
        p.display();
    }
    cout << string(60, '=') << endl;
}

vector<product> searchByName(const vector<product>& products, const string& searchName) {
    vector<product> results;
    for (const auto& p : products) {
        if (p.getName().find(searchName) != string::npos) {
            results.push_back(p);
        }
    }
    return results;
}

vector<product> searchByDate(const vector<product>& products, const date& searchDate) {
    vector<product> results;
    for (const auto& p : products) {
        if (p.getManufacturingDate() == searchDate) {
            results.push_back(p);
        }
    }
    return results;
}

void sortByPriceDescending(vector<product>& products) {
    sort(products.begin(), products.end(), [](const product& a, const product& b) {
        return a.getPrice() > b.getPrice();
        });
}

void loadFromFile(vector<product>& products, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "File not found. Starting with empty product list.\n";
        return;
    }

    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++) {
        string name;
        int quantity, day, month, year;
        double price;

        getline(file, name);
        file >> quantity >> price >> day >> month >> year;
        file.ignore();

        date d(day, month, year);
        product p(name, quantity, price, d);
        products.push_back(p);
    }

    file.close();
    cout << "Loaded " << count << " products from file.\n";
}

void saveToFile(const vector<product>& products, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not save to file.\n";
        return;
    }

    file << products.size() << endl;
    for (const auto& p : products) {
        file << p.getName() << endl;
        file << p.getQuantity() << " " << p.getPrice() << " ";
        file << p.getManufacturingDate().getDay() << " "
            << p.getManufacturingDate().getMonth() << " "
            << p.getManufacturingDate().getYear() << endl;
    }

    file.close();
    cout << "Saved " << products.size() << " products to file.\n";
}

int main() {
    vector<product> products;
    string filename = "products.txt";
    int choice;

    cout << "=== Product Information System ===\n";

    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Add new product (keyboard input)\n";
        cout << "2. Load products from file\n";
        cout << "3. Display all products\n";
        cout << "4. Search product by name\n";
        cout << "5. Sort products by price (descending)\n";
        cout << "6. Search products by manufacturing date\n";
        cout << "7. Save products to file\n";
        cout << "8. Show product counter\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            product p;
            p.input();
            products.push_back(p);
            cout << "Product added successfully!\n";
            break;
        }
        case 2: {
            loadFromFile(products, filename);
            break;
        }
        case 3: {
            displayProducts(products);
            break;
        }
        case 4: {
            string searchName;
            cout << "Enter product name to search: ";
            cin.ignore();
            getline(cin, searchName);
            vector<product> results = searchByName(products, searchName);
            if (results.empty()) {
                cout << "No products found with name containing '" << searchName << "'\n";
            }
            else {
                cout << "\nSearch results for '" << searchName << "':\n";
                displayProducts(results);
            }
            break;
        }
        case 5: {
            sortByPriceDescending(products);
            cout << "Products sorted by price (descending).\n";
            displayProducts(products);
            break;
        }
        case 6: {
            date searchDate;
            cout << "Enter manufacturing date to search:\n";
            searchDate.input();
            vector<product> results = searchByDate(products, searchDate);
            if (results.empty()) {
                cout << "No products found with manufacturing date ";
                searchDate.display();
                cout << endl;
            }
            else {
                cout << "\nProducts manufactured on ";
                searchDate.display();
                cout << ":\n";
                displayProducts(results);
            }
            break;
        }
        case 7: {
            saveToFile(products, filename);
            break;
        }
        case 8: {
            cout << "Total number of product objects: " << product::getCounter() << endl;
            break;
        }
        case 0: {
            cout << "Exiting program. Goodbye!\n";
            break;
        }
        default: {
            cout << "Invalid choice. Please try again.\n";
        }
        }
    } while (choice != 0);

    return 0;
}
