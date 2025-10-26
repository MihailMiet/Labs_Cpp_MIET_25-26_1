#include "prepod.h"

int Prepod::count = 0;

Prepod::Prepod() : fam(nullptr), name(nullptr), status(1) {
    count++;
}

Prepod::Prepod(const char* surname, const char* firstName, int academicStatus) {
    fam = new char[strlen(surname) + 1];
    strcpy(fam, surname);

    name = new char[strlen(firstName) + 1];
    strcpy(name, firstName);

    status = academicStatus;
    count++;
}

Prepod::Prepod(const Prepod& other) {
    if (other.fam) {
        fam = new char[strlen(other.fam) + 1];
        strcpy(fam, other.fam);
    }
    else {
        fam = nullptr;
    }
    
    if (other.name) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
    }
    else {
        name = nullptr;
    }

    status = other.status;
    count++;
}

Prepod::~Prepod() {
    delete[] fam;
    delete[] name;
    count--;
}

Prepod& Prepod::operator=(const Prepod& other) {
    if (this != &other) {
        delete[] fam;
        delete[] name;

        if (other.fam) {
            fam = new char[strlen(other.fam) + 1];
            strcpy(fam, other.fam);
        }
        else {
            fam = nullptr;
        }

        if (other.name) {
            name = new char[strlen(other.name) + 1];
            strcpy(name, other.name);
        }
        else {
            name = nullptr;
        }

        status = other.status;
    }
    return *this;
}

const char* Prepod::getFam() const {
    return fam;
}

const char* Prepod::getName() const {
    return name;
}

int Prepod::getStatus() const {
    return status;
}

void Prepod::setFam(const char* surname) {
    delete[] fam;
    fam = new char[strlen(surname) + 1];
    strcpy(fam, surname);
}

void Prepod::setName(const char* firstName) {
    delete[] name;
    name = new char[strlen(firstName) + 1];
    strcpy(name, firstName);
}

void Prepod::setStatus(int academicStatus) {
    if (academicStatus >= 1 && academicStatus <= 3) {
        status = academicStatus;
    }
}

int Prepod::getCount() {
    return count;
}

const char* Prepod::getStatusName(int status) {
    switch (status) {
    case 1: return "Преподаватель";
    case 2: return "Доцент";
    case 3: return "Доктор наук";
    default: return "Неизвестно";
    }
}

void Prepod::display() const {
    std::cout << "Фамилия: " << (fam ? fam : "Н/Д")
        << ", Имя: " << (name ? name : "Н/Д")
        << ", Звание: " << getStatusName(status) << std::endl;
}
