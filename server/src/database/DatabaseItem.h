#ifndef _DATABASE_ITEM_H_
#define _DATABASE_ITEM_H_

#include <string>

class DatabaseItem
{
public:
    std::string serialize();
    static DatabaseItem deserialize(const std::string &);
};

#endif // _DATABASE_ITEM_H_