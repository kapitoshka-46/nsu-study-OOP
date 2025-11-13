#ifndef ICONVERTER_H
#define ICONVERTER_H

class IConverter
{
public:
    IConverter();
    virtual ~IConverter() = default;

    virtual void Apply() = 0;
};

#endif // ICONVERTER_H
