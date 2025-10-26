#ifndef PREPOD_H
#define PREPOD_H

#include <iostream>
#include <cstring>

class Prepod {
private:
    char* fam;
    char* name;
    int status;
    static int count;

public:
    Prepod();
    Prepod(const char* surname, const char* firstName, int academicStatus);
    Prepod(const Prepod& other);
    ~Prepod();

    Prepod& operator=(const Prepod& other);

    const char* getFam() const;
    const char* getName() const;
    int getStatus() const;

    void setFam(const char* surname);
    void setName(const char* firstName);
    void setStatus(int academicStatus);

    static int getCount();

    void display() const;

    static const char* getStatusName(int status);
};

#endif
