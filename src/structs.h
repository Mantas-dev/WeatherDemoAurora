#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>

struct ApiReply {
    int httpCode;
    QString body;

    void clear() {
        httpCode = 0;
        body.clear();
    }
};

#endif // STRUCTS_H
