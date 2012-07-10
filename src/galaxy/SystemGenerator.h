#ifndef _SYSTEMGENERATOR_H
#define _SYSTEMGENERATOR_H

class StarSystem;

class SystemGenerator {
public:
	virtual ~SystemGenerator() {}

	virtual StarSystem *GenerateSystem() const = 0;

protected:
	SystemGenerator() {}
};

#endif
