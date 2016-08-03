
#pragma once

#include "Prisma.h"
#include "Implementation.h"


class PrismaImplementation : public Implementation
{
public:
    PrismaImplementation(const std::string & name);
    virtual ~PrismaImplementation();

    virtual void setPrisma(size_t index, const Prisma & prisma) = 0;
};
